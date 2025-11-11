#include "globals.h"
#include "mem.h"

int main() 
{
	// get pid first so mem::processID is set
	if (!mem::getPID()) 
	{
		std::cerr << "Error: Could not find PID for " << globals::processName << std::endl;
		system("pause");
		return 1;
	}

	// get our module base address
    DWORD_PTR moduleBase = mem::getModuleBaseAddress(globals::processName);
    if (moduleBase == 0) {
        std::cerr << "Error: Could not find module address" << std::endl;
        system("pause");
        return 1;
    }
    std::cout << "Module Base (\"" << globals::processName << "\"): 0x" << std::hex << moduleBase << std::endl;

	// open the process handle
    if(!mem::openProcess())
	{
		std::cerr << "Error: Could not open a handle for the process: " << globals::processName << std::endl;
		system("pause");
		return 1;
	}

	// get our pointer address
    DWORD_PTR pointerAddress = moduleBase + globals::baseOffset;
	std::cout << "Pointer Address: 0x" << std::hex << pointerAddress << std::endl;

	DWORD basePointerValue = 0;
	if (mem::read((LPCVOID)pointerAddress, (BYTE*)&basePointerValue, sizeof(basePointerValue)))
	{
		DWORD_PTR finalAddress = basePointerValue + globals::finalOffset;


		std::cout << "Base Pointer Value: 0x" << std::hex << basePointerValue << std::endl; // debug
		std::cout << "Final Address to Write: 0x" << std::hex << finalAddress << std::endl;  // debug


		DWORD newValue = 0; // set the timer to 0, skips the auth clock
		if (mem::write((LPVOID)finalAddress, (BYTE*)&newValue, sizeof(newValue)))
		{
			std::cout << "Success: Wrote value to 0x" << std::hex << finalAddress << std::endl;
		}
		else
		{
			std::cerr << "Error: Failed to write memory at 0x" << std::hex << finalAddress << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Failed to read Base Pointer from 0x" << std::hex << pointerAddress << std::endl;
	}
	return 0;
    
}