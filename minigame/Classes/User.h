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

	cocos2d::Point dragonPosition;											//드래곤 위치
	cocos2d::Sprite* dragon;												//드래곤 객체
	cocos2d::Animate * animate;												//드래곤 애니메이션
	bool isAction;															//드래곤 액션 여부
	bool isRunning;															//드래곤이 이동중인지
	bool isKeepKeyPressed;													//키를 누르고 있는지
	cocos2d::EventKeyboard::KeyCode seeDirection;							//드래곤이 어딜 보고있는지
};

#endif