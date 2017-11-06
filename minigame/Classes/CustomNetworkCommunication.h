#ifndef __CUSTOM_NETWORK_COMMUNICATION_H__
#define __CUSTOM_NETWORK_COMMUNICATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <process.h>

#include "cocos2d.h"
#include "User.h"

USING_NS_CC;

#define BUF_SIZE 1024

#define REQUEST_USER_INFO				1
#define REQUEST_LOGIN					2
#define CHATTING_PROCESS				3
#define USER_MOVE_UPDATE				4
#define OTHER_USER_MAP_MOVE				5

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
	User * mainUser;
	Vector<Node*> usersInfo;

	bool isLogin = false;
	bool isGetUserInfo = false;
	bool popupLoginFail = false;
	char sendBuf[BUF_SIZE];
	char recvBuf[BUF_SIZE];
	Vector<String*> chattingInfo;

	void init();
	void close();
	SOCKET getSock();

	//서버와의 패킷통신 함수
	void error_handling(char * message);
	int sendCommand(int code, char * message);
	int readCommand(int * code, char * buf);

	//채팅함수
	void chatting(const char * name, const char * content);

	//메인 유저 정보 얻는함수
	void getUserInfo();

	//로그인 승낙함수
	void requestLogin(char * userName);

	//메인유저 좌표이동 함수
	void userMoveUpdate(char * userName, Point point, char * field);

	//문자열 변환함수들
	int SeparateString(char * str, char(*arr)[BUF_SIZE], int arrLen, char flag);
	void IntToChar(int value, char * result);
	void CharToInt(char * value, int * result);
	~CustomNetworkCommunication();
};

#endif