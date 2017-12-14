#include "CustomNetworkCommunication.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
unsigned WINAPI SendMsg(void * arg)   // send thread main
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void * SendMsg(void * arg)
#endif
{
	//while (1)
	//{
		//CCLOG("SendMsg");
		//Sleep(1000);
	//}

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

	while (1)
	{
		str_len = com->readCommand(&code, com->recvBuf);

		if (str_len == -1)
			return NULL;

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
				CCLOG("Login Okey!!");
			}
			else
			{
				com->isLogin = false;
				CCLOG("Login Fail!!");
				com->popupLoginFail = true;
			}
			break;
		case USER_MOVE_UPDATE:
			CCLOG("user move!!");
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
					othersUser->sprite->setVisible(false);
					othersUser->balloon->setVisible(false);
					othersUser->balloonContent->setVisible(false);

					if (!strcmp(othersUser->name, user->name))
					{
						com->usersInfo->erase(com->usersInfo->begin() + i);
						CCLOG("User : %s OUT! (%d, %d)", user->name, user->xpos, user->ypos);
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
				//알 수 없는 오류
				//CCLOG("User : %s IN! (%d, %d)", user->name, user->xpos, user->ypos);
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
						//제자리일시 움직이는 모션을 생략한다.
						if (othersUser->xpos != user->xpos || othersUser->ypos != user->ypos)
							othersUser->isAction = true;

						othersUser->seeDirection = user->seeDirection;
						othersUser->xpos = user->xpos;
						othersUser->ypos = user->ypos;
						othersUser->position = Point(user->xpos * 32 + 32/2, user->ypos * 32);
						othersUser->sprite->setPosition(othersUser->position);

						//말풍선은 항상 캐릭터를 따라다녀야함
						othersUser->balloon->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2, othersUser->position.y + othersUser->sprite->getContentSize().height));
						othersUser->balloonContent->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2 - 50, othersUser->position.y + othersUser->sprite->getContentSize().height + 50));

						CCLOG("User : %s MOVE! (%d, %d)", user->name, user->xpos, user->ypos);
						break;
					}
				}
			}
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

	host = gethostbyname("sourcecake.iptime.org");
	if (!host)
		error_handling("gethost... error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
	serv_adr.sin_port = htons(atoi("9190"));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
		error_handling("connect() error!");
	else
		puts("Connected...............");
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

int CustomNetworkCommunication::sendCommand(int code, char * message)
{
	if (message == NULL)
		return -1;

	char buf[BUF_SIZE];
	int len = strlen(message);

	IntToChar(len, &buf[0]);
	IntToChar(code, &buf[4]);

	strcpy(&buf[8], message);

	int writeLen = send(this->sock, buf, len + 8, 0);

	if (writeLen == -1)
		return -1;
	else
		return len;

	CCLOG("Code : %d, Content : %s", code, message);
}

int CustomNetworkCommunication::readCommand(int * code, char * buf)
{
	int len;

	if (buf == NULL)
		return -1;

	int readLen = recv(this->sock, buf, 4, 0);

	if (readLen == -1)
		return -1;
	else if (readLen == 0)
		return 0;

	CharToInt(&buf[0], &len);

	readLen = recv(this->sock, buf, 4, 0);

	if (readLen == -1)
		return -1;

	CharToInt(&buf[0], code);

	recv(this->sock, buf, len, 0);
	buf[len] = 0;

	return len;
}

void CustomNetworkCommunication::chatting(const char * name, const char * content)
{
	String * message = String::createWithFormat("%s\n%s\n", name, content);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(CHATTING_PROCESS, this->sendBuf);
}

void CustomNetworkCommunication::getUserInfo()
{
	String * message = String::createWithFormat("%d", sock);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(REQUEST_USER_INFO, "request user information");
}

void CustomNetworkCommunication::requestLogin(char * userName)
{
	String * message = String::createWithFormat("%s", userName);

	strcpy(this->sendBuf, message->getCString());

	str_len = sendCommand(REQUEST_LOGIN, this->sendBuf);
}

void CustomNetworkCommunication::userMoveUpdate(char * userName, Point fromPoint, char * from, Point toPoint, char * to, cocos2d::EventKeyboard::KeyCode seeDirection)
{
	sprintf(this->sendBuf, "%s\n%d\n%d\n%s\n%d\n%d\n%s\n%d", userName, (int)fromPoint.x, (int)fromPoint.y, from, (int)toPoint.x, (int)toPoint.y, to, seeDirection);
	CCLOG("%s %d %d %s %d %d %s", userName, (int)fromPoint.x, (int)fromPoint.y, from, (int)toPoint.x, (int)toPoint.y, to);

	//맵 이동시 현재 유저목록 초기화
	if(strcmp(from, to))
		usersInfo->clear();

	str_len = sendCommand(USER_MOVE_UPDATE, this->sendBuf);
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


CustomNetworkCommunication::~CustomNetworkCommunication()
{
	if(mainUser != NULL)
		delete mainUser;
}
