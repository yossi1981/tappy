#include "GameScene.h"

USING_NS_CC;


#define BOARD_DIMENSION 10

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}


	_rows = BOARD_DIMENSION;
	_cols = BOARD_DIMENSION;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	_colWidth = visibleSize.width / _cols;
	_rowWidth = visibleSize.height / _rows;

	for (unsigned x = 0 ; x < _rows ; ++x)
	{
		for (unsigned long y = 0 ; y < _cols ; ++y)
		{
			_freeLocations.push_back(Vec2(x,y));
		}
	}

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [](Touch* touch, Event* event){
		return true; 
	};

	listener->onTouchMoved = [](Touch* touch, Event* event){
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event){
		Vector<Node*> nodesToRemove;
		for (Vector<Node*>::iterator pos = this->getChildren().begin(); 
			pos != this->getChildren().end(); 
			++pos) 
		{
			Node* node = *pos;
			if (node->getTag() == 1 && node->getBoundingBox().containsPoint(touch->getLocation()))
			{
				nodesToRemove.pushBack(node);
				Label* scoreLabel = ((Label*)this->getChildByTag(10));
				int score = atoi(scoreLabel->getString().c_str());
				score++;
				((Label*)this->getChildByTag(10))->setString(std::to_string(score));
			}
		}

		for (Vector<Node*>::iterator pos = nodesToRemove.begin();
			pos != nodesToRemove.end();
			++pos)
		{
			this->removeChild(*pos);

			/* Explosion */
			auto m_emitter = ParticleExplosion::createWithTotalParticles(10);
			m_emitter->setPosition((*pos)->getPosition());
			m_emitter->setDuration(0.1f);
			m_emitter->setContentSize(Size(20, 20));
			m_emitter->setAutoRemoveOnFinish(true);
			m_emitter->setStartSize(10);
			m_emitter->setStartSizeVar(10);
			m_emitter->setEndSize(1);
			m_emitter->setEndSizeVar(1);
			m_emitter->setLife(3);
			m_emitter->setLifeVar(1);
			this->addChild(m_emitter);
		}


		
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	auto myMenu = Menu::create();
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
	labelConfig.fontSize = 48;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	auto playItem = Label::createWithTTF(labelConfig, "0");
	playItem->setPosition(Vec2(200, 0));
	playItem->setTag(10);
	playItem->setColor(Color3B(255, 255, 0));
	this->addChild(playItem);
	

	this->schedule(schedule_selector(GameScene::update), 0.2f);
	return true;
}

void GameScene::update(float dt)
{
	auto node = CCSprite::create("CloseSelected.png");

	/* get a free location randomally*/
	int numOfFreeLocations = _freeLocations.size();
	if (numOfFreeLocations <= 0) 
		return;
	int freeLocationIndex = RandomHelper::random_int(0, numOfFreeLocations - 1);
	PointsCollection_t::iterator it =  _freeLocations.begin() + freeLocationIndex;
	Vec2 _freeLocation = *it;
	_freeLocations.erase(it);

	/* add it to the occupied locations */
	_occupiedLocations.push_back(_freeLocation);

	/* build a node based on that location */
	node->setPosition(
		_freeLocation.x * _colWidth,
		_freeLocation.y * _rowWidth);
	node->setScale(2.0f);
	node->setTag(1);
	this->addChild(node);
}