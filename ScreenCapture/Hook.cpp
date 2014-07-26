#include "Hook.h"

Hook::Hook(void* originalFunc, void* hookFunc)
{
	m_OriginalFunction = originalFunc;
	m_HookFunction = hookFunc;
	m_IsHooked = false;

	memset(m_JmpInstruction, 0, sizeof(m_JmpInstruction));
	m_JmpInstruction[0] = 0xe9;
	m_JmpInstruction[5] = 0xc3;

	DWORD relativeAddress = (DWORD)m_HookFunction - (DWORD)m_OriginalFunction - 5; // 5 = length of the jump instruction (jmp itself is one byte and 4 other bytes is the address)
	memcpy(&m_JmpInstruction[1], &relativeAddress, 4);
	DWORD oldProtection;
	VirtualProtect(m_OriginalFunction, 1024, PAGE_EXECUTE_READWRITE, &oldProtection); //1024 is an artibarary size, hope it's enough...
	ReadProcessMemory(GetCurrentProcess(), m_OriginalFunction, m_BackupInstruction, sizeof(m_BackupInstruction), NULL);
}

Hook::~Hook()
{
	if (m_IsHooked)
		Remove();
}

void Hook::Setup()
{
	WriteProcessMemory(GetCurrentProcess(), m_OriginalFunction, m_JmpInstruction, sizeof(m_JmpInstruction), NULL);
	m_IsHooked = true;
}

void Hook::Remove()
{
	WriteProcessMemory(GetCurrentProcess(), m_OriginalFunction, m_BackupInstruction, sizeof(m_BackupInstruction), NULL);
	m_IsHooked = false;
}