#include "Board.h"

#include "Piece.h"
#include "FramePoint.h"

Board* Board::create() {
	Board* board = new (std::nothrow) Board();
	if(board && board->init()) {
		board->autorelease();
		return board;
	}
	CC_SAFE_DELETE(board);
	return nullptr;
}
bool Board::init() {
	if(!cocos2d::Scene::init()) {
		return false;
	}
	//// Grid/Background:
	cocos2d::Sprite* grid_sprite = cocos2d::Sprite::create("Grid.png");
	grid_sprite->setPosition(cocos2d::Vec2(640, 480));
	this->addChild(grid_sprite, 0);

	//// Buttons:
	startButton->setAnchorPoint(cocos2d::Vec2(0, 0));
	startButton->setPosition(cocos2d::Vec2(590, 310));
	this->addChild(startButton);

	//// Titles and Subtitles:
	titleSprite->setPosition(cocos2d::Vec2(640, 620));
	this->addChild(titleSprite);
	subtitleSprite->setPosition(cocos2d::Vec2(640, 550));
	this->addChild(subtitleSprite);
	nameTitleSprite->setPosition(cocos2d::Vec2(640, 480));
	this->addChild(nameTitleSprite);
	//// MILL:
	millSprite->setGlobalZOrder(4);
	millSprite->setVisible(false);
	this->addChild(millSprite);

	/* STACKS */
	//// Visibility:
	whiteStack->setVisible(false);
	whiteStackTop->setVisible(false);
	blackStack->setVisible(false);
	blackStackTop->setVisible(false);

	//// Positioning and Anchoring:
	whiteStack->setAnchorPoint(cocos2d::Vec2(0, 0));
	whiteStack->setPosition(cocos2d::Vec2(390, 430));
	whiteStackTop->setAnchorPoint(cocos2d::Vec2(0, 0));
	whiteStackTop->setPosition(cocos2d::Vec2(560, 430));
	this->addChild(whiteStack);
	this->addChild(whiteStackTop);

	blackStack->setAnchorPoint(cocos2d::Vec2(1, 0));
	blackStack->setPosition(cocos2d::Vec2(890, 430));
	blackStackTop->setAnchorPoint(cocos2d::Vec2(0, 0));
	blackStackTop->setPosition(cocos2d::Vec2(640, 430));
	this->addChild(blackStack);
	this->addChild(blackStackTop);

	//// Label Placeholders:
	whiteLabel->setPosition(500, 400);
	whiteLabel->setVisible(false);
	this->addChild(whiteLabel, 1);

	blackLabel->setPosition(780, 400);
	blackLabel->setColor(cocos2d::Color3B::BLACK);
	blackLabel->setVisible(false);
	this->addChild(blackLabel, 1);

	return true;
}

void Board::initGameUI() {
	startButton->loadTextures(Board::BUTTON_RESTART, Board::BUTTON_RESTART_PRESSED);
	nameTitleSprite->setVisible(false);

	initStacks();
}
void Board::resetGameUI() {
	startButton->loadTextures(Board::BUTTON_START, Board::BUTTON_START_PRESSED);
}

void Board::setTitle(std::string title) {
	titleSprite->setTexture(title);
}
void Board::setSubtitle(std::string title) {
	subtitleSprite->setTexture(title);
}
void Board::shakeTitle() {
	titleSprite->runAction(shakeSprite(titleSprite));
}
void Board::spawnMILL(cocos2d::Vec2 position) {
	// Spawns a MILL! title in position for a short moment.
	millSprite->setPosition(position);
	cocos2d::Sequence* shakeMill = shakeSprite(millSprite);
	cocos2d::Sequence* sequence = cocos2d::Sequence::create(
		cocos2d::Show::create(),
		shakeMill,
		cocos2d::Hide::create(),
		nullptr
	);

	millSprite->runAction(sequence);
}

void Board::addFramePoint(FramePoint* point) {
	this->addChild(point, 1);
}
void Board::addPiece(Piece* piece) {
	this->addChild(piece, 2);
}

void Board::popWhite() {
	if(whiteOnStack == 0) return;
	whiteOnStack--;
	updateWhiteStack();
}
void Board::popBlack() {
	if(blackOnStack == 0) return;
	blackOnStack--;
	updateBlackStack();
}
void Board::putWhite() {
	if(whiteOnStack == STACK_SIZE) return;
	whiteOnStack++;
	updateWhiteStack();
}
void Board::putBlack() {
	if(blackOnStack == STACK_SIZE) return;
	blackOnStack++;
	updateBlackStack();
}

void Board::initStacks() {
	whiteOnStack = STACK_SIZE;
	blackOnStack = STACK_SIZE;
	updateWhiteStack();
	updateBlackStack();
}
void Board::updateWhiteStack() {
	if(whiteOnStack == 0) {
		whiteStackTop->setVisible(false);
		whiteStack->setVisible(false);
		whiteLabel->setVisible(false);
		return;
	}
	whiteLabel->setString(std::to_string(whiteOnStack));

	int shift = 20 * (STACK_SIZE - whiteOnStack);

	whiteStackTop->setPosition(cocos2d::Vec2(560 - shift, 430));
	whiteStack->setTextureRect(cocos2d::Rect(30, 0, 170 - shift, 80));

	whiteStackTop->setVisible(true);
	whiteStack->setVisible(true);
	whiteLabel->setVisible(true);
}
void Board::updateBlackStack() {
	if(blackOnStack == 0) {
		blackStackTop->setVisible(false);
		blackStack->setVisible(false);
		blackLabel->setVisible(false);
		return;
	}
	blackLabel->setString(std::to_string(blackOnStack));

	int shift = 20 * (STACK_SIZE - blackOnStack);

	blackStackTop->setPosition(cocos2d::Vec2(640 + shift, 430));
	blackStack->setTextureRect(cocos2d::Rect(100 + shift, 0, 170 - shift, 80));

	blackStackTop->setVisible(true);
	blackStack->setVisible(true);
	blackLabel->setVisible(true);
}

cocos2d::Sequence* Board::shakeSprite(cocos2d::Sprite* sprite) {
	// Shakes sprite, usually called when piece isn't selectable
	cocos2d::Vec2 originalPosition = sprite->getPosition();
	cocos2d::Sequence* shakeAction = cocos2d::Sequence::create(
		// Shake sprite around
		cocos2d::MoveBy::create(0.025f, cocos2d::Vec2(CCRANDOM_MINUS1_1() * 10, CCRANDOM_MINUS1_1() * 10)),
		cocos2d::MoveTo::create(0.025f, originalPosition),
		cocos2d::MoveBy::create(0.025f, cocos2d::Vec2(CCRANDOM_MINUS1_1() * 10, CCRANDOM_MINUS1_1() * 10)),
		cocos2d::MoveTo::create(0.025f, originalPosition),
		cocos2d::MoveBy::create(0.025f, cocos2d::Vec2(CCRANDOM_MINUS1_1() * 10, CCRANDOM_MINUS1_1() * 10)),
		cocos2d::MoveTo::create(0.025f, originalPosition),
		cocos2d::MoveBy::create(0.025f, cocos2d::Vec2(CCRANDOM_MINUS1_1() * 10, CCRANDOM_MINUS1_1() * 10)),
		// Move sprite back to its original position
		cocos2d::MoveTo::create(0.05f, originalPosition),
		nullptr
	);

	return shakeAction;
}