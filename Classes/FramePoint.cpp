#include "FramePoint.h"

#include "Board.h"

FramePoint* FramePoint::create(float xPos, float yPos) {
    FramePoint* point = new (std::nothrow) FramePoint();
    if(point && point->init(cocos2d::Vec2(xPos, yPos))) {
        point->autorelease();
        return point;
    }
    CC_SAFE_DELETE(point);
    return nullptr;
}
bool FramePoint::init(cocos2d::Vec2 position) {
    if(!cocos2d::Sprite::initWithFile(Board::FRAMEPOINT)) {
        return false;
    }

    this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    this->setPosition(position);

    return true;
}

void FramePoint::setEmpty() {
    currentState = State::EMPTY;
}
bool FramePoint::isEmpty() {
    return currentState == State::EMPTY;
}

void FramePoint::setHighlight(bool flag) {
    if(flag) {
        this->setTexture(Board::FRAMEPOINT_HIGHLIGHT);
    } else {
        this->setTexture(Board::FRAMEPOINT);
    }
}
