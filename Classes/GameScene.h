#ifndef __GameScene_LAYER_H__
#define __GameScene_LAYER_H__

#include "cocos2d.h"
class GameScene : public cocos2d::Scene
{
public:
	virtual bool init();
	CREATE_FUNC(GameScene);


private:
	void update(float dt);
};

#endif