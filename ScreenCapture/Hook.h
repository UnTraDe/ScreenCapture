#pragma once

#include <Windows.h>
#include <d3d9.h>

class Hook
{
public:
	Hook(void* originalFunc, void* hookFunc);
	~Hook();

	void Setup();
	void Remove();
	HRESULT DoOriginal(IDirect3DDevice9* device);
private:
	void* m_OriginalFunction;
	void* m_HookFunction;
	bool m_IsHooked;
	BYTE m_JmpInstruction[6];
	BYTE m_BackupInstruction[6];
};