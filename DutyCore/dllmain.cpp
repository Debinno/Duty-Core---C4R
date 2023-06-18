#include "dllmain.h"
#include "MinHook.h"
#include "dllproxy.h"
#include "resolve_imports.h"
#include "game.h"
#include "script.h"
#include "renderer.h"
#include "anticheat.h"
#include "exception_handler.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        dutycore::main::CreateEntryPoint();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" HRESULT __declspec(dllexport) WICConvertBitmapSource(void* dstFormat, void* pISrc, void** ppIDst)
{
    dutycore::main::Log.WriteLine("Called WICConvertBitmapSource");
    if (!p_WICConvertBitmapSource) {
        p_WICConvertBitmapSource = (tWICConvertBitmapSource)DLLProxy::Instance().XLoadExport("WICConvertBitmapSource", "WindowsCodecs.dll");
        dutycore::main::Log.WriteLine("Found WICConvertBitmapSource");
    }

    return p_WICConvertBitmapSource(dstFormat, pISrc, ppIDst);
}

extern "C"
{
    int __declspec(dllexport) init(void* L)
    {
        dutycore::main::CreateEntryPoint();
        return 1;
    }
}

using tWaitForSingleObject = DWORD(*)(HANDLE, DWORD);

static DWORD OldProtect = {};

static tWaitForSingleObject oWaitForSingleObject = {};
static tWaitForSingleObject p_WaitForSingleObject = {};
static bool InitOnce = false;

namespace dutycore
{
    MinLog main::Log = MinLog("DutyCore.log", "dutycore");

    void main::CreateEntryPoint()
    {
        game::ResizeAssetLimits(XAssetType::AttachmentUnique, 10000);
    }

    DWORD main::InterceptWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {

        // Check if the AssetPool has been initialized
        if (game::GetXAssetPool(XAssetType::Localize) && !InitOnce)
        {
            InitOnce = true;

            main::Log.WriteLine("Duty Core Init");
            //main::Log << std::hex << (uint64_t)game::ModuleBase << "\n";
            // Unhook WaitForSingleObject
            *((void**)(PTR_WAIT_FOR_SINGLE_OBJECT)) = (void*)p_WaitForSingleObject;

            // All Code that should be executed after hooking should be done here.
            // This entry point may be too delayed for certain features
            // Must evaluate on a case-by-case basis

            //game::ResizeAssetLimits(XAssetType::Weapon, 10000);
            //game::ResizeAssetLimits(XAssetType::WeaponDef, 10000);
            //game::ResizeAssetLimits(XAssetType::WeaponVariant, 10000);
            //game::ResizeAssetLimits(XAssetType::WeaponFull, 10000);
            //game::ResizeAssetLimits(XAssetType::Attachment, 10000);
            //game::ResizeAssetLimits(XAssetType::AttachmentUnique, 10000);
            //game::ResizeAssetLimits(XAssetType::WeaponCamo, 10000);
            //game::ResizeAssetLimits(XAssetType::Fx, 100000);

            auto cfuncs = callofduty::script::GetCommonFunctions();
            cfuncs->AddDebugCommand.actionFunc = callofduty::script::GScr_AddDebugCommand;

            //renderer::SetupDXDevice();
            ExceptionHandler::InstallExceptionDispatcher();
        }

        return p_WaitForSingleObject(hHandle, dwMilliseconds);
    }
}