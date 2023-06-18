#include "exception_handler.h"

namespace dutycore
{
	// Sourced from: https://github.com/shiversoftdev/t7dwidm_patch/blob/master/t7dwidm_patch/hook.cpp#L10
	void ExceptionHandler::InstallExceptionDispatcher()
	{
		auto ntdllModule = GetModuleHandleA("ntdll.dll");
		if (!ntdllModule) {
			return;
		}
		auto dispatcherptr = (char*)GetProcAddress(ntdllModule, "KiUserExceptionDispatcher");
		auto distance = *(int *)(dispatcherptr + 4); // LdrParentRtlInitializeNtUserPfn
		auto ptr = (dispatcherptr + 8) + distance;

		auto OldProtection = 0ul;
		VirtualProtect(reinterpret_cast<void*>(ptr), 8, PAGE_EXECUTE_READWRITE, &OldProtection);
		*reinterpret_cast<void**>(ptr) = (void*)ExceptionHandler::ExceptionDispatcher;
		VirtualProtect(reinterpret_cast<void*>(ptr), 8, OldProtection, &OldProtection);
	}

	void ExceptionHandler::ExceptionDispatcher(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord)
	{
		if (ExceptionRecord->ExceptionAddress == EXCP_HEAT_PERCENT)
		{
			ContextRecord->Xmm1 = (M128A)0;					// Set the heat percent multiplier to 0
			ContextRecord->Rip = (DWORD64)EXCP_HEAT_RESUME; // jump to the next instruction
			ZwContinue(ContextRecord, false);
		}
		/*else
		{
			main::Log << "Exception occured at 0x" << std::hex << ExceptionRecord->ExceptionAddress << "\n";
		}*/
	}
}