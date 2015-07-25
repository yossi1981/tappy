#include "GameScene.h"
#include "GameSceneHudLayer.h"
#include "SimpleAudioEngine.h"
#include "PolygonLayer.h"
#include "GameOverScene.h"
#include "editor-support\cocostudio\CocoStudio.h"


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


bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

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

	/* Events handling*/
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
		TTFConfig labelConfig;
		labelConfig.fontFilePath = "fonts/ARCADE.TTF";
		labelConfig.fontSize = 75;
		labelConfig.glyphs = GlyphCollection::DYNAMIC;
		labelConfig.outlineSize = 2;
		labelConfig.customGlyphs = nullptr;
		labelConfig.distanceFieldEnabled = false;


		Vec2 _touchLocationWithOffset = touch->getLocation();

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

				/*update hUD with score and multiplier */
				int _addedScore = (int) (10.0f * _multiplier);
				_score += _addedScore;
				_multiplier += 0.1f;
				((GameSceneHudLayer*)getChildByTag(20))->setScore(_score);
				((GameSceneHudLayer*)getChildByTag(20))->setMultiplier(_multiplier);

				/*TODO : REMOVE THIS STUPID CRAP WHY DID I EVEN THINK ABOUT THIS SHIT?!
				((PolygonLayer*)getChildByTag(19))->addVertex(_touchLocationWithOffset);*/


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

				/*score label*/
				auto scoreLabel = Label::createWithTTF(labelConfig, tappy_to_string(_addedScore));
				scoreLabel->setColor(Color3B(0, 255, 0));
				scoreLabel->setPosition(_touchLocationWithOffset);

				scoreLabel->runAction(Sequence::create(
					MoveBy::create(2.0f,Vec2(0,100)),
					RemoveSelf::create(true),
					nullptr));

				nodesToAdd.pushBack(scoreLabel);
			}
		}

		if (nodesToRemove.empty())
		{
			_score --;
			((GameSceneHudLayer*)getChildByTag(20))->setScore(_score);

			_multiplier -= 1.0f;
			if (_multiplier < 1.0f) _multiplier = 1.0f;
			((GameSceneHudLayer*)getChildByTag(20))->setMultiplier(_multiplier);
			

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blop.wav", false, 1.0f, 1.0f, 0.5f);


			auto scoreLabel = Label::createWithTTF(labelConfig, "-1");
			scoreLabel->setTag(10);
			scoreLabel->setColor(Color3B(255, 0, 0));
			scoreLabel->setPosition(_touchLocationWithOffset);

			scoreLabel->runAction(Sequence::create(
				MoveBy::create(2.0f,Vec2(0,100)),
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
			m_emitter->setDuration(2.0f);
			m_emitter->setContentSize(Size(50, 50));
			m_emitter->setStartSize(40);
			m_emitter->setStartSizeVar(20);
			m_emitter->setEndSize(2);
			m_emitter->setEndSizeVar(2);
			m_emitter->setLife(10);
			m_emitter->setLifeVar(5);
			m_emitter->setAutoRemoveOnFinish(true);
			this->addChild(m_emitter);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	/*HUD*/
	GameSceneHudLayer* hud = GameSceneHudLayer::create();
	hud->setTag(20);
	hud->setScore(_score);
	hud->setMultiplier(_multiplier);

	this->addChild(hud);
	this->reorderChild(hud, 50);

	/*Polygon*/
	PolygonLayer* polygon = PolygonLayer::create();
	polygon->setTag(19);
	polygon->setOpacity(50);
	this->addChild(polygon);
	this->reorderChild(polygon, -1);

	/*background*/
	auto background = Sprite::create("galaxy_1.png");
	background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	float scalex = visibleSize.width / background->getContentSize().width;
	float scaley = visibleSize.height / background->getContentSize().height;
	background->setScale(scalex,scaley);
	reorderChild(background,-10);
	addChild(background);


	/*Start the 'update' loop*/
	this->schedule(schedule_selector(GameScene::update), 1.0f);
	
	return true;
}

void GameScene::update(float dt)
{
	/*total time passed*/
	_totalTimePassed += dt;

	/* get a free location randomally*/
	int numOfFreeLocations = _freeLocations.size();
	if (numOfFreeLocations <= 0) 
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
	int numOfNodes = 1 + (int)( _totalTimePassed / 5.0f);
	CCLOG("%d", numOfNodes);
	while (numOfNodes-- > 0)
	{
		int numOfFreeLocations = _freeLocations.size();
		if (numOfFreeLocations == 0)
		{
			break;
		}

		int freeLocationIndex = RandomHelper::random_int(0, numOfFreeLocations - 1);
		PointsCollection_t::iterator it =  _freeLocations.begin() + freeLocationIndex;
		Vec2 _freeLocation = *it;
		_freeLocations.erase(it);

		/* add it to the occupied locations */
		_occupiedLocations.push_back(_freeLocation);
		
		/* build a node based on that location */
		auto node = CCSprite::create("CloseSelected.png");
		node->setPosition(
			_freeLocation.x * _colWidth,
			_freeLocation.y * _rowWidth);
		node->setScale(3.0f);
		node->setTag(1);

		this->addChild(node);
	}
}


