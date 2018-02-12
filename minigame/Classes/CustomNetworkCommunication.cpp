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
				com->MyPrintDebug("updateLoginTime error\n");
				com->comm = false;
			}
		}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Sleep(5000);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		sleep(5);
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
	int readLen;
	char buf[50][BUF_SIZE];
	User * user;
	char message[100];

	while (1)
	{
		readLen = com->readCommand(&code, com->recvBuf);

		if (readLen == -1)
		{
			com->comm = false;
			return NULL;
		}

		switch (code)
		{
		case REQUEST_USER_INFO:
			{
				StructCustomUser structCustomUser;
				memcpy(&structCustomUser, com->recvBuf, sizeof(StructCustomUser));
				com->mainUser->setUser(structCustomUser);
				com->isGetUserInfo = true;
			}
			break;
		case CHATTING_PROCESS:
			{
				StructCustomChatting structCustomChatting;

				memcpy(&structCustomChatting, com->recvBuf, sizeof(StructCustomChatting));
				com->chattingInfo.pushBack(String::createWithFormat("%s : %s", structCustomChatting.name, structCustomChatting.content));

				//다른 사람들이 채팅하면 그 사람의 말풍선에 대화내용을 넣어준다.
				for (int i = 0; i < com->usersInfo->size(); i++)
				{
					User * othersUser = com->usersInfo->at(i);

					if (!strcmp(othersUser->name, structCustomChatting.name))
					{
						//캐릭터 위에 말풍선으로 문자열 출력.

						String * message = String::createWithFormat("%s", structCustomChatting.content);

						othersUser->balloon->setVisible(true);
						othersUser->balloonContent->setString(message->getCString());
						othersUser->balloonContent->setVisible(true);
						othersUser->balloonTime = 0;
					}
				}
			}
			break;
		case REQUEST_LOGIN:
			if (!strcmp(com->recvBuf, "login okey"))
				com->isLogin = true;
			else
			{
				com->isLogin = false;
				com->popupLoginFail = true;
			}
			break;
		case USER_MOVE_UPDATE:
			break;
		case OTHER_USER_MAP_MOVE:
			{
				StructCustomUser moveUser;
				memcpy(&moveUser, com->recvBuf, sizeof(StructCustomUser));

				if (moveUser.action == ACTION_MAP_OUT)
				{
					for (int i = 0; i < com->usersInfo->size(); i++)
					{
						User * othersUser = com->usersInfo->at(i);

						if (!strcmp(othersUser->name, moveUser.name))
						{
							othersUser->sprite->setVisible(false);
							othersUser->balloon->setVisible(false);
							othersUser->balloonContent->setVisible(false);
							break;
						}
					}
				}
				else if (moveUser.action == ACTION_MAP_IN)
				{
					//맵에 진입한 다른 유저를 구현
					user = new User(moveUser);
					user->seeDirection = (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection;
					user->sprite = NULL;

					com->usersInfo->push_back(user);
				}
				else if (moveUser.action == ACTION_MAP_MOVE)
				{
					for (int i = 0; i < com->usersInfo->size(); i++)
					{
						User * othersUser = (User*)com->usersInfo->at(i);

						if (!strcmp(othersUser->name, moveUser.name))
						{
							//제자리 방향회전
							switch ((cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection)
							{
							case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
								//제자리에서 방향키눌러 방향전환할때
								if (othersUser->seeDirection != (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection && othersUser->isRunning != true)
								{
									othersUser->sprite->setSpriteFrame("man_13.png");
								}
								else
									othersUser->isRunning = true;
								othersUser->isKeepKeyPressed = true;
								othersUser->seeDirection = (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection;
								break;
							case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
								//제자리에서 방향키눌러 방향전환할때
								if (othersUser->seeDirection != (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection && othersUser->isRunning != true)
								{
									othersUser->sprite->setSpriteFrame("man_01.png");
								}
								else
									othersUser->isRunning = true;
								othersUser->isKeepKeyPressed = true;
								othersUser->seeDirection = (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection;
								break;
							case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
								//제자리에서 방향키눌러 방향전환할때
								if (othersUser->seeDirection != (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection && othersUser->isRunning != true)
								{
									othersUser->sprite->setSpriteFrame("man_09.png");
								}
								else
									othersUser->isRunning = true;
								othersUser->isKeepKeyPressed = true;
								othersUser->seeDirection = (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection;
								break;
							case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
								//제자리에서 방향키눌러 방향전환할때
								if (othersUser->seeDirection != (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection && othersUser->isRunning != true)
								{
									othersUser->sprite->setSpriteFrame("man_05.png");
								}
								else
									othersUser->isRunning = true;
								othersUser->isKeepKeyPressed = true;
								othersUser->seeDirection = (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection;
								break;
							default:
								break;
							}

							othersUser->xpos = moveUser.xpos;
							othersUser->ypos = moveUser.ypos;
							othersUser->position = Point(moveUser.xpos * TILE_SIZE + TILE_SIZE / 2, moveUser.ypos * TILE_SIZE);
							othersUser->sprite->setPosition(othersUser->position);

							//말풍선은 항상 캐릭터를 따라다녀야함
							othersUser->balloon->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2, othersUser->position.y + othersUser->sprite->getContentSize().height));
							othersUser->balloonContent->setPosition(Point(othersUser->position.x + othersUser->sprite->getContentSize().width / 2 - 50, othersUser->position.y + othersUser->sprite->getContentSize().height + 50));
							break;
						}
					}
				}
				else if (moveUser.action == ACTION_MAP_MOVE_END)
				{
					for (int i = 0; i < com->usersInfo->size(); i++)
					{
						User * othersUser = (User*)com->usersInfo->at(i);

						if (!strcmp(othersUser->name, moveUser.name))
						{
							//여러개의 방향키가 눌렸을 경우 가장 마지막으로 키를 땠을때 이동을 중지한다.
							if (othersUser->seeDirection == (cocos2d::EventKeyboard::KeyCode)moveUser.seeDirection)
							{
								othersUser->isKeepKeyPressed = false;
								othersUser->isRunning = false;
							}
						}
					}
				}
			}
			break;
		case REQUEST_JOIN:
			{
				if (!strcmp(com->recvBuf, "join okey"))
					com->permissionJoin = 1;
				else
					com->permissionJoin = -1;
			}
			break;
		case REQUEST_TILED_MAP:
			{
				com->tiledMapBuf->clear();

				//맵 정보를 가져온다.
				for (int i = 0; i < readLen; i++)
				{
					com->tiledMapBuf->push_back((byte)com->recvBuf[i]);
				}
				com->getTiledMap = true;
			}
			break;
		case REQUEST_IMAGE:
			{
				com->imageBuf->clear();

				//이미지 정보를 가져온다.
				for (int i = 0; i < readLen; i++)
				{
					com->imageBuf->push_back((byte)com->recvBuf[i]);
				}
				com->getImage = true;
			}
			break;
		case DELETE_FIELD_ITEM:
			{
				//지워진 오브젝트 정보를 저장.
				memcpy(&com->deleteCustomObject, com->recvBuf, sizeof(StructCustomObject));

				//현재 필드의 오브젝트 변동사항 알림
				com->isDeleteMapObject = true;
			}
			break;
		case REQUEST_FIELD_INFO:
			{
				int count;
				memcpy(&count, &com->recvBuf[0], 4);
				com->objectInfo->clear();
				com->monsterInfo->clear();

				for (int i = 0; i < count; i++)
				{
					StructCustomObject imsiStructCustomObject;
					memcpy(&imsiStructCustomObject, &com->recvBuf[4 + i * sizeof(StructCustomObject)], sizeof(StructCustomObject));

					CustomObject * customObject = new CustomObject(imsiStructCustomObject);

					if (!strcmp(customObject->type, "item"))
						com->objectInfo->push_back(customObject);
					else if (!strcmp(customObject->type, "monster"))
						com->monsterInfo->push_back(customObject);
				}
				com->isObjectBufferFill = true;
			}
			break;
		case REQUEST_INVENTORY_ITEM_INFO:
			{
				int itemCount;
				memcpy(&itemCount, &com->recvBuf[0], 4);

				for (int i = 0; i < itemCount; i++)
				{
					StructCustomObject imsiStructCustomObject;
					memcpy(&imsiStructCustomObject, &com->recvBuf[4 + i * sizeof(StructCustomObject)], sizeof(StructCustomObject));

					CustomObject * customObject = new CustomObject(imsiStructCustomObject);
					com->inventory_items_Info[3 - (customObject->ypos + 1)][customObject->xpos] = customObject;
				}
				com->isInventoryFill = true;
			}
			break;
		case THROW_ITEM:
			{
				StructCustomObject imsiStructCustomObject;
				memcpy(&imsiStructCustomObject, com->recvBuf, sizeof(StructCustomObject));

				CustomObject * customObject = new CustomObject(imsiStructCustomObject);
				com->previousItemCount = com->objectInfo->size();
				com->objectInfo->push_back(customObject);
				com->isOtherThrowItem = true;
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
	this->objectInfo = new std::vector<CustomObject*>();
	this->monsterInfo = new std::vector<CustomObject*>();

	tiledMapBuf = new std::vector<byte>();
	imageBuf = new std::vector<byte>();

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

	host = gethostbyname("192.168.56.101");
	//host = gethostbyname("sourcecake.iptime.org");
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
	char buf[9];
	int writeLen;

	if (message == NULL)
		return -1;

	IntToChar(size, &buf[0]);
	IntToChar(code, &buf[4]);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    writeLen = send(this->sock, buf, 8, 0);
    writeLen += send(this->sock, message, size, 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    writeLen = write(this->sock, buf, 8);
    writeLen += write(this->sock, message, size);
#endif

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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (recv(this->sock, buf, 4, 0) == -1)
        return -1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (read(this->sock, buf, 4) == -1)
        return -1;
#endif

	CharToInt(&buf[0], &len);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (recv(this->sock, buf, 4, 0) == -1)
        return -1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (read(this->sock, buf, 4) == -1)
        return -1;
#endif

	CharToInt(&buf[0], code);

	if (len > (100000 - 8))
		return 8;

	//버퍼가 완전히 비워질때까지 받는다.
	while (1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        readLen = recv(this->sock, &buf[size], len, 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        readLen = read(this->sock, &buf[size], len);
#endif

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
	StructCustomChatting structCustomChatting;
	strcpy(structCustomChatting.name, name);
	strcpy(structCustomChatting.content, content);
	strcpy(structCustomChatting.field, field);

	memcpy(this->sendBuf, &structCustomChatting, sizeof(StructCustomChatting));

	str_len = sendCommand(CHATTING_PROCESS, this->sendBuf, sizeof(StructCustomChatting));

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

int CustomNetworkCommunication::userMoveUpdate(StructCustomUser user)
{
	memcpy(this->sendBuf, &user, sizeof(StructCustomUser));

	//맵 이동시 현재 유저목록 초기화
	if(user.action == ACTION_MAP_POTAL)
		usersInfo->clear();

	str_len = sendCommand(USER_MOVE_UPDATE, this->sendBuf, sizeof(StructCustomUser));

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
	if (userName != NULL)
	{
		strcpy(this->sendBuf, userName);
		str_len = sendCommand(UPDATE_LOGIN_TIME, this->sendBuf, strlen(this->sendBuf));

		return str_len;
	}
	else
		return -1;
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

//인벤토리에서 아이템 이동시 서버에 알리는 함수
int CustomNetworkCommunication::moveInventoryItem(CustomObject * customObject)
{
	StructCustomObject structCustomObject;
	structCustomObject.idx = customObject->idx;
	strcpy(structCustomObject.name, customObject->name);
	strcpy(structCustomObject.type, customObject->type);
	structCustomObject.xpos = customObject->xpos;
	structCustomObject.ypos = customObject->ypos;
	structCustomObject.order = customObject->order;
	strcpy(structCustomObject.fileDir, customObject->fileDir);
	structCustomObject.count = customObject->count;

	char * message = new char[sizeof(StructCustomObject) + 1];
	memcpy(message, &structCustomObject, sizeof(StructCustomObject));

	str_len = sendCommand(MOVE_INVENTORY_ITEM, message, sizeof(StructCustomObject));

	delete message;

	return str_len;
}

//인벤토리에서 아이템을 버릴시 서버에 알리는 함수
int CustomNetworkCommunication::throwItem(CustomObject * customObject)
{
	StructCustomObject structCustomObject;
	structCustomObject.idx = customObject->idx;
	strcpy(structCustomObject.name, customObject->name);
	strcpy(structCustomObject.type, customObject->type);
	structCustomObject.xpos = customObject->xpos;
	structCustomObject.ypos = customObject->ypos;
	structCustomObject.order = customObject->order;
	strcpy(structCustomObject.fileDir, customObject->fileDir);
	structCustomObject.count = customObject->count;

	char * message = new char[sizeof(StructCustomObject) + 1];
	memcpy(message, &structCustomObject, sizeof(StructCustomObject));

	str_len = sendCommand(THROW_ITEM, message, sizeof(StructCustomObject));

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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    OutputDebugString(pszCharacterString);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    printf("%s", pszCharacterString);
#endif
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
	if (monsterInfo != NULL)
		delete monsterInfo;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (inventory_items_Info[i][j] != NULL)
				delete inventory_items_Info[i][j];
		}
	}
}
