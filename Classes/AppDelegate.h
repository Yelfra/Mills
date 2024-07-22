#pragma once

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Board.h"
#include "Game.h"

class  AppDelegate : private cocos2d::Application {
private:
    Game* game;
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};