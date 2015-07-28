#include "GameScene.h"
#include "GameSceneHudLayer.h"
#include "SimpleAudioEngine.h"
#include "PolygonLayer.h"
#include "GameOverScene.h"
#include "editor-support\cocostudio\CocoStudio.h"

struct BaloonState
{
	int colorId;
	bool shouldBlink;
};

template <typename T>
std::string tappy_to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}


USING_NS_CC;

#define BOARD_DIMENSION 10
#define HUD_LAYER_TAG 1
#define MAX_BALOONS_PER_COLOR 5

#define COLOR_ID_RED 0
#define COLOR_ID_GREEN 1
#define COLOR_ID_BLUE 2
#define COLOR_ID_PURPLE 3
#define COLOR_ID_YELLOW 4

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mid", true);


	_baloonsPerColor.insert(std::pair<int,int>(COLOR_ID_RED, 0));
	_baloonsPerColor.insert(std::pair<int, int>(COLOR_ID_GREEN, 0));
	_baloonsPerColor.insert(std::pair<int, int>(COLOR_ID_BLUE, 0));
	_baloonsPerColor.insert(std::pair<int, int>(COLOR_ID_PURPLE, 0));
	_baloonsPerColor.insert(std::pair<int, int>(COLOR_ID_YELLOW, 0));

	_score = 0;
	_multiplier = 1.0f;
	_totalTimePassed = 0;

	_rows = BOARD_DIMENSION;
	_cols = BOARD_DIMENSION;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	_colWidth = visibleSize.width / _cols;
	_rowWidth = visibleSize.height / _rows;

	
	for (unsigned x = 1 ; x < _cols  ; ++x)
	{ 
		for (unsigned long y = 1 ; y < _rows -1 ; ++y)
		{
			_freeLocations.push_back(Vec2(x,y));
		}
	}

	addEventListener();
	createHudLayer();
	createPolygonLayer();
	addBackground(visibleSize);
	this->schedule(schedule_selector(GameScene::update), 1.0f);
	
	return true;
}

void GameScene::update(float dt)
{
	static unsigned long currentSoundId = 0;
	/*total time passed*/
	_totalTimePassed += dt;

	/* blink the baloons if needed (whatever)*/
	bool playAlert = false;
	bool maxNumOfBaloonsPerColorReached = false;
	for (BaloonsPerColor::const_iterator it = _baloonsPerColor.begin();
		it != _baloonsPerColor.end();
		++it)
	{
		if (it->second >= MAX_BALOONS_PER_COLOR)
		{
			/* go over all nodes , see what should be blinked*/
			for (Vector<Node*>::iterator pos = this->getChildren().begin();
				pos != this->getChildren().end();
				++pos)
			{
				Node* node = *pos;
				if (node->getTag() != 1) continue;
				if (((BaloonState*)(node->getUserData()))->colorId != it->first) continue;


				Action* blink = Blink::create(10, 10);
				blink->setTag(300);
				node->runAction(blink);
			}
			playAlert = true;
		}
		else
		{
			for (Vector<Node*>::iterator pos = this->getChildren().begin();
				pos != this->getChildren().end();
				++pos)
			{
				Node* node = *pos;
				if (node->getTag() != 1) continue;
				if (((BaloonState*)(node->getUserData()))->colorId != it->first) continue;
				node->stopAllActionsByTag(300);
			}
		}
	}

	if (playAlert)
		currentSoundId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("alert.wav", false, 1.0f, 1.0f, 1.0f);
	else
	{
		if (currentSoundId != 0)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(currentSoundId);
			currentSoundId = 0;
		}
	}

	/*Check for game over*/
	int numOfFreeLocations = _freeLocations.size();

	bool gameOver = numOfFreeLocations <= 0 || maxNumOfBaloonsPerColorReached;
	if (false) 
	{
		auto scene = GameOverScene::create();
		scene->setScore(_score);
		Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
		return;
	}


	/*multiplier*/
	_multiplier -= 0.01f;
	if (_multiplier < 1.0f) _multiplier = 1.0f;
	GameSceneHudLayer* hud = (GameSceneHudLayer*)getChildByTag(20);
	hud->setMultiplier(_multiplier);

	/* create new nodes */
	int numOfNodes = 1 + (int)( sqrt((float)_totalTimePassed) / 4.0f);
	CCLOG("%d", numOfNodes);
	while (numOfNodes-- > 0)
	{
		int numOfFreeLocations = _freeLocations.size();
		if (numOfFreeLocations == 0)
		{
			break;
		}

		int freeLocationIndex = RandomHelper::random_int(0, numOfFreeLocations - 1);

		Color3B color;
		int colorId = RandomHelper::random_int(0, 4);
		_baloonsPerColor[colorId]++;

		switch (colorId)
		{
		case 0 :
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

		PointsCollection_t::iterator it =  _freeLocations.begin() + freeLocationIndex;
		Vec2 _freeLocation = *it;
		_freeLocations.erase(it);

		/* add it to the occupied locations */
		_occupiedLocations.push_back(_freeLocation);
		
		/* build a node based on that location */
		auto node = CCSprite::create("Baloon.png");
		node->setPosition(
			_freeLocation.x * _colWidth,
			_freeLocation.y * _rowWidth);

		node->setScale(0.5f);
		auto scaleAction = ScaleBy::create(3.0f, 5.0f);
		auto move_ease_in = EaseInOut::create(scaleAction->clone(),3.0);

		node->runAction(Sequence::create(
			move_ease_in,
			nullptr));

		node->setTag(1);
		node->setColor(color);

		BaloonState* baloonState = new BaloonState();
		baloonState->colorId = colorId;
		baloonState->shouldBlink = false;
		node->setUserData((void*)baloonState);

		this->addChild(node);
	}
}

cocos2d::TTFConfig GameScene::createScoreLabelConfig()
{
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/ARCADE.TTF";
	labelConfig.fontSize = 75;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
	return labelConfig;
}

void GameScene::createHudLayer()
{
	GameSceneHudLayer* hud = GameSceneHudLayer::create();
	hud->setTag(20);
	hud->setScore(_score);
	hud->setMultiplier(_multiplier);

	this->addChild(hud);
	this->reorderChild(hud, 50);
}

void GameScene::createPolygonLayer()
{
	PolygonLayer* polygon = PolygonLayer::create();
	polygon->setTag(19);
	polygon->setOpacity(50);
	this->addChild(polygon);
	this->reorderChild(polygon, -1);
}

void GameScene::addBackground(Size &visibleSize)
{
	auto background = Sprite::create("galaxy_1.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	float scalex = visibleSize.width / background->getContentSize().width;
	float scaley = visibleSize.height / background->getContentSize().height;
	background->setScale(scalex, scaley);
	reorderChild(background, -10);
	addChild(background);
}

void GameScene::addEventListener()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [](Touch* touch, Event* event)
	{
		return true;
	};

	listener->onTouchMoved = [](Touch* touch, Event* event)
	{
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		Vec2 _touchLocationWithOffset = touch->getLocation();
		/*score label*/

		Vector<Node*> nodesToRemove;
		Vector<Node*> nodesToAdd;

		for (Vector<Node*>::iterator pos = this->getChildren().begin();
			pos != this->getChildren().end();
			++pos)
		{
			Node* node = *pos;


			if (node->getTag() == 1 && node->getBoundingBox().containsPoint(_touchLocationWithOffset))
			{
				nodesToRemove.pushBack(node);
				BaloonState* baloonState = (BaloonState*)node->getUserData();
				_baloonsPerColor[baloonState->colorId]--;

				/*update hUD with score and multiplier */
				int _addedScore = (int)(10.0f * _multiplier);
				_score += _addedScore;
				_multiplier += 0.1f;
				((GameSceneHudLayer*)getChildByTag(20))->setScore(_score);
				((GameSceneHudLayer*)getChildByTag(20))->setMultiplier(_multiplier);

				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav", false, 1.0f, 1.0f, 1.0f);

				/*location*/
				Vec2 location = Vec2(node->getPositionX() / _colWidth,
					node->getPositionY() / _rowWidth);

				/*remove from occupied locations*/
				auto occupiedLocationIt = std::find(_occupiedLocations.begin(),
					_occupiedLocations.end(),
					location);
				_occupiedLocations.erase(occupiedLocationIt);


				/*reinsert free location*/
				_freeLocations.push_back(location);
				auto labelConfig = createScoreLabelConfig();
				auto scoreLabel = Label::createWithTTF(labelConfig, tappy_to_string(_addedScore));
				scoreLabel->setColor(Color3B(0, 255, 0));
				scoreLabel->setPosition(_touchLocationWithOffset);

				scoreLabel->runAction(Sequence::create(
					MoveBy::create(2.0f, Vec2(0, 100)),
					RemoveSelf::create(true),
					nullptr));

				nodesToAdd.pushBack(scoreLabel);
			}
		}

		if (nodesToRemove.empty())
		{
			_score--;
			((GameSceneHudLayer*)getChildByTag(20))->setScore(_score);

			_multiplier -= 1.0f;
			if (_multiplier < 1.0f) _multiplier = 1.0f;
			((GameSceneHudLayer*)getChildByTag(20))->setMultiplier(_multiplier);

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blop.wav", false, 1.0f, 1.0f, 0.5f);

			auto labelConfig = createScoreLabelConfig();
			auto scoreLabel = Label::createWithTTF(labelConfig, "-1");
			scoreLabel->setTag(10);
			scoreLabel->setColor(Color3B(255, 0, 0));
			scoreLabel->setPosition(_touchLocationWithOffset);

			scoreLabel->runAction(Sequence::create(
				MoveBy::create(2.0f, Vec2(0, 100)),
				RemoveSelf::create(true),
				nullptr));

			nodesToAdd.pushBack(scoreLabel);
		}


		for (Vector<Node*>::iterator pos = nodesToAdd.begin();
			pos != nodesToAdd.end();
			++pos)
		{
			addChild(*pos);
		}


		for (Vector<Node*>::iterator pos = nodesToRemove.begin();
			pos != nodesToRemove.end();
			++pos)
		{
			this->removeChild(*pos);

			/* Explosion */
			auto m_emitter = ParticleExplosion::createWithTotalParticles(20);
			m_emitter->setPosition((*pos)->getPosition());
			m_emitter->setDuration(1.0f);
			m_emitter->setContentSize(Size(20, 20));
			m_emitter->setStartSize(20);
			m_emitter->setStartSizeVar(10);
			m_emitter->setEndSize(1);
			m_emitter->setEndSizeVar(1);
			m_emitter->setLife(5);
			m_emitter->setLifeVar(2);
			m_emitter->setAutoRemoveOnFinish(true);
			this->addChild(m_emitter);
			reorderChild(m_emitter, -5);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

