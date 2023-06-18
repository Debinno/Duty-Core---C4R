#pragma once
#include <Windows.h>
#include "game.h"

namespace dutycore
{
#define EXCP_HEAT_PERCENT (game::ModuleBase + 0x129A2C7)
#define EXCP_HEAT_RESUME (game::ModuleBase + 0x129A2F2)

	typedef unsigned long long(__fastcall* ZwContinue_t)(PCONTEXT ThreadContext, BOOLEAN RaiseAlert);
	static ZwContinue_t ZwContinue = reinterpret_cast<ZwContinue_t>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwContinue"));

	class ExceptionHandler
	{
		static void ExceptionDispatcher(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord);
	public:
		static void InstallExceptionDispatcher();
	};
}
