#pragma once

#include "cocos2d.h"

class GameSceneHudLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameSceneHudLayer);
	GameSceneHudLayer();
	virtual bool init();
	virtual ~GameSceneHudLayer();
	void setScore(unsigned long score);
	void setMultiplier(float multiploer);
private:
	unsigned long _score;
	float _multiplier;
	void UpdateUI();
};

