#pragma once

#include "resource.h"
#define MAX_LOADSTRING 100


// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL				Demo(HWND, UINT, WPARAM, LPARAM);
LRESULT				OnMM_WIM_DATA(UINT wParam, LONG lParam);
DWORD WINAPI		AcceptThreadFunc(LPVOID lpParam);


// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

//变量声明
GWaveIn* m_pWaveIn = NULL;
GWaveOut* m_pWaveOut = NULL;
BYTE* m_pBuf1 = NULL;
BYTE* m_pBuf2 = NULL;
BYTE* m_pBuf3 = NULL;
BYTE* m_pBuf4 = NULL;
int iBufNum = 0;
bool flag = false;