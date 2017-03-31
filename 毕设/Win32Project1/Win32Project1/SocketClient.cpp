#include "SocketClient.h"


//客户端socket
SocketClient::SocketClient()
{
}

SocketClient::~SocketClient()
{
}

bool SocketClient::Init()
{
	/*初始化socket资源*/
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		return false;   
	}
	return true;
}

bool SocketClient::Connect(const char *ip, short port)
{
	m_cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_cSocket == 0)
		return false;

	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, (void*)&m_Addr.sin_addr.S_un.S_addr);

	if (::connect(m_cSocket, (struct sockaddr*)&m_Addr, sizeof(m_Addr)) == -1)
	{
		closesocket(m_cSocket);
		return false;
	}

	return true;
}

SOCKET SocketClient::GetSocketID()
{
	return m_cSocket;
}

void SocketClient::MessageDispatch(char* msg)
{
	XYStruct xy = ParseMsg(msg);
	switch (xy.xyid)
	{
	case XYStruct::XYID_CONNECT:
	{
		break;
	}
	case XYStruct::XYID_SEND_VOICE:
	{
		break;
	}
	default:
		break;
	}
	
}

XYStruct SocketClient::ParseMsg(char * msg)
{
	// XYID + MSGLEN + MSG
	// 2    +   2    + MSGLEN 
	XYStruct xy;
	if (strlen(msg)>=4)
	{
		xy.xyid = msg[0] * 10 + msg[1];
		xy.msgLen = msg[2] * 10 + msg[3];
		xy.msg = msg + 4;
	}
	return xy;
}
