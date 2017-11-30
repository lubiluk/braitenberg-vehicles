//
//  ActionLayer.cpp
//  Space
//
//  Created by Paweł Gajewski on 05/09/2017.
//
//

#include "ActionLayer.h"

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
        // Create a vehicle
        this->vehicleA = Node::create();
        this->vehicleA->setContentSize(Size(30.0f, 30.0f));
        this->vehicleA->setAnchorPoint(Vec2(0.5f, 0.5f));
        auto body = PhysicsBody::create();
        auto main = PhysicsShapeBox::create(Size(30.0f, 30.0f));
        auto material = PhysicsMaterial(10.0f, 1.0f, 0.2f);
        auto wheelLeft = PhysicsShapeBox::create(Size(5.0f, 12.0f), material, Vec2(-17.0f, -15.0f));
        auto wheelRight = PhysicsShapeBox::create(Size(5.0f, 12.0f), material, Vec2(17.0f, -15.0f));
        body->addShape(main);
        body->addShape(wheelLeft);
        body->addShape(wheelRight);
        this->vehicleA->addComponent(body);
        
        auto sensorLeft = DrawNode::create();
        sensorLeft->drawSolidCircle(Vec2(0.0f, 0.0f), 4.0f, 0.0f, 10, Color4F::RED);
        sensorLeft->setPosition(Vec2(0.0f, 30.0f));
        sensorLeft->setTag(1);
        this->vehicleA->addChild(sensorLeft);
        
        auto sensorRight = DrawNode::create();
        sensorRight->drawSolidCircle(Vec2(0.0f, 0.0f), 4.0f, 0.0f, 10, Color4F::RED);
        sensorRight->setPosition(Vec2(30.0f, 30.0f));
        sensorRight->setTag(2);
        this->vehicleA->addChild(sensorRight);
        
        this->vehicleA->setPosition(center.x + 100.0f, center.y - 100.0f);
        this->addChild(this->vehicleA);
    }
    
    {
        // Create a vehicle
        this->vehicleB = Node::create();
        this->vehicleB->setContentSize(Size(30.0f, 30.0f));
        this->vehicleB->setAnchorPoint(Vec2(0.5f, 0.5f));
        auto body = PhysicsBody::create();
        auto main = PhysicsShapeBox::create(Size(30.0f, 30.0f));
        auto material = PhysicsMaterial(10.0f, 1.0f, 0.2f);
        auto wheelLeft = PhysicsShapeBox::create(Size(5.0f, 12.0f), material, Vec2(-17.0f, -15.0f));
        auto wheelRight = PhysicsShapeBox::create(Size(5.0f, 12.0f), material, Vec2(17.0f, -15.0f));
        body->addShape(main);
        body->addShape(wheelLeft);
        body->addShape(wheelRight);
        this->vehicleB->addComponent(body);
        
        auto sensorLeft = DrawNode::create();
        sensorLeft->drawSolidCircle(Vec2(0.0f, 0.0f), 4.0f, 0.0f, 10, Color4F::RED);
        sensorLeft->setPosition(Vec2(0.0f, 30.0f));
        sensorLeft->setTag(1);
        this->vehicleB->addChild(sensorLeft);
        
        auto sensorRight = DrawNode::create();
        sensorRight->drawSolidCircle(Vec2(0.0f, 0.0f), 4.0f, 0.0f, 10, Color4F::RED);
        sensorRight->setPosition(Vec2(30.0f, 30.0f));
        sensorRight->setTag(2);
        this->vehicleB->addChild(sensorRight);
        
        this->vehicleB->setPosition(center.x - 100.0f, center.y - 100.0f);
//        this->vehicleB->setRotation(90);
        this->addChild(this->vehicleB);
    }
    
    // Touch listeners
    auto controlTouchListener = EventListenerTouchOneByOne::create();
    controlTouchListener->onTouchBegan = CC_CALLBACK_2(ActionLayer::onControlTouchBegan, this);
    controlTouchListener->onTouchMoved = CC_CALLBACK_2(ActionLayer::onControlTouchMoved, this);
    controlTouchListener->onTouchEnded = CC_CALLBACK_2(ActionLayer::onControlTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(controlTouchListener, this);
    
    this->scheduleUpdate();
    
    return true;
}

void ActionLayer::update(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto center = Vec2((visibleSize.width - origin.x) / 2, (visibleSize.height - origin.y) / 2);
    
    {
        auto sensorLeft = this->vehicleB->getChildByTag(1);
        auto positionSenorLeft = this->vehicleB->convertToWorldSpace(sensorLeft->getPosition());
        auto distanceLeft = center - positionSenorLeft;
        auto distanceLengthLeft = distanceLeft.length();
        auto distanceDirectionLeft = distanceLeft.getNormalized();
        
        auto sensorRight = this->vehicleB->getChildByTag(2);
        auto positionSenorRight = this->vehicleB->convertToWorldSpace(sensorRight->getPosition());
        auto distanceRight = center - positionSenorRight;
        auto distanceLengthRight = distanceRight.length();
        auto distanceDirectionRight = distanceRight.getNormalized();
        
        auto forceLeft = clampf(100000000000.0f/distanceLengthRight/distanceLengthRight/distanceLengthRight, 0.0f, 400000.0f);
        auto forceRight = clampf(100000000000.0f/distanceLengthLeft/distanceLengthLeft/distanceLengthLeft, 0.0f, 400000.0f);
        
        Vec2 forward {0.0f, 1.0f};
        forward.rotate(Vec2(0.0f, 0.0f), -CC_DEGREES_TO_RADIANS(this->vehicleB->getRotation()));
        
        auto angleLeft = CC_RADIANS_TO_DEGREES(forward.getAngle(distanceDirectionLeft));
        auto angleRight = CC_RADIANS_TO_DEGREES(forward.getAngle(distanceDirectionRight));
        
//        CCLOG("force: %f %f", forceLeft, forceRight);
//        CCLOG("angle left %f %f", angleLeft, angleRight);
        
        auto body = this->vehicleB->getPhysicsBody();

        if (angleLeft > -90 && angleLeft < 90) {
            body->applyForce(Vec2(0.0f, forceLeft), Vec2(-17.0f, -15.0f));
        }

        if (angleRight > -90 && angleRight < 90) {
            body->applyForce(Vec2(0.0f,  forceRight), Vec2(17.0f, -15.0f));
        }
        
        auto velocity = body->getVelocity();
        body->setVelocity(velocity * 0.98f);
        
        auto angularVelocity = body->getAngularVelocity();
        body->setAngularVelocity(angularVelocity * 0.98f);
    }
    
    {
        auto sensorLeft = this->vehicleA->getChildByTag(1);
        auto positionSenorLeft = this->vehicleA->convertToWorldSpace(sensorLeft->getPosition());
        auto distanceLeft = center - positionSenorLeft;
        auto distanceLengthLeft = distanceLeft.length();
        auto distanceDirectionLeft = distanceLeft.getNormalized();
        
        auto sensorRight = this->vehicleA->getChildByTag(2);
        auto positionSenorRight = this->vehicleA->convertToWorldSpace(sensorRight->getPosition());
        auto distanceRight = center - positionSenorRight;
        auto distanceLengthRight = distanceRight.length();
        auto distanceDirectionRight = distanceRight.getNormalized();
        
        auto forceLeft = clampf(100000000000.0f/distanceLengthLeft/distanceLengthLeft/distanceLengthLeft, 0.0f, 400000.0f);
        auto forceRight = clampf(100000000000.0f/distanceLengthRight/distanceLengthRight/distanceLengthRight, 0.0f, 400000.0f);
        
        Vec2 forward {0.0f, 1.0f};
        forward.rotate(Vec2(0.0f, 0.0f), -CC_DEGREES_TO_RADIANS(this->vehicleA->getRotation()));
        
        auto angleLeft = CC_RADIANS_TO_DEGREES(forward.getAngle(distanceDirectionLeft));
        auto angleRight = CC_RADIANS_TO_DEGREES(forward.getAngle(distanceDirectionRight));
        
        //    CCLOG("distance: %f %f", distanceLeft, distanceRight);
        //    CCLOG("force: %f %f", forceLeft, forceRight);
        //    CCLOG("angle left %f", angle);
        
        auto body = this->vehicleA->getPhysicsBody();
        
        if (angleLeft > -90 && angleLeft < 90) {
            body->applyForce(Vec2(0.0f, forceLeft), Vec2(-17.0f, -15.0f));
        }
        
        if (angleRight > -90 && angleRight < 90) {
            body->applyForce(Vec2(0.0f,  forceRight), Vec2(17.0f, -15.0f));
        }
        
        auto velocity = body->getVelocity();
        body->setVelocity(velocity * 0.98f);
        
        auto angularVelocity = body->getAngularVelocity();
        body->setAngularVelocity(angularVelocity * 0.98f);
    }
}

#pragma mark Touch event listeners

bool ActionLayer::onControlTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = this->convertTouchToNodeSpace(touch);
    
    if (this->vehicleB->getBoundingBox().containsPoint(location)) {
        this->dragB = true;
        return true;
    }
    
    if (this->vehicleA->getBoundingBox().containsPoint(location)) {
        this->dragB = false;
        return true;
    }
    
    return false;
}

void ActionLayer::onControlTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = this->convertTouchToNodeSpace(touch);
    
    if (this->dragB) {
        this->vehicleB->setPosition(location);
    } else {
        this->vehicleA->setPosition(location);
    }
}

void ActionLayer::onControlTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = this->convertTouchToNodeSpace(touch);
    
    if (this->dragB) {
        this->vehicleB->setPosition(location);
    } else {
        this->vehicleA->setPosition(location);
    }
}
