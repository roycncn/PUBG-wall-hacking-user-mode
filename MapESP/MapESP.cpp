// MapESP.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MapESP.h"
#include "LoadDriver.hpp"
#include "DataReader.hpp"
#include <thread>
#include <fstream> 
#include <Windows.h>
#include <atlcomtime.h>
#include <Shellapi.h>
#include "Config.h"



using namespace std;
#define MAX_LOADSTRING 100
#define DRIVER_NAME_SYS L"dnsqa"
#define DRIVER_PATH_SYS L".\\dnsqa.sys"

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
std::thread* thread_1;
DataReader* dataReader;
bool run = false;
struct config rtConfig;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ControlPanel(HWND, UINT, WPARAM, LPARAM);
void threadWorker(DataReader* dataReader, HWND textEdit);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAPESP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPESP));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPESP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAPESP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      600, 600, 400, 100, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   DialogBox(hInst, MAKEINTRESOURCE(IDD_MAPESP_DIALOG), hWnd, ControlPanel);





   //No Show Windows
   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    }
    return (INT_PTR)FALSE;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK ControlPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND edit = GetDlgItem(hDlg, IDC_EDIT1);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if (dataReader == nullptr) {
			try
			{
				rtConfig = get_config(".//setting.ini");
			}
			catch (std::exception& e)
			{
				MessageBox(hDlg, LPCWSTR(L"Open Config Failed."), LPCWSTR(L"Open Config Failed"), MB_OK);
			}

			try
			{
				dataReader = new DataReader(rtConfig.PostURL);
			}
			catch (std::exception& e)
			{
				MessageBox(hDlg, LPCWSTR(L"Opening driver handle failed."), LPCWSTR(L"Please Load SYS first"), MB_OK);
			}



		}
		

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_LOADSYS )
		{
			COleDateTime tm;
			tm.ParseDateTime(L"2014-04-01 8:00:00");
			
			COleDateTime ori_tm;
			ori_tm = COleDateTime::GetCurrentTime();

			SYSTEMTIME st;
			tm.GetAsSystemTime(st);
			SetLocalTime(&st);

			BOOL bRet = LoadNTDriver(DRIVER_NAME_SYS, DRIVER_PATH_SYS, edit);
			if (!bRet)
			{
				MessageBox(hDlg, LPCWSTR(L"Error!"), LPCWSTR(L"Load Sys"), MB_OK);
				return 0;
			}

			TestDriver(hDlg);

			ori_tm.GetAsSystemTime(st);
			SetLocalTime(&st);


			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_RUN)
		{

			if (thread_1 == nullptr)
			{
				thread_1 = new std::thread(threadWorker, dataReader, edit);
			}
			wchar_t buffer[2048];
			HWND edit = GetDlgItem(hDlg, IDC_EDIT1);

			int64_t gameBase =	dataReader->readGameBase();

			wsprintfW(buffer, L"Read Game Base Successful, Base:%I64u", gameBase);
			SetWindowText(edit, buffer);

			run = true;
			

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_STOP)
		{
			run = false;

			return (INT_PTR)TRUE;

		}
		else if (LOWORD(wParam) == IDC_OPENESP)
		{
			string espURL = rtConfig.ESPURL;
			wstring wespURL = wstring(espURL.begin(), espURL.end());
			const wchar_t* result = wespURL.c_str();
			ShellExecute(NULL, L"open", result, NULL, NULL, SW_SHOWNORMAL);
			return (INT_PTR)TRUE;

		}else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}

void threadWorker(DataReader* dataReader,HWND textEdit)
{

	while (1)
	{

		if(run)
		{
			dataReader->doRead(textEdit);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}



	}
}