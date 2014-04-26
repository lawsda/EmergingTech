#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#import "MyContactListener.h"

#define PTM_RATIO 32
#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)

using namespace cocos2d;

class HelloWorld : public cocos2d::LayerColor
{
public:
    
    MyContactListener *_contactListener;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    void tick(float dt);
    void createIcicle(Point p);
    void movePlayer(Point p);
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
