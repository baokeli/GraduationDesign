#include "SocketServer.h"
#include <stdio.h>
///////////////////////////////////////

//服务端socket
SocketServer::SocketServer()
	:m_nLinkNum(0),
	m_sSocket(0)
{
	memset(&m_Addr, 0, sizeof(m_Addr));
	memset(&wsa, 0, sizeof(wsa));
	memset(m_SocketArr, 0, sizeof(m_SocketArr));
}

SocketServer::~SocketServer()
{
}

bool SocketServer::Init()
{
	/*初始化socket资源*/
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		printf("socket init error!\n");
		return false;   //代表失败
	}

	m_sSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sSocket == INVALID_SOCKET)
	{
		printf("socket error!\n");
		return false;
	}

	return true;
}

bool SocketServer::Listen(short port)
{
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(port);
	m_Addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(m_sSocket, (LPSOCKADDR)&m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
	{
		printf("bind error!\n");
		closesocket(m_sSocket);
		return false;
	}

	if (::listen(m_sSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error!\n");
		closesocket(m_sSocket);
		return false;
	}

	return true;
}

bool SocketServer::Run(short port)
{
	if (!Listen(port))
	{
		return false;
	}

	SOCKET resultSocket;
	int nAddrLen = sizeof(m_Addr);

	printf("Server Start Runing ！\n");
	while (1)
	{
		resultSocket = accept(m_sSocket, (sockaddr*)&m_Addr, &nAddrLen);
		if (resultSocket == INVALID_SOCKET)
		{
			printf("Failed accept()\n");
		}
		else
		{
			printf("Succeed accept()\n");
			SaveSocket(resultSocket);
			m_nLinkNum++;
		}
		Sleep(1);
	}

	return true;
}

SOCKET SocketServer::GetSvrSocket()
{
	return m_sSocket;
}

SOCKET SocketServer::GetSocket(int index)
{
	if (index < 0 || index > 1)
		return GetSvrSocket();
	else
		return m_SocketArr[index];
}

void SocketServer::SaveSocket(SOCKET socket)
{
	for (int i = 0; i < 2; i++)
	{
		if (m_SocketArr[i] == 0)
		{
			m_SocketArr[i] = socket;
			break;
		}
	}
}

bool SocketServer::ClearSocket(SOCKET socket)
{
	for (int i = 0; i < 2; i++)
	{
		if (m_SocketArr[i] == socket)
		{
			m_SocketArr[i] = 0;
			return true;
		}
	}
	return false;
}

int SocketServer::SendtoClient(SOCKET socket, char* buf)
{
	SendInfo sendInfo;
	sendInfo.cSocket = socket;
	sendInfo.sendMsg = buf;
	m_qSendQueue.push(sendInfo);
	return 0;
}

void SocketServer::MessageDispatch(SOCKET socket ,char * msg)
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
		SendtoClient(socket, msg);
		break;
	}
	default:
		break;
	}
}


XYStruct SocketServer::ParseMsg(char * msg)
{
	// XYID + MSGLEN + MSG
	// 2    +   2    + MSGLEN 
	XYStruct xy;
	if (strlen(msg) >= 4)
	{
		xy.xyid = msg[0] + msg[1];
		xy.msgLen = msg[2] * 10 + msg[3];
		xy.msg = msg + 4;
	}
	return xy;
}
