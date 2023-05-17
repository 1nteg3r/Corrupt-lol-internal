#include "pch.h"
#include <dxgi.h>
#include "includes.h"
#include "overlay.h"
#include "visuals.h"
#include <fstream>
#include <regex>
#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <memory>
#include <array>
#pragma comment(lib, "winhttp.lib")


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
bool ShowAttackRange = false;

void RainbowText(const char* text) {
	static float time = 0.0f;
	time += ImGui::GetIO().DeltaTime;
	ImVec4 color = ImColor::HSV(std::fmod(time * 0.2f, 1.0f), 1.0f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text("%s", text);
	ImGui::PopStyleColor();
}


HRESULT WINAPI HKPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
  if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0) OrbWalker::Execute(Type::AutoKite);
  else if ((GetAsyncKeyState('V') & 0x8000) != 0) OrbWalker::Execute(Type::CleanLane);
  else if ((GetAsyncKeyState('X') & 0x8000) != 0) OrbWalker::Execute(Type::LastHit);

  static bool lastState = false;
  static bool currentState = false;
  currentState = GetAsyncKeyState(VK_SPACE) & 0x8000;
  if (currentState) *(OrbWalker::p_aco) = true;
  else if (lastState) *(OrbWalker::p_aco) = false;
  lastState = currentState;

  if (!init)
  {
	  if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
	  {
		  pDevice->GetImmediateContext(&pContext);
		  DXGI_SWAP_CHAIN_DESC sd;
		  pSwapChain->GetDesc(&sd);
		  window = sd.OutputWindow;
		  ID3D11Texture2D* pBackBuffer = nullptr;
		  pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		  pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		  pBackBuffer->Release();
		  oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		  InitImGui();
		  init = true;
	  }

	  else
		  return oPresent(pSwapChain, SyncInterval, Flags);
  }
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  overlay.RenderScene();
  Visuals::range();
  if (ShowAttackRange)
	  Visuals::drawCircles();

  if (GetAsyncKeyState('K') & 1)
	  ShowAttackRange = !ShowAttackRange;
  
  ImVec2 screenSize = ImVec2((float)ImGui::GetIO().DisplaySize.x, (float)ImGui::GetIO().DisplaySize.y);
  ImVec2 windowSize = ImVec2(200, 50);
  ImGui::SetNextWindowPos(ImVec2((screenSize.x - windowSize.x) * 0.5f, 0));
  ImGui::SetNextWindowSize(windowSize);
  ImGui::Begin("Message", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);

  ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("bas1c").x) * 0.5f);
  RainbowText("bas1c");

  ImGui::End();

  ImGui::Render();

  pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  return oPresent(pSwapChain, SyncInterval, Flags);

}

std::string GetHWID() {
	HW_PROFILE_INFO hwProfileInfo;
	GetCurrentHwProfile(&hwProfileInfo);
	char* HWID = hwProfileInfo.szHwProfileGuid;
	std::string HWIDSTR(HWID);
	std::ofstream hwidFile("hwid.txt");
	hwidFile << HWIDSTR;
	hwidFile.close();

	return HWIDSTR;
}

bool CheckHWID(const std::string& hwid) {
	std::wstring url = L"<URL>";
	std::wstring host = L"pastebin.com";
	std::wstring path = L"/raw/8QGbbxy5";

	HINTERNET hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession) {
		HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
		if (hConnect) {
			HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
			if (hRequest) {
				if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
					if (WinHttpReceiveResponse(hRequest, NULL)) {
						DWORD dwSize = 0;
						do {
							dwSize = 0;
							if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
								break;
							}
							std::vector<char> buffer(dwSize + 1);
							DWORD dwDownloaded = 0;
							if (!WinHttpReadData(hRequest, buffer.data(), dwSize, &dwDownloaded)) {
								break;
							}
							buffer[dwDownloaded] = '\0';
							std::string response(buffer.begin(), buffer.end());
							if (response.find(hwid) != std::string::npos) {
								WinHttpCloseHandle(hSession);
								WinHttpCloseHandle(hConnect);
								WinHttpCloseHandle(hRequest);
								return true;
							}
						} while (dwSize > 0);
					}
				}
				WinHttpCloseHandle(hRequest);
			}
			WinHttpCloseHandle(hConnect);
		}
		WinHttpCloseHandle(hSession);
	}

	return false;
}

void Start(void*) {
	// Example usage
	std::string hwid = GetHWID();

	if (CheckHWID(hwid)) {
		// Inject the DLL here
		LM_LoadModule(std::string("R3nzSkin.dll").data(), LM_NULL);
		Memory::Initialize();
		OrbWalker::Initialize();
		Visuals::initialize();
		while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success ||
			kiero::bind(8, (void**)&oPresent, (void*)HKPresent) != kiero::Status::UnknownError);
	}
	else {
		MessageBoxA(NULL, "Hwid not valid.", "Error", MB_ICONERROR | MB_OK);
		ExitProcess(0);
	}
}


BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		_beginthread(Start, 0, nullptr);
	}
	return TRUE;
}

