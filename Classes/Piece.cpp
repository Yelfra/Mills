#include "Piece.h"

#include "FramePoint.h"
#include "Game.h";
#include "Board.h"

Piece* Piece::create(Game* game, Color color) {
	Piece* piece = new (std::nothrow) Piece();
	if(piece && piece->init(game, color)) {
		piece->autorelease();
		return piece;
	}
	CC_SAFE_DELETE(piece);
	return nullptr;
}
bool Piece::init(Game* game, Color color) {
	this->game = game;

	// Initialize textures according to initialized color.
	switch(color) {
		case (Color::WHITE):
		{
			spriteInitialFileName = Board::WHITE_HIGHLIGHT;
			spriteFileName = Board::WHITE;
			spriteMoveFileName = Board::WHITE_MOVE;
			break;
		}
		case (Color::BLACK):
		{
			spriteInitialFileName = Board::BLACK_HIGHLIGHT;
			spriteFileName = Board::BLACK;
			spriteMoveFileName = Board::BLACK_MOVE;
			break;
		}
		default:
			return false;
	}
	pieceDimensions = dimensions;
	pieceMoveDimensions = moveDimensions;
	if(!cocos2d::Sprite::initWithFile(spriteInitialFileName, pieceDimensions)) {
		return false;
	}
	this->color = color;

	// Initialize touchListener
	cocos2d::EventListenerTouchOneByOne* touchListener = cocos2d::EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Piece::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Piece::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Piece::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

bool Piece::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	cocos2d::Vec2 touchLocation = touch->getLocation();
	cocos2d::Vec2 piecePosition = this->getPosition();
	previousPosition = piecePosition;
	
	if(this->getBoundingBox().containsPoint(touchLocation)) {
		dragOffset = touchLocation - piecePosition;

		// Select piece if possible
		if(game->selectPiece(this)) {
			this->setTexture(spriteFileName);
			this->setTextureRect(pieceDimensions);
			return true;
		} else {
			this->runAction(Board::shakeSprite(this));
		}
	}
	return false;
}
void Piece::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
	cocos2d::Vec2 pickUpPosition = touch->getLocation() - dragOffset;
	this->setPosition(pickUpPosition);

	// isMoving will prevent the code underneath to be run multiple times
	if(!isMoving) {
		isMoving = true;
		if(!this->hasBeenPlaced()) {
			if(color == Color::WHITE) {
				game->board->popWhite();
			} else {
				game->board->popBlack();
			}
		}
		this->setTexture(spriteMoveFileName);
		this->setTextureRect(pieceMoveDimensions);

		this->setLocalZOrder(this->getLocalZOrder() + 1);

		game->highlightPossibleMoves(this, true);
	}
}
void Piece::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {	
	if(isMoving) game->highlightPossibleMoves(this, false);

	this->game->dropPiece(this);

	// If the piece isn't placed on a new point, return to previous position
	if(hasBeenPlaced()) {
		this->setTexture(spriteFileName);
		this->setPosition(currentPoint->getPosition());
	} else if (isMoving) {
		if(color == Color::WHITE) {
			game->board->putWhite();
		} else {
			game->board->putBlack();
		}
		this->setPosition(previousPosition);
	
		this->setTexture(spriteInitialFileName);
		this->setTextureRect(pieceDimensions);

		this->setLocalZOrder(this->getLocalZOrder() - 1);
	} else {
		this->setTexture(spriteInitialFileName);
	}

	isMoving = false;
}

void Piece::enableFlying() {
	switch(color) {
		case (Color::WHITE):
		{
			spriteFileName = Board::WHITE_FLY;
			spriteMoveFileName = Board::WHITE_FLY_MOVE;
			break;
		}
		case (Color::BLACK):
		{
			spriteFileName = Board::BLACK_FLY;
			spriteMoveFileName = Board::BLACK_FLY_MOVE;
			break;
		}
		default:
			return;
	}
	pieceDimensions = flyDimensions;
	pieceMoveDimensions = flyMoveDimensions;
	this->setTexture(spriteFileName);
	this->setTextureRect(pieceDimensions);

	canFly = true;
}
bool Piece::hasBeenPlaced() {
	return currentPoint != nullptr;
}

void Piece::setHighlight(bool flag) {
	if(flag) {
		if(color == Color::WHITE) {
			if(canFly) {
				this->setTexture(Board::WHITE_FLY_HIGHLIGHT);
			} else {
				this->setTexture(Board::WHITE_HIGHLIGHT);
			}
		} else {
			if(canFly) {
				this->setTexture(Board::BLACK_FLY_HIGHLIGHT);
			} else {
				this->setTexture(Board::BLACK_HIGHLIGHT);
			}
		}
	} else {
		if(color == Color::WHITE) {
			if(canFly) {
				this->setTexture(Board::WHITE_FLY);
			} else {
				this->setTexture(Board::WHITE);
			}
		} else {
			if(canFly) {
				this->setTexture(Board::BLACK_FLY);
			} else {
				this->setTexture(Board::BLACK);
			}
		}
	}
}
