#ifndef __USER_H__
#define __USER_H__

#include "cocos2d.h"

USING_NS_CC;

#define TILE_SIZE	32														//타일 사이즈

#define ACTION_MAP_IN		0
#define ACTION_MAP_MOVE		1
#define ACTION_MAP_OUT		2
#define ACTION_MAP_POTAL	3
#define ACTION_MAP_MOVE_END	4
#define ACTION_ATTACK       5
#define ACTION_ATTACK_END   6

struct StructCustomUser
{
	int sock;
	char name[50];
	char password[50];
	int xpos;
	int ypos;
	char field[100];
	int seeDirection;
	int action;
};

class User
{
public:
	int sock;
	char name[50];
	char password[50];
	int xpos;
	int ypos;
	char field[100];
	int action;

	bool isLogin;

	cocos2d::Point position;												//드래곤 위치
	cocos2d::Sprite* sprite;												//드래곤 객체
	cocos2d::Animate * animate;												//드래곤 애니메이션
	bool isAction;															//드래곤 액션 여부
	bool isRunning;															//드래곤이 이동중인지
	bool isKeepKeyPressed;													//키를 누르고 있는지
    bool isAttack;                                                          //공격 중인지
	cocos2d::EventKeyboard::KeyCode seeDirection;							//드래곤이 어딜 보고있는지
	cocos2d::LabelTTF * balloonContent;										//말풍선 내용
	Sprite * balloon;														//말풍선
	int balloonTime = 0;													//말풍선 떠있는 시간.

	User();
	User(StructCustomUser strucrCustomUser);
	void setUser(StructCustomUser strucrCustomUser);
	StructCustomUser getUser();
	~User();
};

#endif
