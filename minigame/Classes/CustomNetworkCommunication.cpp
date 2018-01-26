#include "CustomNetworkCommunication.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
unsigned WINAPI SendMsg(void * arg)   // send thread main
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void * SendMsg(void * arg)
#endif
{
	CustomNetworkCommunication * com = ((CustomNetworkCommunication*)arg);

	while (1)
	{
		if (com->isLogin == true && com->isGetUserInfo == true)
		{
			//일정 주기로 접속시간 업데이트
			if (com->updateLoginTime(com->mainUser->name) <= 0)
			{
				com->comm = false;
			}
		}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Sleep(5000);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		sleep(5000);
#endif
	}

	return 0;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
unsigned WINAPI RecvMsg(void * arg)   // read thread main
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void * RecvMsg(void * arg)
#endif
{
	CustomNetworkCommunication * com = ((CustomNetworkCommunication*)arg);
	int code;
	int str_len;
	char buf[50][BUF_SIZE];
	User * user;
	char message[100];

	while (1)
	{
		str_len = com->readCommand(&code, com->recvBuf);

		if (str_len == -1)
		{
			com->comm = false;
			return NULL;
		}

		switch (code)
		{
		case REQUEST_USER_INFO:
			com->SeparateString(com->recvBuf, buf, 50, '\n');
			strcpy(com->mainUser->name, buf[0]);
			com->mainUser->xpos = atoi(buf[1]);
			com->mainUser->ypos = atoi(buf[2]);
			strcpy(com->mainUser->field, buf[3]);
			com->mainUser->seeDirection = (cocos2d::EventKeyboard::KeyCode)atoi(buf[4]);
			com->isGetUserInfo = true;
			break;
		case CHATTING_PROCESS:
			com->SeparateString(com->recvBuf, buf, 50, '\n');
			com->chattingInfo.pushBack(String::createWithFormat("%s : %s", buf[0], buf[1]));

			//다른 사람들이 채팅하면 그 사람의 말풍선에 대화내용을 넣어준다.
			for (int i = 0; i < com->usersInfo->size(); i++)
			{
				User * othersUser = com->usersInfo->at(i);

				if (!strcmp(othersUser->name, buf[0]))
				{
					//캐릭터 위에 말풍선으로 문자열 출력.

					String * message = String::createWithFormat("%s", buf[1]);

					othersUser->balloon->setVisible(true);
					othersUser->balloonContent->setString(message->getCString());
					othersUser->balloonContent->setVisible(true);
					othersUser->balloonTime = 0;
				}
			}
			break;
		case REQUEST_LOGIN:
			if (!strcmp(com->recvBuf, "login okey"))
			{
				com->isLogin = true;
			}
			else
			{
				com->isLogin = false;
				com->popupLoginFail = true;
			}
			break;
		case USER_MOVE_UPDATE:
			break;
		case OTHER_USER_MAP_MOVE:
			com->SeparateString(com->recvBuf, buf, 50, '\n');

			if (!strcmp(buf[0], "out"))
			{
				user = new User();

				strcpy(user->name, buf[1]);
				user->xpos = atoi(buf[2]);
				user->ypos = atoi(buf[3]);

				for (int i = 0; i < com->usersInfo->size(); i++)
				{
					User * othersUser = com->usersInfo->at(i);

					if (!strcmp(othersUser->name, user->name))
					{
						othersUser->sprite->setVisible(false);
						othersUser->balloon->setVisible(false);
						othersUser->balloonContent->setVisible(false);
						break;
					}
				}
			}
			else if (!strcmp(buf[0], "in"))
			{
				//맵에 진입한 다른 유저를 구현
				user = new User();
				strcpy(user->name, buf[1]);
				user->xpos = atoi(buf[2]);
				user->ypos = atoi(buf[3]);
				user->seeDirection = (cocos2d::EventKeyboard::KeyCode)atoi(buf[4]);
				user->sprite = NULL;

				com->usersInfo->push_back(user);
			}
			else if (!strcmp(buf[0], "move"))
			{
				user = new User();

				strcpy(user->name, buf[1]);
				user->xpos = atoi(buf[2]);
				user->ypos = atoi(buf[3]);
				user->seeDirection = (cocos2d::EventKeyboard::KeyCode)atoi(buf[4]);

				for (int i = 0; i < com->usersInfo->size(); i++)
				{
					User * othersUser = (User*)com->usersInfo->at(i);

					if (!strcmp(othersUser->name, user->name))
					{
						if (othersUser->xpos != user->xpos || othersUser->ypos != user->ypos)
						{
							//이동
							othersUser->isAction = true;
							othersUser->isRunning = true;
						}
						else
						{
							//제자리 방향회전
							othersUser->isAction = true;
							othersUser->isRunning = false;
						}

						othersUser->seeDirection = user->seeDirection;
						othersUser->xpos = user->xpos;
						othersUser->ypos = user->ypos;
						othersUser->position = Point(user->xpos * 32 + 32 / 2, user->ypos * 32);
						othersUser->sprite->setPosition(othersUser->position);

						//말풍선은 항상 캐릭터를 따라다녀야함
						othersUser->balloon->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2, othersUser->position.y + othersUser->sprite->getContentSize().height));
						othersUser->balloonContent->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2 - 50, othersUser->position.y + othersUser->sprite->getContentSize().height + 50));
						break;
					}
				}
			}
			break;
		case REQUEST_JOIN:
			if (!strcmp(com->recvBuf, "join okey"))
			{
				com->permissionJoin = 1;
			}
			else
			{
				com->permissionJoin = -1;
			}
			break;
		case REQUEST_TILED_MAP:
			//맵 정보를 가져온다.
			com->tiledMapBuf = new std::vector<byte>();
			for (int i = 0; i < str_len; i++)
			{
				com->tiledMapBuf->push_back((byte)com->recvBuf[i]);
			}
			com->getTiledMap = true;
			break;
		case REQUEST_IMAGE:
			//이미지 정보를 가져온다.
			com->imageBuf = new std::vector<byte>();
			for (int i = 0; i < str_len; i++)
			{
				com->imageBuf->push_back((byte)com->recvBuf[i]);
			}
			com->getImage = true;
			break;
		case DELETE_FIELD_ITEM:
			//지워진 오브젝트 정보를 저장.
			memcpy(&com->deleteCustomObject, com->recvBuf, sizeof(StructCustomObject));

			//현재 필드의 오브젝트 변동사항 알림
			com->isDeleteMapObject = true;
			break;
		case REQUEST_FIELD_INFO:
			int count;
			memcpy(&count, &com->recvBuf[0], 4);
			for (int i = 0; i < count; i++)
			{
				StructCustomObject imsiStructCustomObject;
				memcpy(&imsiStructCustomObject, &com->recvBuf[4 + i * sizeof(StructCustomObject)], sizeof(StructCustomObject));
				CustomObject * customObject = new CustomObject(imsiStructCustomObject);
				com->objectInfo->push_back(customObject);
			}
			com->isObjectBufferFill = true;
			break;
		default:
			break;
		}
	}

	return 0;
}

void CustomNetworkCommunication::init()
{
	this->mainUser = new User();
	this->usersInfo = new std::vector<User*>();
	this->objectInfo = new std::vector<CustomObject*>();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif

	sock = socket(PF_INET, SOCK_STREAM, 0);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (sock == INVALID_SOCKET)
		error_handling("socket() error");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (sock == -1)
        error_handling("socket() error");
#endif

	//host = gethostbyname("192.168.56.102");
	host = gethostbyname("sourcecake.iptime.org");
	if (!host)
		error_handling("gethost... error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
	serv_adr.sin_port = htons(atoi("9190"));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
	{
		error_handling("connect() error!");
		comm = false;
	}
	else
	{
		puts("Connected...............");
		comm = true;
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...............");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// 수시로 서버와 송수신할때 사용할 쓰레드
	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)this, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)this, 0, NULL);

	WaitForSingleObject(hSndThread, 1000);
	WaitForSingleObject(hRcvThread, 1000);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (pthread_create(&hSndThread, NULL, SendMsg, (void*)this) != 0)
	{
		puts("pthread_create() error : hSndThread");
		return;
	}
	if (pthread_create(&hRcvThread, NULL, RecvMsg, (void*)this) != 0)
	{
		puts("pthread_create() error : hRcvThread");
		return;
	}
#endif
}

void CustomNetworkCommunication::sockClose()
{
	usersInfo->clear();
	if(tiledMapBuf != NULL)
		delete tiledMapBuf;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	shutdown(sock, SHUT_WR);
	close(sock);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
SOCKET CustomNetworkCommunication::getSock()
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
int CustomNetworkCommunication::getSock()
#endif
{
	return this->sock;
}

void CustomNetworkCommunication::error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int CustomNetworkCommunication::sendCommand(int code, char * message, int size)
{
	char * buf;
	int writeLen;

	if (message == NULL)
		return -1;

	buf = new char[8];
	IntToChar(size, &buf[0]);
	IntToChar(code, &buf[4]);

	writeLen = send(this->sock, buf, 8, 0);
	writeLen += send(this->sock, message, size, 0);

	delete[] buf;

	if (writeLen < 0)
		return -1;
	else
		return writeLen;
}

int CustomNetworkCommunication::readCommand(int * code, char * buf)
{
	int len;
	int readLen;
	int size = 0;

	if (buf == NULL)
		return -1;

	if (recv(this->sock, buf, 4, 0) == -1)
		return -1;

	CharToInt(&buf[0], &len);

	if (recv(this->sock, buf, 4, 0) == -1)
		return -1;

	CharToInt(&buf[0], code);

	if (len > (100000 - 8))
		return 8;

	//버퍼가 완전히 비워질때까지 받는다.
	while (1)
	{
		readLen = recv(this->sock, &buf[size], len, 0);

		if (readLen == -1)
			return -1;
		size += readLen;
		if (size >= len || readLen == 0)
			break;
	}
	buf[size] = 0;

	return size + 1;
}

int CustomNetworkCommunication::chatting(const char * name, const char * content, const char * field)
{
	String * message = String::createWithFormat("%s\n%s\n%s\n", name, content, field);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(CHATTING_PROCESS, this->sendBuf, strlen(this->sendBuf));

	return str_len;
}

int CustomNetworkCommunication::getUserInfo()
{
	String * message = String::createWithFormat("%d", sock);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(REQUEST_USER_INFO, "request user information", strlen("request user information"));

	return str_len;
}

int CustomNetworkCommunication::requestLogin(char * userName)
{
	String * message = String::createWithFormat("%s", userName);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(REQUEST_LOGIN, this->sendBuf, strlen(this->sendBuf));

	return str_len;
}

int CustomNetworkCommunication::userMoveUpdate(char * userName, Point fromPoint, char * from, Point toPoint, char * to, cocos2d::EventKeyboard::KeyCode seeDirection)
{
	sprintf(this->sendBuf, "%s\n%d\n%d\n%s\n%d\n%d\n%s\n%d", userName, (int)fromPoint.x, (int)fromPoint.y, from, (int)toPoint.x, (int)toPoint.y, to, seeDirection);

	//맵 이동시 현재 유저목록 초기화
	if(strcmp(from, to))
		usersInfo->clear();

	str_len = sendCommand(USER_MOVE_UPDATE, this->sendBuf, strlen(this->sendBuf));

	return str_len;
}

int CustomNetworkCommunication::requestJoin(char * userName)
{
	String * message = String::createWithFormat("%s", userName);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(REQUEST_JOIN, this->sendBuf, strlen(this->sendBuf));

	return str_len;
}

int CustomNetworkCommunication::updateLoginTime(char * userName)
{
	String * message = String::createWithFormat("%s", userName);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(UPDATE_LOGIN_TIME, this->sendBuf, strlen(this->sendBuf));

	return str_len;
}

//유저가 땅에 떨어진 아이템을 먹을시 서버에 알리는 함수
int CustomNetworkCommunication::eatFieldItem(StructCustomObject structCustomObject)
{
	char * message = new char[sizeof(StructCustomObject) + 1];
	memcpy(message, &structCustomObject, sizeof(StructCustomObject));

	str_len = sendCommand(DELETE_FIELD_ITEM, message, sizeof(StructCustomObject));

	delete message;

	return str_len;
}

int CustomNetworkCommunication::SeparateString(char * str, char(*arr)[BUF_SIZE], int arrLen, char flag)
{
	char imsi[BUF_SIZE];
	char strarr[BUF_SIZE];
	int count = 0;
	int j = 0;

	strcpy(imsi, str);

	for (int i = 0; i < strlen(imsi) + 1; i++)
	{
		if (imsi[i] == flag)
		{
			strarr[j] = 0;
			if (count >= arrLen)
				break;
			else
				strcpy(arr[count++], strarr);
			j = 0;
			continue;
		}
		else if (i == strlen(imsi))
		{
			strarr[j] = 0;
			if (count >= arrLen)
				break;
			else
				strcpy(arr[count++], strarr);
			break;
		}

		strarr[j] = imsi[i];
		j++;
	}

	return count;
}

void CustomNetworkCommunication::IntToChar(int value, char * result)
{
	result[0] = (value & 0xff000000) >> 24;
	result[1] = (value & 0x00ff0000) >> 16;
	result[2] = (value & 0x0000ff00) >> 8;
	result[3] = (value & 0x000000ff);
}
void CustomNetworkCommunication::CharToInt(char * value, int * result)
{
	*result = (int)(((unsigned char)value[0] << 24) & 0xff000000);
	*result += (int)(((unsigned char)value[1] << 16) & 0x00ff0000);
	*result += (int)(((unsigned char)value[2] << 8) & 0x0000ff00);
	*result += (unsigned char)value[3];
}

void CustomNetworkCommunication::MyPrintDebug(char * message)
{
	wchar_t pszCharacterString[100000];

	int nLen = (int)strlen(message) + 1;
	mbstowcs(pszCharacterString, message, nLen);

	OutputDebugString(pszCharacterString);
}

CustomNetworkCommunication::~CustomNetworkCommunication()
{
	if(mainUser != NULL)
		delete mainUser;
	if (usersInfo != NULL)
		delete usersInfo;
	if (objectInfo != NULL)
		delete objectInfo;
	if(tiledMapBuf != NULL)
		delete tiledMapBuf;
	if(imageBuf != NULL)
		delete imageBuf;
}
