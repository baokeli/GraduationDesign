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

bool SocketServer::init()
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

bool SocketServer::run(short port)
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
			saveSocket(resultSocket);
			m_nLinkNum++;
		}
	}

	return true;
}

SOCKET SocketServer::getSvrSocket()
{
	return m_sSocket;
}

SOCKET SocketServer::getSocket(int index)
{
	if (index < 0 || index > 1)
		return getSvrSocket();
	else
		return m_SocketArr[index];
}

void SocketServer::saveSocket(SOCKET socket)
{
	for (int i=0;i<2;i++)
	{
		if (m_SocketArr[i] == 0)
		{
			m_SocketArr[i] = socket;
			break;
		}
	}
}

bool SocketServer::clearSocket(SOCKET socket)
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

int SocketServer::SendtoClient(SOCKET socket,char* buf)
{
	SendInfo sendInfo;
	sendInfo.cSocket = socket;
	sendInfo.sendMsg = buf;
	m_qSendQueue.push(sendInfo);
	return 0;
}
