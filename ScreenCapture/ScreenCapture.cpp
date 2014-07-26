#include "ScreenCapture.h"
#include <d3dx9.h>

void ScreenCapture::Capture(IDirect3DDevice9* device)
{
	IDirect3DSurface9* surface;
	HRESULT res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

	if (res != D3D_OK)
		MessageBoxA(NULL, "Not ok", "D3DERROR", MB_OK);

	D3DXSaveSurfaceToFileA("filename.png", D3DXIFF_PNG, surface, NULL, NULL);
}