#include "stdafx.h"
#include "SocketServer.h"

SocketServer* lpSocketSvr;

int main(int argc, _TCHAR* argv[])
{
	
	HANDLE recvThread = CreateThread(NULL, 0, acceptThread, NULL, 0, NULL);

	while (true)
	{

	}
	CloseHandle(recvThread);
	return 0;
}

DWORD WINAPI acceptThread(LPVOID lpParam)
{
	lpSocketSvr = new SocketServer();
	if (lpSocketSvr && lpSocketSvr->init())
	{
		lpSocketSvr->run(4001);
	}
	return 0;
}
