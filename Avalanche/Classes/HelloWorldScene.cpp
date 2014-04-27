#include "HelloWorldScene.h"
#include "MainMenu.h"

b2World* world;
b2Vec2 gravity;
b2Fixture *bottomFixture;
b2Fixture *icicleFixture;
Point lastTouchLocation;
int icicleTimer = 0;
int gameEndTimer = 0;

Sprite* icicleSprite;

b2Body *playerBody;
b2Fixture *playerFixture;
b2BodyDef playerBodyDef;

b2PolygonShape playerShape;

bool died;

vector<b2Fixture*> icicleFixtures;
vector<Point> icicleSpots;

int screenWidth;
int screenHeight;

int score;
LabelTTF* scoreItem;

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
    Color4B background = Color4B(185, 225, 245, 255);
    if ( !LayerColor::initWithColor(background) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + screenWidth - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto closeMenu = Menu::create(closeItem, NULL);
    closeMenu->setPosition(Point::ZERO);
    this->addChild(closeMenu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    
    /////////////////////////////
    // Score
    score = 0;
    string scoreText = "Score: "+ to_string(score);
    scoreItem = LabelTTF::create(scoreText, "Comic Sans", 32);
    scoreItem->setColor(Color3B(0,0,0));
    scoreItem->setPosition(Point(screenWidth-96, screenHeight-32));
    this->addChild(scoreItem, 1);
    
    
    /////////////////////////////
    //world setup
    died = false;
    icicleTimer = 0;
    gameEndTimer = 0;
    icicleSpots.clear();
    
    gravity.Set(0.0f, - WORLD_TO_SCREEN(9.8));
    bool doSleep = true;
    
    world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
    
    _contactListener = new MyContactListener();
    world->SetContactListener(_contactListener);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::touchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::touchEnded, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
    

    ///////////////////////
    //Create ice floor and walls
    auto iceSprite = Sprite::create("Ice.png");
    int iceWidth = iceSprite->getContentSize().width;
    int iceHeight = iceSprite->getContentSize().height;
    for(int i = iceWidth/2; i <= screenWidth + 100; i += iceWidth){
        auto iceSprite = Sprite::create("Ice.png");
        iceSprite->setPosition(Point(i, iceHeight/2));
        this->addChild(iceSprite, 0);
    }
    
    b2BodyDef cloudsBodyDef;
    cloudsBodyDef.position.Set(0,0);
    
    b2Body* cloudsBody = world->CreateBody(&cloudsBodyDef);
    b2EdgeShape cloudBox;
    b2FixtureDef cloudBoxDef;
    cloudBoxDef.shape = &cloudBox;
    
    cloudBox.Set(b2Vec2(0,iceHeight/2), b2Vec2(screenWidth, iceHeight/2));
    bottomFixture = cloudsBody->CreateFixture(&cloudBoxDef);
    
    cloudBox.Set(b2Vec2(0,0), b2Vec2(0, screenHeight));
    cloudsBody->CreateFixture(&cloudBoxDef);
    
    cloudBox.Set(b2Vec2(screenWidth,0), b2Vec2(screenWidth, screenHeight));
    cloudsBody->CreateFixture(&cloudBoxDef);
    
 
 
    //////////////////////
    //Create player
    
    auto playerSprite = Sprite::create("Climber.png");
    playerSprite->setPosition(Point(screenWidth/2, (playerSprite->getContentSize().height/2)+iceHeight/2));
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
    
    
    ///////////////////////
    //Create IcicleArray
    icicleSprite = Sprite::create("Icicle.png");
    int icicleWidth = icicleSprite->getContentSize().width;
    
    for(int i = icicleWidth/2; i <= screenWidth; i+= icicleWidth){
        Point p = Point(i, screenHeight);
        icicleSpots.push_back(p);
    }
    
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
    
    icicleSprite = Sprite::create("Icicle.png");
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
    icicleFixtureDef.density = 50.0f;
    icicleFixtureDef.friction = 0.0f;
    icicleFixtureDef.restitution = 0.0f;
    icicleFixture = icicleBody->CreateFixture(&icicleFixtureDef);
    
    //icicleFixture->s
    
    icicleFixtures.push_back(icicleFixture);
}

void HelloWorld::gameOver(){
    scoreItem = LabelTTF::create("Game Over", "Comic Sans", 64);
    scoreItem->setColor(Color3B(255,0,0));
    scoreItem->setPosition(Point(screenWidth/2, screenHeight/2));
    this->addChild(scoreItem, 1);
}


void HelloWorld::tick(float dt){
    
    if(gameEndTimer == 120){
        Director::getInstance()->popScene();
    }
    
    if(died)
        gameEndTimer++;
    
    if(!died)
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
    
    
    //check contacts
    for(int p = 0; p < _contactListener->_contacts.size(); p++) {
        MyContact contact = _contactListener->_contacts.at(p);

        auto fixA = contact.fixtureA;
        auto fixB = contact.fixtureB;
        //icicle hits floor
        if (fixA == bottomFixture && fixB->GetDensity() == 50.0f)  {
            auto icicle = (Sprite*) fixB->GetBody()->GetUserData();
            removeChild(icicle);
            world->DestroyBody(fixB->GetBody());
        }
        //player and icicle collide
        if(fixA->GetDensity() == 100.0f && fixB->GetDensity() == 50.0f){
            auto icicle = (Sprite*) fixB->GetBody()->GetUserData();
            removeChild(icicle);
            world->DestroyBody(fixB->GetBody());
            auto player = (Sprite*) fixA->GetBody()->GetUserData();
            removeChild(player);
            world->DestroyBody(fixA->GetBody());
            died = true;
            gameOver();
            
        }
    }
    
    if(icicleTimer >= 55 && !died){
        HelloWorld::updateScore();
        srand(time(NULL));
        int spots[icicleSpots.size()];
        for(int j = 0; j < icicleSpots.size(); j++){
            spots[j] = j;
        }
        
        //spawn up to 6 icicles each tick
        int multi = rand() % 6;
        int m = 0;
        while(m <= multi){
            int i = rand() % icicleSpots.size();
            if(spots[i] != -1){
                HelloWorld::createIcicle(icicleSpots[i]);
                spots[i] = -1;
                m++;
            }
        }
        
        icicleTimer = 0;
    }
    else icicleTimer++;

}

void HelloWorld::updateScore(){
    score += 15;
    scoreItem->setString("Score: "+to_string(score));
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    delete _contactListener;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
