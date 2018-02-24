#include "CustomObject.h"


CustomObject::CustomObject(int idx, char * name, char * type, int xpos, int ypos, int order, std::vector<byte> * imageBuf, int count, int hp, int object_number)
{
	this->idx = idx;
	strcpy(this->name, name);
	strcpy(this->type, type);
	this->xpos = xpos;
	this->ypos = ypos;
	this->order = order;
	this->imageBuf = new std::vector<byte>();
	for (int i = 0; i < imageBuf->size(); i++)
	{
		this->imageBuf->push_back(imageBuf->at(i));
	}
	this->count = count;
    this->hp = hp;
    this->object_number = object_number;
}

CustomObject::CustomObject(StructCustomObject structCustomObject)
{
	this->idx = structCustomObject.idx;
	strcpy(this->name, structCustomObject.name);
	strcpy(this->type, structCustomObject.type);
	this->xpos = structCustomObject.xpos;
	this->ypos = structCustomObject.ypos;
	this->order = structCustomObject.order;
	strcpy(this->fileDir, structCustomObject.fileDir);
	this->imageBuf = new std::vector<byte>();
	this->count = structCustomObject.count;
    this->hp = structCustomObject.hp;
    this->object_number = structCustomObject.object_number;
}

StructCustomObject CustomObject::getObject()
{
	StructCustomObject structCustomObject;
	structCustomObject.idx = this->idx;
	strcpy(structCustomObject.name, this->name);
	strcpy(structCustomObject.type, this->type);
	structCustomObject.xpos = this->xpos;
	structCustomObject.ypos = this->ypos;
	structCustomObject.order = this->order;
	strcpy(structCustomObject.fileDir, this->fileDir);
	structCustomObject.count = this->count;
	structCustomObject.hp = this->hp;
	structCustomObject.object_number = this->object_number;

	return structCustomObject;
}

CustomObject::~CustomObject()
{
	if (imageBuf != NULL)
		delete imageBuf;
}
