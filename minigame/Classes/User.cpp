#include "User.h"


User::User()
{
	this->sprite = NULL;
	this->animate = NULL;
	this->balloonContent = NULL;
	this->balloon = NULL;
}

void User::setUser(StructCustomUser strucrCustomUser)
{
	this->sock = strucrCustomUser.sock;
	strcpy(this->name, strucrCustomUser.name);
	strcpy(this->password, strucrCustomUser.password);
	this->xpos = strucrCustomUser.xpos;
	this->ypos = strucrCustomUser.ypos;
	strcpy(this->field, strucrCustomUser.field);
	this->seeDirection = (cocos2d::EventKeyboard::KeyCode)strucrCustomUser.seeDirection;
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