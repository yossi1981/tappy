#ifndef __SplashScene_LAYER_H__
#define __SplashScene_LAYER_H__

#include "cocos2d.h"

class SplashScene : public cocos2d::Scene
{
public:
    virtual bool init();
	void UpdateTimer(float dt);
	CREATE_FUNC(SplashScene);
};
#endif
