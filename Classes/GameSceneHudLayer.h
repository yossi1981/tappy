#pragma once

#include "cocos2d.h"
#include "Defs.h"

class GameSceneHudLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameSceneHudLayer);
	GameSceneHudLayer();
	virtual bool init();
	virtual ~GameSceneHudLayer();
	void setScore(unsigned long score);
	void setMultiplier(float multiploer);
	void setColorSequenceState(const ColorSequenceState_t& colorSequenceState, bool newSequence);
	void addMultiplier();
	void setTime(int time);
private:
	ColorSequenceState_t _colorSequenceState;
	unsigned long _score;
	unsigned long _time;
	float _multiplier;
	void UpdateUI();
};

