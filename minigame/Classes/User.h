#ifndef __USER_H__
#define __USER_H__

#include "cocos2d.h"

USING_NS_CC;

class User : public Node
{
public:
	int sock;
	char name[50];
	char password[50];
	int xpos;
	int ypos;
	char field[100];

	bool isLogin;

	cocos2d::Point dragonPosition;											//�巡�� ��ġ
	cocos2d::Sprite* dragon;												//�巡�� ��ü
	cocos2d::Animate * animate;												//�巡�� �ִϸ��̼�
	bool isAction;															//�巡�� �׼� ����
	bool isRunning;															//�巡���� �̵�������
	bool isKeepKeyPressed;													//Ű�� ������ �ִ���
	cocos2d::EventKeyboard::KeyCode seeDirection;							//�巡���� ��� �����ִ���
};

#endif