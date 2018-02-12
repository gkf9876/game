#include "User.h"


User::User()
{
	this->sprite = NULL;
	this->animate = NULL;
	this->balloonContent = NULL;
	this->balloon = NULL;
}

User::User(StructCustomUser structCustomUser)
{
	this->sprite = NULL;
	this->animate = NULL;
	this->balloonContent = NULL;
	this->balloon = NULL;

	this->sock = structCustomUser.sock;
	strcpy(this->name, structCustomUser.name);
	strcpy(this->password, structCustomUser.password);
	this->xpos = structCustomUser.xpos;
	this->ypos = structCustomUser.ypos;
	strcpy(this->field, structCustomUser.field);
	this->seeDirection = (cocos2d::EventKeyboard::KeyCode)structCustomUser.seeDirection;
	this->action = structCustomUser.action;
}

void User::setUser(StructCustomUser structCustomUser)
{
	this->sock = structCustomUser.sock;
	strcpy(this->name, structCustomUser.name);
	strcpy(this->password, structCustomUser.password);
	this->xpos = structCustomUser.xpos;
	this->ypos = structCustomUser.ypos;
	strcpy(this->field, structCustomUser.field);
	this->seeDirection = (cocos2d::EventKeyboard::KeyCode)structCustomUser.seeDirection;
	this->action = structCustomUser.action;
}

StructCustomUser User::getUser()
{
	StructCustomUser structCustomUser;
	structCustomUser.sock = this->sock;
	strcpy(structCustomUser.name, this->name);
	strcpy(structCustomUser.password, this->password);
	structCustomUser.xpos = this->xpos;
	structCustomUser.ypos = this->ypos;
	strcpy(structCustomUser.field, this->field);
	structCustomUser.seeDirection = (int)this->seeDirection;
	structCustomUser.action = this->action;

	return structCustomUser;
}

User::~User()
{
	if(sprite != NULL)
		delete sprite;												//드래곤 객체
	if(animate != NULL)
		delete animate;												//드래곤 애니메이션
	if(balloonContent != NULL)
		delete balloonContent;										//말풍선 내용
	if (balloon != NULL)														
		delete balloon;												//말풍선
}