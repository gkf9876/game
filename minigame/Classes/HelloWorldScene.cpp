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
	this->schedule(schedule_selector(HelloWorld::update), 0.15);

	return true;
}

void HelloWorld::start()
{
	//맵 세팅. 서버로부터 타일맵 데이터를 받아서 구현한다.
	address = "TileMaps/KonyangUniv.Daejeon/JukhunDigitalFacilitie/floor_08/";
	std::string map = address;
	map.append("floor.tmx");

	com->getTiledMap = false;
	if (fieldInfo != NULL)
	{
		free(fieldInfo);
		fieldInfo = NULL;
	}
	fieldInfo = (char*)malloc(strlen(this->mainUser->field) + strlen("Resources/") + 1);
	strcpy(fieldInfo, "Resources/");
	strcat(fieldInfo, this->mainUser->field);
	com->sendCommand(REQUEST_TILED_MAP, fieldInfo, strlen(fieldInfo));
	free(fieldInfo);
	fieldInfo = NULL;
	while (com->getTiledMap != true);

	tmap = TMXTiledMap::createWithXML((char*)com->tiledMapBuf->data(), "");
	currentFlag = com->mainUser->field;
	background = tmap->getLayer("Background");
	items = tmap->getLayer("Items");
	metainfo = tmap->getLayer("MetaInfo");
	metainfo->setVisible(false);
	this->addChild(tmap, MAP_PRIORITY_Z_ORDER, MAP_TAG);
	//

	//플레이어 만들기
	objects = tmap->getObjectGroup("Objects");
	this->mainUser->position = Point(com->mainUser->xpos * TILE_SIZE + TILE_SIZE / 2, com->mainUser->ypos * TILE_SIZE);

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
#endif

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

	tableView = TableView::create(this, Size(200, 120));
	tableView->setAnchorPoint(Point(0, 0));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setDelegate(this);
	this->addChild(tableView, MAP_NAME_PRIORITY_Z_ORDER, CHATTING_VIEW);

	//tableCellAtIndex 등등 데이터 소스를 다시 한번 부른다. 테이블 뷰를 다시 그리므로 처음으로 포커스가 맞춰진다.
	tableView->reloadData();

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

	//listener->setEnabled(true);

	listener1->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener1->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void HelloWorld::onExit()
{
	if (fieldInfo != NULL)
		free(fieldInfo);

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
	//this->mainUser->seeDirection = cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW;
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
	CCLOG("-- onTouchesBegan --");
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
	for (int i = 0; i<touches.size(); i++)
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
				CCLOG("Catch!!");
				joystick->setPosition(tap);
				joystickTouched = true;
			}
#endif
		}
		it++;
	}
}


void HelloWorld::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
	CCLOG("-- onTouchesMoved --");
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//조이스틱 터치시
	if (joystickTouched)
	{
		for (int i = 0; i<touches.size(); i++)
		{
			touch = (Touch*)(*it);

			//터치가 된경우
			if (touch)
			{
				tap = touch->getLocation() + plag;
				//CCLOG("%f , %f", tap.x, tap.y);

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
						//CCLOG("Touch Direction : DOWN, %d", joystickDirectionSet);
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
                            CCLOG("onKeyPressed...");
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW, NULL);
						}
					}
					else
					{
						//CCLOG("Touch Direction : RIGHT, %d", joystickDirectionSet);
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
                            CCLOG("onKeyPressed...");
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, NULL);
						}
					}
				else
					if (ntap.y < -1 * incline * ntap.x + yAxisValueDown)
					{
						//CCLOG("Touch Direction : LEFT, %d", joystickDirectionSet);
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
                            CCLOG("onKeyPressed...");
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW, NULL);
						}
					}
					else
					{
						//CCLOG("Touch Direction : UP, %d", joystickDirectionSet);
                        if(this->mainUser->seeDirection != cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
                            joystickDirectionSet = false;
                        
						if (joystickDirectionSet == false)
						{
							joystickDirectionSet = true;
                            CCLOG("onKeyPressed...");
							onKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW, NULL);
						}
					}

				joystick->setPosition(ntap);
			}
			it++;
		}
	}
#endif

}


void HelloWorld::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
	CCLOG("-- onTouchesEnded --");

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

void HelloWorld::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    if (this->mainUser->isLogin != true)
    {
        return;
    }
    
	CCLOG("-- onTouchesCancelled --");
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

	int tileGid = this->metainfo->getTileGIDAt(tileCoord);

	Point regionPoint = this->mainUser->position;


	//이동할 위치에 다른 유저가 있을시
	for (int i = 0; i < com->usersInfo->size(); i++)
	{
		User * user = com->usersInfo->at(i);

		if (user->position == position)
		{
			CCLOG("other user...");
			return;
		}
	}

	if (tileGid)
	{
		Value properties = tmap->getPropertiesForGID(tileGid);

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

				//기존 타일맵 소스 삭제
				tmap->setVisible(false);
				this->removeChildByTag(MAP_TAG);

				//새 타일맵 서버에 요청
				com->getTiledMap = false;
				if (fieldInfo != NULL)
				{
					free(fieldInfo);
					fieldInfo = NULL;
				}
				fieldInfo = (char*)malloc(strlen(destination.data()) + strlen("Resources/") + 1);
				strcpy(fieldInfo, "Resources/");
				strcat(fieldInfo, destination.data());
				com->sendCommand(REQUEST_TILED_MAP, fieldInfo, strlen(fieldInfo));
				free(fieldInfo);
				fieldInfo = NULL;
				while (com->getTiledMap != true);

				//서버에서 받은 타일맵 소스를 표시
				tmap = TMXTiledMap::createWithXML((char*)com->tiledMapBuf->data(), "");
				tmap->setVisible(true);
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
				
				this->mainUser->position = Point(x, y);
				this->mainUser->sprite->setPosition(this->mainUser->position);
				this->setViewpointCenter(this->mainUser->sprite->getPosition());

				//맵의 이름을 화면 상단에 출력.
				mapName->setString(objects->getProperty("Name").asString());

				//지금 맵의 다른 유저들을 안보이게 한다.
				for (int i = 0; i < com->usersInfo->size(); i++)
				{
					User * user = com->usersInfo->at(i);
					user->sprite->setVisible(false);
				}

				//이동한 내용을 DB에 반영
				char sendMapName[100];
				strcpy(sendMapName, String(currentFlag).getCString());
				writeSize = com->userMoveUpdate(com->mainUser->name, Point(regionPoint.x / TILE_SIZE, regionPoint.y / TILE_SIZE), this->mainUser->field,
					cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), sendMapName, this->mainUser->seeDirection);
				if (writeSize == -1)
				{
					com->comm = false;
				}
				strcpy(this->mainUser->field, sendMapName);

				return;
			}
		}
	}

	this->mainUser->position = position;
	this->mainUser->sprite->setPosition(this->mainUser->position);

	CCLOG("xpos : %d, ypos : %d", (int)position.x, (int)position.y);

	//이동한 내용을 DB에 반영
	Value properties = tmap->getPropertiesForGID(tileGid);
	char sendMapName[100];
	strcpy(sendMapName, String(currentFlag).getCString());
	writeSize = com->userMoveUpdate(com->mainUser->name, Point(regionPoint.x / TILE_SIZE, regionPoint.y / TILE_SIZE), this->mainUser->field,
		cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), sendMapName, this->mainUser->seeDirection);
	if (writeSize == -1)
	{
		com->comm = false;
	}
	strcpy(this->mainUser->field, sendMapName);

	//플레이어가 화면 가운데로 오게 조절
	this->setViewpointCenter(this->mainUser->sprite->getPosition());
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	CCLOG("-- onKeyPressed --");

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

	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		//제자리에서 방향키눌러 방향전환할때
		if (this->mainUser->seeDirection != key && this->mainUser->isRunning != true)
		{
			this->mainUser->sprite->setSpriteFrame("man_13.png");

			//방향전환을 서버에 알려준다.
			writeSize = com->userMoveUpdate(com->mainUser->name, cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field,
				cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field, key);
			if (writeSize == -1)
			{
				com->comm = false;
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

			//방향전환을 서버에 알려준다.
			writeSize = com->userMoveUpdate(com->mainUser->name, cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field,
				cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field, key);
			if (writeSize == -1)
			{
				com->comm = false;
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

			//방향전환을 서버에 알려준다.
			writeSize = com->userMoveUpdate(com->mainUser->name, cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field,
				cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field, key);
			if (writeSize == -1)
			{
				com->comm = false;
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

			//방향전환을 서버에 알려준다.
			writeSize = com->userMoveUpdate(com->mainUser->name, cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field,
				cocos2d::Point(this->mainUser->sprite->getPosition().x / TILE_SIZE, this->mainUser->sprite->getPosition().y / TILE_SIZE), this->mainUser->field, key);
			if (writeSize == -1)
			{
				com->comm = false;
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
	}
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (this->mainUser->isLogin != true)
		return;

	CCLOG("KeyRelease..(%d) %d", key, this->mainUser->seeDirection);

	//여러개의 방향키가 눌렸을 경우 가장 마지막으로 키를 땠을때 이동을 중지한다.
	if (this->mainUser->seeDirection == key)
	{
		this->mainUser->isKeepKeyPressed = false;
		this->mainUser->isRunning = false;
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
	auto actionStartCallback = CallFunc::create(this, callfunc_selector(HelloWorld::actionStarted));
	auto actionFinishCallback = CallFunc::create(this, callfunc_selector(HelloWorld::actionFinished));

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
	CCLOG("action finished!");

}

void HelloWorld::update(float fDelta)
{
	updateTime += fDelta;

	if (updateTime >= 60)
		updateTime = 0;

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
			CCLOG(String::createWithFormat("%s is Login !!", user->name)->getCString());

			tableView->reloadData();
			tableView->setContentOffset(Vec2(0, 0), false);
		}
		else
		{
			if (user->sprite->isVisible() == false)
			{
				//채팅창에 다른유저의 접속종료를 알림
				element.pushBack(String::createWithFormat("%s is Logout !!", user->name));
				com->chattingInfo.pushBack(String::createWithFormat("%s is Logout !!", user->name));
				CCLOG(String::createWithFormat("%s is Logout !!", user->name)->getCString());

				tableView->reloadData();
				tableView->setContentOffset(Vec2(0, 0), false);

				this->removeChild(user->sprite);
				user->sprite = NULL;
				com->usersInfo->erase(com->usersInfo->begin() + i);
				i--;
			}
			else
			{
				if (user->isAction == true && user->isRunning == true)
				{
					this->setOtherUsersAnimation(user, user->seeDirection);

					//액션을 취하고 가만히 있을때
					user->isAction = false;
					user->isRunning = false;
				}

				if(user->isAction == true && user->isRunning == false)
				{
					CCLOG("제자리 방향 전환");
					switch (user->seeDirection)
					{
					case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
						user->sprite->setSpriteFrame("man_13.png");
						break;
					case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
						user->sprite->setSpriteFrame("man_01.png");
						break;
					case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
						user->sprite->setSpriteFrame("man_09.png");
						break;
					case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
						user->sprite->setSpriteFrame("man_05.png");
						break;
					default:
						user->sprite->setSpriteFrame("man_01.png");
						break;
					}

					//액션을 취하고 가만히 있을때
					user->isAction = false;
					user->isRunning = false;
				}
			}
		}
	}

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
			}

			//유저 정보 불러올때까지 대기.
			while (com->isGetUserInfo != true);
			strcpy(this->mainUser->name, com->mainUser->name);
			strcpy(this->mainUser->field, com->mainUser->field);
			this->mainUser->xpos = com->mainUser->xpos;
			this->mainUser->ypos = com->mainUser->ypos;
			this->mainUser->seeDirection = com->mainUser->seeDirection;

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

	if (this->mainUser->isKeepKeyPressed == true)
	{
		readyCount++;

		if (readyCount == 3)
		{
			this->mainUser->isRunning = true;
			readyCount = 0;
		}
	}

	//메인유저가 방향키를 눌러 이동시 실행
	if (this->mainUser->isRunning == true && this->mainUser->isAction == false)
	{
		CCLOG("Keep Running..");

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
	if (this->mainUser->balloonTime % 120 == 119)
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
		if (othersUser->balloonTime % 120 == 119)
		{
			othersUser->balloon->setVisible(false);
			othersUser->balloonContent->setVisible(false);
			othersUser->balloonTime = 0;
		}
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

	//서버와 통신 두절시 종료한다.
	if (com->comm == false)
	{
		CCLOG("Comm Error!!");
		commErrorPopUp->setVisible(true);
		commErrorPopUpOk->setVisible(true);
	}

	//일정 주기로 접속시간 업데이트
	if (com->updateLoginTime(this->mainUser->name) == -1)
	{
		CCLOG("Comm Error!!");
		com->comm = false;
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

			CCLOG("isLogin : %d", com->isLogin);
		}

		return;
	}

	CCLOG("--- editBoxReturn ---");

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
	this->mainUser->balloon->setVisible(true);
	this->mainUser->balloonContent->setString(message->getCString());
	this->mainUser->balloonContent->setVisible(true);
	this->mainUser->balloonTime = 0;
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
		LabelTTF * label = (LabelTTF*)cell->getChildByTag(CHATTING_VIEW_ELEMENT);
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

void HelloWorld::joinButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("JOIN BUTTON TOUCH BEGAN");
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
		CCLOG("JOIN BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("JOIN BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::joinOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("JOIN OK BUTTON TOUCH BEGAN");
		char userID[50];
		strcpy(userID, joinID->getText());

		//해당 아이디로 가입요청
		if (com->requestJoin(userID) == -1)
		{
			com->comm = false;
		}
		CCLOG("Request Join : (ID : %s)", userID);
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("JOIN OK BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("JOIN OK BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::joinCancelButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("JOIN CANCEL BUTTON TOUCH BEGAN");
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
		CCLOG("JOIN CANCEL BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("JOIN CANCEL BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::joinFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("JOIN FAIL OK BUTTON TOUCH BEGAN");
		if (joinFailPopUp->isVisible() == true)
		{
			joinFailPopUp->setVisible(false);
			joinFailPopUpOk->setVisible(false);
		}
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("JOIN FAIL OK BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("JOIN FAIL OK BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::loginButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("LOGIN BUTTON TOUCH BEGAN");
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
		CCLOG("LOGIN BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("LOGIN BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::loginFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("LOGIN FAIL OK BUTTON TOUCH BEGAN");
		if (loginFailPopUp->isVisible() == true)
		{
			loginFailPopUp->setVisible(false);
			loginFailPopUpOk->setVisible(false);
			com->popupLoginFail = false;
		}
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("LOGIN FAIL OK BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("LOGIN FAIL OK BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::passwordModiButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("PasswordModi BUTTON TOUCH BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("PasswordModi BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("PasswordModi BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::backGroundStoryButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("BackGroundStory BUTTON TOUCH BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("BackGroundStory BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("BackGroundStory BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::makePeoplesButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("MakePeople BUTTON TOUCH BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("MakePeople BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("MakePeople BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::exitButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("EXIT BUTTON TOUCH BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("EXIT BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("EXIT BUTTON TOUCH ENDED");
		break;
	}
}

void HelloWorld::commErrorPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("COMM ERROR POPUP OK BUTTON TOUCH BEGAN");
		if (commErrorPopUp->isVisible() == true)
		{
			commErrorPopUp->setVisible(false);
			commErrorPopUpOk->setVisible(false);
			CCDirector::sharedDirector()->end();
		}
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("COMM ERROR POPUP OK BUTTON TOUCH MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("COMM ERROR POPUP OK BUTTON TOUCH ENDED");
		break;
	}
}