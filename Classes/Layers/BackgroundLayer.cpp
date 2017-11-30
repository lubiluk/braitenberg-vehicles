//
//  BackgroundLayer.cpp
//  BraitenbergVehicles
//
//  Created by Paweł Gajewski on 23/11/2017.
//
//
#include "BackgroundLayer.h"

USING_NS_CC;

bool BackgroundLayer::init()
{
    if ( !LayerColor::initWithColor(Color4B(133, 193, 233, 255)) ) {
        return false;
    }
    
    return true;
}
