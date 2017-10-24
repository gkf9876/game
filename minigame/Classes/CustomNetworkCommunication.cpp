#include "CustomNetworkCommunication.h"

void CustomNetworkCommunication::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		error_handling("socket() error");

	host = gethostbyname("sourcecake.iptime.org");
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
}

void CustomNetworkCommunication::close()
{
	closesocket(sock);
	WSACleanup();
}

void CustomNetworkCommunication::error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int CustomNetworkCommunication::sendCommand(int sock, int code, char * message)
{
	if (message == NULL)
		return -1;

	char buf[BUF_SIZE];
	int len = strlen(message);

	buf[0] = (len & 0xff000000) >> 24;
	buf[1] = (len & 0x00ff0000) >> 16;
	buf[2] = (len & 0x0000ff00) >> 8;
	buf[3] = (len & 0x000000ff);

	buf[4] = (code & 0xff000000) >> 24;
	buf[5] = (code & 0x00ff0000) >> 16;
	buf[6] = (code & 0x0000ff00) >> 8;
	buf[7] = (code & 0x000000ff);
	strcpy(&buf[8], message);

	int writeLen = send(sock, buf, len + 8, 0);

	if (writeLen == -1)
		return -1;
	else
		return len;
}

int CustomNetworkCommunication::readCommand(int sock, int * code, char * buf)
{
	if (buf == NULL)
		return -1;

	int readLen = recv(sock, buf, 4, 0);

	if (readLen == -1)
		return -1;
	else if (readLen == 0)
		return 0;

	int len = (int)((buf[0] << 24) & 0xff000000) + (int)((buf[1] << 16) & 0x00ff0000) + (int)((buf[2] << 8) & 0x0000ff00) + buf[3];

	readLen = recv(sock, buf, 4, 0);

	if (readLen == -1)
		return -1;

	*code = (int)((buf[0] << 24) & 0xff000000) + (int)((buf[1] << 16) & 0x00ff0000) + (int)((buf[2] << 8) & 0x0000ff00) + buf[3];

	recv(sock, buf, len, 0);
	buf[len] = 0;

	return len;
}

void CustomNetworkCommunication::chatting(const char * name, const char * content)
{
	String * message = String::createWithFormat("%s\n%s\n", name, content);

	strcpy(this->message, message->getCString());

	str_len = sendCommand(sock, 2, this->message);
	recv_cnt = readCommand(sock, &code, this->message);

	if (recv_cnt == -1)
		error_handling("read() error!");
}

void CustomNetworkCommunication::communication()
{
	while (1)
	{
		fputs("1 : chatting confirm, 2 : message send, 3 : Exit\n", stdout);
		//fgets(message, BUF_SIZE, stdin);
		strcpy(message, "2\n");

		if (!strcmp(message, "1\n"))
		{
			printf("1 : chatting confirm\n");
			fgets(message, BUF_SIZE, stdin);

			str_len = sendCommand(sock, 1, message);
			recv_cnt = readCommand(sock, &code, message);

			if (recv_cnt == -1)
				error_handling("read() error!");

			printf("Message from server : %d, %s\n", code, message);
		}
		else if (!strcmp(message, "2\n"))
		{
		}
		else if (!strcmp(message, "3\n"))
		{
			printf("3 : Exit\n");
			break;
		}
		else
		{
			message[strlen(message) - 1] = 0;
			int code1 = atoi(message);

			printf("Exception Menn\n");
			fgets(message, BUF_SIZE, stdin);

			printf("code : %d\n", atoi(message));
			str_len = sendCommand(sock, code1, message);
			recv_cnt = readCommand(sock, &code, message);

			if (recv_cnt == -1)
				error_handling("read() error!");

			printf("Message from server : %d, %s\n", code, message);
		}

		memset(message, 0, BUF_SIZE);
	}
}