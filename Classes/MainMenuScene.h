#ifndef __MainMenuScene_LAYER_H__
#define __MainMenuScene_LAYER_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
public:
	virtual ~MainMenuScene();
	virtual bool init();
	CREATE_FUNC(MainMenuScene);

private:
	void quit();
	void transitionToGameScene();
};

#endif