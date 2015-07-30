#include "MainMenuScene.h"
#include "GameScene.h"

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

	auto playItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "Play"),
		[&](Ref* sender){
		this->transitionToGameScene();
	});
	playItem->setColor(Color3B(255, 255, 0));
	MenuItems.pushBack(playItem);

	auto closeItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "Quit"),
		[&](Ref* sender){
		this->quit();
	});
	closeItem->setColor(Color3B(255, 255, 0));
	MenuItems.pushBack(closeItem);


	auto menu = Menu::createWithArray(MenuItems);
	menu->alignItemsVerticallyWithPadding(50.0f);
	this->addChild(menu, 1);

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