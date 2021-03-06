#ifndef __CUSTOM_OBJECT_H__
#define __CUSTOM_OBJECT_H__

#include "cocos2d.h"

USING_NS_CC;

#define TILE_SIZE	32														//타일 사이즈

typedef uint8_t byte;

struct StructCustomObject
{
	int idx;
	char name[50];
	char type[50];
	int xpos;
	int ypos;
	int order;
	char fileDir[100];
	int count;
    int hp;
    int object_number;
};

class CustomObject
{
public:
	int idx;
	char name[50];
	char type[50];
	int xpos;
	int ypos;
	int order;
	char fileDir[100];
	std::vector<byte> *imageBuf = NULL;
	int count;
    int hp;
    int object_number;

public:
	CustomObject(int idx, char * name, char * type, int xpos, int ypos, int order, std::vector<byte> * imageBuf, int count, int hp, int object_number);
	CustomObject(StructCustomObject structCustomObject);
	StructCustomObject getObject();
	~CustomObject();
};

#endif
