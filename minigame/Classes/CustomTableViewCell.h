#ifndef __CUSTOMTABLEVIEWCELL_H__
#define __CUSTOMTABLEVIEWCELL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CustomTableViewCell : public TableViewCell
{
public:
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

};

#endif // __CUSTOMTABLEVIEWCELL_H__