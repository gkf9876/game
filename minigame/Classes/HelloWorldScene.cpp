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
	updateTime = 0;
	readyCount = 0;


	//로그인 화면
	loginBackground = Sprite::create("login.jpg");
	loginBackground->setAnchorPoint(Point::ZERO);
	loginBackground->setPosition(Point(0, 0));
	loginBackground->setContentSize(winSize);
	loginBackground->setVisible(true);
	this->addChild(loginBackground, LOGIN_WINDOW_Z_ORDER, LOGIN_WINDOW);

	//회원가입 버튼
	joinButton = Button::create("joinButton.jpg", "joinButton.jpg");
	joinButton->setAnchorPoint(Point(0.5, 0.5));
	joinButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 460 / 720));
	joinButton->setScaleX(0.375);
	joinButton->setScaleY(0.444);
	joinButton->setVisible(true);
	this->addChild(joinButton, JOIN_BUTTON_Z_ORDER, JOIN_BUTTON);
	joinButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::joinButtonTouchEvent, this));

	//회원가입 창
	joinPopUp = Sprite::create("joinPopUp.png");
	joinPopUp->setAnchorPoint(Point(0.5, 0.5));
	joinPopUp->setContentSize(winSize);
	joinPopUp->setPosition(winSize / 2);
	joinPopUp->setVisible(false);
	this->addChild(joinPopUp, JOIN_POPUP_Z_ORDER, JOIN_POPUP);

	//회원가입 아이디 입력창
	joinID = EditBox::create(Size(100, 30), Scale9Sprite::create());
	joinID->setAnchorPoint(Point(0, 0));
	joinID->setFont("Arial", 20);
	joinID->setInputMode(EditBox::InputMode::SINGLE_LINE);
	joinID->setFontColor(Color3B::GREEN);
	joinID->setPlaceholderFontColor(Color3B::GREEN);
	joinID->setPlaceHolder("Insert ID");
	joinID->setMaxLength(20);
	joinID->setDelegate(this);
	joinID->setPosition(Point(92, winSize.height - 98));
	joinID->setVisible(false);
	this->addChild(joinID, JOIN_TEXT_INPUT_Z_ORDER, JOIN_TEXT_INPUT);

	//회원가입 확인버튼
	joinOK = Button::create("joinOK.png", "joinOK.png");
	joinOK->setAnchorPoint(Point(0, 0));
	joinOK->setPosition(Point(185, 0));
	joinOK->setVisible(false);
	this->addChild(joinOK, JOIN_OK_BUTTON_Z_ORDER, JOIN_OK_BUTTON);
	joinOK->addTouchEventListener(CC_CALLBACK_2(HelloWorld::joinOkButtonTouchEvent, this));

	//회원가입 취소버튼
	joinCancel = Button::create("joinCancel.png", "joinCancel.png");
	joinCancel->setAnchorPoint(Point(0, 0));
	joinCancel->setPosition(Point(244, 0));
	joinCancel->setVisible(false);
	this->addChild(joinCancel, JOIN_CANCEL_BUTTON_Z_ORDER, JOIN_CANCEL_BUTTON);
	joinCancel->addTouchEventListener(CC_CALLBACK_2(HelloWorld::joinCancelButtonTouchEvent, this));

	//회원가입 실패시 띄우는 창
	joinFailPopUp = Sprite::create("joinFailPopUp.jpg");
	joinFailPopUp->setAnchorPoint(Point(0.5, 0.5));
	joinFailPopUp->setPosition(Point(origin.x + winSize.width / 2, origin.y + winSize.height / 2));
	joinFailPopUp->setContentSize(Size(288, 105));
	joinFailPopUp->setVisible(false);
	this->addChild(joinFailPopUp, JOIN_FAIL_POPUP_Z_ORDER, JOIN_FAIL_POPUP);

	//회원가입 실패 창 확인버튼
	joinFailPopUpOk = Button::create("joinFailPopUpOk.png", "joinFailPopUpOk.png");
	joinFailPopUpOk->setAnchorPoint(Point(0, 0));
	joinFailPopUpOk->setPosition(Point(287, winSize.height - 187));
	joinFailPopUpOk->setVisible(false);
	this->addChild(joinFailPopUpOk, JOIN_FAIL_POPUP_OK_Z_ORDER, JOIN_FAIL_POPUP_OK);
	joinFailPopUpOk->addTouchEventListener(CC_CALLBACK_2(HelloWorld::joinFailPopUpOkButtonTouchEvent, this));

	//로그인 버튼
	loginButton = Button::create("loginButton.jpg", "loginButton.jpg");
	loginButton->setAnchorPoint(Point(0.5, 0.5));
	loginButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 506 / 720));
	loginButton->setScaleX(0.380);
	loginButton->setScaleY(0.470);
	loginButton->setVisible(true);
	this->addChild(loginButton, LOGIN_BUTTON_Z_ORDER, LOGIN_BUTTON);
	loginButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::loginButtonTouchEvent, this));

	//로그인 창
	loginPopUp = Sprite::create("loginPopUp.png");
	loginPopUp->setAnchorPoint(Point(0.5, 0.5));
	loginPopUp->setPosition(Point(winSize.width / 2, winSize.height / 2));
	loginPopUp->setVisible(false);
	this->addChild(loginPopUp, LOGIN_POPUP_Z_ORDER, LOGIN_POPUP);

	//로그인 아이디 입력창
	loginID = EditBox::create(Size(100, 30), Scale9Sprite::create());
	loginID->setAnchorPoint(Point(0, 0));
	loginID->setFont("Arial", 20);
	loginID->setInputMode(EditBox::InputMode::SINGLE_LINE);
	loginID->setFontColor(Color3B::GREEN);
	loginID->setPlaceholderFontColor(Color3B::GREEN);
	loginID->setPlaceHolder("Insert ID");
	loginID->setMaxLength(20);
	loginID->setDelegate(this);
	loginID->setPosition(Point(200, winSize.height - 140));
	loginID->setVisible(false);
	this->addChild(loginID, LOGIN_TEXT_INPUT_Z_ORDER, LOGIN_TEXT_INPUT);
	
	//로그인 실패시 띄우는 창
	loginFailPopUp = Sprite::create("loginfail.jpg");
	loginFailPopUp->setAnchorPoint(Point(0.5,0.5));
	loginFailPopUp->setPosition(Point(origin.x + winSize.width / 2, origin.y + winSize.height / 2));
	loginFailPopUp->setContentSize(Size(288, 105));
	loginFailPopUp->setVisible(false);
	this->addChild(loginFailPopUp, LOGIN_FAIL_POPUP_Z_ORDER, LOGIN_FAIL_POPUP);

	//로그인 실패 창 확인버튼
	loginFailPopUpOk = Button::create("loginFailPopUpOk.png", "loginFailPopUpOk.png");
	loginFailPopUpOk->setAnchorPoint(Point(0, 0));
	loginFailPopUpOk->setPosition(Point(287, winSize.height - 187));
	loginFailPopUpOk->setVisible(false);
	this->addChild(loginFailPopUpOk, LOGIN_FAIL_POPUP_OK_Z_ORDER, LOGIN_FAIL_POPUP_OK);
	loginFailPopUpOk->addTouchEventListener(CC_CALLBACK_2(HelloWorld::loginFailPopUpOkButtonTouchEvent, this));

	//암호변경 버튼
	passwordModiButton = Button::create("passwordModiButton.jpg", "passwordModiButton.jpg");
	passwordModiButton->setAnchorPoint(Point(0.5, 0.5));
	passwordModiButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 551 / 720));
	passwordModiButton->setScaleX(0.381);
	passwordModiButton->setScaleY(0.500);
	passwordModiButton->setVisible(true);
	this->addChild(passwordModiButton, PASSWORDMODI_BUTTON_Z_ORDER, PASSWORDMODI_BUTTON);
	passwordModiButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::passwordModiButtonTouchEvent, this));

	//배경이야기 버튼
	backGroundStoryButton = Button::create("backGroundStoryButton.jpg", "backGroundStoryButton.jpg");
	backGroundStoryButton->setAnchorPoint(Point(0.5, 0.5));
	backGroundStoryButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 596 / 720));
	backGroundStoryButton->setScaleX(0.380);
	backGroundStoryButton->setScaleY(0.500);
	backGroundStoryButton->setVisible(true);
	this->addChild(backGroundStoryButton, BACKGROUNDSTORY_BUTTON_Z_ORDER, BACKGROUNDSTORY_BUTTON);
	backGroundStoryButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::backGroundStoryButtonTouchEvent, this));

	//만든사람들 버튼
	makePeoplesButton = Button::create("makePeoplesButton.jpg", "makePeoplesButton.jpg");
	makePeoplesButton->setAnchorPoint(Point(0.5, 0.5));
	makePeoplesButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 641 / 720));
	makePeoplesButton->setScaleX(0.380);
	makePeoplesButton->setScaleY(0.484);
	makePeoplesButton->setVisible(true);
	this->addChild(makePeoplesButton, MAKEPEOPLE_BUTTON_Z_ORDER, MAKEPEOPLE_BUTTON);
	makePeoplesButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::makePeoplesButtonTouchEvent, this));

	//종료 버튼
	exitButton = Button::create("exitButton.jpg", "exitButton.jpg");
	exitButton->setAnchorPoint(Point(0.5, 0.5));
	exitButton->setPosition(Point(winSize.width * 1094 / 1280, winSize.height - winSize.height * 686 / 720));
	exitButton->setScaleX(0.378);
	exitButton->setScaleY(0.457);
	exitButton->setVisible(true);
	this->addChild(exitButton, EXIT_BUTTON_Z_ORDER, EXIT_BUTTON);
	exitButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::exitButtonTouchEvent, this));

	//통신 실패시 띄우는 창
	commErrorPopUp = Sprite::create("commErrorPopUp.jpg");
	commErrorPopUp->setAnchorPoint(Point(0.5, 0.5));
	commErrorPopUp->setPosition(Point(origin.x + winSize.width / 2, origin.y + winSize.height / 2));
	commErrorPopUp->setContentSize(Size(288, 105));
	commErrorPopUp->setVisible(false);
	this->addChild(commErrorPopUp, COMM_ERROR_POPUP_Z_ORDER, COMM_ERROR_POPUP);

	//통신 실패 창 확인버튼
	commErrorPopUpOk = Button::create("commErrorPopUpOk.png", "commErrorPopUpOk.png");
	commErrorPopUpOk->setAnchorPoint(Point(0, 0));
	commErrorPopUpOk->setPosition(Point(287, winSize.height - 187));
	commErrorPopUpOk->setVisible(false);
	this->addChild(commErrorPopUpOk, COMM_ERROR_POPUP_OK_Z_ORDER, COMM_ERROR_POPUP_OK);
	commErrorPopUpOk->addTouchEventListener(CC_CALLBACK_2(HelloWorld::commErrorPopUpOkButtonTouchEvent, this));

	//서버와 통신 설정
	com = new CustomNetworkCommunication();
	com->init();

	//매 프레임마다 update() 함수를 호출
	this->schedule(schedule_selector(HelloWorld::update), 0.5);

	return true;
}

void HelloWorld::start()
{
	//맵 세팅. 서버로부터 타일맵 데이터를 받아서 구현한다.
	createTileMap(this->mainUser->field);

	//맵의 오브젝트 정보를 요청한다.
	createObject();

	//맵의 몬스터 구현하기
	createMonster();

	//플레이어 만들기
	this->createSprite();
	//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//조이스틱 구현
	joystickPad = Sprite::create("Images/inventory.png");
	joystickPad->setOpacity(100);
	joystickPad->setAnchorPoint(Point(1, 0));
	this->addChild(joystickPad, JOYSTICK_PRIORITY_Z_ORDER, JOYSTICK);

	joystick = Sprite::create("Images/a.png");
	joystick->setOpacity(150);
	joystick->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(joystick, JOYSTICK_PRIORITY_Z_ORDER + 1, JOYSTICK);

	joystickDirectionSet = false;
	joystickTouched = false;
    
    //아이템창 띄우는 버튼
    inventoryButton = Button::create("Images/inventoryButton.png", "Images/inventoryButton.png");
    inventoryButton->setAnchorPoint(Point(0.5, 0.5));
    this->addChild(inventoryButton, INVENTORY_BUTTON_Z_ORDER, INVENTORY_BUTTON);
    inventoryButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::inventoryButtonTouchEvent, this));
    
    //아이템 먹는 버튼
    itemEatButton = Button::create("Images/inventoryButton.png", "Images/inventoryButton.png");
    itemEatButton->setAnchorPoint(Point(0.5, 0.5));
    this->addChild(itemEatButton, ITEM_EAT_BUTTON_Z_ORDER, ITEM_EAT_BUTTON);
    itemEatButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::itemEatButtonTouchEvent, this));
#endif

	//아이템창 만들기
	inventory = TMXTiledMap::create("Images/Inventory.tmx");
	inventory->setVisible(false);
	inventory->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(inventory, INVENTORY_PRIORITY_Z_ORDER, INVENTORY);

	inventoryItemTouched = false;
	//

	//플레이어의 인벤토리창 정보를 요청한다.
    createInventory();

	//화면에 맵이름 띄우기
	mapName = LabelTTF::create(objects->getProperty("Name").asString(), "Arial", 25);
	mapName->setColor(Color3B::RED);
	mapName->setAnchorPoint(Point(0.5, 1));
	this->addChild(mapName, MAP_NAME_PRIORITY_Z_ORDER, MAP_NAME);
	//

	//채팅창 띄우기
	chattingInput = EditBox::create(Size(200, 20), Scale9Sprite::create());
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

	tableView = TableView::create(this, Size(200, 60));
	tableView->setAnchorPoint(Point(0, 0));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setDelegate(this);
	this->addChild(tableView, MAP_NAME_PRIORITY_Z_ORDER, CHATTING_VIEW);

	//맵이름을 중앙 상단에 띄움
	title = Sprite::create("Images/title.png");
	title->setAnchorPoint(Point(0.5, 1));
	this->addChild(title, TITLE_PRIORITY_Z_ORDER, TITLE);

	//플레이어를 화면의 중앙에 위치하도록 화면을 이동.
	this->setViewpointCenter(this->mainUser->sprite->getPosition());
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	//멀티터치 함수 등록
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(HelloWorld::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	auto listener1 = EventListenerKeyboard::create();

	listener1->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener1->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	com->sockClose();

	Layer::onExit();
}

void HelloWorld::createSprite()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/man.plist");

	switch (this->mainUser->seeDirection)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		this->mainUser->sprite = Sprite::createWithSpriteFrameName("man_13.png");
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		this->mainUser->sprite = Sprite::createWithSpriteFrameName("man_01.png");
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		this->mainUser->sprite = Sprite::createWithSpriteFrameName("man_09.png");
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		this->mainUser->sprite = Sprite::createWithSpriteFrameName("man_05.png");
		break;
	default:
		this->mainUser->sprite = Sprite::createWithSpriteFrameName("man_01.png");
		break;
	}

	this->mainUser->sprite->setAnchorPoint(Point(0.5, 0));
	this->mainUser->sprite->setPosition(this->mainUser->position);
	this->mainUser->isAction = false;
	this->mainUser->isRunning = false;
	this->mainUser->isKeepKeyPressed = false;
	this->addChild(this->mainUser->sprite, DRAGON_PRIORITY_Z_ORDER, DRAGON_TAG);

	//캐릭터 위에 말풍선으로 문자열 출력.
	this->mainUser->balloon = Sprite::create("Images/balloon.png");
	this->mainUser->balloon->setAnchorPoint(Point(1, 0));
	this->mainUser->balloon->setPosition(Point(this->mainUser->position.x + this->mainUser->sprite->getContentSize().width / 2, this->mainUser->position.y + this->mainUser->sprite->getContentSize().height));
	this->mainUser->balloon->setVisible(false);
	this->addChild(this->mainUser->balloon, BALLON_PRIORITY_Z_ORDER, CHATTING_BALLOON);

	this->mainUser->balloonContent = LabelTTF::create("", "NanumBarunGothic", 15);
	this->mainUser->balloonContent->setColor(Color3B::BLACK);
	this->mainUser->balloonContent->setAnchorPoint(Point(1, 0));
	this->mainUser->balloonContent->setPosition(Point(this->mainUser->position.x + this->mainUser->sprite->getContentSize().width / 2 - 50, this->mainUser->position.y + this->mainUser->sprite->getContentSize().height + 50));
	this->mainUser->balloonContent->setVisible(false);
	this->addChild(this->mainUser->balloonContent, BALLON_CONTENT_PRIORITY_Z_ORDER, CHATTING_BALLOON_CONTENT);
}

void HelloWorld::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
	std::vector<Touch *>::const_iterator it = touches.begin();
	Touch* touch;
	Point tap;

	if (this->mainUser->isLogin != true)
	{
		return;
	}

	//화면 터치포인트 기준값 구하기. 맵의 좌표와 터치 좌표를 맞추기위함
	Point position = this->mainUser->sprite->getPosition();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	//화면에 보이는 중심 좌표
	Point actualPosition = Point(x, y);

	//화면에 보이는 좌측 최하단
	Point plag = Point(actualPosition.x - winSize.width / 2, actualPosition.y - winSize.height / 2);

	//여러개의 터치포인트 확인
	for (int i = 0; i < touches.size(); i++)
	{
		touch = (Touch*)(*it);

		//터치된거
		if (touch)
		{
			tap = touch->getLocation() + plag;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//조이스틱을 터치했을 경우
			if (joystick->getBoundingBox().containsPoint(tap) && joystickTouched == false)
			{
				joystick->setPosition(tap);
				joystickTouched = true;
			}
#endif
			//아이템창의 아이템을 터치했을 경우
			if (inventory->isVisible() == true)
			{
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 5; j++)
					{
						if (com->inventory_items_Info[i][j] != NULL && inventoryItemTouched == false)
						{
							Point itemPoint = Point(com->inventory_items_Info[i][j]->xpos, com->inventory_items_Info[i][j]->ypos);
							this->touchedItem = (Sprite *)inventory->getChildByTag(INVENTORY_ITEM + com->inventory_items_Info[i][j]->idx);
							Point inventoryTouchPos = Point(tap.x - (inventory->getPosition().x - inventory->getContentSize().width/2),
								tap.y - (inventory->getPosition().y - inventory->getContentSize().height/2));

							if (touchedItem->getBoundingBox().containsPoint(inventoryTouchPos))
							{
								inventoryTouchPos.x -= touchedItem->getContentSize().width / 2;
								inventoryTouchPos.y -= touchedItem->getContentSize().height / 2;
								touchedItem->setPosition(inventoryTouchPos);
								inventoryItemTouched = true;
							}
						}
					}
				}
			}
		}
		it++;
	}
}


void HelloWorld::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
	std::vector<Touch *>::const_iterator it = touches.begin();
	Touch* touch;
	Point tap;
	Point ntap;

    if (this->mainUser->isLogin != true)
    {
        return;
    }
    
	//화면 터치포인트 기준값 구하기. 맵의 좌표와 터치 좌표를 맞추기위함
	Point position = this->mainUser->sprite->getPosition();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	//화면에 보이는 중심 좌표
	Point actualPosition = Point(x, y);

	//화면에 보이는 좌측 최하단
	Point plag = Point(actualPosition.x - winSize.width / 2, actualPosition.y - winSize.height / 2);

	for (int i = 0; i<touches.size(); i++)
	{
		touch = (Touch*)(*it);

		//터치가 된경우
		if (touch)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//조이스틱 터치시
			if (joystickTouched)
			{
				tap = touch->getLocation() + plag;

				//조이스틱을 조이패드 밖으로 나가지 못하게 한다.
				if (tap.x > joystickPad->getPosition().x)
					ntap.x = joystickPad->getPosition().x;
				else if (tap.x < joystickPad->getPosition().x - joystickPad->getContentSize().width)
					ntap.x = joystickPad->getPosition().x - joystickPad->getContentSize().width;
				else
					ntap.x = tap.x;

				if (tap.y > joystickPad->getPosition().y + joystickPad->getContentSize().height)
					ntap.y = joystickPad->getPosition().y + joystickPad->getContentSize().height;
				else if (tap.y < joystickPad->getPosition().y)
					ntap.y = joystickPad->getPosition().y;
				else
					ntap.y = tap.y;

				//조이패드 기울기
				double incline = joystickPad->getContentSize().height / joystickPad->getContentSize().width;

				//좌측하단에서 우측상단으로 올라가는 그래프 y접점 좌표
				double yAxisValueUp = joystickPad->getPosition().y
					- incline * (joystickPad->getPosition().x - joystickPad->getContentSize().width);
				//좌측상단에서 우측하단으로 내려가는 그래프 y접점 좌표
				double yAxisValueDown = joystickPad->getPosition().y
					+ incline * joystickPad->getPosition().x;

				if (ntap.y < incline * ntap.x + yAxisValueUp)
					if (ntap.y < -1 * incline * ntap.x + yAxisValueDown)
					{
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW, NULL);
						}
					}
					else
					{
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, NULL);
						}
					}
				else
					if (ntap.y < -1 * incline * ntap.x + yAxisValueDown)
					{
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW, NULL);
						}
					}
					else
					{
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW, NULL);
						}
					}

				joystick->setPosition(ntap);
			}
#endif
			//아이템창의 아이템을 드래그할 경우
			if (inventoryItemTouched == true)
			{
				tap = touch->getLocation() + plag;
				Point inventoryTouchPos = Point(tap.x - (inventory->getPosition().x - inventory->getContentSize().width / 2) - touchedItem->getContentSize().width / 2,
					tap.y - (inventory->getPosition().y - inventory->getContentSize().height / 2) - touchedItem->getContentSize().height / 2);
				touchedItem->setPosition(inventoryTouchPos);
			}

			it++;
		}
	}

}


void HelloWorld::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
	std::vector<Touch *>::const_iterator it = touches.begin();
	Touch* touch;
	Point tap;
	Point ntap;

	if (this->mainUser->isLogin != true)
	{
		return;
	}

	//화면 터치포인트 기준값 구하기. 맵의 좌표와 터치 좌표를 맞추기위함
	Point position = this->mainUser->sprite->getPosition();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	//화면에 보이는 중심 좌표
	Point actualPosition = Point(x, y);

	//화면에 보이는 좌측 최하단
	Point plag = Point(actualPosition.x - winSize.width / 2, actualPosition.y - winSize.height / 2);

	for (int i = 0; i<touches.size(); i++)
	{
		touch = (Touch*)(*it);

		//터치가 된경우
		if (touch)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//조이스틱 터치시
			if (joystickTouched)
			{
				Point startPoint = Point(joystickPad->getPosition().x - joystickPad->getContentSize().width / 2,
					joystickPad->getPosition().y + joystickPad->getContentSize().height / 2);
                
                tap = touch->getLocation() + plag;
                
                //조이패드 기울기
                double incline = joystickPad->getContentSize().height / joystickPad->getContentSize().width;
                
                //좌측하단에서 우측상단으로 올라가는 그래프 y접점 좌표
                double yAxisValueUp = joystickPad->getPosition().y
                - incline * (joystickPad->getPosition().x - joystickPad->getContentSize().width);
                //좌측상단에서 우측하단으로 내려가는 그래프 y접점 좌표
                double yAxisValueDown = joystickPad->getPosition().y
                + incline * joystickPad->getPosition().x;
                
                if (tap.y < incline * tap.x + yAxisValueUp)
                    if (tap.y < -1 * incline * tap.x + yAxisValueDown)
                        onKeyReleased(cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW, NULL);
                    else
                        onKeyReleased(cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, NULL);
                    else
                        if (tap.y < -1 * incline * tap.x + yAxisValueDown)
                            onKeyReleased(cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW, NULL);
                        else
                            onKeyReleased(cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW, NULL);
                
				joystick->setPosition(startPoint);
				joystickTouched = false;

				joystickDirectionSet = false;
				this->mainUser->isKeepKeyPressed = false;
				this->mainUser->isRunning = false;
			}
#endif
			//아이템창의 아이템을 드래그했을때
			if (inventoryItemTouched == true)
			{
				CustomObject * customObject = NULL;
				int xpos, ypos;
				
				//드래그한 아이템의 원래위치 확인
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 5; j++)
					{
						customObject = com->inventory_items_Info[i][j];
						if (customObject != NULL)
						{
							if (customObject->idx == (touchedItem->getTag() - INVENTORY_ITEM))
							{
								xpos = customObject->xpos;
								ypos = customObject->ypos;
							}
						}
					}
				}

				tap = touch->getLocation() + plag;

				//아이템창 밖으로 드래그했다가 놓을시
				if (inventory->getBoundingBox().containsPoint(tap) == false)
				{
					//아이템을 내려놓는다.
					Sprite * inventoryItem = Sprite::createWithTexture(touchedItem->getTexture());
					int order = 1;

					for (int i = 0; i < com->objectInfo->size(); i++)
					{
						CustomObject * targetObject = com->objectInfo->at(i);
						if (targetObject->xpos == this->mainUser->xpos && targetObject->ypos == this->mainUser->ypos)
						{
							if (order <= targetObject->order)
							{
								order = targetObject->order + 1;
							}
						}
					}

					inventory->removeChild(touchedItem);
					inventoryItem->setPosition(Point(this->mainUser->xpos * TILE_SIZE + TILE_SIZE / 2, this->mainUser->ypos * TILE_SIZE + TILE_SIZE / 2));
					this->addChild(inventoryItem, OTHERS_PRIORITY_Z_ORDER + order, OTHERS_TAG + com->inventory_items_Info[3 - (ypos + 1)][xpos]->idx);

					com->inventory_items_Info[3 - (ypos + 1)][xpos]->xpos = this->mainUser->xpos;
					com->inventory_items_Info[3 - (ypos + 1)][xpos]->ypos = this->mainUser->ypos;
					com->inventory_items_Info[3 - (ypos + 1)][xpos]->order = order;
					com->throwItem(com->inventory_items_Info[3 - (ypos + 1)][xpos]);
					com->objectInfo->push_back(com->inventory_items_Info[3 - (ypos + 1)][xpos]);
					com->inventory_items_Info[3 - (ypos + 1)][xpos] = NULL;
				}
				else
				{
					//아이템창 안에서 드래그할때 아이템 옮긴위치 반영
					Point inventoryTouchPos = Point(tap.x - (inventory->getPosition().x - inventory->getContentSize().width / 2),
						tap.y - (inventory->getPosition().y - inventory->getContentSize().height / 2));
					int setXpos = (int)(inventoryTouchPos.x / TILE_SIZE);
					int setYpos = (int)(inventoryTouchPos.y / TILE_SIZE);

					touchedItem->setPosition(Point(setXpos * TILE_SIZE, setYpos * TILE_SIZE));

					com->inventory_items_Info[3 - (setYpos + 1)][setXpos] = com->inventory_items_Info[3 - (ypos + 1)][xpos];
					com->inventory_items_Info[3 - (setYpos + 1)][setXpos]->xpos = setXpos;
					com->inventory_items_Info[3 - (setYpos + 1)][setXpos]->ypos = setYpos;
					com->inventory_items_Info[3 - (ypos + 1)][xpos] = NULL;
					com->moveInventoryItem(com->inventory_items_Info[3 - (setYpos + 1)][setXpos]);
				}
				inventoryItemTouched = false;
			}
			it++;
		}
	}
}

void HelloWorld::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    if (this->mainUser->isLogin != true)
    {
        return;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Point startPoint = Point(joystickPad->getPosition().x - joystickPad->getContentSize().width / 2,
		joystickPad->getPosition().y + joystickPad->getContentSize().height / 2);
	joystick->setPosition(startPoint);
	joystickTouched = false;

	joystickDirectionSet = false;
	this->mainUser->isKeepKeyPressed = false;
	this->mainUser->isRunning = false;
#endif
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
	this->mainUser->balloon->setPosition(Point(this->mainUser->position.x + this->mainUser->sprite->getContentSize().width / 2, this->mainUser->position.y + this->mainUser->sprite->getContentSize().height));
	this->mainUser->balloonContent->setPosition(Point(this->mainUser->position.x + this->mainUser->sprite->getContentSize().width / 2 - 50, this->mainUser->position.y + this->mainUser->sprite->getContentSize().height + 50));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//조이스틱은 항상 우측 하단에 붙어야함.
	Point joystickMoveRange = Point(actualPosition.x + winSize.width / 2, actualPosition.y - winSize.height / 2) - joystickPad->getPosition();

	joystickPad->setPosition(Point(actualPosition.x + winSize.width / 2, actualPosition.y - winSize.height / 2));
	if (!joystickTouched)
		joystick->setPosition(Point(joystickPad->getPosition().x - joystickPad->getContentSize().width / 2,
			joystickPad->getPosition().y + joystickPad->getContentSize().height / 2));
	else
		joystick->setPosition(joystick->getPosition() + joystickMoveRange);
    
    //우측 중간부분에 붙임
    inventoryButton->setPosition(Point(actualPosition.x + winSize.width / 2 - TILE_SIZE,
                                       actualPosition.y + TILE_SIZE / 2));
    itemEatButton->setPosition(Point(actualPosition.x + winSize.width / 2 - TILE_SIZE,
                                     actualPosition.y - TILE_SIZE));
#endif

	//통신 실패시 띄우는 창
	commErrorPopUp->setPosition(Point(actualPosition.x, actualPosition.y));
	commErrorPopUpOk->setPosition(Point(actualPosition.x - winSize.width / 2 + 287, actualPosition.y - winSize.height / 2 + (winSize.height - 187)));

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

	int metaInfoTileGid = this->metainfo->getTileGIDAt(tileCoord);

	Point regionPoint = this->mainUser->position;


	//이동할 위치에 다른 유저가 있을시
	for (int i = 0; i < com->usersInfo->size(); i++)
	{
		User * user = com->usersInfo->at(i);

		if (user->position == position)
		{
			return;
		}
	}

	//이동할 위치에 몬스터가 있을시
	//for (int i = 0; i < com->monsterInfo->size(); i++)
	//{
	//	CustomObject * customObject = (com->monsterInfo)->at(i);
	//	auto sprite = getChildByTag(MONSTER_TAG + customObject->idx);
	//	if (sprite->getPosition() == position)
	//		return;
	//}

	if (metaInfoTileGid)
	{
		Value metaInfoProperties = tmap->getPropertiesForGID(metaInfoTileGid);

		if (!metaInfoProperties.isNull())
		{
			std::string wall = metaInfoProperties.asValueMap()["Wall"].asString();

			if (wall == "YES")
			{
				return;
			}

			//맵 이동과정.
			//현재 위치의 맵의 정보, 목적지 정보를 타일에 담고
			//목적지 위치의 맵의 정보. 목적지 정보를 이동할 맵의 타일에 담는다.

			std::string exit = metaInfoProperties.asValueMap()["Entrance"].asString();
				
			if (exit == "YES")
			{
				std::string stair = metaInfoProperties.asValueMap()["Stair"].asString();
				std::string frontdoor = metaInfoProperties.asValueMap()["Frontdoor"].asString();

				if (stair == "YES" || frontdoor == "YES")
				{
					address = metaInfoProperties.asValueMap()["Address"].asString();
				}

				std::string destination = address;

				destination.append(metaInfoProperties.asValueMap()["Destination"].asString());
				destination.append(".tmx");

				std::string code = metaInfoProperties.asValueMap()["Code"].asString();
				std::string place = objects->getProperty("Place").asString();

				//기존 타일맵 소스 삭제
				createTileMap((char*)destination.c_str());

				//맵의 오브젝트 정보를 요청한다.
				createObject();

				//맵의 몬스터 구현하기
				createMonster();

				spawnPoint = objects->getObject(code);

				int x = spawnPoint.at("x").asInt() + tmap->getTileSize().width / 2;
				int y = spawnPoint.at("y").asInt();
				
				this->mainUser->position = Point(x, y);
				this->mainUser->sprite->setPosition(this->mainUser->position);
				this->setViewpointCenter(this->mainUser->sprite->getPosition());

				//맵의 이름을 화면 상단에 출력.
				mapName->setString(objects->getProperty("Name").asString());

				//Hello World
				//지금 맵의 다른 유저들을 안보이게 한다.
				for (int i = 0; i < com->usersInfo->size(); i++)
				{
					User * user = com->usersInfo->at(i);
					user->sprite->setVisible(false);
				}

				//이동한 내용을 DB에 반영
				strcpy(this->mainUser->field, String(destination).getCString());
				strcpy(com->mainUser->field, String(destination).getCString());

				StructCustomUser user;
				strcpy(user.name, com->mainUser->name);
				user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
				user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
				strcpy(user.field, this->mainUser->field);
				user.seeDirection = (int)this->mainUser->seeDirection;
				user.action = ACTION_MAP_POTAL;

				if (com->userMoveUpdate(user) == -1)
				{
					com->comm = false;
					CCLOG("setPlayerPosition comm error");
				}

				this->mainUser->xpos = (int)(this->mainUser->sprite->getPosition().x / TILE_SIZE);
				this->mainUser->ypos = (int)(this->mainUser->sprite->getPosition().y / TILE_SIZE);

				return;
			}
		}
	}

	this->mainUser->position = position;
	this->mainUser->sprite->setPosition(this->mainUser->position);

	//이동한 내용을 DB에 반영
	Value metaInfoProperties = tmap->getPropertiesForGID(metaInfoTileGid);

	StructCustomUser user;
	strcpy(user.name, com->mainUser->name);
	user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
	user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
	strcpy(user.field, this->mainUser->field);
	user.seeDirection = (int)this->mainUser->seeDirection;
	user.action = ACTION_MAP_MOVE;

	if (com->userMoveUpdate(user) == -1)
	{
		com->comm = false;
		CCLOG("setPlayerPosition comm error");
	}

	this->mainUser->xpos = (int)(this->mainUser->sprite->getPosition().x / TILE_SIZE);
	this->mainUser->ypos = (int)(this->mainUser->sprite->getPosition().y / TILE_SIZE);

	//플레이어가 화면 가운데로 오게 조절
	this->setViewpointCenter(this->mainUser->sprite->getPosition());
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	//로그인화면에서는 키동작을 잠근다.
	if (this->mainUser->isLogin != true)
	{
		switch (key)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
			if (com->popupLoginFail == true)
				com->popupLoginFail = false;
			else
				if(loginPopUp->isVisible() == true)
					loginID->touchDownAction(loginID, cocos2d::ui::Widget::TouchEventType::ENDED);
			break;
		}
		return;
	}

	Point playerPos = this->mainUser->sprite->getPosition();
	int order = 0;

	//획득한 아이템
	CustomObject * customObject;
	int customObjectIndex;

	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		//제자리에서 방향키눌러 방향전환할때
		if (this->mainUser->seeDirection != key && this->mainUser->isRunning != true)
		{
			this->mainUser->sprite->setSpriteFrame("man_13.png");

			StructCustomUser user;
			strcpy(user.name, com->mainUser->name);
			user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
			user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
			strcpy(user.field, this->mainUser->field);
			user.seeDirection = (int)key;
			user.action = ACTION_MAP_MOVE;

			//방향전환을 서버에 알려준다.
			if (com->userMoveUpdate(user) == -1)
			{
				com->comm = false;
				CCLOG("onKeyPressed comm error");
			}
		}
		else
			this->mainUser->isRunning = true;
		this->mainUser->isKeepKeyPressed = true;
		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		//제자리에서 방향키눌러 방향전환할때
		if (this->mainUser->seeDirection != key && this->mainUser->isRunning != true)
		{
			this->mainUser->sprite->setSpriteFrame("man_01.png");

			StructCustomUser user;
			strcpy(user.name, com->mainUser->name);
			user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
			user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
			strcpy(user.field, this->mainUser->field);
			user.seeDirection = (int)key;
			user.action = ACTION_MAP_MOVE;

			//방향전환을 서버에 알려준다.
			if (com->userMoveUpdate(user) == -1)
			{
				com->comm = false;
				CCLOG("onKeyPressed comm error");
			}
		}
		else
			this->mainUser->isRunning = true;
		this->mainUser->isKeepKeyPressed = true;
		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		//제자리에서 방향키눌러 방향전환할때
		if (this->mainUser->seeDirection != key && this->mainUser->isRunning != true)
		{
			this->mainUser->sprite->setSpriteFrame("man_09.png");

			StructCustomUser user;
			strcpy(user.name, com->mainUser->name);
			user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
			user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
			strcpy(user.field, this->mainUser->field);
			user.seeDirection = (int)key;
			user.action = ACTION_MAP_MOVE;

			//방향전환을 서버에 알려준다.
			if (com->userMoveUpdate(user) == -1)
			{
				com->comm = false;
				CCLOG("onKeyPressed comm error");
			}
		}
		else
			this->mainUser->isRunning = true;
		this->mainUser->isKeepKeyPressed = true;
		this->mainUser->seeDirection = key;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		//제자리에서 방향키눌러 방향전환할때
		if (this->mainUser->seeDirection != key && this->mainUser->isRunning != true)
		{
			this->mainUser->sprite->setSpriteFrame("man_05.png");

			StructCustomUser user;
			strcpy(user.name, com->mainUser->name);
			user.xpos = this->mainUser->sprite->getPosition().x / TILE_SIZE;
			user.ypos = this->mainUser->sprite->getPosition().y / TILE_SIZE;
			strcpy(user.field, this->mainUser->field);
			user.seeDirection = (int)key;
			user.action = ACTION_MAP_MOVE;

			//방향전환을 서버에 알려준다.
			if (com->userMoveUpdate(user) == -1)
			{
				com->comm = false;
				CCLOG("onKeyPressed comm error");
			}
		}
		else
			this->mainUser->isRunning = true;
		this->mainUser->isKeepKeyPressed = true;
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
	case cocos2d::EventKeyboard::KeyCode::KEY_COMMA:

		//해당 위치에 아이템이 있는지 확인한다.
		customObject = NULL;

		for (int i = 0; i < com->objectInfo->size(); i++)
		{
			CustomObject * imsiCustomObject = com->objectInfo->at(i);
			if (imsiCustomObject->xpos == this->mainUser->xpos && imsiCustomObject->ypos == this->mainUser->ypos)
				if (order < imsiCustomObject->order)
				{
					customObject = imsiCustomObject;
					customObjectIndex = i;
				}
		}

		//캐릭터가 위치한 곳에 아이템이 있을시 아이템을 맵에서 지우고 인벤토리창에 추가한다.
		if (customObject != NULL)
		{
			if(addInventoryItem(customObject) == 1)
				com->objectInfo->erase(com->objectInfo->begin() + customObjectIndex);
		}
		break;
	}
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (this->mainUser->isLogin != true)
		return;

	//여러개의 방향키가 눌렸을 경우 가장 마지막으로 키를 땠을때 이동을 중지한다.
	if (this->mainUser->seeDirection == key)
	{
		this->mainUser->isKeepKeyPressed = false;
		this->mainUser->isRunning = false;

		this->mainUser->action = ACTION_MAP_MOVE_END;

		if (com->userMoveUpdate(this->mainUser->getUser()) == -1)
		{
			com->comm = false;
			CCLOG("onKeyReleased comm error");
		}
	}
	return;
}

void HelloWorld::setAnimation(cocos2d::EventKeyboard::KeyCode key)
{
	SpriteFrame * frame;
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	Point position = this->mainUser->sprite->getPosition() / TILE_SIZE;

	this->mainUser->isAction = true;

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
	this->mainUser->sprite->runAction(sequence);
}

void HelloWorld::setOtherUsersAnimation(User * user, cocos2d::EventKeyboard::KeyCode moveDirection)
{
	SpriteFrame * frame;
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	Point position = user->sprite->getPosition() / TILE_SIZE;

	user->isAction = true;

	switch (moveDirection)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		if ((int)position.y % 2 != 0)
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

	user->animate = Animate::create(animation);
	auto actionStartCallback = CallFuncN::create(CC_CALLBACK_1(HelloWorld::otherUserActionStarted, this, (void*)user));
	auto actionFinishCallback = CallFuncN::create(CC_CALLBACK_1(HelloWorld::otherUserActionFinished, this, (void*)user));

	auto sequence = Sequence::create(actionStartCallback, user->animate, actionFinishCallback, NULL);
	user->sprite->runAction(sequence);
}

void HelloWorld::actionStarted()
{
	// do something on complete
}

void HelloWorld::actionFinished()
{
	// do something on complete
	this->mainUser->isAction = false;
}

void HelloWorld::otherUserActionStarted(Ref * sender, void * d)
{
	// do something on complete
}

void HelloWorld::otherUserActionFinished(Ref * sender, void * d)
{
	// do something on complete
	User * user = (User*)d;
	user->isAction = false;
}

void HelloWorld::update(float fDelta)
{
	updateTime += fDelta;

	if (updateTime >= 60)
		updateTime = 0;

	createOtherUser();

	if (this->mainUser->isLogin != true)
	{
		//로그인 됬는지 확인
		if (com->isLogin == true)
		{
			this->mainUser->isLogin = true;
			loginBackground->setVisible(false);
			joinID->setVisible(false);
			joinButton->setVisible(false);
			loginButton->setVisible(false);
			loginPopUp->setVisible(false);
			loginID->setVisible(false);
			passwordModiButton->setVisible(false);
			backGroundStoryButton->setVisible(false);
			makePeoplesButton->setVisible(false);
			exitButton->setVisible(false);

			// 유저 정보 불러오기
			if (com->getUserInfo() == -1)
			{
				com->comm = false;
				CCLOG("fail getUserInfo()");
			}

			//유저 정보 불러올때까지 대기.
			while (com->isGetUserInfo != true);

			strcpy(this->mainUser->name, com->mainUser->name);
			strcpy(this->mainUser->field, com->mainUser->field);
			this->mainUser->xpos = com->mainUser->xpos;
			this->mainUser->ypos = com->mainUser->ypos;
			this->mainUser->seeDirection = com->mainUser->seeDirection;
			this->mainUser->position = Point(com->mainUser->xpos * TILE_SIZE + TILE_SIZE / 2, com->mainUser->ypos * TILE_SIZE);

			start();
		}

		if (com->popupLoginFail == true)
		{
			loginFailPopUp->setVisible(true);
			loginFailPopUpOk->setVisible(true);
		}
		else
		{
			loginFailPopUp->setVisible(false);
			loginFailPopUpOk->setVisible(false);
		}

		if (com->permissionJoin == 1)
		{
			//회원가입창 닫기
			joinPopUp->setVisible(false);
			joinID->setVisible(false);
			joinOK->setVisible(false);
			joinCancel->setVisible(false);
			com->permissionJoin = 0;

			//로그인화면 띄우기
			loginBackground->setVisible(true);
			joinButton->setVisible(true);
			loginButton->setVisible(true);
			passwordModiButton->setVisible(true);
			backGroundStoryButton->setVisible(true);
			makePeoplesButton->setVisible(true);
			exitButton->setVisible(true);
		}
		else if (com->permissionJoin == -1)
		{
			joinFailPopUp->setVisible(true);
			joinFailPopUpOk->setVisible(true);
			com->permissionJoin = 0;
		}

		return;
	}

	//방향키를 몇초동안 누르고 있느냐에 따라 정지와 이동을 결정
	if (this->mainUser->isKeepKeyPressed == true)
	{
		readyCount++;

		if (readyCount >= 1)
		{
			this->mainUser->isRunning = true;
			readyCount = 0;
		}
	}

	//메인유저가 방향키를 눌러 이동시 실행
	if (this->mainUser->isRunning == true && this->mainUser->isAction == false)
	{
		Point playerPos = this->mainUser->sprite->getPosition();

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

		this->setAnimation(this->mainUser->seeDirection);
	}

	//말풍선이 떠있으면 일정시간후 없앤다.
	if (this->mainUser->balloon->isVisible() == true)
		this->mainUser->balloonTime++;
	if (this->mainUser->balloonTime % 10 == 9)
	{
		this->mainUser->balloon->setVisible(false);
		this->mainUser->balloonContent->setVisible(false);
		this->mainUser->balloonTime = 0;
	}

	//다른 사람들의 말풍선이 떠있으면 일정시간후 없앤다.
	for (int i = 0; i < com->usersInfo->size(); i++)
	{
		User * othersUser = com->usersInfo->at(i);

		if (othersUser->balloon->isVisible() == true)
			othersUser->balloonTime++;
		if (othersUser->balloonTime % 10 == 9)
		{
			othersUser->balloon->setVisible(false);
			othersUser->balloonContent->setVisible(false);
			othersUser->balloonTime = 0;
		}
	}

	//채팅창을 업데이트. 다른 사람의 채팅내용을 띄운다.
	if (element.size() < com->chattingInfo.size())
	{
		element.pushBack(com->chattingInfo.at(com->chattingInfo.size() - 1));
	}

	//tableCellAtIndex 등등 데이터 소스를 다시 한번 부른다. 테이블 뷰를 다시 그리므로 처음으로 포커스가 맞춰진다.
	if (tableView != NULL)
	{
		tableView->reloadData();
		tableView->setContentOffset(Vec2(0, 0), false);
	}

	//서버와 통신 두절시 종료한다.
	if (com->comm == false)
	{
		commErrorPopUp->setVisible(true);
		commErrorPopUpOk->setVisible(true);
	}

	//현재 필드의 지워진 오브젝트 변동사항 업데이트
	if (com->isDeleteMapObject == true)
	{
		CustomObject * deleteCustomObject = new CustomObject(com->deleteCustomObject);
		CustomObject * customObject = NULL;

		for (int i = 0; i < com->objectInfo->size(); i++)
		{
			customObject = com->objectInfo->at(i);

			if (customObject->idx == deleteCustomObject->idx)
			{
				com->objectInfo->erase(com->objectInfo->begin() + i);
				this->removeChildByTag(OTHERS_TAG + com->deleteCustomObject.idx);
				break;
			}
		}
		delete deleteCustomObject;

		com->isDeleteMapObject = false;
	}

	//현재 필드의 추가된 오브젝트 업데이트
	if (com->isOtherThrowItem == true)
	{
		for (int i = com->previousItemCount; i < com->objectInfo->size(); i++)
		{
			Sprite * sprite;
			Image* image = new Image();
			Texture2D* texture = new Texture2D();
			CustomObject * customObject = (com->objectInfo)->at(i);

			com->sendCommand(REQUEST_IMAGE, customObject->fileDir, strlen(customObject->fileDir));

			//요청한 오브젝트 이미지가 수신되길 기다린다.
			while (com->getImage != true);

			//수신된 버퍼데이터로 이미지를 만든다.
			image->initWithImageData(&(com->imageBuf->front()), com->imageBuf->size());
			com->getImage = false;

			//스프라이트를 만들어 맵에 구현한다.
			texture->initWithImage(image);

			sprite = Sprite::createWithTexture(texture);
			sprite->setPosition(Point(customObject->xpos * TILE_SIZE, customObject->ypos * TILE_SIZE));
			sprite->setAnchorPoint(Point(0, 0));
			this->addChild(sprite, OTHERS_PRIORITY_Z_ORDER + customObject->order, OTHERS_TAG + customObject->idx);
		}
		com->isOtherThrowItem = false;
	}
}

void HelloWorld::editBoxReturn(EditBox * editBox)
{
	if (this->mainUser->isLogin != true)
	{
		if (loginPopUp->isVisible() == true)
		{
			char userName[50];
			strcpy(userName, loginID->getText());
			if (com->requestLogin(userName) == -1)
			{
				com->comm = false;
			}
		}

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
	com->chatting(com->mainUser->name, message->getCString(), this->mainUser->field);
	element.pushBack(String::createWithFormat("%s : %s", this->mainUser->name, message->getCString()));
	com->chattingInfo.pushBack(String::createWithFormat("%s : %s", this->mainUser->name, message->getCString()));

	//캐릭터 위에 말풍선으로 문자열 출력.
	this->mainUser->balloon->setVisible(true);
	this->mainUser->balloonContent->setString(message->getCString());
	this->mainUser->balloonContent->setVisible(true);
	this->mainUser->balloonTime = 0;
}

void HelloWorld::editBoxEditingDidBegin(EditBox * editBox)
{
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::editBoxEditingDidEnd(EditBox * editBox)
{
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::editBoxTextChanged(EditBox * editBox, const std::string& text)
{
	if (this->mainUser->isLogin != true)
		return;
}

void HelloWorld::scrollViewDidScroll(ScrollView* view)
{
}

void HelloWorld::scrollViewDidZoom(ScrollView* view)
{
}

//셀을 터치하면 콜백
void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

Size HelloWorld::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(60, 10);
}

//reload가 호출되거나, 스크롤이 움직여 안보이는 셀이 보여질 때 호출
TableViewCell* HelloWorld::tableCellAtIndex(TableView *table, ssize_t idx)
{
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
		LabelTTF * label = (LabelTTF*)cell->getChildByTag(CHATTING_VIEW_ELEMENT);
		label->setString(string->getCString());
	}

	return cell;
}

//테이블이 셀 갯수에 대한 정보를 가져가는 곳
ssize_t HelloWorld::numberOfCellsInTableView(TableView *table)
{
	int count = element.size();
	return count;
}

void HelloWorld::joinButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (joinPopUp->isVisible() == false)
		{
			loginBackground->setVisible(false);
			joinButton->setVisible(false);
			loginButton->setVisible(false);
			loginPopUp->setVisible(false);
			loginID->setVisible(false);
			loginFailPopUp->setVisible(false);
			passwordModiButton->setVisible(false);
			backGroundStoryButton->setVisible(false);
			makePeoplesButton->setVisible(false);
			exitButton->setVisible(false);

			joinPopUp->setVisible(true);
			joinID->setVisible(true);
			joinOK->setVisible(true);
			joinCancel->setVisible(true);
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::joinOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		char userID[50];
		strcpy(userID, joinID->getText());

		//해당 아이디로 가입요청
		if (com->requestJoin(userID) == -1)
		{
			com->comm = false;
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::joinCancelButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (joinPopUp->isVisible() == true)
		{
			loginBackground->setVisible(true);
			joinButton->setVisible(true);
			loginButton->setVisible(true);
			passwordModiButton->setVisible(true);
			backGroundStoryButton->setVisible(true);
			makePeoplesButton->setVisible(true);
			exitButton->setVisible(true);

			joinPopUp->setVisible(false);
			joinID->setVisible(false);
			joinOK->setVisible(false);
			joinCancel->setVisible(false);
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::joinFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (joinFailPopUp->isVisible() == true)
		{
			joinFailPopUp->setVisible(false);
			joinFailPopUpOk->setVisible(false);
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::loginButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (loginPopUp->isVisible() == false)
		{
			loginPopUp->setVisible(true);
			loginID->setVisible(true);
		}
		else
		{
			loginPopUp->setVisible(false);
			loginID->setVisible(false);
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::loginFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (loginFailPopUp->isVisible() == true)
		{
			loginFailPopUp->setVisible(false);
			loginFailPopUpOk->setVisible(false);
			com->popupLoginFail = false;
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::passwordModiButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::backGroundStoryButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::makePeoplesButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::exitButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

void HelloWorld::commErrorPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		if (commErrorPopUp->isVisible() == true)
		{
			commErrorPopUp->setVisible(false);
			commErrorPopUpOk->setVisible(false);
			CCDirector::sharedDirector()->end();
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void HelloWorld::inventoryButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
    
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            if(inventory->isVisible() == true)
                inventory->setVisible(false);
            else
                inventory->setVisible(true);
            break;
        case Widget::TouchEventType::MOVED:
            break;
        case Widget::TouchEventType::ENDED:
            break;
    }
}

void HelloWorld::itemEatButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
    int order = 0;
    
    //획득한 아이템
    CustomObject * customObject;
    int customObjectIndex;
    
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            customObject = NULL;
            
            //해당 위치에 아이템이 있는지 확인한다.
            for (int i = 0; i < com->objectInfo->size(); i++)
            {
                CustomObject * imsiCustomObject = com->objectInfo->at(i);
                if (imsiCustomObject->xpos == this->mainUser->xpos && imsiCustomObject->ypos == this->mainUser->ypos)
                    if (order < imsiCustomObject->order)
                    {
                        customObject = imsiCustomObject;
                        customObjectIndex = i;
                    }
            }
            
            //캐릭터가 위치한 곳에 아이템이 있을시 아이템을 맵에서 지우고 인벤토리창에 추가한다.
            if (customObject != NULL)
            {
                if(addInventoryItem(customObject) == 1)
                    com->objectInfo->erase(com->objectInfo->begin() + customObjectIndex);
            }
            break;
        case Widget::TouchEventType::MOVED:
            break;
        case Widget::TouchEventType::ENDED:
            break;
    }
}
#endif

int HelloWorld::addInventoryItem(CustomObject * customObject)
{
	int result;

	//아이템 창이 꽉 차있나 확인.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (com->inventory_items_Info[i][j] == NULL)
			{
				//아이템을 주워서 아이템창에 추가하는 과정.
				Sprite * imsiGetItem = (Sprite*)this->getChildByTag(OTHERS_TAG + customObject->idx);
				Sprite * inventoryItem = Sprite::createWithTexture(imsiGetItem->getTexture());
				inventoryItem->setAnchorPoint(Point(0, 0));
				inventoryItem->setPosition(Point(j * TILE_SIZE, (3 - (i + 1)) * TILE_SIZE));

				StructCustomObject imsiStructCustomObject;
				imsiStructCustomObject.idx = customObject->idx;
				strcpy(imsiStructCustomObject.name, customObject->name);
				strcpy(imsiStructCustomObject.type, customObject->type);
				imsiStructCustomObject.xpos = j;
				imsiStructCustomObject.ypos = (3 - (i + 1));
				imsiStructCustomObject.order = customObject->order;
				strcpy(imsiStructCustomObject.fileDir, customObject->fileDir);
				imsiStructCustomObject.count = customObject->count;

				//아이템 획득 사실을 서버에 알림
				if (com->eatFieldItem(imsiStructCustomObject) <= 0)
				{
				}

				//인벤토리창에 아이템을 넣고
				this->removeChild(imsiGetItem);
				inventory->addChild(inventoryItem, INVENTORY_PRIORITY_Z_ORDER + 1, INVENTORY_ITEM + customObject->idx);
				customObject->xpos = j;
				customObject->ypos = 2 - i;

				//인벤토리 아이템 목록에 추가한다.
				com->inventory_items_Info[i][j] = customObject;
				return 1;
			}
			else
			{
				//이미 아이템창이 동일종류의 아이템이 있으면 수량만 증가시킨다.
				if (com->inventory_items_Info[i][j]->idx == customObject->idx)
				{
					StructCustomObject imsiStructCustomObject;
					imsiStructCustomObject.idx = customObject->idx;
					strcpy(imsiStructCustomObject.name, customObject->name);
					strcpy(imsiStructCustomObject.type, customObject->type);
					imsiStructCustomObject.xpos = j;
					imsiStructCustomObject.ypos = (3 - (i + 1));
					imsiStructCustomObject.order = customObject->order;
					strcpy(imsiStructCustomObject.fileDir, customObject->fileDir);
					imsiStructCustomObject.count = customObject->count;

					//아이템 획득 사실을 서버에 알림
					if (com->eatFieldItem(imsiStructCustomObject) <= 0)
					{
					}

					Sprite * imsiGetItem = (Sprite*)this->getChildByTag(OTHERS_TAG + customObject->idx);
					this->removeChild(imsiGetItem);

					com->inventory_items_Info[i][j]->count += customObject->count;
					return 1;
				}
			}
		}
	}

	return 0;
}

void HelloWorld::createTileMap(char * mapName)
{
	//기존 타일맵 소스 삭제
	if (tmap != NULL)
	{
		tmap->setVisible(false);
		this->removeChildByTag(MAP_TAG);
		tmap = NULL;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	com->sendCommand(REQUEST_TILED_MAP, mapName, strlen(mapName));

	while (com->getTiledMap != true);
    com->getTiledMap = false;

	tmap = TMXTiledMap::createWithXML((char*)com->tiledMapBuf->data(), "");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tmap = TMXTiledMap::create(mapName);
#endif
    
	background = tmap->getLayer("Background");
	address = background->getProperty("Address").asString();

	items = tmap->getLayer("Items");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	objects = tmap->getObjectGroup("Objects");
	this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);
	//

	//기존의 맵 아이템 소스 삭제
	for (int i = 0; i < com->objectInfo->size(); i++)
	{
		CustomObject * imsiObjectInfo = com->objectInfo->at(i);
		this->removeChildByTag(OTHERS_TAG + imsiObjectInfo->idx);
		this->removeChildByTag(OTHERS_TAG + imsiObjectInfo->idx);
	}
	com->objectInfo->clear();

	//기존의 맵 몬스터 소스 삭제
	for (int i = 0; i < com->monsterInfo->size(); i++)
	{
		CustomObject * imsiMonsterInfo = com->monsterInfo->at(i);
		this->removeChildByTag(MONSTER_TAG + imsiMonsterInfo->idx);
		this->removeChildByTag(MONSTER_TAG + imsiMonsterInfo->idx);
	}
	//
	com->monsterInfo->clear();

	com->sendCommand(REQUEST_FIELD_INFO, mapName, strlen(mapName));

	while (com->isObjectBufferFill != true);
	com->isObjectBufferFill = false;
}

void HelloWorld::createObject()
{	
	//맵의 아이템 구현하기
	for (int i = 0; i < (int)com->objectInfo->size(); i++)
	{
        CustomObject * customObject = (com->objectInfo)->at(i);
        Sprite * sprite;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		Image* image = new Image();
		Texture2D* texture = new Texture2D();

		com->sendCommand(REQUEST_IMAGE, customObject->fileDir, strlen(customObject->fileDir));

		//요청한 오브젝트 이미지가 수신되길 기다린다.
		while (com->getImage != true);

		//수신된 버퍼데이터로 이미지를 만든다.
		image->initWithImageData(&(com->imageBuf->front()), com->imageBuf->size());
		com->getImage = false;

		//스프라이트를 만들어 맵에 구현한다.
		texture->initWithImage(image);

		sprite = Sprite::createWithTexture(texture);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sprite = Sprite::create(customObject->fileDir);
#endif
        
		sprite->setPosition(Point(customObject->xpos * TILE_SIZE, customObject->ypos * TILE_SIZE));
		sprite->setAnchorPoint(Point(0, 0));
		this->addChild(sprite, OTHERS_PRIORITY_Z_ORDER + customObject->order, OTHERS_TAG + customObject->idx);
	}
}

void HelloWorld::createMonster()
{
	for (int i = 0; i < com->monsterInfo->size(); i++)
	{
        CustomObject * customObject = (com->monsterInfo)->at(i);
        Sprite * sprite;
        char fileDir[100];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		Image* image = new Image();
		Texture2D* texture = new Texture2D();

		sprintf(fileDir, "%s.png", customObject->fileDir);
		com->sendCommand(REQUEST_IMAGE, fileDir, strlen(fileDir));

		//요청한 오브젝트 이미지가 수신되길 기다린다.
		while (com->getImage != true);

		//수신된 버퍼데이터로 이미지를 만든다.
		image->initWithImageData(&(com->imageBuf->front()), com->imageBuf->size());
		com->getImage = false;

		//스프라이트를 만들어 맵에 구현한다.
		texture->initWithImage(image);

		//.list 를 요청한다.
		com->getTiledMap = false;
		sprintf(fileDir, "%s.plist", customObject->fileDir);
		com->sendCommand(REQUEST_TILED_MAP, fileDir, strlen(fileDir));

		while (com->getTiledMap != true);

		SpriteFrameCache::getInstance()->addSpriteFramesWithFileContent((char*)com->tiledMapBuf->data(), texture);

		sprintf(fileDir, "%s_01.png", customObject->name);
		sprite = Sprite::createWithSpriteFrameName(fileDir);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sprintf(fileDir, "%s.plist", customObject->fileDir);
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileDir);
        
        sprintf(fileDir, "%s_01.png", customObject->name);
        sprite = Sprite::createWithSpriteFrameName(fileDir);
#endif
		sprite->setPosition(Point(customObject->xpos * TILE_SIZE + TILE_SIZE / 2, customObject->ypos * TILE_SIZE));
		sprite->setAnchorPoint(Point(0.5, 0));
		this->addChild(sprite, MONSTER_PRIORITY_Z_ORDER, MONSTER_TAG + customObject->idx);
	}
}

void HelloWorld::createOtherUser()
{
	for (int i = 0; i < com->usersInfo->size(); i++)
	{
		User * user = com->usersInfo->at(i);

		if (user->sprite == NULL)
		{
			//다른 유저의 모습을 출력한다.
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/man.plist");

			switch (user->seeDirection)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				user->sprite = Sprite::createWithSpriteFrameName("man_13.png");
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				user->sprite = Sprite::createWithSpriteFrameName("man_01.png");
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				user->sprite = Sprite::createWithSpriteFrameName("man_09.png");
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				user->sprite = Sprite::createWithSpriteFrameName("man_05.png");
				break;
			default:
				user->sprite = Sprite::createWithSpriteFrameName("man_01.png");
				break;
			}

			user->sprite->setAnchorPoint(Point(0.5, 0));
			user->isAction = false;
			user->isRunning = false;
			user->isKeepKeyPressed = false;
			user->position = Point(user->xpos * TILE_SIZE + TILE_SIZE / 2, user->ypos * TILE_SIZE);
			user->sprite->setPosition(user->position);
			this->addChild(user->sprite, OTHERS_USERS_Z_ORDER, OTHERS_USERS + i);

			//캐릭터 위에 말풍선으로 문자열 출력.
			user->balloon = Sprite::create("Images/balloon.png");
			user->balloon->setAnchorPoint(Point(1, 0));
			user->balloon->setPosition(Point(user->position.x + user->sprite->getContentSize().width / 2, user->position.y + user->sprite->getContentSize().height));
			user->balloon->setVisible(false);
			this->addChild(user->balloon, BALLON_PRIORITY_Z_ORDER, CHATTING_BALLOON);

			user->balloonContent = LabelTTF::create("", "NanumBarunGothic", 15);
			user->balloonContent->setColor(Color3B::BLACK);
			user->balloonContent->setAnchorPoint(Point(1, 0));
			user->balloonContent->setPosition(Point(user->position.x + user->sprite->getContentSize().width / 2 - 50, user->position.y + user->sprite->getContentSize().height + 50));
			user->balloonContent->setVisible(false);
			this->addChild(user->balloonContent, BALLON_CONTENT_PRIORITY_Z_ORDER, CHATTING_BALLOON_CONTENT);

			//채팅창에 다른유저의 접속을 알림
			element.pushBack(String::createWithFormat("%s is Login !!", user->name));
			com->chattingInfo.pushBack(String::createWithFormat("%s is Login !!", user->name));
		}
		else
		{
			if (user->sprite->isVisible() == false)
			{
				//채팅창에 다른유저의 접속종료를 알림
				element.pushBack(String::createWithFormat("%s is Logout !!", user->name));
				com->chattingInfo.pushBack(String::createWithFormat("%s is Logout !!", user->name));

				this->removeChild(user->sprite);
				user->sprite = NULL;
				com->usersInfo->erase(com->usersInfo->begin() + i);
				i--;
			}
			else
			{
				//방향키를 몇초동안 누르고 있느냐에 따라 정지와 이동을 결정
				if (user->isKeepKeyPressed == true)
					user->isRunning = true;

				if (user->isRunning == true && user->isAction == false)
					this->setOtherUsersAnimation(user, user->seeDirection);
			}
		}
	}
}

void HelloWorld::createInventory()
{
    com->sendCommand(REQUEST_INVENTORY_ITEM_INFO, com->mainUser->name, strlen(com->mainUser->name));
    
    //아이템창 정보를 받아서 구현
    while (com->isInventoryFill != true);
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (com->inventory_items_Info[i][j] != NULL)
            {
                Sprite * sprite;
                Image* image = new Image();
                Texture2D* texture = new Texture2D();
                CustomObject * customObject = com->inventory_items_Info[i][j];
                
                com->sendCommand(REQUEST_IMAGE, customObject->fileDir, strlen(customObject->fileDir));
                
                //요청한 오브젝트 이미지가 수신되길 기다린다.
                while (com->getImage != true);
                
                //수신된 버퍼데이터로 이미지를 만든다.
                image->initWithImageData(&(com->imageBuf->front()), com->imageBuf->size());
                com->getImage = false;
                
                //스프라이트를 만들어 맵에 구현한다.
                texture->initWithImage(image);
                sprite = Sprite::createWithTexture(texture);
                sprite->setPosition(Point(customObject->xpos * TILE_SIZE, customObject->ypos * TILE_SIZE));
                sprite->setAnchorPoint(Point(0, 0));
                
                inventory->addChild(sprite, INVENTORY_PRIORITY_Z_ORDER + 1, INVENTORY_ITEM + customObject->idx);
            }
        }
    }
    
    com->isInventoryFill = false;
    //
}
