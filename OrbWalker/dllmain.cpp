#include "pch.h"
#include <dxgi.h>
#include "includes.h"
#include "overlay.h"
#include "visuals.h"

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
bool bMenuOpen = true;
bool showAttackRange = false;

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
		  ID3D11Texture2D* pBackBuffer;
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


 
  if (showAttackRange)
	  Visuals::range();


  if (GetAsyncKeyState(VK_INSERT) & 1)
	  bMenuOpen = !bMenuOpen;

  if (bMenuOpen) {
	  ImGui::Begin("Corrupt.lol");
	  ImGui::Checkbox("Show Attack Range", &showAttackRange);
	  ImGui::End();
  }

  ImGui::Render();

  pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  return oPresent(pSwapChain, SyncInterval, Flags);

}

void Start(void*) {
  LM_LoadModule(std::string("R3nzSkin.dll").data(), LM_NULL);
  Memory::Initialize();
  OrbWalker::Initialize();
  Visuals::initialize();
  while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success ||
    kiero::bind(8, (void**)&oPresent, (void*)HKPresent) != kiero::Status::UnknownError);
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID) {
  if (dwReason == DLL_PROCESS_ATTACH) {
    _beginthread(Start, 0, nullptr);
  }
  return TRUE;
}
