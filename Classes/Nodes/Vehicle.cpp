//
//  Vehicle.cpp
//  BraitenbergVehicles
//
//  Created by Paweł Gajewski on 18/12/2017.
//

#include "Vehicle.h"

using namespace cocos2d;

static const Size vehicleSize { 15.0f, 15.0f };
static const Size wheelSize { 1.5f, 5.0f };
static const float sensorRadius { 2.0f };
static const float sensorAngle { 90.0f };
static const float sensorFactor = 100.0f;
static const float lightSourceIntensity = 100000.0f;
static const float actuationFactor = 10000.0f;
static const float actuationMaxForce = 5000.0f;
static const Vec2 wheelCenterLeft = Vec2(-vehicleSize.width / 2 - wheelSize.width, -vehicleSize.height / 2);
static const Vec2 wheelCenterRight = Vec2(vehicleSize.width / 2 + wheelSize.width, -vehicleSize.height / 2);
static const float waterFriction = 0.8f;

Vehicle::Vehicle(): crossed(false)
{
}

bool Vehicle::init()
{
    if (!Node::init()) {
        return false;
    }
    
    createShape();
    attachEventListeners();
    
    return true;
}

void Vehicle::createShape()
{
    setContentSize(vehicleSize);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    auto body = PhysicsBody::create();
    auto main = PhysicsShapeBox::create(vehicleSize);
    auto material = PhysicsMaterial(1.0f, 1.0f, 0.2f);
    auto wheelLeft = PhysicsShapeBox::create(wheelSize, material, wheelCenterLeft);
    auto wheelRight = PhysicsShapeBox::create(wheelSize, material, wheelCenterRight);
    
    body->addShape(main);
    body->addShape(wheelLeft);
    body->addShape(wheelRight);
    body->setLinearDamping(waterFriction);
    body->setAngularDamping(waterFriction);
    
    addComponent(body);
    
    auto sensorLeft = DrawNode::create();
    sensorLeft->drawSolidCircle(Vec2(0.0f, 0.0f), sensorRadius, 0.0f, 10, Color4F::RED);
    sensorLeft->setPosition(Vec2(0.0f, vehicleSize.height));
    sensorLeft->setTag(1);
    
    addChild(sensorLeft);
    
    auto sensorRight = DrawNode::create();
    sensorRight->drawSolidCircle(Vec2(0.0f, 0.0f), sensorRadius, 0.0f, 10, Color4F::RED);
    sensorRight->setPosition(vehicleSize);
    sensorRight->setTag(2);
    
    addChild(sensorRight);
    
    cross = Node::create();
    cross->setVisible(false);
    
    auto line1 = DrawNode::create();
    line1->drawLine(Vec2(0.0f, 0.0f), vehicleSize, Color4F::RED);
    line1->setPosition(Vec2(0.5f, 0.5f));
    
    auto line2 = DrawNode::create();
    line2->drawLine(Vec2(0.0f, vehicleSize.width), Vec2(vehicleSize.height, 0.0f), Color4F::RED);
    line2->setPosition(Vec2(0.5f, 0.5f));
    
    cross->addChild(line1);
    cross->addChild(line2);
    
    addChild(cross);
}

void Vehicle::attachEventListeners()
{
    // Touch listeners
    auto controlTouchListener = EventListenerTouchOneByOne::create();
    controlTouchListener->onTouchBegan = CC_CALLBACK_2(Vehicle::onControlTouchBegan, this);
    controlTouchListener->onTouchMoved = CC_CALLBACK_2(Vehicle::onControlTouchMoved, this);
    controlTouchListener->onTouchEnded = CC_CALLBACK_2(Vehicle::onControlTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(controlTouchListener, this);
}

void Vehicle::updatePropulsion(cocos2d::Vec2 lightPosition)
{
    if (dragged)
        return;
    
    const auto sensorLeft = getChildByTag(1);
    const auto positionSenorLeft = convertToWorldSpace(sensorLeft->getPosition());
    
    const auto sensorOutputLeft = calculateSensorSignal(positionSenorLeft, lightPosition);
    
    const auto sensorRight = getChildByTag(2);
    const auto positionSenorRight = convertToWorldSpace(sensorRight->getPosition());
    const auto sensorOutputRight = calculateSensorSignal(positionSenorRight, lightPosition);
    
    const auto forceLeft = clampf(sensorOutputLeft * actuationFactor, 0.0f, actuationMaxForce);
    const auto forceRight = clampf(sensorOutputRight * actuationFactor, 0.0f, actuationMaxForce);
    
    CCLOG("force: %f %f", forceLeft, forceRight);
    
    const auto body = getPhysicsBody();
    
    if (crossed) {
        body->applyForce(Vec2(0.0f, forceLeft), wheelCenterRight);
        body->applyForce(Vec2(0.0f, forceRight), wheelCenterLeft);
    } else {
        body->applyForce(Vec2(0.0f, forceLeft), wheelCenterLeft);
        body->applyForce(Vec2(0.0f, forceRight), wheelCenterRight);
    }
}

#pragma mark Touch event listeners

bool Vehicle::onControlTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = touch->getLocation();
    
    if (getBoundingBox().containsPoint(location)) {
        dragged = true;
        
        auto body = getPhysicsBody();
        body->resetForces();
        body->setVelocity(Vec2::ZERO);
        body->setAngularVelocity(0.0f);
        
        return true;
    }

    return false;
}

void Vehicle::onControlTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = touch->getLocation();
    
    setPosition(location);
}

void Vehicle::onControlTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = touch->getLocation();
    
    setPosition(location);
    dragged = false;
}

#pragma mark - Sensor calculation

float Vehicle::calculateLightIntensity(float distance)
{
    return lightSourceIntensity / distance / distance;
}

float Vehicle::calculateSensorSignal(Vec2 sensorPosition, Vec2 lightPosition)
{
    auto distance = lightPosition - sensorPosition;
    auto distanceLength = distance.length();
    auto distanceDirection = distance.getNormalized();
    auto lightIntensity = calculateLightIntensity(distanceLength);
    
    CCLOG("distance: %f", distanceLength);
    CCLOG("lightIntensity: %f", lightIntensity);
    
    Vec2 forward {0.0f, 1.0f};
    forward.rotate(Vec2(0.0f, 0.0f), -CC_DEGREES_TO_RADIANS(getRotation()));
    
    auto angle = CC_RADIANS_TO_DEGREES(forward.getAngle(distanceDirection));
    
    float angleFactor = 0.0f;
    
    if (angle < -sensorAngle || angle > sensorAngle) {
        angleFactor = 0.0f;
    } else {
        angleFactor = 1.0f;
    }
    
    return clampf(lightIntensity * angleFactor / sensorFactor, 0.0f, 1.0f);
}

#pragma mark - Getters & Setters

void Vehicle::setCrossed(bool crossed)
{
    this->crossed = crossed;
    
    cross->setVisible(crossed);
}

bool Vehicle::getCrossed()
{
    return crossed;
}
