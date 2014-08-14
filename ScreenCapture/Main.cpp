#include <string>
#include <Windows.h>
#define D3D_DEBUG_INFO
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <Vfw.h>
#include "D3D9Hook.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern "C" __declspec(dllexport) void Startup();

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	default:
		break;
	}

	return TRUE;
}

__declspec(dllexport) void Startup()
{
	MessageBox(NULL, "Injected2", "DirectXHook", NULL);
	Hook_Initialize();
}