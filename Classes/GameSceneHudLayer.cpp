#include "GameSceneHudLayer.h"

USING_NS_CC;

#define UNDEFINED_SCORE 0xFFFFFFFF

GameSceneHudLayer::GameSceneHudLayer()
{
	_score = UNDEFINED_SCORE;
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

	setColor(cocos2d::Color3B(0, 0, 0));
	setContentSize(cocos2d::Size::Size(1, 1)); //?
	setAnchorPoint(cocos2d::Vec2(0, 0)); //?


	
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/ARCADE.TTF";
	labelConfig.fontSize = 60;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	auto label = Label::createWithTTF(labelConfig, "0");
	label->setPosition(Vec2(0, 0));
	label->setTag(10);
	label->setColor(Color3B(255, 255, 0));
	label->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 100,
		Director::getInstance()->getVisibleSize().height-100));

	this->addChild(label);

	UpdateUI();

	return true;
}

void GameSceneHudLayer::setScore(unsigned long score)
{
	_score = score;
	UpdateUI();
}

void GameSceneHudLayer::UpdateUI()
{
	Label* label = (Label*)(getChildByTag(10)); // check whether any other kind of cast is needed
	if (_score != UNDEFINED_SCORE)
	{
		label->setString(std::to_string(_score));
	}
}

void GameSceneHudLayer::onDraw(const Mat4& transform, uint32_t flags)
{

}