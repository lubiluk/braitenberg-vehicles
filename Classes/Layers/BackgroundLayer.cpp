#include "BackgroundLayer.h"

USING_NS_CC;

bool BackgroundLayer::init()
{
    if ( !LayerColor::initWithColor(Color4B(133, 193, 233, 255)) ) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}
