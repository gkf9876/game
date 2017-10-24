#ifndef __CUSTOM_NETWORK_COMMUNICATION_H__
#define __CUSTOM_NETWORK_COMMUNICATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include "cocos2d.h"

USING_NS_CC;

#define BUF_SIZE 1024

class CustomNetworkCommunication
{
private :
	WSADATA wsaData;
	SOCKET sock;
	char message[BUF_SIZE], buf[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	SOCKADDR_IN serv_adr;
	int code;
	char name[50];
	char content[100];

public :
	void init();
	void close();

	void error_handling(char * message);
	int sendCommand(int sock, int code, char * message);
	int readCommand(int sock, int * code, char * buf);
	void chatting(const char * name, const char * content);
	void communication();
};

#endif