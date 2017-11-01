#include "CustomNetworkCommunication.h"

unsigned WINAPI SendMsg(void * arg)   // send thread main
{
	while (1)
	{
		//CCLOG("SendMsg");
		Sleep(1000);
	}

	return 0;
}

unsigned WINAPI RecvMsg(void * arg)   // read thread main
{
	CustomNetworkCommunication * com = ((CustomNetworkCommunication*)arg);
	int code;
	int str_len;
	char buf[50][BUF_SIZE];

	while (1)
	{
		int readCommand(int * code, char * buf);
		str_len = com->readCommand(&code, com->recvBuf);

		if (str_len == -1)
			return -1;

		switch (code)
		{
		case REQUEST_USER_INFO:
			com->SeparateString(com->recvBuf, buf, 50, '\n');
			strcpy(com->user.name, buf[0]);
			com->user.xpos = atoi(buf[1]);
			com->user.ypos = atoi(buf[2]);
			com->user.field = atoi(buf[3]);
			break;
		case CHATTING_PROCESS:
			com->SeparateString(com->recvBuf, buf, 50, '\n');
			com->chattingInfo.pushBack(String::createWithFormat("%s : %s", buf[0], buf[1]));
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
		default:
			break;
		}
	}

	return 0;
}

void CustomNetworkCommunication::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		error_handling("socket() error");

	host = gethostbyname("192.168.56.101");
	if (!host)
		error_handling("gethost... error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
	serv_adr.sin_port = htons(atoi("9190"));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
		error_handling("connect() error!");
	else
		puts("Connected...............");

	// ���÷� ������ �ۼ����Ҷ� ����� ������
	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)this, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)this, 0, NULL);

	WaitForSingleObject(hSndThread, 1000);
	WaitForSingleObject(hRcvThread, 1000);
}

void CustomNetworkCommunication::close()
{
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}

SOCKET CustomNetworkCommunication::getSock()
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

void CustomNetworkCommunication::userMoveUpdate(char * userName, Point point, char * field)
{
	sprintf(this->sendBuf, "%s\n%d\n%d\n%s", userName, (int)point.x, (int)point.y, field);
	CCLOG("%s %d %d %s", userName, (int)point.x, (int)point.y, field);

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
	*result = (int)((value[0] << 24) & 0xff000000);
	*result += (int)((value[1] << 16) & 0x00ff0000);
	*result += (int)((value[2] << 8) & 0x0000ff00);
	*result += value[3];
}