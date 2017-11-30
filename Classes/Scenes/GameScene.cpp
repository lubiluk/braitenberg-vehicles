//
//  GameScene.cpp
//  BraitenbergVehicles
//
//  Created by Paweł Gajewski on 23/11/2017.
//
//
#include "GameScene.h"
#include "BackgroundLayer.h"
#include "ActionLayer.h"

USING_NS_CC;


GameScene* GameScene::create()
{
    GameScene* scene = new (std::nothrow) GameScene();
    if (scene && scene->initWithPhysics() && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    
    CC_SAFE_DELETE(scene);
    return nullptr;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    // Turn off the gravity
    auto physicsWorld = this->getPhysicsWorld();
    physicsWorld->setGravity(Vec2(0.0, 0.0));
    // Turn on debug drawing
    physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // Background layer
    this->addChild(BackgroundLayer::create());
    // Action layer
    auto actionLayer = ActionLayer::create();
    this->addChild(actionLayer);
    
    return true;
}
