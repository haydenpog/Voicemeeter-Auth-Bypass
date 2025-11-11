#pragma once
#include "globals.h"

namespace mem 
{

	extern HANDLE hProcess;

	extern DWORD processID;

	bool getPID();

	DWORD_PTR getModuleBaseAddress(const std::string& moduleName);

	bool openProcess();

	bool write(LPVOID address, BYTE* data, SIZE_T size);

	bool read(LPCVOID address, BYTE* buffer, SIZE_T size);

}