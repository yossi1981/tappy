#include "GameOverScene.h"
#include "GameScene.h"


#define TAG_SCORE_TEXT 1
#define TAG_MENU 2

template <typename T>
std::string tappy_to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

USING_NS_CC;

GameOverScene::~GameOverScene()
{
}

bool GameOverScene::init()
{
	if (!Scene::init()){
		return false;
	}


	Vector<MenuItem*> MenuItems;

	auto myMenu = Menu::create();
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/Marker Felt.ttf";
	labelConfig.fontSize = 48;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 2;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;


	auto gameoverItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "GAME OVER"));
	gameoverItem->setColor(Color3B(255, 255, 255));
	MenuItems.pushBack(gameoverItem);
	
	auto scorelabel = Label::createWithTTF(labelConfig, "");
	auto scoreItem = MenuItemLabel::create(scorelabel);
	scoreItem->setTag(TAG_SCORE_TEXT);
	scoreItem->setColor(Color3B(255, 255, 255));
	MenuItems.pushBack(scoreItem);

	auto playItem = MenuItemLabel::create(Label::createWithTTF(labelConfig, "Play again"),
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
	menu->setTag(TAG_MENU);
	menu->alignItemsVerticallyWithPadding(50.0f);
	this->addChild(menu, 1);

	return true;
}

void GameOverScene::quit()
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void GameOverScene::transitionToGameScene()
{
	auto scene = GameScene::create();
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}

void GameOverScene::setScore(unsigned long score)
{
	_score = score;
	std::string scoreSrting = std::string("Your final score is ").append(tappy_to_string(score)); 
	auto scorelabel = (MenuItemLabel*)(getChildByTag(TAG_MENU)->getChildByTag(TAG_SCORE_TEXT));
	scorelabel->setString(scoreSrting);
}