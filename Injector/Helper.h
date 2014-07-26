#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>

class Helper
{
public:
	static DWORD GetProcessID(std::string name);
};