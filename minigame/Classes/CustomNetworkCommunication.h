#ifndef __CUSTOM_NETWORK_COMMUNICATION_H__
#define __CUSTOM_NETWORK_COMMUNICATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <process.h>

#include "cocos2d.h"

USING_NS_CC;

#define BUF_SIZE 1024

typedef struct user
{
	int sock;
	char name[50];
	char password[50];
	int xpos;
	int ypos;
	int field;
}User;

class CustomNetworkCommunication
{
private :
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serv_adr;
	struct hostent * host;
	int str_len, recv_len, recv_cnt;
	int code;
	char name[50];
	char content[100];
	HANDLE hSndThread;
	HANDLE hRcvThread;

public :
	User user;
	char sendBuf[BUF_SIZE];
	char recvBuf[BUF_SIZE];
	Vector<String*> chattingInfo;

	void init();
	void close();
	SOCKET getSock();

	void error_handling(char * message);
	int sendCommand(int code, char * message);
	int readCommand(int * code, char * buf);

	void chatting(const char * name, const char * content);
	void getUserInfo();

	int SeparateString(char * str, char(*arr)[BUF_SIZE], int arrLen, char flag);
};

#endif