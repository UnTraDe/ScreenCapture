#include "Helper.h"

DWORD Helper::GetProcessID(std::string name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!hSnapshot)
		return 0;
	
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	DWORD processID = 0;

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (std::string(pe32.szExeFile) == name)
			{
				processID = pe32.th32ProcessID;
				break;
			}
		}
		while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);

	if (processID)
		return processID;
	
	return 0;
}