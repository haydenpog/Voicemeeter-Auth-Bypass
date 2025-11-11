#include "mem.h"


namespace mem 
{
    HANDLE hProcess = nullptr;
    DWORD processID = 0;
    
    bool getPID() 
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (snapshot == INVALID_HANDLE_VALUE) 
        {
			std::cerr << "Error: Could not create process snapshot." << std::endl;
            return false;
        }

        if (Process32First(snapshot, &entry)) {
            do {
                if (_stricmp(entry.szExeFile, globals::processName.c_str()) == 0) {
                    CloseHandle(snapshot);
					processID = entry.th32ProcessID;
					return true;
                }
            } while (Process32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
		std::cerr << "Error: Process '" << globals::processName << "' not found." << std::endl;
        return false;
    }

    DWORD_PTR getModuleBaseAddress(const std::string& moduleName) {
        MODULEENTRY32 entry;
        entry.dwSize = sizeof(MODULEENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }

        if (Module32First(snapshot, &entry)) {
            do {
                if (_stricmp(entry.szModule, moduleName.c_str()) == 0) {
                    CloseHandle(snapshot);
                    return (DWORD_PTR)entry.modBaseAddr;
                }
            } while (Module32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return 0; // Not found
    }

    bool openProcess()
    {
        hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, processID);
        if (hProcess == NULL) {
            std::cerr << "Error: Could not open process. Try running as administrator." << std::endl;
            system("pause");
            return false;
        }
        return true;
    }

    bool write(LPVOID address, BYTE* data, SIZE_T size)
    {
        SIZE_T bytesWritten = 0;
        return WriteProcessMemory(hProcess, address, data, size, &bytesWritten);
    }

    bool read(LPCVOID address, BYTE* buffer, SIZE_T size)
    {
        SIZE_T bytesRead = 0;
        return ReadProcessMemory(hProcess, address, buffer, size, &bytesRead);
    }
}