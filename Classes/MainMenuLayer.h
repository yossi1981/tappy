#ifndef __MAINMENU_LAYER_H__
#define __MAINMENY_LAYER_H__

#include "cocos2d.h"

class MainMenuLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(MainMenuLayer);
};
#endif
