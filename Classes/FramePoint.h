#pragma once

#include "cocos2d.h"

/*
FramePoints represent the intersections (points) on the board.
FramePoint plays the role of positioning pieces and provides logic for easier placement and position tracking.

In standard Mills (aka Nine Men's Morris) there are 24 points on a grid of 3 frames with 4 bridges.
*/
class FramePoint : public cocos2d::Sprite {
private:

public:
    enum class State { EMPTY, WHITE, BLACK };
    State currentState = State::EMPTY;

    int indexI;
    int indexJ;

    static FramePoint* create(float xPos, float yPos);
    virtual bool init(cocos2d::Vec2 position);

    void setEmpty();
    bool isEmpty();

    // Turns on/off highlights on points when player is moving pieces.
    void setHighlight(bool flag);
};