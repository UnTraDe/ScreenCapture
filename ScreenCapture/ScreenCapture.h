#pragma once

#include <d3d9.h>

class ScreenCapture
{
public:
	static void Capture(IDirect3DDevice9* device);
};