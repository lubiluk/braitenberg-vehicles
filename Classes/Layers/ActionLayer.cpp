//
//  ActionLayer.cpp
//  BraitenbergVehicles
//
//  Created by Paweł Gajewski on 23/11/2017.
//
//

#include "ActionLayer.h"
#include "Vehicle.h"

USING_NS_CC;

bool ActionLayer::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto center = Vec2((visibleSize.width - origin.x) / 2, (visibleSize.height - origin.y) / 2);
    
    auto bounds = Node::create();
    bounds->addComponent(PhysicsBody::createEdgeBox(visibleSize));
    bounds->setPosition(center);
    this->addChild(bounds);
    
    // Light
    auto light = DrawNode::create();
    light->drawSolidCircle(Vec2(0.0f, 0.0f), 10.0f, 0.0f, 10, Color4F::YELLOW);
    light->setAnchorPoint(Vec2(0.5f, 0.5f));
    light->setPosition(center);
    
    this->addChild(light);
    
    {
        auto vehicle = Vehicle::create();
        vehicle->setPosition(center.x + 100.0f, center.y - 100.0f);
        addChild(vehicle);
        vehicles.pushBack(vehicle);
    }
    
    {
        auto vehicle = Vehicle::create();
        vehicle->setCrossed(true);
        vehicle->setPosition(center.x - 100.0f, center.y - 100.0f);
        addChild(vehicle);
        vehicles.pushBack(vehicle);
    }
    
    this->scheduleUpdate();
    
    return true;
}

void ActionLayer::update(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto center = Vec2((visibleSize.width - origin.x) / 2, (visibleSize.height - origin.y) / 2);
    
    for(auto v : vehicles) {
        v->updatePropulsion(center);
    }
}
