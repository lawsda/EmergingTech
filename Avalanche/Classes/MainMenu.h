//
//  MainMenu.h
//  Avalanche
//
//  Created by David Laws on 4/27/14.
//
//

#ifndef __Avalanche__MainMenu__
#define __Avalanche__MainMenu__

#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

class MainMenu : public cocos2d::LayerColor
{
public:
    
    int HIGH_SCORE;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void menuPlayCallback(Object* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);

};

#endif /* defined(__Avalanche__MainMenu__) */
