#include "ScreenCapture.h"
#include <d3dx9.h>

void ScreenCapture::ScreenShot(IDirect3DDevice9* device)
{
	IDirect3DSurface9* surface;
	HRESULT res = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
	D3DXSaveSurfaceToFileA("filename.png", D3DXIFF_PNG, surface, NULL, NULL);
}