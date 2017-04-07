// VoiceServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SocketServer.h"

SocketServer* lpSocketSvr;
bool recvLock = false;
bool sendLock = false;

int main(int argc, _TCHAR* argv[])
{	
	lpSocketSvr = new SocketServer();

	HANDLE recvThread1 = CreateThread(NULL, 0, RecvThread1, NULL, 0, NULL);
	HANDLE recvThread2 = CreateThread(NULL, 0, RecvThread2, NULL, 0, NULL);
	HANDLE sendThread = CreateThread(NULL, 0, SendThread, NULL, 0, NULL);
	
	if (lpSocketSvr && lpSocketSvr->Init())
	{
		lpSocketSvr->Run(4001);
	}

	CloseHandle(recvThread1);
	CloseHandle(recvThread2);
	CloseHandle(sendThread);

	return 0;
}

DWORD WINAPI RecvThread1(LPVOID lpParam)
{
	SOCKET resultSocket;
	while (1)
	{
		if (lpSocketSvr->GetSocket(0) != 0)
		{
			resultSocket = lpSocketSvr->GetSocket(0);
			RecvBuff(resultSocket);
		}
		Sleep(10);
	}
	
	return 0;
}

DWORD WINAPI RecvThread2(LPVOID lpParam)
{
	SOCKET resultSocket;
	while (1)
	{
		if (lpSocketSvr->GetSocket(1) != 0)
		{
			resultSocket = lpSocketSvr->GetSocket(1);
			RecvBuff(resultSocket);
		}
		Sleep(10);
	}

	return 0;
}

void RecvBuff(SOCKET resultSocket)
{
	int rval;
	char buf[1024];
	while (1)
	{
		if (resultSocket != 0)
		{
			memset(buf, 0, sizeof(buf));
			rval = recv(resultSocket, buf, 1024, 0);

			if (rval == SOCKET_ERROR)
			{
				printf("recv socket error\n\n");
				break;
			}
			else if (rval == 0)
			{	//recv返回0表示正常退出
				printf("ending connection\n");
				break;
			}
			else
			{	//显示接收到的数据
				//printf("recv :%s\n", buf);
				
				lpSocketSvr->MessageDispatch(resultSocket ,buf);
			}

		}
		Sleep(10);
	}
	closesocket(resultSocket);
	lpSocketSvr->m_nLinkNum--;
	lpSocketSvr->ClearSocket(resultSocket);
}
DWORD WINAPI SendThread(LPVOID lpParam)
{
	int len;
	while (1)
	{
		if (lpSocketSvr->m_qSendQueue.size()>0)
		{	
			auto msg = lpSocketSvr->m_qSendQueue.front();
			len = strlen(msg.sendMsg)+1;
			::send(msg.cSocket, msg.sendMsg, len, 0);
			lpSocketSvr->m_qSendQueue.pop();
		}
		Sleep(10);
	}

	return 0;
}