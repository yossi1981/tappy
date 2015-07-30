#include "GameSceneHudLayer.h"
#include <streambuf>


#define TAG_SCORE		1
#define TAG_MULTIPLIER	2
#define TAG_TIME 3

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
	_time = 0;

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
	scorelabel->setTag(TAG_SCORE);
	scorelabel->setColor(Color3B(0, 255, 255));
	scorelabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 150,
		Director::getInstance()->getVisibleSize().height-100));
	this->addChild(scorelabel);

	/*multiplier*/
	auto multiplierLabel = Label::createWithTTF(labelConfig, "0");
	multiplierLabel->setTag(TAG_MULTIPLIER);
	multiplierLabel->setColor(Color3B(255, 0, 0));
	multiplierLabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 600,
		Director::getInstance()->getVisibleSize().height-100));
	this->addChild(multiplierLabel);

	/*timer*/
	auto timeLabel = Label::createWithTTF(labelConfig, "0");
	timeLabel->setTag(TAG_TIME);
	timeLabel->setColor(Color3B(255, 255, 255));
	timeLabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width - 400,
		Director::getInstance()->getVisibleSize().height - 100));
	this->addChild(timeLabel);

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


void GameSceneHudLayer::setColorSequenceState(const ColorSequenceState_t& colorSequenceState, bool newSequence)
{
	_colorSequenceState = colorSequenceState;

	std::vector<Node*> childsToRemove;
	for (auto child : getChildren())
	{
		if (child->getTag() == 1000)
		{
			childsToRemove.push_back(child);
		}
	}

	for (auto child : childsToRemove)
	{
		removeChild(child);
	}


	int _currentXPost = 100;
	for (ColorSequenceState_t::const_iterator it = _colorSequenceState.begin();
		it != _colorSequenceState.end();
		++it)
	{
		Color3B color;
		switch (it->colorId)
		{
		case 0:
			color = Color3B(255, 0, 0); // red
			break;

		case 1:
			color = Color3B(0, 255, 0); // green	
			break;

		case 2:
			color = Color3B(0, 0, 255); // blue
			break;

		case 3:
			color = Color3B(255, 0, 255); // purple
			break;

		case 4:
			color = Color3B(0, 255, 255); // yellow
			break;
		}

		/* build a node based on that location */
		auto node = CCSprite::create("Baloon.png");

		/*calc y position*/

		int posY = getContentSize().height - 100;

		node->setPosition(
			_currentXPost,
			posY);

		
		node->setColor(color);

		if (newSequence)
		{
			node->setScale(1.0f);
			node->runAction(EaseBackOut::create(ScaleBy::create(3.0f, 2.0f)));
		}
		else
		{
			node->setScale(2.0);
		}

		if (it->marked)
		{
			auto arrowNode = CCSprite::create("arrow.png");
			arrowNode->setScale(0.4f);
			arrowNode->setPosition(node->getContentSize().width / 2, node->getContentSize().height / 2);
			arrowNode->setColor(Color3B(0, 255, 0));
			arrowNode->runAction(ScaleBy::create(2.0f, 0.3f));
			node->addChild(arrowNode);
		}
		
		node->setTag(1000);
		this->addChild(node);
		_currentXPost += 100;
	}

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

	/*timer*/
	Label* timeLabel = (Label*)(getChildByTag(TAG_TIME)); // check whether any other kind of cast is needed
	timeLabel->setString(tappy_to_string(_time));
}

void GameSceneHudLayer::addMultiplier()
{
	/*font config*/
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/ARCADE.TTF";
	labelConfig.fontSize = 100;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	/*score*/
	auto scorelabel = Label::createWithTTF(labelConfig, "+1");
	scorelabel->setPosition(Vec2(0, 0));
	scorelabel->setColor(Color3B(255, 255, 255));
	scorelabel->setPosition(600, getContentSize().height - 100);
	scorelabel->runAction(Sequence::create(
		EaseBackInOut::create(
			MoveTo::create(2.0f, ((Label*)(getChildByTag(TAG_MULTIPLIER)))->getPosition())),
		RemoveSelf::create(true),
		nullptr));

	this->addChild(scorelabel);
}

void GameSceneHudLayer::setTime(int time)
{
	_time = time;
	UpdateUI();
}
