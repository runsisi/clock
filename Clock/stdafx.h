// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>

#include <ATLComTime.h>


#include <shellapi.h>
#pragma comment( lib, "shell32.lib")


#include "./GdiplusH.h"
#include "./CXml/Xml.h"

using namespace Generic;
