/*
 * runsisi AT hust.edu.cn
 */

#define GLOBALHOOKEXPORT __declspec(dllexport)
#include "GlobalHook.h"

#pragma data_seg(".hook")
HHOOK gs_hGlobalHook = 0;
#pragma data_seg()

#pragma comment(linker,"/section:.hook,rws")

HMODULE g_hInstDll = 0;

static LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam);

bool InstallHook()
{
    if (IsHookInstalled())
    {
        return false;
    }

    gs_hGlobalHook = ::SetWindowsHookExW(WH_MOUSE, (HOOKPROC)MouseHookProc, g_hInstDll, 0);

    if (!gs_hGlobalHook)
    {
        return false;
    }

    return true;
}

bool UninstallHook()
{
    if (!IsHookInstalled())
    {
        return false;
    }
    if (!::UnhookWindowsHookEx(gs_hGlobalHook))
    {
        return false;
    }

    gs_hGlobalHook = 0;

    return true;
}

bool IsHookInstalled()
{
    return (gs_hGlobalHook != 0);
}

static LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
    {
        return ::CallNextHookEx(gs_hGlobalHook, code, wParam, lParam);
    }

    return 1;
}

BOOL __stdcall DllMain(HMODULE hInstDll, DWORD dwReason, LPVOID)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        ::DisableThreadLibraryCalls(hInstDll);

        g_hInstDll = hInstDll;
    default:
        break;
    }

    return TRUE;
}
