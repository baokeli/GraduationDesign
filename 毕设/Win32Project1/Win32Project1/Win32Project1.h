#pragma once

#include "resource.h"
#define MAX_LOADSTRING 100


// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL				Demo(HWND, UINT, WPARAM, LPARAM);
LRESULT				OnMM_WIM_DATA(UINT wParam, LONG lParam);
DWORD WINAPI		AcceptThreadFunc(LPVOID lpParam);


// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//��������
GWaveIn* m_pWaveIn = NULL;
GWaveOut* m_pWaveOut = NULL;
BYTE* m_pBuf1 = NULL;
BYTE* m_pBuf2 = NULL;
BYTE* m_pBuf3 = NULL;
BYTE* m_pBuf4 = NULL;
int iBufNum = 0;
bool flag = false;