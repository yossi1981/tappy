#include "GameScene.h"

USING_NS_CC;


bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto listener1 = EventListenerTouchOneByOne::create();

	// trigger when you push down
	listener1->onTouchBegan = [](Touch* touch, Event* event){
		// your code
		return true; // if you are consuming it
	};

	// trigger when moving touch
	listener1->onTouchMoved = [](Touch* touch, Event* event){
		// your code
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event){

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

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);


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
	

	this->schedule(schedule_selector(GameScene::update), 0.5f);
	return true;
}

void GameScene::update(float dt)
{
	float maxRadius = 100;
	float minRadius = 10;
	float scaleFactor = maxRadius / minRadius;

	int animationTime = 10;
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	int minx = origin.x + maxRadius;
	int maxx = visibleSize.width - maxRadius;
	
	int miny = origin.y + maxRadius;
	int maxy = visibleSize.height - maxRadius;
	
	auto x = RandomHelper::random_int(minx, maxx);
	auto y = RandomHelper::random_int(miny, maxy);

	auto rescaleAction = ScaleBy::create(animationTime, scaleFactor);

	auto node = CCSprite::create("CloseNormal.png");
	node->setPosition(x, y);
	node->setTag(1);

	node->runAction(rescaleAction);

	


	this->addChild(node);
}