#ifndef __GameOverScene_LAYER_H__
#define __GameOverScene_LAYER_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Scene
{
public:
	virtual ~GameOverScene();
	virtual bool init();
	void setScore(unsigned long score);
	CREATE_FUNC(GameOverScene);

private:
	unsigned long _score;
	void quit();
	void transitionToGameScene();
};

#endif