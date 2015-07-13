#include "SplashScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

bool SplashScene::init()
{
	if (!Scene::init())
    {
        return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("splash.png");

	sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	float scalex = visibleSize.width / sprite->getContentSize().width;
	float scaley = visibleSize.height / sprite->getContentSize().height;
	sprite->setScale(scalex,scaley);

	this->addChild(sprite, 0);
	this->scheduleOnce(schedule_selector(SplashScene::UpdateTimer), 0.4f);

    return true;
}

void SplashScene::UpdateTimer(float dt)
{
	auto scene = MainMenuScene::create();
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}

