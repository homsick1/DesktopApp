
/*// Прямая замена заголовка окна
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{	
	int a = 0x000503BE; // Укажите дескриптор окна
	HWND hwnd = (HWND)a;
	SetWindowText(hwnd, L"Новый заголовок");
}
*/


/*
#include <string>
#include<Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{



}
*/

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <atlstr.h>
#include <iostream>
#include <iomanip>
#include <sstream>


#define ID_FIND 1
#define IDM_EXIT 2
#define ID_FINDWINDOWS 3
#define CHANGE_TITLE 4

HINSTANCE hInst;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;
HWND hEditTitle;
HWND hEditDescriptor;
HWND hEditWindow;
HWND a;

TCHAR title[256] = _T("safasfsafasfasf");
TCHAR CommandLine[256] = _T("notepad ReadMe.txt");

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;
	if (!RegisterClass(&wc))
		return -1;

	CreateWindow(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500,
		NULL, NULL, NULL, NULL);
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

// Поиск всех дескрипторов окон
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM	lp)
{
	static std::string st("hProcess hParent idProcess hWindowsTitle\r\n");
	HWND hFind;

	static HWND hEdit;
	TCHAR str[80], title[256], descriptor[256], title1[50];
	LONG hProcess, hParentEdit;
	DWORD id;

	switch (msg)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	//case WM_SIZE:
		//MoveWindow(hEditWindow, 0, 40, LOWORD(lp), HIWORD(lp), TRUE);
		//break;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case ID_FIND:
			hFind = GetTopWindow(NULL);
			do
			{
				hProcess = GetWindowLong(hFind, GWL_HINSTANCE);
				hParentEdit = GetWindowLong(hFind, GWL_HWNDPARENT);
				SendMessageA(hFind, WM_GETTEXT, (WPARAM)256, (LPARAM)title);
				GetWindowThreadProcessId(hFind, &id);
				sprintf((CHAR*)str, "%.8x %.8x %.8d %.8x %s\r\n", hProcess, hParentEdit, id, hFind, title);
				st.append((CHAR*)str);
				hFind = GetWindow(hFind, GW_HWNDNEXT);
			} while (hFind);
			SetWindowTextA(hEditWindow, st.c_str());
			break;
		case CHANGE_TITLE:
			GetWindowText(hEditTitle, title1, 50);
			GetWindowText(hEditDescriptor, descriptor, 50);
			a = descriptor;
			SetWindowText((HWND)a, title1);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default: 
			return DefWindowProc(hWnd, msg, wp, lp);
		}
		break;
	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, ID_FIND, L"Show All Provess");
	AppendMenu(hMenu, MF_STRING, CHANGE_TITLE, L"Change Title");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	CreateWindow(L"static", L"Enter a title here :", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 200, 20, hWnd, NULL, NULL, NULL);
	hEditTitle = CreateWindow(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 0, 250, 20, hWnd, NULL, NULL, NULL);
	CreateWindow(L"static", L"Enter a descriptor here :", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 20, 200, 20, hWnd, NULL, NULL, NULL);
	hEditDescriptor = CreateWindow(L"Edit", L"0x", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 20, 250, 20, hWnd, NULL, NULL, NULL);
	hEditWindow = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0, 40, 450, 400, hWnd, NULL, NULL, NULL);
}

/*
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM	lp)
{
	static STARTUPINFO tin;
	static PROCESS_INFORMATION pInfo;
	HWND hFind;
	hFind = GetTopWindow(NULL);
	DWORD id, hParentEdit;
	switch (msg)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		tin.cb = sizeof(STARTUPINFO);
		tin.dwFlags = STARTF_USESHOWWINDOW;
		tin.wShowWindow = SW_SHOWNORMAL;
		CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL,
			&tin, &pInfo);
		CloseHandle(pInfo.hThread);
		CloseHandle(pInfo.hProcess);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case ID_FINDWINDOWS:
			hFind = GetTopWindow(NULL);
			do
			{
				if (hFind == 0)
				{
					MessageBox(hWnd, _T("Окно не найдено"),
						_T("Ошибка поиска"), MB_OK | MB_ICONQUESTION);
					DestroyWindow(hWnd);
						return 1;
				}
				GetWindowThreadProcessId(hFind, &id);
				hParentEdit = GetWindowLong(hFind, GWL_HWNDPARENT);
				if (hParentEdit == 0 && id == pInfo.dwProcessId)
				{
					SendMessage(hFind, WM_SETTEXT, 0, (LPARAM)title);
					return 0;
				}
				hFind = GetWindow(hFind, GW_HWNDNEXT);
			} while (true);
			break;
		case ID_FIND:
			SetWindowText(hFind, L"Я люблю тебя");
			break;
		case IDM_EXIT: DestroyWindow(hWnd); break;
		default: return DefWindowProc(hWnd, msg, wp, lp);
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

*/