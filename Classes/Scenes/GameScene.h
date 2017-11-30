#ifndef __VACUUM_SCENE_H__
#define __VACUUM_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
    static GameScene* create();
    
    bool init() override;
    
private:
    cocos2d::Layer *hudLayer;
    
    void onMissionSuccess(cocos2d::EventCustom *event);
    void onMissionFailure(cocos2d::EventCustom *event);
};

#endif // __VACUUM_SCENE_H__
