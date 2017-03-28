#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

class SocketClient
{
public:
	SocketClient();
	~SocketClient();
	bool init();
	bool Connect(const char *ip, short port);
	SOCKET getSocketID();

private:
	WSADATA wsa;
	sockaddr_in m_Addr;
	SOCKET m_cSocket;
	SOCKET m_svrSocket;
};
