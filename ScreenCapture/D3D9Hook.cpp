#include "D3D9Hook.h"
#include "ScreenCapture.h"

/* DirectX 9 Hooking */

//Hooks
void EndScene_Hook();
void SetupHook(void* originalFunc, void* hookFunc);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Hook_Initialize()
{
	char* clsName = "test";
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = NULL;
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = clsName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
		MessageBox(NULL, "Failed to register class. maybe the dll already injected into this process?", "DirectXHook", NULL);

	HWND hWnd = CreateWindow(
		clsName,
		"Temporary",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		NULL,
		NULL
		);

	if (!hWnd)
		MessageBox(NULL, std::to_string(GetLastError()).c_str(), "DirectXHook: Call to CreateWindow failed!", NULL);

	UpdateWindow(hWnd);

	LPDIRECT3D9 d3d9Device = Direct3DCreate9(D3D_SDK_VERSION);
	LPDIRECT3DDEVICE9 pDevice = NULL;
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	IDirect3DDevice9* d3dDevice;

	d3d9Device->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3dDevice);

	DWORD* vTable = (DWORD*)*((DWORD*)d3dDevice);
	DWORD endSceneAddr = vTable[42]; //42 is the offset to the EndScene function in the vtable

	MessageBox(NULL, std::to_string(endSceneAddr).c_str(), "DirectXHook", NULL);

	SetupHook((void*)endSceneAddr, EndScene_Hook);

	DestroyWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD address = 0;

void SetupHook(void* originalFunc, void* hookFunc)
{
	BYTE instruction[] = { 0xe9, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90 };
	DWORD relativeAddress = (DWORD)hookFunc - (DWORD)originalFunc - 5; // 5 = length of the jump instruction (jmp itself is one byte and 4 other bytes is the address)
	memcpy(&instruction[1], &relativeAddress, 4);
	DWORD oldProtection;
	VirtualProtect(originalFunc, 1024, PAGE_EXECUTE_READWRITE, &oldProtection); //1024 is an artibarary size, hope it's enough...
	WriteProcessMemory(GetCurrentProcess(), originalFunc, instruction, sizeof(instruction), NULL);
	address = (DWORD)originalFunc + sizeof(instruction);
}

bool first = true;
ScreenCapture* screenCapture;


__declspec(naked) void EndScene_Hook()
{

	__asm
	{
		PUSH EAX
		MOV EAX, [ESP + 8]		
	}

	IDirect3DDevice9* device;

	__asm
	{
		MOV device, EAX
		POP EAX
	}
	
	if (first)
	{
		ScreenCapture::ScreenShot(device);
		first = false;
	}
	
	__asm
	{
		PUSH 0x14
		MOV EAX, 0x718E6478
		JMP address
	}
}