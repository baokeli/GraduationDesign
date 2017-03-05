#pragma once
//#include "stdafx.h"

class SocketClient
{
public:
	SocketClient();
	~SocketClient();
	bool init();
	bool Connect(const char *ip, short port);

private:
	WSADATA wsa;
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
	SOCKET m_Socket;
	sockaddr_in m_Addr;
	WSADATA wsa;
};
