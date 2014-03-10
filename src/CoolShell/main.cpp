// CoolShell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CoolShell.h"

#include "CoolShellApp.h"
#include "CoolShellLib/Logging.h"
#include "CoolShellLib/Console.h"

#if _DEBUG && !_WIN64
#include "vld.h"
#pragma message( "VisualLeakDetector Library enabled" )
#endif

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    int retCode = 0;

    try
    {
        Logging::SetLogLevel(Logging::Trace);

        Console::Create();
        LOG_DEBUG(_T("Console created"));
#ifndef _DEBUG
        Console::Hide();
        LOG_DEBUG(_T("Console hidden"));
#endif
        // register a handler to console events so we can terminate the process in a clean way
        DWORD tid = ::GetCurrentThreadId();
        Console::RegisterCtrlHandler([=] (DWORD /*dwCtrlType*/) -> BOOL {
            return ::PostThreadMessage(tid, WM_QUIT, 0, NULL);
        });

        CoolShellApp app;
        retCode = app.Run();
    }
    catch (std::exception& e)
    {
        LOG_FATAL("[WinMain] Exception: %s", e.what());
        retCode = 1;
    }
    catch (...)
    {
        LOG_FATAL("[WinMain] Exception of unknown type");
        retCode = 1;
    }

    return retCode;
}

//#define MAX_LOADSTRING 100
//
//// Global Variables:
//HINSTANCE hInst;								// current instance
//HWND hMainWnd;                                  // main window handle
//TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
//TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//
//// Forward declarations of functions included in this code module:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	AboutWndProc(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY _tWinMain(HINSTANCE hInstance,
//                     HINSTANCE hPrevInstance,
//                     LPTSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Place code here.
//
//    // Initialize global strings
//    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadString(hInstance, IDC_COOLSHELL, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Perform application initialization:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COOLSHELL));
//
//    CoolShellApp application(hMainWnd);
//    if(!application.InitInstance())
//    {
//        return 1L;
//    }
//
//
//    //WORD getVkFlagsNoSide() const
//    //{
//    //    return (WORD)m_vkFlags | (WORD)(m_vkFlags >> vkFlagsRightOffset);
//    //}
//
//    //DWORD m_vkKey = 81;
//    //DWORD m_vkFlags = MOD_WIN;
//    //DWORD flags = (WORD)m_vkFlags | (WORD)(m_vkFlags >> 16);
//    //DWORD m_id = MAKEWORD(m_vkKey, flags);
//    //if(!RegisterHotKey(hMainWnd, m_vkFlags, m_vkFlags, m_vkKey))
//    //    TRACE("RegisterHotKey failed\n");
//
//    // Main message loop:
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    application.ExitInstance();
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  FUNCTION: MyRegisterClass()
////
////  PURPOSE: Registers the window class.
////
////  COMMENTS:
////
////    This function and its usage are only necessary if you want this code
////    to be compatible with Win32 systems prior to the 'RegisterClassEx'
////    function that was added to Windows 95. It is important to call this function
////    so that the application will get 'well formed' small icons associated
////    with it.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEX wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style			= CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc	= WndProc;
//    wcex.cbClsExtra		= 0;
//    wcex.cbWndExtra		= 0;
//    wcex.hInstance		= hInstance;
//    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COOLSHELL));
//    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_COOLSHELL);
//    wcex.lpszClassName	= szWindowClass;
//    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassEx(&wcex);
//}
//
////
////   FUNCTION: InitInstance(HINSTANCE, int)
////
////   PURPOSE: Saves instance handle and creates main window
////
////   COMMENTS:
////
////        In this function, we save the instance handle in a global variable and
////        create and display the main program window.
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // Store instance handle in our global variable
//
//   hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hMainWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hMainWnd, nCmdShow);
//   UpdateWindow(hMainWnd);
//
//   return TRUE;
//}
//
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE:  Processes messages for the main window.
////
////  WM_COMMAND	- process the application menu
////  WM_PAINT	- Paint the main window
////  WM_DESTROY	- post a quit message and return
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    int wmId, wmEvent;
//    PAINTSTRUCT ps;
//    HDC hdc;
//
//    switch (message)
//    {
//    case WM_HOTKEY:
//        TRACE(_T("WM_HOTKEY %d\n"), wParam);
//        break;
//    case WM_COMMAND:
//        wmId    = LOWORD(wParam);
//        wmEvent = HIWORD(wParam);
//        // Parse the menu selections:
//        switch (wmId)
//        {
//        case IDM_ABOUT:
//            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutWndProc);
//            break;
//        case IDM_EXIT:
//            DestroyWindow(hWnd);
//            break;
//        default:
//            return DefWindowProc(hWnd, message, wParam, lParam);
//        }
//        break;
//    case WM_PAINT:
//        hdc = BeginPaint(hWnd, &ps);
//        // TODO: Add any drawing code here...
//        EndPaint(hWnd, &ps);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// Message handler for about box.
//INT_PTR CALLBACK AboutWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
