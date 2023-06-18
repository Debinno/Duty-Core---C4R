#include "anticheat.h"
#include <MinHook.h>

namespace dutycore
{
	anticheat::tProcess32Next anticheat::oProcess32Next = {};
	void anticheat::Init()
	{
		auto kernel32Module = GetModuleHandleA("Kernel32.dll");
		if (!kernel32Module)
		{
			main::Log.WriteLine("Failed to init Anticheat bypasses - Could not find Kernel32");
			return;
		}
		
		auto proc32NextPtr = GetProcAddress(kernel32Module, "Process32NextW");
		if (!proc32NextPtr)
		{
			main::Log.WriteLine("Failed to init Anticheat bypasses - Could not find Process32NextW");
			return;
		}
		
		if (MH_CreateHook(proc32NextPtr, &anticheat::Process32NextHook, reinterpret_cast<LPVOID*>(&anticheat::oProcess32Next)) != MH_OK)
		{
			main::Log.WriteLine("Failed to init Anticheat bypasses - Could not create Process32Next hook");
			return;
		}

		if (MH_EnableHook(proc32NextPtr) != MH_OK)
		{
			main::Log.WriteLine("Failed to init Anticheat bypasses - Could not enable Process32Next hook");
			return;
		}
		main::Log.WriteLine("Created Anticheat bypasses");
	}

	/*anticheat::blacklist = new std::wstring[1]{
			L"ida.exe"
	};*/

	int anticheat::Process32NextHook(HANDLE hSnapshot, LPPROCESSENTRY32 lppe)
	{
		for (;;)
		{
			auto ret = anticheat::oProcess32Next(hSnapshot, lppe);

			if ((lppe->szExeFile != L"ida.exe" && lppe->szExeFile != L"ida64.exe") || ret == 0) {
				return ret;
			}

			main::Log.WriteLine("Bypassing IDA Process");
		}
	}
}