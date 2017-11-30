//
//  ActionLayer.h
//  Space
//
//  Created by Paweł Gajewski on 05/09/2017.
//
//

#ifndef __ACTION_LAYER_H__
#define __ACTION_LAYER_H__

#include <cocos2d.h>
#include <vector>

enum class PositionStatus;

class ActionLayer : public cocos2d::Layer
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(ActionLayer);
    
    void update(float dt) override;
    
    bool onControlTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onControlTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onControlTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
protected:
    cocos2d::Node *vehicleA;
    cocos2d::Node *vehicleB;
    
    bool dragB;
};

#endif /* __ACTION_LAYER_H__ */
