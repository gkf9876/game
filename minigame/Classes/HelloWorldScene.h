#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

#define DRAGON_TAG 10														//화면에서 그리는 개체 태그.
#define MAP_TAG 11
#define HOME_TAG 1
#define OTHERS_TAG 2
#define TESTDESERT_TAG 2
#define MAP_NAME		3
#define TITLE			4
#define INVENTORY		5

//수치가 높을수록 그림이 맨 위에 위치.
#define MAP_PRIORITY_Z_ORDER		1										//맵 우선순위	
#define DRAGON_PRIORITY_Z_ORDER		2										//드래곤 우선순위
#define OTHERS_PRIORITY_Z_ORDER		3										//기타 객체 우선순위
#define MAP_NAME_PRIORITY_Z_ORDER	6										//맵 이름 우선순위
#define TITLE_PRIORITY_Z_ORDER		5										//맵 이름 간판 우선순위
#define INVENTORY_PRIORITY_Z_ORDER	4										//아이템창 우선순위

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	cocos2d::Size winSize;													//화면 크기
	cocos2d::Point dragonPosition;											//드래곤 위치
	cocos2d::Sprite* dragon;												//드래곤 객체
	cocos2d::Animate * animate;												//드래곤 애니메이션
	bool isAction;															//드래곤 액션 여부
	bool isRunning;															//드래곤이 이동중인지
	bool isKeepKeyPressed;													//키를 누르고 있는지
	cocos2d::EventKeyboard::KeyCode seeDirection;							//드래곤이 어딜 보고있는지

	cocos2d::TMXTiledMap * inventory;										//아이템창
	cocos2d::TMXLayer * inventory_background;								//아이템창의 배경 레이어
	cocos2d::TMXLayer * inventory_items;									//아이템창의 아이템 레이어
	cocos2d::TMXLayer * inventory_metainfo;									//아이템창의 지표 레이어
	cocos2d::Point items_coodinate;											//아이템창에 아이템을 저장할 위치.

	cocos2d::TMXTiledMap * tmap;											//맵
	cocos2d::TMXLayer * background;											//맵의 배경 레이어
	cocos2d::TMXLayer * items;												//맵의 아이템 레이어
	cocos2d::TMXLayer * metainfo;											//맵의 지표 레이어
	cocos2d::TMXObjectGroup * objects;										//맵의 오브젝트
	cocos2d::ValueMap spawnPoint;											//맵에서 드래곤이 출력될 위치.

	std::string address;													//출력할 맵이미지 경로
	cocos2d::LabelTTF * mapName;											//맵 이름
	cocos2d::Sprite * title;												//맵 이름 배경.

	cocos2d::LabelTTF * text;									//테스트 할때 사용하는 객체.


	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

	void createDragon();
	void setViewpointCenter(cocos2d::Point position);
	cocos2d::Point tileCoordForPosition(cocos2d::Point position);
	void setPlayerPosition(cocos2d::Point position);

	void setAnimation(cocos2d::EventKeyboard::KeyCode key);

	void actionStarted();
	void actionFinished();

	//매 프레임마다 호출되는 함수.
	void update(float fDelta);												
};

#endif // __HELLOWORLD_SCENE_H__
