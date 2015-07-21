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
	virtual void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

private:
	unsigned long _score;
	void UpdateUI();
};

