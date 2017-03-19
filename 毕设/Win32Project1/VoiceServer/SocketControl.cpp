//#include "stdafx.h"
#include "SocketControl.h"
#include <stdio.h>
////////////////////////////////////
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !1
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

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

bool SocketClient::Connect(const char *ip, short port)
{
	m_cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_cSocket == 0)
	{
		printf(" Failed socket() \n");
		return false;
	}

	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(port);
	//m_Addr.sin_addr.S_un.S_addr = inet_addr(ip);
	inet_pton(AF_INET, ip, (void*)&m_Addr.sin_addr.S_un.S_addr);

	if (::connect(m_cSocket, (struct sockaddr*)&m_Addr, sizeof(m_Addr)) == -1)
	{
		printf(" Failed connect() \n");
		closesocket(m_cSocket);
		return false;
	}

	return true;
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

	m_sSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sSocket == INVALID_SOCKET)
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

	if (::bind(m_sSocket, (LPSOCKADDR)&m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
	{
		printf("bind error!");
		closesocket(m_sSocket);
		return false;
	}

	if (::listen(m_sSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error!");
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
	

	SOCKET newSocket;
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	int rval;
	char buf[1024];
	while (1)
	{
		printf("Server Start Runing ！");
		newSocket = accept(m_sSocket, (sockaddr*)&remoteAddr, &nAddrLen);
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