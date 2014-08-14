#include <iostream>
#include <string>
#include <windows.h>
#include "Helper.h"

using namespace std;

int main(int argc, char const *argv[])
{
	//TODO Find the dll path dynamically
	const char* dllPath = "C:\\Users\\UnTra_000\\Source\\Repos\\ScreenCapture\\Debug\\ScreenCapture.dll";

	/* Find Process ID */
	int processID = Helper::GetProcessID("Terraria.exe");
	cout << "Processs ID found: " << processID << endl;

	/* Inject the DLL to the remote process */
	HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	LPVOID dllPathAddress = VirtualAllocEx(procHandle, 0, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(procHandle, dllPathAddress, dllPath, strlen(dllPath), NULL);
	HMODULE kernelHandle = GetModuleHandle("kernel32.dll");
	FARPROC func = GetProcAddress(kernelHandle, "LoadLibraryA");
	HANDLE remoteThread = CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)func, dllPathAddress, 0, NULL);
	WaitForSingleObject(remoteThread, INFINITE);
	DWORD remoteDllAddress = 0;
	GetExitCodeThread(remoteThread, &remoteDllAddress);
	
	/* Call Startup() on the injected DLL */
	HMODULE localDllModule = LoadLibraryA(dllPath);
	FARPROC funcPointer = GetProcAddress(localDllModule, "Startup");
	DWORD relativeAddress = (DWORD)funcPointer - (DWORD)localDllModule;
	DWORD addressInRemoteProcess = remoteDllAddress + relativeAddress;
	CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)addressInRemoteProcess, NULL, 0, NULL);
	
	if (remoteThread == NULL)
		cout << "Error creating remote thread: " << GetLastError() << endl;
	else
		cout << "Injected" << endl;

	cout << "Press any key to continue...";
	getchar();

	return 0;
}