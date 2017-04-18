#include "SocketClient.h"
#include "string"

//客户端socket
SocketClient::SocketClient():
m_cSocket(0)
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
void SocketClient::Close()
{
	closesocket(m_cSocket);
	m_cSocket = NULL;
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
	// 1    +   2    + MSGLEN 
	XYStruct xy;
	xy.xyid = msg[0];
	xy.msgLen = msg[1] * 10 + msg[2];
	memcpy(xy.msg, msg + 3, 4096 );

	return xy;
}

void SocketClient::SendtoServer(SOCKET socket, int xyid, char* buff)
{
	char str[4096 +3];
	int len = sizeof(str)-3;
	str[0] = xyid;
	str[1] =1 ;
	str[2] =1 ;
	memcpy(str+3,buff,len);
	::send(socket, str, len+3, 0);
}