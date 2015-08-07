#include "MainMenuScene.h"
#include "GameScene.h"
#include "ui/UIButton.h"

USING_NS_CC;

MainMenuScene::~MainMenuScene()
{
}

bool MainMenuScene::init()
{
	if (!Scene::init()){
		return false;
	}

	Vector<MenuItem*> MenuItems;

	auto myMenu = Menu::create();
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/MainFont.ttf";
	labelConfig.fontSize = 120;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
/*
	auto playItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "Play"),
		[&](Ref* sender){
		this->transitionToGameScene();
	});
	playItem->setColor(Color3B(255, 255, 0));
	MenuItems.pushBack(playItem);*/

	

	auto playButtonItem = MenuItem::create();

	auto playButton = ui::Button::create("Play.png");

	playButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		transitionToGameScene();

	});

	playButtonItem->setScale(0.1f);
	{
		auto scaleby = ScaleBy::create(1.0f, 20.0f);
		auto ease = EaseBounceOut::create(scaleby);
		playButtonItem->runAction(ease);
	}
	playButtonItem->addChild(playButton);
	MenuItems.pushBack(playButtonItem);




	auto quitButtonItem = MenuItem::create();
	auto quitButton = ui::Button::create("quit.png");
	/*quitButton->addTouchEventListener([=](Touch* touch, Event* event)
	{
		this->quit();
	});
*/
	quitButtonItem->setScale(0.1f);
	{
		auto scaleby = ScaleBy::create(2.0f, 20.0f);
		auto ease = EaseBounceOut::create(scaleby);
		quitButtonItem->runAction(ease);
	}
	quitButtonItem->addChild(quitButton);
	MenuItems.pushBack(quitButtonItem);





	/*auto closeItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "Quit"),
		[&](Ref* sender){
		this->quit();
	});
	closeItem->setColor(Color3B(255, 255, 0));
	MenuItems.pushBack(closeItem);

	*/
	auto menu = Menu::createWithArray(MenuItems);
	menu->alignItemsVerticallyWithPadding(200.0f);
	this->addChild(menu, 1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("MainMenuBackground.jpg");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	float scalex = visibleSize.width / background->getContentSize().width;
	float scaley = visibleSize.height / background->getContentSize().height;
	background->setScale(scalex, scaley);
	reorderChild(background, -10);
	addChild(background);

	return true;
}

void MainMenuScene::quit()
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainMenuScene::transitionToGameScene()
{
	auto scene = GameScene::create();
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}