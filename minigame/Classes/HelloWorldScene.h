#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CustomTableViewCell.h"
#include "CustomNetworkCommunication.h"
#include "User.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#define TILE_SIZE	32														//타일 사이즈

#define DRAGON_TAG 10														//화면에서 그리는 개체 태그.
#define MAP_TAG 11
#define HOME_TAG 1
#define OTHERS_TAG 2
#define TESTDESERT_TAG 2
#define MAP_NAME		3
#define TITLE			4
#define INVENTORY		5
#define CHATTING_INPUT	6
#define CHATTING_VIEW	7
#define CHATTING_VIEW_ELEMENT 8
#define CHATTING_BALLOON 9
#define CHATTING_BALLOON_CONTENT 10
#define OTHERS_USERS						500
#define LOGIN_WINDOW	11
#define JOIN_BUTTON		12
#define LOGIN_BUTTON	13
#define LOGIN_TEXT_INPUT	14
#define LOGIN_FAIL_POPUP		15
#define PASSWORDMODI_BUTTON	16
#define BACKGROUNDSTORY_BUTTON	17
#define MAKEPEOPLE_BUTTON	18
#define EXIT_BUTTON	19
#define LOGIN_POPUP	20
#define JOIN_POPUP	21
#define JOIN_OK_BUTTON	22
#define JOIN_CANCEL_BUTTON	23
#define JOIN_TEXT_INPUT	24
#define LOGIN_FAIL_POPUP_OK 25
#define JOIN_FAIL_POPUP 26
#define JOIN_FAIL_POPUP_OK 27
#define COMM_ERROR_POPUP 28
#define COMM_ERROR_POPUP_OK 29
#define MAP_OBJECT 1000

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define JOYSTICK        11
#endif


//수치가 높을수록 그림이 맨 위에 위치.
#define MAP_PRIORITY_Z_ORDER			1										//맵 우선순위	
#define DRAGON_PRIORITY_Z_ORDER			200										//드래곤 우선순위
#define OTHERS_PRIORITY_Z_ORDER			2										//기타 객체 우선순위
#define BALLON_PRIORITY_Z_ORDER			201										//말풍선 우선순위
#define BALLON_CONTENT_PRIORITY_Z_ORDER	202										//말풍선 내용 우선순위
#define INVENTORY_PRIORITY_Z_ORDER		203										//아이템창 우선순위
#define TITLE_PRIORITY_Z_ORDER			204										//맵 이름 간판 우선순위
#define MAP_NAME_PRIORITY_Z_ORDER		205										//맵 이름 우선순위
#define OTHERS_USERS_Z_ORDER			200										//다른 유저 모습 우선순위

#define LOGIN_WINDOW_Z_ORDER			1										//로그인 화면 우선순위
#define JOIN_BUTTON_Z_ORDER				2										//회원가입 버튼
#define JOIN_POPUP_Z_ORDER				3										//회원가입 창
#define JOIN_TEXT_INPUT_Z_ORDER			4										//회원가입 아이디 입력창 우선순위
#define JOIN_OK_BUTTON_Z_ORDER			4										//회원가입 확인버튼
#define JOIN_CANCEL_BUTTON_Z_ORDER		4										//회원가입 취소버튼
#define JOIN_FAIL_POPUP_Z_ORDER			4										//회원가입 실패 메시지창
#define JOIN_FAIL_POPUP_OK_Z_ORDER		5										//회원가입 실패창 확인버튼
#define LOGIN_BUTTON_Z_ORDER			2										//로그인 버튼
#define LOGIN_POPUP_Z_ORDER				3										//로그인 창
#define LOGIN_TEXT_INPUT_Z_ORDER		4										//아이디 입력창 우선순위
#define LOGIN_FAIL_POPUP_Z_ORDER		4										//로그인 실패 메시지창
#define LOGIN_FAIL_POPUP_OK_Z_ORDER		5										//로그인 실패창 확인버튼
#define PASSWORDMODI_BUTTON_Z_ORDER		2										//암호변경 버튼
#define BACKGROUNDSTORY_BUTTON_Z_ORDER	2										//배경이야기 버튼
#define MAKEPEOPLE_BUTTON_Z_ORDER		2										//만든사람들 버튼
#define EXIT_BUTTON_Z_ORDER				2										//종료 버튼
#define COMM_ERROR_POPUP_Z_ORDER		206										//통신두절시 종료창
#define COMM_ERROR_POPUP_OK_Z_ORDER		207										//통신두절시 확인버튼

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define JOYSTICK_PRIORITY_Z_ORDER    7											//조이스틱 우선순위
#endif

class HelloWorld : public cocos2d::Layer, public EditBoxDelegate, public TableViewDataSource, public TableViewDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	Sprite * loginBackground;												//로그인 화면
	Button * joinButton;													//회원가입 버튼
	Sprite * joinPopUp;														//회원가입 창
	EditBox * joinID;														//회원가입 ID 입력칸
	Button * joinOK;														//회원가입 확인
	Button * joinCancel;													//회원가입 취소
	Sprite * joinFailPopUp;													//회원가입 실패창
	Button * joinFailPopUpOk;												//회원가입 실패창 버튼
	Button * loginButton;													//로그인 버튼
	Sprite * loginPopUp;													//로그인 창
	EditBox * loginID;														//로그인 ID 입력칸
	Sprite * loginFailPopUp;												//로그인 실패창
	Button * loginFailPopUpOk;												//로그인 실패창 버튼
	Button * passwordModiButton;											//암호변경 버튼
	Button * backGroundStoryButton;											//배경이야기 버튼
	Button * makePeoplesButton;												//만든사람들 버튼
	Button * exitButton;													//종료 버튼
	Sprite * commErrorPopUp;												//통신두절시 종료창
	Button * commErrorPopUpOk;												//통신실패창 확인버튼

	User * mainUser;

	cocos2d::Size winSize;													//화면 크기
	cocos2d::Vec2 origin;													//화면 좌표

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
	std::string currentFlag;												//현재 있는 맵 경로

	cocos2d::ui::EditBox * chattingInput;									//채팅 입력창

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	cocos2d::Sprite * joystick;												//조이스틱
	cocos2d::Sprite * joystickPad;											//조이스틱 패드
	bool joystickTouched;													//조이스틱이 터치됬는지 확인
	bool joystickDirectionSet;												//조이스틱으로 방향 조절하고 있는지 확인
#endif

	cocos2d::LabelTTF * text;												//테스트 할때 사용하는 객체.

	TableView* tableView = NULL;											//채팅 띄우는 창
	LabelTTF* showLabel;													//채팅창에 띄울 문장
	Vector<String*> element;												//채팅창 채팅목록

	CustomNetworkCommunication * com;										//서버와 통신하는 객체

	int code;																//서버로부터 받아올 데이터 Code
	String content;															//서버로부터 받아올 데이터 Content

	float updateTime;														//update 함수가 호출되는 시간을 기록. 60초가 될시 0으로 초기화
	int readyCount;															//유저의 행동 카운트

	int writeSize;															//서버에 보내는 버퍼 크기

	void start();

	//게임 시작/종료시 함수
	virtual void onEnter();
	virtual void onExit();

	//조작 관련 함수
	virtual void onTouchesBegan(const std::vector<Touch*>&touches, Event *event);
	virtual void onTouchesMoved(const std::vector<Touch*>&touches, Event *event);
	virtual void onTouchesEnded(const std::vector<Touch*>&touches, Event *event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *event);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

	//채팅 입력창 관련 함수(EditBox)
	virtual void editBoxReturn(EditBox * editBox);
	virtual void editBoxEditingDidBegin(EditBox * editBox);
	virtual void editBoxEditingDidEnd(EditBox * editBox);
	virtual void editBoxTextChanged(EditBox * editBox, const std::string& text);

	//채팅창 관련 함수(ScrollView)
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	//유저 캐릭터 생성
	void createSprite();

	//유저에 포커스를 맞춰 화면 이동
	void setViewpointCenter(cocos2d::Point position);

	//해당 위치에 뭐가 있는지 확인할때 사용
	cocos2d::Point tileCoordForPosition(cocos2d::Point position);
	void setPlayerPosition(cocos2d::Point position);

	//유저 액션
	void setAnimation(cocos2d::EventKeyboard::KeyCode key);
	void setOtherUsersAnimation(User * user, cocos2d::EventKeyboard::KeyCode moveDirection);

	void actionStarted();
	void actionFinished();

	void joinButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void joinOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void joinCancelButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void joinFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void loginButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void loginFailPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void passwordModiButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void backGroundStoryButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void makePeoplesButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void exitButtonTouchEvent(Ref * sender, Widget::TouchEventType type);
	void commErrorPopUpOkButtonTouchEvent(Ref * sender, Widget::TouchEventType type);

	//매 프레임마다 호출되는 함수.
	void update(float fDelta);			
};

#endif // __HELLOWORLD_SCENE_H__
