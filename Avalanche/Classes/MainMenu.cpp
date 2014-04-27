//
//  MainMenu.cpp
//  Avalanche
//
//  Created by David Laws on 4/27/14.
//
//

#include "MainMenu.h"
#include "HelloWorldScene.h"

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    Color4B background = Color4B(185, 225, 245, 255);
    if ( !LayerColor::initWithColor(background) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto closeMenu = Menu::create(closeItem, NULL);
    closeMenu->setPosition(Point::ZERO);
    this->addChild(closeMenu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    
    /////////////////////////////
    //Main Menu

    auto menuText_1 = MenuItemFont::create("Start Game", CC_CALLBACK_1(MainMenu::menuPlayCallback, this));
    menuText_1->setFontSizeObj(visibleSize.width/15);
    menuText_1->setColor(Color3B(0,0,0));
    menuText_1->setPosition(visibleSize.width/2, visibleSize.height/2);
    
    auto menu = Menu::create(menuText_1, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    return true;
}


void MainMenu::menuPlayCallback(Object* pSender){
    Director::getInstance()->pushScene(HelloWorld::createScene());
}


void MainMenu::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
