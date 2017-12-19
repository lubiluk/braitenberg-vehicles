//
//  ActionLayer.h
//  Space
//
//  Created by Paweł Gajewski on 05/09/2017.
//
//

#ifndef __ACTION_LAYER_H__
#define __ACTION_LAYER_H__

#include "Vehicle.h"
#include <cocos2d.h>
#include <vector>

enum class PositionStatus;

class ActionLayer : public cocos2d::Layer
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(ActionLayer);
    
    void update(float dt) override;
    
protected:
    cocos2d::Vector<Vehicle *> vehicles;
};

#endif /* __ACTION_LAYER_H__ */
