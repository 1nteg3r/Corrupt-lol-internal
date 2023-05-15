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

  ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Corrupt.lol").x) * 0.5f);
  ImGui::Text("bas1c");

  ImGui::End();

  ImGui::Render();

  pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  return oPresent(pSwapChain, SyncInterval, Flags);

}
bool isKeyPresent(const std::string& link, const std::string& key) {
	HINTERNET hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) return false;

	URL_COMPONENTS urlComp;
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);
	urlComp.dwSchemeLength = (DWORD)-1;
	urlComp.dwHostNameLength = (DWORD)-1;
	urlComp.dwUrlPathLength = (DWORD)-1;
	urlComp.dwExtraInfoLength = (DWORD)-1;

	if (!WinHttpCrackUrl(std::wstring(link.begin(), link.end()).c_str(), (DWORD)link.length(), 0, &urlComp)) {
		WinHttpCloseHandle(hSession);
		return false;
	}

	HINTERNET hConnect = WinHttpConnect(hSession, std::wstring(urlComp.lpszHostName, urlComp.dwHostNameLength).c_str(), urlComp.nPort, 0);
	if (!hConnect) {
		WinHttpCloseHandle(hSession);
		return false;
	}

	DWORD dwOpenRequestFlag = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", std::wstring(urlComp.lpszUrlPath, urlComp.dwUrlPathLength).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, dwOpenRequestFlag);
	if (!hRequest) {
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return false;
	}

	if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return false;
	}

	if (!WinHttpReceiveResponse(hRequest, NULL)) {
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return false;
	}

	DWORD dwSize = 0;
	std::string response;
	do {
		dwSize = 0;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;

		char* pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer) break;

		ZeroMemory(pszOutBuffer, dwSize + 1);

		DWORD dwDownloaded = 0;
		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) break;

		response.append(pszOutBuffer);

		delete[] pszOutBuffer;
	} while (dwSize > 0);

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);

	return response.find(key) != std::string::npos;
}

void Start(void*) {
	// Construct the path to the key.ini file
	std::string keyFilePath = "key.ini";

	// Open the key.ini file
	std::ifstream keyFile(keyFilePath);
	if (!keyFile.is_open()) {
		MessageBox(NULL, "Failed to open key.ini file!", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	// Read the key from the file
	std::string key;
	std::getline(keyFile, key);
	keyFile.close();

	// Example usage
	std::string link = "https://pastebin.com/raw/8QGbbxy5";

	if (isKeyPresent(link, key)) {
		// Inject the DLL here
		LM_LoadModule(std::string("R3nzSkin.dll").data(), LM_NULL);
		Memory::Initialize();
		OrbWalker::Initialize();
		Visuals::initialize();
		while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success ||
			kiero::bind(8, (void**)&oPresent, (void*)HKPresent) != kiero::Status::UnknownError);
	}
	else {
		MessageBoxA(NULL, "Key not valid.", "Error", MB_ICONERROR | MB_OK);
		ExitProcess(0);
	}
}


BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		_beginthread(Start, 0, nullptr);
	}
	return TRUE;
}

