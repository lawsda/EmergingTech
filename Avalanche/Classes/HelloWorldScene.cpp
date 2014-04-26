#include "HelloWorldScene.h"

b2World* world;
b2Vec2 gravity;
b2Fixture *bottomFixture;
b2Fixture *icicleFixture;
Point lastTouchLocation;

b2Body *playerBody;
b2Fixture *playerFixture;
b2BodyDef playerBodyDef;

b2PolygonShape playerShape;

b2MouseJoint *mouseJoint;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    Color4B background = Color4B(30, 195, 235, 255);
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + /*visibleSize.height - */closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    //Earth G
    gravity.Set(0.0f, - WORLD_TO_SCREEN(9.8));
    //Zero G
    //gravity = b2Vec2(0.0f, 0.0f);
    bool doSleep = true;
    
    world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::touchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::touchEnded, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
    
    ///////////////////////
    //Create cloud floor and walls
    for(int i = 0; i <= visibleSize.width + 100; i += 200){
        auto cloudSprite = Sprite::create("cloud.png");
        cloudSprite->setPosition(Point(i, 10));
        this->addChild(cloudSprite, 0);
    }
    b2BodyDef cloudsBodyDef;
    cloudsBodyDef.position.Set(0,0);
    
    b2Body* cloudsBody = world->CreateBody(&cloudsBodyDef);
    b2EdgeShape cloudBox;
    b2FixtureDef cloudBoxDef;
    cloudBoxDef.shape = &cloudBox;
    
    cloudBox.Set(b2Vec2(0,0), b2Vec2(visibleSize.width, 0));
    bottomFixture = cloudsBody->CreateFixture(&cloudBoxDef);
    
    cloudBox.Set(b2Vec2(0,0), b2Vec2(0, visibleSize.height));
    cloudsBody->CreateFixture(&cloudBoxDef);
    
    cloudBox.Set(b2Vec2(visibleSize.width,0), b2Vec2(visibleSize.width, visibleSize.height));
    cloudsBody->CreateFixture(&cloudBoxDef);
    
    //////////////////////
    //Create player
    
    auto playerSprite = Sprite::create("Climber.png");
    playerSprite->setPosition(Point(visibleSize.width/2, playerSprite->getContentSize().height/2));
    this->addChild(playerSprite, 0);
    
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(playerSprite->getPositionX(), playerSprite->getPositionY());
    playerBodyDef.userData = playerSprite;
    
    playerBody = world->CreateBody(&playerBodyDef);
    
    playerShape.SetAsBox(playerSprite->getContentSize().width/2, playerSprite->getContentSize().height/2);
    
    b2FixtureDef playerShapeDef;
    playerShapeDef.shape = &playerShape;
    playerShapeDef.density = 100.0f;
    playerShapeDef.friction = 0.0f;
    playerShapeDef.restitution = 0.0f;
    
    playerFixture = playerBody->CreateFixture(&playerShapeDef);

    lastTouchLocation = Point(playerBody->GetPosition().x, playerBody->GetPosition().y);
    
    
    /////////////////////////
    schedule(schedule_selector(HelloWorld::tick));
    
    return true;
}

bool HelloWorld::touchBegan(Touch* touch, Event* event){
    
    lastTouchLocation = touch->getLocation();
    
 //   HelloWorld::createIcicle(touch->getLocation());
    
    return true;
}

void HelloWorld::touchMoved(Touch* touch, Event* event){
    
    lastTouchLocation = touch->getLocation();
    
}

void HelloWorld::touchEnded(Touch* touch, Event* event){
    lastTouchLocation = Point(playerBody->GetPosition().x, playerBody->GetPosition().y);
    
}

void HelloWorld::movePlayer(Point p){

    int forceDir = playerBody->GetPosition().x - p.x;
    
    playerBody->SetLinearVelocity(b2Vec2(1000 * -forceDir, 0));
    
}

void HelloWorld::createIcicle(Point p){
    
    Sprite* icicleSprite = Sprite::create("Icicle.png");
    icicleSprite->setPosition(p);
    
    this->addChild(icicleSprite, 0);
    
    b2BodyDef icicleBodyDef;
    icicleBodyDef.type = b2_dynamicBody;
    icicleBodyDef.position = b2Vec2(icicleSprite->getPositionX(), icicleSprite->getPositionY());
    icicleBodyDef.userData = icicleSprite;
    
    b2Body* icicleBody = world->CreateBody(&icicleBodyDef);
    
    b2PolygonShape icicle;
    icicle.SetAsBox(icicleSprite->getContentSize().width/2, icicleSprite->getContentSize().height/2);
    
    b2FixtureDef icicleFixtureDef;
    icicleFixtureDef.shape = &icicle;
    icicleFixtureDef.density = 1.0f;
    icicleFixtureDef.friction = 0.6f;
    icicleFixtureDef.restitution = 0.8f;
    icicleFixture = icicleBody->CreateFixture(&icicleFixtureDef);
    
    
}


void HelloWorld::tick(float dt){
    
    HelloWorld::movePlayer(lastTouchLocation);
    
    int velocityIterations = 8;
    int positionIterations = 3;
    
    world->Step(dt, velocityIterations, positionIterations);
    
    for(auto b = world->GetBodyList(); b; b = b->GetNext()){
        if (b->GetUserData() != NULL){
            auto myActor = (Sprite*) b->GetUserData();
            myActor->setPosition(Point(b->GetPosition().x, b->GetPosition().y));
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
            if(myActor->getPositionY() < - WORLD_TO_SCREEN(1)
               || myActor->getPositionX() < - WORLD_TO_SCREEN(1)
               || myActor->getPositionX() > Director::getInstance()->getVisibleSize().width + WORLD_TO_SCREEN(1)){
                
                removeChild(myActor);
                world->DestroyBody(b);
            }
            
        }
    }
    
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
