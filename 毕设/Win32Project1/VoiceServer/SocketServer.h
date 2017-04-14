#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <queue>
#include"XY.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "Ws2_32.lib")



struct SendInfo
{
	SOCKET cSocket;
	char* sendMsg;
};


class SocketServer
{
public:
	SocketServer();
	~SocketServer();
	bool Init();
	bool Listen(short port);
	bool Run(short port);

	SOCKET GetSvrSocket();
	SOCKET GetSocket(int index);
	SOCKET GetOtherSocket(SOCKET socket);
	void SaveSocket(SOCKET socket);
	bool ClearSocket(SOCKET socket);
	int SendtoClient(SOCKET socket, int xyid, char* buf);
	void MessageDispatch(SOCKET socket ,char * msg);
	XYStruct ParseMsg(char * msg);

private:
	SOCKET m_sSocket;
	sockaddr_in m_Addr;
	WSADATA wsa;
	SOCKET m_SocketArr[2];

public:
	int m_nLinkNum;			//socket服务连接数
	std::queue<SendInfo> m_qSendQueue;		//发送队列
};

DWORD WINAPI RecvThread1(LPVOID lpParam);
DWORD WINAPI RecvThread2(LPVOID lpParam);
DWORD WINAPI SendThread(LPVOID lpParam);
void RecvBuff(SOCKET resultSocket);