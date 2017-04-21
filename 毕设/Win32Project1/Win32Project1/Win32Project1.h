#pragma once

#include "resource.h"
GWaveIn* m_pWaveIn = NULL;
GWaveOut* m_pWaveOut = NULL;
BYTE* m_pBuf1 = NULL;
BYTE* m_pBuf2 = NULL;
BYTE* m_pBuf3 = NULL;
BYTE* m_pBuf4 = NULL;



int iBufNum = 0;
bool flag = false;
HANDLE hBtnStart;
HANDLE hBtnClose;
HWND hEdit;
HWND hWnd;
HANDLE hText1;
