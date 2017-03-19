#pragma once
//#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

class SocketClient
{
public:
	SocketClient();
	~SocketClient();
	bool init();
	bool Connect(const char *ip, short port);

private:
	WSADATA wsa;
	sockaddr_in m_Addr;
	SOCKET m_cSocket;
};

class SocketServer
{
public:
	SocketServer();
	~SocketServer();
	bool init();
	bool Listen(short port);
	bool run(short port);
private:
	SOCKET m_sSocket;
	sockaddr_in m_Addr;
	WSADATA wsa;
};
