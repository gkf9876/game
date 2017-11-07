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

	this->mainUser = new User();
	this->mainUser->isLogin = false;

	winSize = Director::getInstance()->getWinSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//로그인 화면
	loginBackground = Sprite::create("login.jpg");
	loginBackground->setAnchorPoint(Point::ZERO);
	loginBackground->setPosition(Point(0, 0));
	loginBackground->setContentSize(winSize);
	loginBackground->setVisible(true);
	this->addChild(loginBackground);

	//로그인 아이디 입력창
	loginID = EditBox::create(Size(200, 20), Scale9Sprite::create());
	loginID->setAnchorPoint(Point(0, 0.5));
	loginID->setFont("Arial", 50);
	loginID->setInputMode(EditBox::InputMode::SINGLE_LINE);
	loginID->setFontColor(Color3B::GREEN);
	loginID->setPlaceholderFontColor(Color3B::GREEN);
	loginID->setPlaceHolder("Insert ID");
	loginID->setMaxLength(20);
	loginID->setDelegate(this);
	loginID->setPosition(Vec2(origin.x + winSize.width / 2 - loginID->getSize().width/2, origin.y + winSize.height / 2));
	this->addChild(loginID, MAP_NAME_PRIORITY_Z_ORDER, CHATTING_INPUT);

	//로그인 실패시 띄우는 창
	loginFail = Sprite::create("loginfail.jpg");
	loginFail->setAnchorPoint(Point(0.5,0.5));
	loginFail->setPosition(Point(origin.x + winSize.width / 2, origin.y + winSize.height / 2));
	loginFail->setContentSize(Size(288, 105));
	loginFail->setVisible(false);
	this->addChild(loginFail, MAP_NAME_PRIORITY_Z_ORDER + 1, CHATTING_INPUT + 1);

	//서버와 통신 설정
	com = new CustomNetworkCommunication();
	com->init();

	//매 프레임마다 update() 함수를 호출
	this->scheduleUpdate();

	return true;
}

void HelloWorld::start()
{
	//맵 세팅
	address = "TileMaps/KonyangUniv.Daejeon/JukhunDigitalFacilitie/floor_08/";
	std::string map = address;
	map.append("floor.tmx");

	tmap = TMXTiledMap::create(com->mainUser->field);
	currentFlag = com->mainUser->field;
	background = tmap->getLayer("Background");
	items = tmap->getLayer("Items");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);
	//

	//플레이어 만들기
	objects = tmap->getObjectGroup("Objects");
	this->mainUser->dragonPosition = Point(com->mainUser->xpos * TILE_SIZE + TILE_SIZE / 2, com->mainUser->ypos * TILE_SIZE);

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

	//채팅창 띄우기
	chattingInput = EditBox::create(Size(400, 20), Scale9Sprite::create());
	chattingInput->setAnchorPoint(Point(0, 0));
	chattingInput->setFont("Arial", 10);
	chattingInput->setInputMode(EditBox::InputMode::SINGLE_LINE);
	chattingInput->setFontColor(Color3B::GREEN);
	chattingInput->setPlaceholderFontColor(Color3B::GREEN);
	chattingInput->setPlaceHolder("");
	chattingInput->setMaxLength(20);
	chattingInput->setDelegate(this);
	this->addChild(chattingInput, MAP_NAME_PRIORITY_Z_ORDER, CHATTING_INPUT);

	//채팅화면 띄우기
	showLabel = LabelTTF::create("non-Select", "Arial", 10);
	showLabel->setPosition(Point(450, 400));
	this->addChild(showLabel);

	tableView = TableView::create(this, Size(400, 120));
	tableView->setAnchorPoint(Point(0, 0));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setDelegate(this);
	this->addChild(tableView, MAP_NAME_PRIORITY_Z_ORDER, CHATTING_VIEW);

	//tableCellAtIndex 등등 데이터 소스를 다시 한번 부른다. 테이블 뷰를 다시 그리므로 처음으로 포커스가 맞춰진다.
	tableView->reloadData();

	//캐릭터 위에 말풍선으로 문자열 출력.
	balloon = Sprite::create("Images/balloon.png");
	balloon->setAnchorPoint(Point(1, 0));
	balloon->setPosition(Point(this->mainUser->dragonPosition.x + this->mainUser->dragon->getContentSize().width / 2, this->mainUser->dragonPosition.y + this->mainUser->dragon->getContentSize().height));
	balloon->setVisible(false);
	this->addChild(balloon, BALLON_PRIORITY_Z_ORDER, CHATTING_BALLOON);

	balloonContent = LabelTTF::create("", "NanumBarunGothic", 15);
	balloonContent->setColor(Color3B::BLACK);
	balloonContent->setAnchorPoint(Point(1, 0));
	balloonContent->setPosition(Point(this->mainUser->dragonPosition.x + this->mainUser->dragon->getContentSize().width / 2 - 50, this->mainUser->dragonPosition.y + this->mainUser->dragon->getContentSize().height + 50));
	balloonContent->setVisible(false);
	this->addChild(balloonContent, BALLON_CONTENT_PRIORITY_Z_ORDER, CHATTING_BALLOON_CONTENT);

	//맵이름을 중앙 상단에 띄움
	title = Sprite::create("images/title.png");
	title->setAnchorPoint(Point(0.5, 1));
	this->addChild(title, TITLE_PRIORITY_Z_ORDER, TITLE);

	//플레이어를 화면의 중앙에 위치하도록 화면을 이동.
	this->setViewpointCenter(this->mainUser->dragon->getPosition());
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
	com->close();

	Layer::onExit();
}

void HelloWorld::createDragon()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/man.plist");

	this->mainUser->dragon = Sprite::createWithSpriteFrameName("man_01.png");
	this->mainUser->dragon->setAnchorPoint(Point(0.5, 0));
	this->mainUser->dragon->setPosition(this->mainUser->dragonPosition);
	this->mainUser->isAction = false;
	this->mainUser->isRunning = false;
	this->mainUser->isKeepKeyPressed = false;
	this->mainUser->seeDirection = cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	this->addChild(this->mainUser->dragon, DRAGON_PRIORITY_Z_ORDER, DRAGON_TAG);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	if (this->mainUser->isLogin != true)
		return false;

	return true;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
	if (this->mainUser->isLogin != true)
		return;

	auto touchPoint = touch->getLocation();
	touchPoint = this->convertToNodeSpace(touchPoint);

	Point playerPos = this->mainUser->dragon->getPosition();

	Point diff = touchPoint - playerPos;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
		{
			playerPos.x += tmap->getTileSize().width;

			this->mainUser->dragon->setFlippedX(true);
		}
		else
		{
			playerPos.x -= tmap->getTileSize().width;

			this->mainUser->dragon->setFlippedX(false);
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

	this->setViewpointCenter(this->mainUser->dragon->getPosition());
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

	//채팅창은 화면 아래, 좌측에 붙어있어야함.
	chattingInput->setPosition(Point(actualPosition.x - winSize.width / 2, actualPosition.y - winSize.height / 2));
	tableView->setPosition(Point(actualPosition.x - winSize.width / 2, actualPosition.y - winSize.height / 2 + 25));

	//말풍선은 항상 캐릭터를 따라다녀야함
	balloon->setPosition(Point(this->mainUser->dragonPosition.x + this->mainUser->dragon->getContentSize().width / 2, this->mainUser->dragonPosition.y + this->mainUser->dragon->getContentSize().height));
	balloonContent->setPosition(Point(this->mainUser->dragonPosition.x + this->mainUser->dragon->getContentSize().width / 2 - 50, this->mainUser->dragonPosition.y + this->mainUser->dragon->getContentSize().height + 50));

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

	Point regionPoint = this->mainUser->dragonPosition;

	if (tileGid)
	{
		Value& properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string wall = properties.asValueMap()["Wall"].asString();

			if (wall == "YES")
			{
				CCLOG("Wall...");
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

					CCLOG("get item!! : (%d, %d)", (int)items_coodinate.x, (int)items_coodinate.y);
				}
				else
				{
					//아이템 창이 꽉찬 경우이므로 아이템을 줍지 않는다.
					CCLOG("full inventory!!");
				}
				

				//가로로 아이템이 꽉 차면 아래부분으로 커서를 이동.
				if (items_coodinate.x >= (TILE_SIZE * 4))
				{
					items_coodinate.x = 0;

					items_coodinate.y -= TILE_SIZE;
				}
				else
				{
					//아이템을 추가하고 옆으로 커서를 이동.
					items_coodinate.x += TILE_SIZE;
				}
			}


			//맵 이동과정.
			//현재 위치의 맵의 정보, 목적지 정보를 타일에 담고
			//목적지 위치의 맵의 정보. 목적지 정보를 이동할 맵의 타일에 담는다.

			std::string exit = properties.asValueMap()["Entrance"].asString();
				
			if (exit == "YES")
			{
				CCLOG("Move other map..");
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
				currentFlag = destination;
				background = tmap->getLayer("Background");
				items = tmap->getLayer("Items");
				metainfo = tmap->getLayer("MetaInfo");
				metainfo->setVisible(false);
				this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);

				objects = tmap->getObjectGroup("Objects");

				spawnPoint = objects->getObject(code);

				int x = spawnPoint.at("x").asInt() + tmap->getTileSize().width / 2;
				int y = spawnPoint.at("y").asInt();
				
				this->mainUser->dragonPosition = Point(x, y);
				this->mainUser->dragon->setPosition(this->mainUser->dragonPosition);
				this->setViewpointCenter(this->mainUser->dragon->getPosition());

				//맵의 이름을 화면 상단에 출력.
				mapName->setString(objects->getProperty("Name").asString());

				//이동한 내용을 DB에 반영
				char sendMapName[100];
				strcpy(sendMapName, String(currentFlag).getCString());
				com->userMoveUpdate(com->mainUser->name, Point(regionPoint.x / TILE_SIZE, regionPoint.y / TILE_SIZE), this->mainUser->field,
					cocos2d::Point(this->mainUser->dragon->getPosition().x / TILE_SIZE, this->mainUser->dragon->getPosition().y / TILE_SIZE), sendMapName);
				strcpy(this->mainUser->field, sendMapName);

				return;
			}
		}
	}

	this->mainUser->dragonPosition = position;
	this->mainUser->dragon->setPosition(this->mainUser->dragonPosition);

	CCLOG("xpos : %d, ypos : %d", (int)position.x, (int)position.y);

	//이동한 내용을 DB에 반영
	Value& properties = tmap->getPropertiesForGID(tileGid);
	char sendMapName[100];
	strcpy(sendMapName, String(currentFlag).getCString());
	com->userMoveUpdate(com->mainUser->name, Point(regionPoint.x / TILE_SIZE, regionPoint.y / TILE_SIZE), this->mainUser->field,
		cocos2d::Point(this->mainUser->dragon->getPosition().x / TILE_SIZE, this->mainUser->dragon->getPosition().y / TILE_SIZE), sendMapName);
	strcpy(this->mainUser->field, sendMapName);

	//모션
	this->setAnimation(this->mainUser->seeDirection);
	//플레이어가 화면 가운데로 오게 조절
	this->setViewpointCenter(this->mainUser->dragon->getPosition());
	this->mainUser->isRunning = true;
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (this->mainUser->isLogin != true)
	{
		switch (key)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
			if (com->popupLoginFail == true)
				com->popupLoginFail = false;
			else
				loginID->touchDownAction(loginID, cocos2d::ui::Widget::TouchEventType::ENDED);
			break;
		}
		return;
	}

	CCLOG("KeyPress..(%d)", key);
	this->mainUser->isKeepKeyPressed = true;

	//이동 모션을 기다린뒤에 이동한다.
	if (this->mainUser->isAction == true)
	{
		CCLOG("Motion Ready...");
		return;
	}

	Point playerPos = this->mainUser->dragon->getPosition();
	Sprite * spr;

	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:

		//이동 방향을 보고있으면 그 방향으로 이동한다.
		if (this->mainUser->seeDirection == key)
		{
			spr = Sprite::createWithSpriteFrameName("man_13.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());

			playerPos.y += tmap->getTileSize().height;

			//좌표 이동
			if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
				playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
				playerPos.x >= 0 &&
				playerPos.x >= 0)
			{
				this->setPlayerPosition(playerPos);
			}
		}
		else
		{
			//방향 전환
			spr = Sprite::createWithSpriteFrameName("man_13.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());
		}

		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:

		//이동 방향을 보고있으면 그 방향으로 이동한다.
		if (this->mainUser->seeDirection == key)
		{
			spr = Sprite::createWithSpriteFrameName("man_01.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());

			playerPos.y -= tmap->getTileSize().height;

			//좌표 이동
			if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
				playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
				playerPos.x >= 0 &&
				playerPos.x >= 0)
			{
				this->setPlayerPosition(playerPos);
			}
		}
		else
		{
			//방향 전환
			spr = Sprite::createWithSpriteFrameName("man_01.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());
		}

		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:

		//이동 방향을 보고있으면 그 방향으로 이동한다.
		if (this->mainUser->seeDirection == key)
		{
			spr = Sprite::createWithSpriteFrameName("man_09.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());

			playerPos.x += tmap->getTileSize().width;

			//좌표 이동
			if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
				playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
				playerPos.x >= 0 &&
				playerPos.x >= 0)
			{
				this->setPlayerPosition(playerPos);
			}
		}
		else
		{
			//방향 전환
			spr = Sprite::createWithSpriteFrameName("man_09.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());
		}

		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:

		//이동 방향을 보고있으면 그 방향으로 이동한다.
		if (this->mainUser->seeDirection == key)
		{
			spr = Sprite::createWithSpriteFrameName("man_05.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());

			playerPos.x -= tmap->getTileSize().width;

			//좌표 이동
			if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
				playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
				playerPos.x >= 0 &&
				playerPos.x >= 0)
			{
				this->setPlayerPosition(playerPos);
			}
		}
		else
		{
			//방향 전환
			spr = Sprite::createWithSpriteFrameName("man_05.png");
			this->mainUser->dragon->setSpriteFrame(spr->getSpriteFrame());
		}

		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		if(inventory->isVisible() == true)
			inventory->setVisible(false);
		else
			inventory->setVisible(true);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		chattingInput->touchDownAction(chattingInput, cocos2d::ui::Widget::TouchEventType::ENDED);
		break;
	}
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (this->mainUser->isLogin != true)
		return;

	CCLOG("KeyRelease..(%d)", key);

	this->mainUser->isKeepKeyPressed = false;
	this->mainUser->isRunning = false;
	return;
}

void HelloWorld::setAnimation(cocos2d::EventKeyboard::KeyCode key)
{
	SpriteFrame * frame;
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	Point position = this->mainUser->dragon->getPosition() / TILE_SIZE;

	//이전에 실행중이던 액션을 중지하고 새로 액션을 실행.
	//dragon->stopAction(animate);

	this->mainUser->isAction = true;
	CCLOG("action started!");
	CCLOG("Motion Run");

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
	
	this->mainUser->animate = Animate::create(animation);
	auto actionStartCallback = CallFunc::create(this, callfunc_selector(HelloWorld::actionStarted));
	auto actionFinishCallback = CallFunc::create(this, callfunc_selector(HelloWorld::actionFinished));

	auto sequence = Sequence::create(actionStartCallback, this->mainUser->animate, actionFinishCallback, NULL);
	this->mainUser->dragon->runAction(sequence);
}

void HelloWorld::actionStarted()
{
	// do something on complete
}

void HelloWorld::actionFinished()
{
	// do something on complete
	this->mainUser->isAction = false;
	CCLOG("action finished!");

}

void HelloWorld::update(float fDelta)
{
	if (this->mainUser->isLogin != true)
	{
		//로그인 됬는지 확인
		if (com->isLogin == true)
		{
			this->mainUser->isLogin = true;
			loginBackground->setVisible(false);
			loginID->setVisible(false);

			// 유저 정보 불러오기
			com->getUserInfo();

			//유저 정보 불러올때까지 대기.
			while (com->isGetUserInfo != true);
			strcpy(this->mainUser->name, com->mainUser->name);
			strcpy(this->mainUser->field, com->mainUser->field);
			this->mainUser->xpos = com->mainUser->xpos;
			this->mainUser->ypos = com->mainUser->ypos;

			start();
		}

		if(com->popupLoginFail == true)
			loginFail->setVisible(true);
		else
			loginFail->setVisible(false);

		return;
	}

	if (this->mainUser->isRunning == true && this->mainUser->isAction == false)
	{
		CCLOG("Keep Running..");

		Point playerPos = this->mainUser->dragon->getPosition();

		switch (this->mainUser->seeDirection)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			playerPos.y += tmap->getTileSize().height;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			playerPos.y -= tmap->getTileSize().height;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			playerPos.x += tmap->getTileSize().width;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			playerPos.x -= tmap->getTileSize().width;
			break;
		}

		//좌표 이동
		if (playerPos.x <= (tmap->getMapSize().width * tmap->getTileSize().width) &&
			playerPos.y <= (tmap->getMapSize().height * tmap->getTileSize().height) &&
			playerPos.x >= 0 &&
			playerPos.x >= 0)
		{
			this->setPlayerPosition(playerPos);
		}
	}

	//말풍선이 떠있으면 일정시간후 없앤다.
	if (balloon->isVisible() == true)
		balloonTime++;
	if (balloonTime % 120 == 119)
	{
		balloon->setVisible(false);
		balloonContent->setVisible(false);
		balloonTime = 0;
	}

	//채팅창을 업데이트. 다른 사람의 채팅내용을 띄운다.

	for (int i = 0; i < com->chattingInfo.size(); i++)
	{
		if (element.size() <= i)
		{
			element.pushBack(com->chattingInfo.at(i));
			CCLOG("%s", com->chattingInfo.at(i));
		}
	}
	tableView->reloadData();
	tableView->setContentOffset(Vec2(0, 0), false);
}

void HelloWorld::editBoxReturn(EditBox * editBox)
{
	CCLOG("--- editBoxReturn ---");

	if (this->mainUser->isLogin != true)
	{
		char userName[50];
		strcpy(userName, loginID->getText());
		com->requestLogin(userName);

		CCLOG("isLogin : %d", com->isLogin);

		return;
	}

	//채팅입력하고 엔터키 누르면 채팅창에 입력한 문자열이 등록.
	const char * buf = chattingInput->getText();

	if (strlen(buf) == 0)
		return;

	//채팅입력창 업데이트
	String * message = String::createWithFormat("%s", buf);
	editBox->setText("");

	char name[50];
	com->chatting(com->mainUser->name, message->getCString());

	//캐릭터 위에 말풍선으로 문자열 출력.
	balloon->setVisible(true);
	balloonContent->setString(message->getCString());
	balloonContent->setVisible(true);
	balloonTime = 0;
}

void HelloWorld::editBoxEditingDidBegin(EditBox * editBox)
{
	//CCLOG("--- editBoxEditingDidBegin ---");
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::editBoxEditingDidEnd(EditBox * editBox)
{
	//CCLOG("--- editBoxEditingDidEnd ---");
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::editBoxTextChanged(EditBox * editBox, const std::string& text)
{
	//CCLOG("--- editBoxTextChanged ---");
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::scrollViewDidScroll(ScrollView* view)
{
	//CCLOG("---- scrollViewDidScroll ----");
}

void HelloWorld::scrollViewDidZoom(ScrollView* view)
{
	//CCLOG("---- scrollViewDidZoom ----");
}

//셀을 터치하면 콜백
void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell)
{
	//CCLOG("---- tableCellTouched ----");
}

Size HelloWorld::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	//CCLOG("---- tableCellSizeForIndex ----");

	return Size(60, 10);
}

//reload가 호출되거나, 스크롤이 움직여 안보이는 셀이 보여질 때 호출
TableViewCell* HelloWorld::tableCellAtIndex(TableView *table, ssize_t idx)
{
	//CCLOG("---- tableCellAtIndex ----");
	int count = element.size();
	String * string = element.at(count - (idx + 1));

	//테이블이 사용 중인 셀이 있다면 재활용한다.
	TableViewCell* cell = table->dequeueCell();

	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();

		LabelTTF * label = LabelTTF::create(string->getCString(), "Arial", 10);
		label->setPosition(Point::ZERO);
		label->setAnchorPoint(Point::ZERO);
		label->setColor(Color3B::GREEN);
		label->setTag(CHATTING_VIEW_ELEMENT);
		cell->addChild(label);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(CHATTING_VIEW_ELEMENT);
		label->setString(string->getCString());
	}

	return cell;
}

//테이블이 셀 갯수에 대한 정보를 가져가는 곳
ssize_t HelloWorld::numberOfCellsInTableView(TableView *table)
{
	//CCLOG("---- numberOfCellsInTableView ----");
	int count = element.size();
	return count;
}