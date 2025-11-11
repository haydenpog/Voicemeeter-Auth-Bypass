#include "globals.h"
#include "mem.h"

int main() {
    const std::string process_name = "voicemeeter8.exe";

    const DWORD base_offset = 0x003C17A8;
    const DWORD final_offset = 0x10D48;

    DWORD pid = mem::getPID(process_name);
    if (pid == 0) {
        std::cerr << "Error: Could not find process '" << process_name << "'." << std::endl;
        std::cerr << "Is it running?" << std::endl;
        system("pause");
        return 1;
    }
    std::cout << "Found " << process_name << " (PID: " << pid << ")" << std::endl;

    // 2. Get the main module's base address ("voicemeeter8.exe")
    DWORD_PTR module_base = mem::getModuleBaseAddress(process_name);
    if (module_base == 0) {
        std::cerr << "Error: Could not find module '" << process_name << "' in process." << std::endl;
        system("pause");
        return 1;
    }
    std::cout << "Module Base (\"" << process_name << "\"): 0x" << std::hex << module_base << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        std::cerr << "Error: Could not open process. Try running as administrator." << std::endl;
        system("pause");
        return 1;
    }
    std::cout << "Process handle opened." << std::endl;


    DWORD_PTR pointer_address = module_base + base_offset;
    std::cout << "Pointer Address: 0x" << std::hex << pointer_address << std::endl;

    DWORD base_pointer_value = 0;
    size_t bytesRead = 0;
    if (ReadProcessMemory(hProcess, (LPCVOID)pointer_address, &base_pointer_value, sizeof(base_pointer_value), &bytesRead)) {

        DWORD_PTR final_address = base_pointer_value + final_offset;

        std::cout << "Base Pointer Value: 0x" << std::hex << base_pointer_value << std::endl;
        std::cout << "Final Address to Write: 0x" << std::hex << final_address << std::endl;

        
        DWORD newValue = 0; 
        if (WriteProcessMemory(hProcess, (LPVOID)final_address, &newValue, sizeof(newValue), NULL)) {
            std::cout << "Success: Wrote value to 0x" << std::hex << final_address << std::endl;
        }
        else {
            std::cerr << "Error: Failed to write memory at 0x" << std::hex << final_address << std::endl;
        }

    }
    else {
        std::cerr << "Error: Failed to read Base Pointer from 0x" << std::hex << pointer_address << std::endl;
    }

	std::cout << "done" << std::endl;
    CloseHandle(hProcess);
    system("pause");
    return 0;
}