#ifndef __CUSTOM_NETWORK_COMMUNICATION_H__
#define __CUSTOM_NETWORK_COMMUNICATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#include <process.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#endif

#include "cocos2d.h"
#include "User.h"
#include "CustomObject.h"

USING_NS_CC;

#define BUF_SIZE 1024

#define REQUEST_USER_INFO				1
#define REQUEST_LOGIN					2
#define CHATTING_PROCESS				3
#define USER_MOVE_UPDATE				4
#define OTHER_USER_MAP_MOVE				5
#define REQUEST_JOIN					6
#define UPDATE_LOGIN_TIME				7
#define REQUEST_TILED_MAP				8
#define REQUEST_IMAGE					9
#define DELETE_FIELD_ITEM				10
#define REQUEST_FIELD_INFO				11

class CustomNetworkCommunication
{
private :
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serv_adr;

	HANDLE hSndThread;
	HANDLE hRcvThread;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int sock;
	struct sockaddr_in serv_adr;

	pthread_t hSndThread;
	pthread_t hRcvThread;
#endif

	struct hostent * host;
	int str_len, recv_len, recv_cnt;
	int code;
	char name[50];
	char content[100];

public :
	User * mainUser = NULL;
	std::vector<User*> * usersInfo = NULL;								//현재 맵의 다른 유저들
	std::vector<CustomObject*> * objectInfo = NULL;						//현재 맵의 오브젝트

	std::vector<byte> *tiledMapBuf = NULL;
	std::vector<byte> *imageBuf = NULL;

	//서버 알람 이력
	bool isLogin = false;
	bool isGetUserInfo = false;
	bool popupLoginFail = false;
	bool comm = true;
	bool getTiledMap = false;
	bool getImage = false;
	bool changeTiledMapObject = false;					//현재 필드의 오브젝트 변동사항

	//현재필드의 아이템 변동사항이 존재할시 변동내용
	char itemUser[BUF_SIZE];
	char itemName[BUF_SIZE];
	int itemXpos;
	int itemYpos;
	int itemOrder;

	//현재필드에 존재하는 오브젝트를 실제 화면에 반영하기위해 임시로 저장해놓은 곳
	int objectIdx;
	char objectName[50];
	char objectType[50];
	int objectXpos;
	int objectYpos;
	int objectOrder;
	char objectFileDir[100];
	int objectCount;
	bool isObjectBufferFill = false;

	//0 : 대기, 1 : 승인, -1 : 불허
	int permissionJoin = 0;
	char sendBuf[BUF_SIZE];
	char recvBuf[100000];
	Vector<String*> chattingInfo;

	void init();
	void sockClose();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SOCKET getSock();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int getSock();
#endif

	//서버와의 패킷통신 함수
	void error_handling(char * message);
	int sendCommand(int code, char * message, int size);
	int readCommand(int * code, char * buf);

	//채팅함수
	int chatting(const char * name, const char * content, const char * field);

	//메인 유저 정보 얻는함수
	int getUserInfo();

	//로그인 승낙함수
	int requestLogin(char * userName);

	//유저 좌표이동 함수
	int userMoveUpdate(char * userName, Point fromPoint, char * from, Point toPoint, char * to, cocos2d::EventKeyboard::KeyCode seeDirection);

	//유저 회원가입 요청 함수
	int requestJoin(char * userName);

	//접속시간 업데이트 함수
	int updateLoginTime(char * userName);

	//유저가 땅에 떨어진 아이템을 먹을시 서버에 알리는 함수
	int eatFieldItem(char * itemName, int xpos, int ypos, int order);

	//문자열 변환함수들
	int SeparateString(char * str, char(*arr)[BUF_SIZE], int arrLen, char flag);
	void IntToChar(int value, char * result);
	void CharToInt(char * value, int * result);
	void MyPrintDebug(char * message);
	~CustomNetworkCommunication();
};

#endif
