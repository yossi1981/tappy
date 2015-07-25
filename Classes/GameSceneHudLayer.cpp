#include "GameSceneHudLayer.h"
#include <streambuf>


#define TAG_SCORE		1
#define TAG_MULTIPLIER	2


template <typename T>
std::string tappy_to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

USING_NS_CC;

#define UNDEFINED_SCORE 0xFFFFFFFF

GameSceneHudLayer::GameSceneHudLayer()
{
	_score = UNDEFINED_SCORE;
	_multiplier = 1.0f;
}

GameSceneHudLayer::~GameSceneHudLayer()
{
}

bool GameSceneHudLayer::init() 
{
	if (!Layer::init()) 
	{
		return false;
	}

	/*font config*/
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/ARCADE.TTF";
	labelConfig.fontSize = 100;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	/*score*/
	auto scorelabel = Label::createWithTTF(labelConfig, "0");
	scorelabel->setPosition(Vec2(0, 0));
	scorelabel->setTag(TAG_SCORE);
	scorelabel->setColor(Color3B(0, 255, 255));
	scorelabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 150,
		Director::getInstance()->getVisibleSize().height-100));
	this->addChild(scorelabel);

	/*multiplier*/
	auto multiplierLabel = Label::createWithTTF(labelConfig, "0");
	multiplierLabel->setPosition(Vec2(0, 0));
	multiplierLabel->setTag(TAG_MULTIPLIER);
	multiplierLabel->setColor(Color3B(255, 0, 0));
	multiplierLabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 600,
		Director::getInstance()->getVisibleSize().height-100));
	this->addChild(multiplierLabel);

	/*
	//background
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("gameScreenBackground.png");
	background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 ));
	float scalex = visibleSize.width / background->getContentSize().width;
	float scaley = visibleSize.height / background->getContentSize().height;
	background->setScale(scalex,scaley);
	reorderChild(background,-10);

	addChild(background);*/


	UpdateUI();

	return true;
}

void GameSceneHudLayer::setScore(unsigned long score)
{
	_score = score;
	UpdateUI();
}

void GameSceneHudLayer::setMultiplier(float multiplier)
{
	_multiplier = multiplier;
	UpdateUI();
}

void GameSceneHudLayer::UpdateUI()
{
	/*score*/
	Label* scorelabel = (Label*)(getChildByTag(TAG_SCORE)); // check whether any other kind of cast is needed
	if (_score != UNDEFINED_SCORE)
	{
		scorelabel->setString(tappy_to_string(_score));
	}

	/*multiplier*/
	Label* multiplierlabel = (Label*)(getChildByTag(TAG_MULTIPLIER)); // check whether any other kind of cast is needed
	multiplierlabel->setString(std::string("x").append(tappy_to_string(_multiplier)));
}