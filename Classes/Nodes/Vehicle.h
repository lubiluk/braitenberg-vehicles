//
//  Vehicle.h
//  BraitenbergVehicles
//
//  Created by Paweł Gajewski on 18/12/2017.
//

#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include <cocos2d.h>
#include <vector>

class Vehicle : public cocos2d::Node {
private:
    bool dragged;
    bool crossed;
    cocos2d::Node *cross;
    
public:
    Vehicle();
    
    bool init() override;
    
    CREATE_FUNC(Vehicle);
    
    void updatePropulsion(cocos2d::Vec2 lightPosition);
    void setCrossed(bool crossed);
    bool getCrossed();
    
    bool onControlTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onControlTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onControlTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    void createShape();
    void attachEventListeners();
    float calculateLightIntensity(float distance);
    float calculateSensorSignal(cocos2d::Vec2 sensorPosition, cocos2d::Vec2 lightPosition);
};

#endif /* __VEHICLE_H__ */
