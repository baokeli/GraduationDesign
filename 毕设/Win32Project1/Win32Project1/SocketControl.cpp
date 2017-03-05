#include "stdafx.h"
#include "SocketControl.h"

////////////////////////////////////
//客户端socket
SocketClient::SocketClient()
{
}

SocketClient::~SocketClient()
{
}

bool SocketClient::init()
{
	/*初始化socket资源*/
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		printf("socket init error!");
		return false;   //代表失败
	}

	return true;
}

bool Connect(const char *ip, short port)
{

	return false;
}





///////////////////////////////////////
//服务端socket
SocketServer::SocketServer()
{
}
SocketServer::~SocketServer()
{
}
bool SocketServer::init()
{
	/*初始化socket资源*/
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		printf("socket init error!");
		return false;   //代表失败
	}

	m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		printf("socket error!");
		return false;
	}

	return true;
}

bool SocketServer::Listen(short port)
{
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(port);
	m_Addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(m_Socket, (LPSOCKADDR)&m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
	{
		printf("bind error!");
		closesocket(m_Socket);
		return false;
	}

	if (::listen(m_Socket, 5) == SOCKET_ERROR)
	{
		printf("listen error!");
		closesocket(m_Socket);
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
	

	SOCKET newSocket;
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	int rval;
	char buf[1024];
	while (1)
	{
		newSocket = accept(m_Socket, (sockaddr*)&remoteAddr, &nAddrLen);
		if (newSocket == INVALID_SOCKET)
		{
			printf("Failed accept()");
		}
		else
		{
			do
			{
				memset(buf, 0, sizeof(buf));
				rval = recv(newSocket, buf, 1024, 0);

				if (rval == SOCKET_ERROR)
				{
					printf("recv socket error\n");
					continue;
				}
				if (rval == 0)
					//recv返回0表示正常退出
					printf("ending connection");
				else
					//显示接收到的数据
					printf("recv :%s\n", buf);
			} while (rval != 0);

			//关闭对应Accept的socket
			closesocket(newSocket);
		}
	}

	return true;
}