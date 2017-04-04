#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include"XY.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

class SocketClient
{
public:
	SocketClient();
	~SocketClient();


	bool Init();
	bool Connect(const char *ip, short port);
	SOCKET GetSocketID();
	void MessageDispatch(char * msg);
	XYStruct ParseMsg(char* msg);
	void SendtoServer(SOCKET socket,int xyid,char* buff);
private:
	WSADATA wsa;
	sockaddr_in m_Addr;
	SOCKET m_cSocket;
	SOCKET m_svrSocket;
};
