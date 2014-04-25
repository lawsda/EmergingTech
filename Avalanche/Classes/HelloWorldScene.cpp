#include "HelloWorldScene.h"

b2World* world;
b2Vec2 gravity;
b2Fixture *bottomFixture;
b2Fixture *starFixture;

b2Body *playerBody;
b2Fixture *playerFixture;
b2BodyDef playerBodyDef;

b2PolygonShape playerBucket;

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
    //    gravity = b2Vec2(0.0f, 0.0f);
    bool doSleep = true;
    
    world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
    
    ///////////////////////
    //Create cloud floor
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
    
    //////////////////////
    //Create player
    
    auto playerSprite = Sprite::create("catcher.png");
    playerSprite->setPosition(Point(visibleSize.width/2, WORLD_TO_SCREEN(3)));
    this->addChild(playerSprite, 0);
    
    
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(playerSprite->getPositionX(), playerSprite->getPositionY());
    playerBodyDef.userData = playerSprite;
    
    playerBody = world->CreateBody(&playerBodyDef);
    
    
    b2Vec2 vertices[4];
    vertices[0].Set(WORLD_TO_SCREEN(-1),WORLD_TO_SCREEN(-3));
    vertices[1].Set(WORLD_TO_SCREEN(1), WORLD_TO_SCREEN(-3));
    vertices[2].Set(WORLD_TO_SCREEN(1), 0);
    vertices[3].Set(WORLD_TO_SCREEN(-1), 0);
    //b2EdgeShape playerBucket;
    playerBucket.Set(vertices, 4);
    b2FixtureDef playerBucketDef;
    playerBucketDef.shape = &playerBucket;
    playerBucketDef.density = 100.0f;
    playerBucketDef.friction = 0.1f;
    playerBucketDef.restitution = 0.0f;
    
    //  playerBucket.Set(b2Vec2(-20, 0), b2Vec2(20,0));
    playerFixture = playerBody->CreateFixture(&playerBucketDef);
    
    
    
    /////////////////////////
    schedule(schedule_selector(HelloWorld::tick));
    
    return true;
}

bool HelloWorld::touchBegan(Touch* touch, Event* event){
    
    HelloWorld::movePlayer(touch->getLocation());
    
 //   HelloWorld::createStar(touch->getLocation());
    
    return true;
}

void HelloWorld::movePlayer(Point p){
    
    int forceDir = playerBody->GetPosition().x - p.x;
    
    b2Vec2 force = b2Vec2(WORLD_TO_SCREEN(100000)* -forceDir, 0);
    
    b2Vec2 forcePoint = b2Vec2(playerBody->GetPosition().x, WORLD_TO_SCREEN(1.5));
    
    playerBody->ApplyLinearImpulse(force, forcePoint);
    
}

void HelloWorld::createStar(Point p){
    
    auto starSprite = Sprite::create("star.png");
    starSprite->setPosition(p);
    
    this->addChild(starSprite, 0);
    
    b2BodyDef starBodyDef;
    starBodyDef.type = b2_dynamicBody;
    starBodyDef.position = b2Vec2(starSprite->getPositionX(), starSprite->getPositionY());
    starBodyDef.userData = starSprite;
    
    b2Body* starBody = world->CreateBody(&starBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = WORLD_TO_SCREEN(0.6);
    
    b2FixtureDef starFixtureDef;
    starFixtureDef.shape = &circle;
    starFixtureDef.density = 1.0f;
    starFixtureDef.friction = 0.6f;
    starFixtureDef.restitution = 0.9f;
    starFixture = starBody->CreateFixture(&starFixtureDef);
    
    b2Vec2 force = b2Vec2(0, WORLD_TO_SCREEN(10));
    starBody->ApplyLinearImpulse(force, starBodyDef.position);
    
}


void HelloWorld::tick(float dt){
    
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
