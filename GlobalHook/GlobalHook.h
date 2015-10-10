/*
 * runsisi AT hust.edu.cn
 */

#ifndef GLOBALHOOK_RUNSISI_H_
#define GLOBALHOOK_RUNSISI_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef GLOBALHOOKEXPORT
#define GLOBALHOOKEXPORT __declspec(dllimport)
#endif

GLOBALHOOKEXPORT bool InstallHook();
GLOBALHOOKEXPORT bool UninstallHook();
GLOBALHOOKEXPORT bool IsHookInstalled();

#endif
