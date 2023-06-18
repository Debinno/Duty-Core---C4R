#pragma once
#include "dllmain.h"
#include <TlHelp32.h>

namespace dutycore {
	class anticheat
	{
	public:
		static void Init();

		using tProcess32Next = int(*)(HANDLE, LPPROCESSENTRY32);
		static std::wstring* blacklist;
		static tProcess32Next oProcess32Next;
		static int Process32NextHook(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
	};
}

