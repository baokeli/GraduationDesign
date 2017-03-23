#include "SocketServer.h"
#include <stdio.h>

///////////////////////////////////////
//�����socket
SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{
}

bool SocketServer::init()
{
	/*��ʼ��socket��Դ*/
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		printf("socket init error!\n");
		return false;   //����ʧ��
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
//	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(m_Addr);
	int rval;
	char buf[1024];
	while (1)
	{
		printf("Server Start Runing ��\n");
		resultSocket = accept(m_sSocket, (sockaddr*)&m_Addr, &nAddrLen);
		if (resultSocket == INVALID_SOCKET)
		{
			printf("Failed accept()");
		}
		else
		{
			do
			{
				memset(buf, 0, sizeof(buf));
				rval = recv(resultSocket, buf, 1024, 0);

				if (rval == SOCKET_ERROR)
				{
					printf("recv socket error\n\n");
					continue;
				}
				else if (rval == 0)
					//recv����0��ʾ�����˳�
					printf("ending connection\n");
				else
				{	//��ʾ���յ�������
					printf("recv :%s\n", buf);
					::send(m_sSocket, "�յ���", 8,0);
				}
			} while (rval != 0);

			//�رն�ӦAccept��socket
			closesocket(resultSocket);
		}
	}

	return true;
}

SOCKET SocketServer::getSocket()
{
	return m_sSocket;
}