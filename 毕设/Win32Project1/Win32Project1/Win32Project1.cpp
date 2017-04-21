// Win32Project1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32Project1.h"
#pragma comment(lib,"ws2_32.lib")  

#define MAX_LOADSTRING 100
#define BTN_CONNECT		101
#define BTN_CLOSE		102
#define EDIT			103
char strBuff[4096];

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
bool isConnect = false;
SocketClient* lpClient=NULL;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL				Demo(HWND, UINT, WPARAM, LPARAM);
LRESULT				OnMM_WIM_DATA(LPSTR lParam);
DWORD WINAPI		AcceptThreadFunc(LPVOID lpParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	HANDLE recvThread = CreateThread(NULL, 0, AcceptThreadFunc, NULL, 0, NULL);

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CloseHandle(recvThread);
	return (int) msg.wParam;
}

DWORD WINAPI AcceptThreadFunc(LPVOID lpParam)
{
	lpClient = new SocketClient();
	if (lpClient && lpClient->Init())
	{
		char buf[4096+3];
		while (1)
		{
			if (!isConnect)
				continue;
			
			Sleep(10);
			memset(buf, 0, sizeof(buf));
			auto rval = ::recv(lpClient->GetSocketID(), buf, sizeof(buf), 0);

			if (rval == SOCKET_ERROR)
			{
				printf("recv socket error\n\n");
				lpClient->Close();
				isConnect = false;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			else if (rval == 0)
			{	//recv����0��ʾ�����˳�
				printf("ending connection\n");
				lpClient->Close();
				isConnect = false;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			else
			{	//��ʾ���յ�������
				for (size_t i = 0; i < 4096 + 3; i++)
				{
					buf[i] ^= key[i % sizeof(key)];
				}
				printf("recv :%s\n", buf);
				//lpClient->MessageDispatch(buf);
				XYStruct xy = lpClient->ParseMsg(buf);
				OnMM_WIM_DATA(xy.msg);
			}
			//MessageBoxA(0, buf, "��ʾ",0);
		}
	}
	return 0;
}

//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  // HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		hBtnStart = CreateWindow(L"Button", L"���ӷ�����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			80, 150, 90, 40, hWnd, (HMENU)BTN_CONNECT, hInst, NULL);
		hBtnClose = CreateWindow(L"Button", L"�ر�����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			245, 150, 90, 40, hWnd, (HMENU)BTN_CLOSE, hInst, NULL);
		hEdit	  = CreateWindow(_T("edit"), _T("192.168.140.58"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE 
			| ES_WANTRETURN | ES_AUTOVSCROLL, 160, 80, 120, 22, hWnd, (HMENU)EDIT, hInst, NULL);

		break;
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
		{	
			Demo(hWnd, message, wParam, lParam);
			break;
		}
		case BTN_CONNECT:
		{
			if (!Demo(hWnd,message,wParam,lParam))
			{
				MessageBoxA(0,"��Ƶ�����豸�쳣��","����",0);
				break;
			}
			char ipAdrr[128];
			GetWindowTextA(hEdit, ipAdrr,128);
			if (!isConnect && lpClient->Connect(ipAdrr, 4001))
			{
				isConnect = true;
				char* str = "socket connect!\n";
				lpClient->SendtoServer(lpClient->GetSocketID(), 1, str);
				InvalidateRect(hWnd,NULL,TRUE);
			}
			break;
		}
		case  BTN_CLOSE:
		{
			lpClient->Close();
			isConnect = false;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		TEXTMETRIC tm;
		// ȡ�����ı���ص�����  
		GetTextMetrics(ps.hdc, &tm);
		RECT rect;
		rect.top = 20;
		rect.bottom = rect.top + tm.tmHeight;
		rect.left = ps.rcPaint.left;
		rect.right = ps.rcPaint.right;
		SetTextColor(ps.hdc, RGB(220, 15, 25));
		if (isConnect)
		{
			DrawText(ps.hdc, L"ͨ���С�����", -1, &rect, DT_CENTER);
		}
		else
		{
			DrawText(ps.hdc, L"δ���ӷ�����������", -1, &rect, DT_CENTER);
		}
		rect.top += 60;
		rect.bottom = rect.top + tm.tmHeight;
		rect.left += 100;
		SetTextColor(ps.hdc, 0);
		DrawText(ps.hdc, L"����IP��", -1, &rect, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (lpClient)
		{
			lpClient->Close();
		}
		PostQuitMessage(0);
		break;
	case MM_WIM_DATA:
	{
	//	OnMM_WIM_DATA(((LPWAVEHDR)lParam)->lpData);
	
		memset(strBuff, 0, sizeof(strBuff));
		memcpy(strBuff, ((LPWAVEHDR)lParam)->lpData, sizeof(strBuff));
		lpClient->SendtoServer(lpClient->GetSocketID(), XYStruct::XYID_SEND_VOICE, strBuff);
		m_pWaveIn->AddBuffer((LPWAVEHDR)lParam);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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
			flag = false;
			if (m_pWaveOut != NULL)
			{
				m_pWaveOut->CloseWaveOut();
			}
			if (m_pWaveIn != NULL)
			{
				m_pWaveIn->CloseWaveIn();
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL Demo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	m_pWaveIn = new GWaveIn;
	if (!m_pWaveIn->IfWaveIn())
	{
		delete m_pWaveIn; m_pWaveIn = NULL;
		return FALSE;
	}
	m_pBuf1 = new BYTE[4096];
	m_pBuf2 = new BYTE[4096];
	if (!m_pWaveIn->PrepareWaveIn(hWnd, m_pBuf1, m_pBuf2) ||
		!m_pWaveIn->AddBuffer(&m_pWaveIn->m_head1) ||
		!m_pWaveIn->AddBuffer(&m_pWaveIn->m_head2))
	{
		delete[] m_pBuf1; m_pBuf1 = NULL;
		delete[] m_pBuf2; m_pBuf2 = NULL;
		delete m_pWaveIn; m_pWaveIn = NULL;
		return FALSE;
	}

	m_pWaveOut = new GWaveOut;
	m_pBuf3 = new BYTE[4096];
	m_pBuf4 = new BYTE[4096];
	if (!m_pWaveOut->IfWaveOut() || !m_pWaveOut->PrepareWaveOut(hWnd, m_pBuf3, m_pBuf4))
	{
		delete[] m_pBuf1; m_pBuf1 = NULL;
		delete[] m_pBuf2; m_pBuf2 = NULL;
		delete[] m_pBuf3; m_pBuf3 = NULL;
		delete[] m_pBuf4; m_pBuf4 = NULL;
		delete m_pWaveIn; m_pWaveIn = NULL;
		delete m_pWaveOut; m_pWaveOut = NULL;
		return FALSE;
	}
	m_pWaveIn->StartRec();

	flag = true;
	return TRUE;
}

LRESULT OnMM_WIM_DATA(LPSTR lParam)
{
	if (iBufNum == 0 || iBufNum == 1)
	{
		memcpy(m_pBuf3, lParam , 4096);
		iBufNum = 2;
		m_pWaveOut->WaveOutWrite(&m_pWaveOut->m_head1);
	}
	else
	{
		memcpy(m_pBuf4, lParam , 4096);
		iBufNum = 1;
		m_pWaveOut->WaveOutWrite(&m_pWaveOut->m_head2);
	}
	return 0;
}