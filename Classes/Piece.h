#pragma once

#include "cocos2d.h"

class Game;
class FramePoint;

/*
Piece provides a way of interaction between the player and the game through selecting, dragging and dropping.
*/
class Piece : public cocos2d::Sprite {
private:	
	Game* game;

	//// Dimensions:
	const cocos2d::Rect dimensions = cocos2d::Rect(20, 20, 80, 80);
	const cocos2d::Rect moveDimensions = cocos2d::Rect(20, 10, 80, 100);
	const cocos2d::Rect flyDimensions = cocos2d::Rect(0, 10, 120, 100);
	const cocos2d::Rect flyMoveDimensions = cocos2d::Rect(0, 0, 120, 100);

	//// Positioning:
	bool isMoving = false;
	cocos2d::Vec2 previousPosition;
	cocos2d::Vec2 dragOffset;

	//// Textures:
	std::string spriteInitialFileName;
	std::string spriteFileName;
	std::string spriteMoveFileName;
	cocos2d::Rect pieceDimensions;
	cocos2d::Rect pieceMoveDimensions;

public:
	enum class Color { WHITE, BLACK };
	Color color = Color::WHITE;
	
	FramePoint* currentPoint = nullptr;
	bool canFly = false;

	//// Init:
	static Piece* create(Game* game, Color color);
	virtual bool init(Game* game, Color color);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void enableFlying();
	bool hasBeenPlaced();

	// Turns highlight on/off, usually to indicate the ability of being selected.
	void setHighlight(bool flag);
};