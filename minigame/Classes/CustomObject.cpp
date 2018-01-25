#include "CustomObject.h"


CustomObject::CustomObject(int idx, char * name, char * type, int xpos, int ypos, int order, std::vector<byte> * imageBuf, int count)
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
}

CustomObject::~CustomObject()
{
	if (imageBuf != NULL)
		delete imageBuf;
}