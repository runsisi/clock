// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "./ClockWnd.h"
#include <map>
#include "resource.h"

#define VERIFY(x)


// Global Variables:
HINSTANCE 				g_hInstance = NULL;								// current instance
CClockWnd 				g_oClock;
HMENU	  				g_hContextMenu = NULL;
NOTIFYICONDATA			g_stNotify = {0};
std::map<INT,CString>	g_mpMenu;

#define WND_CLASS_NAME		_T("ClockWndCls")
#define WND_TITLE			_T("Clock")
#define CLOCK_TIMER			0xFF00
#define TRAY_MSG_ID			(WM_APP + 0x44)

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,TRUE,NULL,0);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	::CoInitialize(NULL);
	g_hInstance = hInstance;

	MSG msg;
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WND_CLASS_NAME;
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);


	HWND hWnd;
	hWnd =  ::CreateWindowEx( WS_EX_LAYERED | WS_EX_LEFT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW
			, WND_CLASS_NAME
			, NULL
			, WS_VISIBLE
			, 0
			, 0
			, 100
			, 100
			, NULL
			, NULL
			, hInstance
			, NULL
			);
	VERIFY( ::IsWindow(hWnd) );
	g_oClock.Attach(hWnd);
	::ShowWindow(hWnd, SW_HIDE);
	

	// enumurate all the skin directories, and then create the menu,
	BOOL bLoad = FALSE;
	{
		TCHAR tszBuf[MAX_PATH] = {'\0'};
		::GetModuleFileName( NULL, tszBuf, MAX_PATH);
		CString strTemp(tszBuf);
		CString strFindPath;
		strFindPath.Format( _T("%sTheme\\*")
			, strTemp.Mid( 0, strTemp.ReverseFind('\\') + 1)
			);

		INT nIndex = WM_USER + 0x1000;
		g_hContextMenu = ::CreatePopupMenu();
		WIN32_FIND_DATA sFD;
		HANDLE hd = ::FindFirstFile( strFindPath, &sFD);
		while(::FindNextFile(hd, &sFD))
		{
			if(sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(sFD.cFileName[0] != '.')
				{
					if( !bLoad )
						bLoad = g_oClock.LoadTheme(sFD.cFileName);
				}
			}
		}
		::FindClose(hd);

		::AppendMenu(g_hContextMenu
			, MF_POPUP
			, nIndex
			, _T("&About")
			);

		nIndex++;
		::AppendMenu( g_hContextMenu
			, MF_MENUBARBREAK
			, nIndex
			, NULL
			);

		nIndex++;
		::AppendMenu( g_hContextMenu
			, MF_POPUP
			, nIndex
			, _T("&Exit")
			);
	}

	if( !bLoad )
	{
		::MessageBox( hWnd, _T("Failed to load skin"), _T("Error"), MB_OK | MB_ICONERROR);
		return -1;
	}	

	// Layout
	RECT rc;
	::GetWindowRect( hWnd, &rc);
	::MoveWindow( hWnd
		, (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2
		, (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2
		, rc.right - rc.left
		, rc.bottom - rc.top
		, FALSE
		);
	
	::SetTimer( hWnd, CLOCK_TIMER, 1000, NULL);
	ShowWindow(hWnd, SW_HIDE);
	//UpdateWindow(hWnd);
	//g_oClock.Render();

	
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	::DestroyMenu(g_hContextMenu);
	::KillTimer( hWnd, CLOCK_TIMER);
	UnregisterClass( WND_CLASS_NAME, g_hInstance);
	::CoUninitialize();

	return (int) msg.wParam;
}

#define SLEEP_SECONDS 10
#define AWAKE_SECONDS 5

bool g_isSleep = true;
int g_sleepedSeconds = 0;
int g_awakedSeconds = 0;


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			g_stNotify.cbSize = sizeof(NOTIFYICONDATA);
			g_stNotify.hWnd = hWnd;
			g_stNotify.uID = IDI_ICON1;
			g_stNotify.hIcon = ::LoadIcon( g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
			g_stNotify.uFlags = NIF_INFO | NIF_TIP | NIF_MESSAGE | NIF_ICON;
			g_stNotify.uCallbackMessage = TRAY_MSG_ID;
			g_stNotify.dwInfoFlags = NIIF_INFO;
			_tcscpy_s( g_stNotify.szInfo, 128, _T("Clock"));
			_tcscpy_s( g_stNotify.szInfo, 256, _T("Use the context menu to swith skin"));
			_tcscpy_s( g_stNotify.szInfoTitle, 64, _T("The clock program is running"));

			::Shell_NotifyIcon( NIM_ADD, &g_stNotify);
		}
		break;

	
	case WM_COMMAND:
		{
			// The menu message
			if( HIWORD(wParam) == 0 )
			{
				if (LOWORD(wParam) == (WM_USER + 0x1000))
				{
					::MessageBox(hWnd, _T("runsisi AT hust.edu.cn"), _T("About"), MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					goto lblDestroy;
				}
			}
		}
		break;
	case TRAY_MSG_ID:
		if( LOWORD(lParam) == WM_LBUTTONUP ||
			LOWORD(lParam) == WM_RBUTTONUP )
		{
			POINT pt;
			::GetCursorPos(&pt);
			::TrackPopupMenu( g_hContextMenu
				, TPM_LEFTALIGN | TPM_RIGHTBUTTON
				, pt.x
				, pt.y
				, 0
				, hWnd
				, NULL
				);
		}
		break;
	case WM_KEYUP:
		if( wParam != VK_ESCAPE )
			break;	
		else
			goto lblDestroy;

	case WM_DESTROY:
lblDestroy:
		::Shell_NotifyIcon( NIM_DELETE, &g_stNotify);
		g_oClock.Release();
		PostQuitMessage(0);
		break;	
	case WM_LBUTTONDOWN:
		SendMessage( hWnd, WM_NCLBUTTONDOWN,(WPARAM)HTCAPTION,(LPARAM)HTCAPTION);
		break;
	case WM_TIMER:
		if (g_isSleep)
		{
			if (g_sleepedSeconds++ == SLEEP_SECONDS)
			{
				// install hook
				HINSTANCE hinstDLL;
				typedef BOOL(*inshook)(HWND);
				inshook instkbhook;
				hinstDLL = LoadLibrary(L"GlobalHook.dll");
				instkbhook = (inshook)GetProcAddress(hinstDLL, "InstallHook");
				instkbhook(0);

				g_sleepedSeconds = 0;
				g_isSleep = false;	// awake it

				ShowWindow(g_oClock.m_hWnd, SW_NORMAL);
			}
		}

		if (!g_isSleep)
		{
			g_oClock.Render();

			if (g_awakedSeconds++ == AWAKE_SECONDS)
			{
				// uninstall hook
				HINSTANCE hinstDLL;
				typedef BOOL(*inshook)(HWND);
				inshook instkbhook;
				hinstDLL = LoadLibrary(L"GlobalHook.dll");
				instkbhook = (inshook)GetProcAddress(hinstDLL, "UninstallHook");
				instkbhook(0);

				g_awakedSeconds = 0;
				g_isSleep = true;	// sleep again

				ShowWindow(g_oClock.m_hWnd, SW_HIDE);
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

