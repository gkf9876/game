#include "HelloWorldScene.h"
#include<stdio.h>
#include<iostream>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	//맵 세팅
	address = "TileMaps/KonyangUniv.Daejeon/JukhunDigitalFacilitie/floor_08/";
	std::string map = address;
	map.append("floor.tmx");

	tmap = TMXTiledMap::create(map);
	background = tmap->getLayer("Background");
	items = tmap->getLayer("Items");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);
	//

	//플레이어 만들기
	objects = tmap->getObjectGroup("Objects");

	spawnPoint = objects->getObject("SpawnPoint");

	int x = spawnPoint.at("x").asInt() + tmap->getTileSize().width / 2;
	int y = spawnPoint.at("y").asInt();

	dragonPosition = Point(x, y);

	this->createDragon();
	//

	//아이템창 만들기
	inventory = TMXTiledMap::create("Images/inventory.tmx");
	inventory_background = inventory->getLayer("Background");
	inventory_items = inventory->getLayer("Items");
	inventory_metainfo = inventory->getLayer("MetaInfo");
	inventory->setVisible(false);
	inventory->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(inventory, INVENTORY_PRIORITY_Z_ORDER, INVENTORY);

	items_coodinate = Point(0, 64);
	//

	//화면에 맵이름 띄우기
	mapName = LabelTTF::create(objects->getProperty("Name").asString(), "Arial", 25);
	mapName->setColor(Color3B::RED);
	mapName->setAnchorPoint(Point(0.5, 1));
	this->addChild(mapName, MAP_NAME_PRIORITY_Z_ORDER, MAP_NAME);
	//

	title = Sprite::create("images/title.png");
	title->setAnchorPoint(Point(0.5, 1));
	this->addChild(title, TITLE_PRIORITY_Z_ORDER, TITLE);

	this->setViewpointCenter(dragon->getPosition());
	return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener1 = EventListenerKeyboard::create();

	listener->setEnabled(true);

	listener1->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener1->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

void HelloWorld::createDragon()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/man.plist");

	dragon = Sprite::createWithSpriteFrameName("man_01.png");
	dragon->setAnchorPoint(Point(0.5, 0));
	dragon->setPosition(dragonPosition);
	this->addChild(dragon, DRAGON_PRIORITY_Z_ORDER, DRAGON_TAG);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	Point playerPos = dragon->getPosition();

	Point diff = touchPoint - playerPos;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			playerPos.x += tmap->getTileSize().width;

			dragon->setFlippedX(true);
		}
		else
		{
			playerPos.x -= tmap->getTileSize().width;

			dragon->setFlippedX(false);
		}
	}
	else
	{
		if (diff.y > 0)
		{
			playerPos.y += tmap->getTileSize().height;
		}
		else
		{
			playerPos.y -= tmap->getTileSize().height;
		}
	}

	if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
		playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
		playerPos.x >= 0 &&
		playerPos.x >= 0)
	{
		this->setPlayerPosition(playerPos);
	}

	this->setViewpointCenter(dragon->getPosition());
}

void HelloWorld::setViewpointCenter(Point position)
{
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);

	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	Point actualPosition = Point(x, y);
	Point centerOfView = Point(winSize.width / 2, winSize.height / 2);
	Point viewPoint = centerOfView - actualPosition;

	//맵 이름과 간판은 항상 플레이어를 따라다녀야함.
	mapName->setPosition(Point(actualPosition.x, actualPosition.y + winSize.height/2));
	title->setPosition(Point(actualPosition.x, actualPosition.y + winSize.height / 2));

	//아이템창은 항상 플레이어를 따라다녀야함.
	inventory->setPosition(Point(actualPosition.x, actualPosition.y));

	this->setPosition(viewPoint);
}

Point HelloWorld::tileCoordForPosition(Point position)
{
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;

	return Point(x, y);
}

void HelloWorld::setPlayerPosition(Point position)
{
	//플레이어가 위치한 타일의 좌표값을 타일중앙으로 맞춰서 판단.
	Point playerPos = position;
	//playerPos.x += tmap->getTileSize().width / 2;
	playerPos.y += tmap->getTileSize().height / 2;
	//

	Point tileCoord = this->tileCoordForPosition(playerPos);

	int tileGid = this->metainfo->getTileGIDAt(tileCoord);

	if (tileGid)
	{
		Value& properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string wall = properties.asValueMap()["Wall"].asString();

			if (wall == "YES")
			{
				log("Wall...");
				return;
			}

			std::string item = properties.asValueMap()["Items"].asString();
			if (item == "YES")
			{
				//아이템 창이 꽉 차있나 확인.
				if (items_coodinate.y >= 0)
				{
					//아이템을 주워서 아이템창에 추가하는 과정.
					Sprite * a = Sprite::create();
					a->setSpriteFrame(this->items->getTileAt(tileCoord)->getDisplayFrame());
					a->setAnchorPoint(Point(0, 0));
					a->setPosition(items_coodinate);
					inventory->addChild(a);

					//기존에 땅에 떨어진 아이템을 지우는 과정.
					this->metainfo->removeTileAt(tileCoord);
					items->removeTileAt(tileCoord);

					log("get item!! : (%d, %d)", (int)items_coodinate.x, (int)items_coodinate.y);
				}
				else
				{
					//아이템 창이 꽉찬 경우이므로 아이템을 줍지 않는다.
					log("full inventory!!");
				}
				

				//가로로 아이템이 꽉 차면 아래부분으로 커서를 이동.
				if (items_coodinate.x >= (32 * 4))
				{
					items_coodinate.x = 0;

					items_coodinate.y -= 32;
				}
				else
				{
					//아이템을 추가하고 옆으로 커서를 이동.
					items_coodinate.x += 32;
				}
			}


			//맵 이동과정.
			//현재 위치의 맵의 정보, 목적지 정보를 타일에 담고
			//목적지 위치의 맵의 정보. 목적지 정보를 이동할 맵의 타일에 담는다.

			std::string exit = properties.asValueMap()["Entrance"].asString();
				
			if (exit == "YES")
			{
				std::string stair = properties.asValueMap()["Stair"].asString();
				std::string frontdoor = properties.asValueMap()["Frontdoor"].asString();

				if (stair == "YES" || frontdoor == "YES")
				{
					address = properties.asValueMap()["Address"].asString();
				}

				std::string destination = address;

				destination.append(properties.asValueMap()["Destination"].asString());
				destination.append(".tmx");

				std::string code = properties.asValueMap()["Code"].asString();

				std::string place = objects->getProperty("Place").asString();

				this->removeChildByTag(MAP_TAG);
				tmap = TMXTiledMap::create(destination);
				background = tmap->getLayer("Background");
				items = tmap->getLayer("Items");
				metainfo = tmap->getLayer("MetaInfo");
				metainfo->setVisible(false);
				this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);

				objects = tmap->getObjectGroup("Objects");

				spawnPoint = objects->getObject(code);

				int x = spawnPoint.at("x").asInt() + tmap->getTileSize().width / 2;
				int y = spawnPoint.at("y").asInt();
				
				dragonPosition = Point(x, y);
				dragon->setPosition(dragonPosition);
				this->setViewpointCenter(dragon->getPosition());

				//맵의 이름을 화면 상단에 출력.
				mapName->setString(objects->getProperty("Name").asString());

				return;
			}
		}
	}

	dragonPosition = position;
	dragon->setPosition(dragonPosition);
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	Point playerPos = dragon->getPosition();
	Sprite * spr;

	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		spr = Sprite::createWithSpriteFrameName("man_13.png");
		dragon->setSpriteFrame(spr->getSpriteFrame());

		playerPos.y += tmap->getTileSize().height;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		spr = Sprite::createWithSpriteFrameName("man_01.png");
		dragon->setSpriteFrame(spr->getSpriteFrame());

		playerPos.y -= tmap->getTileSize().height;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		spr = Sprite::createWithSpriteFrameName("man_09.png");
		dragon->setSpriteFrame(spr->getSpriteFrame());

		playerPos.x += tmap->getTileSize().width;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		spr = Sprite::createWithSpriteFrameName("man_05.png");
		dragon->setSpriteFrame(spr->getSpriteFrame());

		playerPos.x -= tmap->getTileSize().width;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		if(inventory->isVisible() == true)
			inventory->setVisible(false);
		else
			inventory->setVisible(true);
		break;
	}

	if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
		playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
		playerPos.x >= 0 &&
		playerPos.x >= 0)
	{
		this->setPlayerPosition(playerPos);
	}

	this->setAnimation(key);
	this->setViewpointCenter(dragon->getPosition());
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	return;
}

void HelloWorld::setAnimation(cocos2d::EventKeyboard::KeyCode key)
{
	SpriteFrame * frame;
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.3);
	Point position = dragon->getPosition() / 32;

	//이전에 실행중이던 액션을 중지하고 새로 액션을 실행.
	dragon->stopAction(animate);

	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		if((int)position.y % 2 != 0) 
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_14.png");
		else
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_16.png");
		animation->addSpriteFrame(frame);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_15.png");
		animation->addSpriteFrame(frame);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if ((int)position.y % 2 != 0)
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_02.png");
		else
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_04.png");
		animation->addSpriteFrame(frame);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_03.png");
		animation->addSpriteFrame(frame);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if ((int)position.x % 2 != 0)
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_10.png");
		else
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_12.png");
		animation->addSpriteFrame(frame);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_11.png");
		animation->addSpriteFrame(frame);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if ((int)position.x % 2 != 0)
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_06.png");
		else
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_08.png");
		animation->addSpriteFrame(frame);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("man_07.png");
		animation->addSpriteFrame(frame);
		break;
	}
	
	animate = Animate::create(animation);
	dragon->runAction(animate);
}