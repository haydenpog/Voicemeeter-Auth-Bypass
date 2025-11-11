#pragma once
#include <iostream>
#include <windows.h>
#include <tlhelp32.h> 
#include <string>
#include <cstdint> 

namespace globals
{

	const std::string processName = "voicemeeter8.exe";

	const DWORD baseOffset = 0x003C17A8;

	const DWORD finalOffset = 0x10D48;

}