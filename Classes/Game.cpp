#include "Game.h"

#include "Board.h"
#include "FramePoint.h"
#include "Piece.h"

Game::Game(Board* board) {
	this->board = board;

	initFramePoints();
	initUI();
}
Game::~Game() {}
void Game::start() {
	//// Reset Game Variables:
	isWhitesTurn = true;
	piecesPlaced = 0;
	whitePiecesOnBoard = 0;
	blackPiecesOnBoard = 0;
	piecesLeftToRemove = 0;

	board->initGameUI();

	board->setTitle(Board::TURN_WHITE);
	board->setSubtitle(Board::INSTR_PLACE_PIECE);

	initFramePoints();
	initPieces();

	spawnPiece();

	gameInProgress = true;
}
void Game::end(bool whiteWins, bool byReducing) {
	if(whiteWins) {
		board->setTitle(Board::WHITE_WINS);
	} else {
		board->setTitle(Board::BLACK_WINS);
	}
	if(byReducing) {
		board->setSubtitle(Board::BY_REDUCING);
	} else {
		board->setSubtitle(Board::BY_BLOCKING);
	}

	board->resetGameUI();

	gameInProgress = false;
}

void Game::initFramePoints() {
	//// Reset framePoints:
	if(framePoints.data()) {
		for(int i = 0; i < NUM_OF_FRAMES; i++) {
			for(int j = 0; j < POINTS_PER_FRAME; j++) {
				framePoints[i][j]->setEmpty();
			}
		}
		return;
	} else {
		framePoints.reserve(NUM_OF_FRAMES);
	}

	// Put FramePoints on specific positions on the board, frame by frame, starting from the outermost.
	// FramePoints are put on a frame clockwise, starting from the top left corner.
	// Even indices will always be corners, odd indices will always be on bridges.
	cocos2d::Rect frame = board->outsideFrame;
	for(int i = 0; i < NUM_OF_FRAMES; i++) {
		std::vector<FramePoint*> currentFrame;
		framePoints.push_back(currentFrame);
		framePoints[i].reserve(POINTS_PER_FRAME);

		framePoints[i].push_back(FramePoint::create(frame.getMinX(), frame.getMaxY()));
		framePoints[i].push_back(FramePoint::create(frame.getMidX(), frame.getMaxY()));
		framePoints[i].push_back(FramePoint::create(frame.getMaxX(), frame.getMaxY()));
		framePoints[i].push_back(FramePoint::create(frame.getMaxX(), frame.getMidY()));
		framePoints[i].push_back(FramePoint::create(frame.getMaxX(), frame.getMinY()));
		framePoints[i].push_back(FramePoint::create(frame.getMidX(), frame.getMinY()));
		framePoints[i].push_back(FramePoint::create(frame.getMinX(), frame.getMinY()));
		framePoints[i].push_back(FramePoint::create(frame.getMinX(), frame.getMidY()));

		// Mark FramePoints and put them on the board.
		for(int j = 0; j < POINTS_PER_FRAME; j++) {
			framePoints[i][j]->indexI = i;
			framePoints[i][j]->indexJ = j;
			board->addFramePoint(framePoints[i][j]);
		}

		// Resize dimensions for the next frame.
		frame.origin += cocos2d::Vec2(130, 100);
		frame.size.width -= 260;
		frame.size.height -= 200;
	}
}
void Game::initPieces() {
	//// Reset pieces:
	if(pieces.data()) {
		for(int i = 0; i < NUM_OF_PIECES; i++) {
			if(pieces[i]) pieces[i]->removeFromParentAndCleanup(true);
		}
		pieces.clear();
	} else {
		pieces.reserve(NUM_OF_PIECES);
	}

	// Pieces are spawned in positions relative to respective stacks.
	cocos2d::Vec2 whitePosition = cocos2d::Vec2(590, 470);
	cocos2d::Vec2 blackPosition = cocos2d::Vec2(690, 470);

	// Even indices are white pieces, odd indices are black pieces, to make iteration over colors simpler.
	for(int i = 0; i < NUM_OF_PIECES; i += 2) {
		pieces.push_back(Piece::create(this, Piece::Color::WHITE));
		pieces[i]->setPosition(whitePosition);
		pieces[i]->setPosition(whitePosition - cocos2d::Vec2(i * 10, 0));
		pieces[i]->retain();

		pieces.push_back(Piece::create(this, Piece::Color::BLACK));
		pieces[i + 1]->setPosition(blackPosition);
		pieces[i + 1]->setPosition(blackPosition + cocos2d::Vec2(i * 10, 0));
		pieces[i + 1]->retain();
	}
}
void Game::initUI() {
	//// Setup Button Functions:
	board->startButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch(type) {
			case (cocos2d::ui::Widget::TouchEventType::BEGAN):
				break;
			case (cocos2d::ui::Widget::TouchEventType::ENDED):
			{
				if(gameInProgress) {
					board->resetGameUI();
					gameInProgress = false;
				}
				start();
				break;
			}
			default:
				break;
		}
	});
}

bool Game::isTurnOf(Piece* piece) {
	return (isWhitesTurn && piece->color == Piece::Color::WHITE) || (!isWhitesTurn && piece->color == Piece::Color::BLACK);
}
bool Game::hasPiecesToRemove() {
	return piecesLeftToRemove > 0;
}
bool Game::allPiecesPlaced() {
	return piecesPlaced == NUM_OF_PIECES;
}

void Game::endTurn() {
	if(checkWinByReducing() || checkWinByBlocking()) return;

	isWhitesTurn = !isWhitesTurn;
	if(isWhitesTurn) {
		board->setTitle(Board::TURN_WHITE);
	} else {
		board->setTitle(Board::TURN_BLACK);
	}

	if(allPiecesPlaced()) {
		board->setSubtitle(Board::INSTR_MOVE_PIECE);
	} else {
		board->setSubtitle(Board::INSTR_PLACE_PIECE);
		spawnPiece();
	}
}
bool Game::checkWinByReducing() {
	if(!allPiecesPlaced()) return false;

	if(whitePiecesOnBoard < 3) {
		end(false, true);
		return true;
	} else if(blackPiecesOnBoard < 3) {
		end(true, true);
		return true;
	}
	return false;
}
bool Game::checkWinByBlocking() {
	if(!allPiecesPlaced()) return false;

	bool whiteBlocked = true;
	bool blackBlocked = true;
	for(int i = 0; i < NUM_OF_PIECES; i++) {
		if(pieces[i] && pieces[i]->hasBeenPlaced() && !isPieceStuck(pieces[i])) {
			if(i % 2) {
				blackBlocked = false;
			} else {
				whiteBlocked = false;
			}
		}
	}

	if(whiteBlocked) {
		end(false, false);
		return true;
	} else if(blackBlocked) {
		end(true, false);
		return true;
	}
	return false;
}
void Game::spawnPiece() {
	if(allPiecesPlaced()) return;

	if(pieces[piecesPlaced]->color == Piece::Color::WHITE) {
		whitePiecesOnBoard++;
	} else {
		blackPiecesOnBoard++;
	}
	board->addPiece(pieces[piecesPlaced++]);
}
void Game::checkFlyingCondition() {
	if(!allPiecesPlaced()) return;

	if(whitePiecesOnBoard == 3) {
		for(int i = 0; i < NUM_OF_PIECES; i += 2) {
			if(pieces[i]) pieces[i]->enableFlying();
		}
	}
	if(blackPiecesOnBoard == 3) {
		for(int i = 1; i < NUM_OF_PIECES; i += 2) {
			if(pieces[i]) pieces[i]->enableFlying();
		}
	}
}

bool Game::selectPiece(Piece* piece) {
	if(!gameInProgress) return false;

	// Selects a piece if allowed to do so.
	if(isTurnOf(piece)) {
		if(!hasPiecesToRemove() && isPieceMoveable(piece)) {
			return true;
		}
	} else {
		if(!hasPiecesToRemove()) {
			board->shakeTitle();
		} else if(removePiece(piece)) {
			unhighlightAllPieces();

			checkFlyingCondition();
			
			highlightRemoveablePieces();
		}
	}

	return false;
}
bool Game::dropPiece(Piece* piece) {
	if(!gameInProgress) return false;

	// Piece can only be placed on empty points.
	FramePoint* pointBelow = getPointBelowPiece(piece);
	if(pointBelow && pointBelow->isEmpty()) {

		// Placed piece can only move to adjacent points (unless it can fly).
		bool pieceAlreadyPlaced = piece->hasBeenPlaced();
		if(pieceAlreadyPlaced) {
			if(!arePointsAdjacent(piece->currentPoint, pointBelow) && !piece->canFly) {
				return false;
			}
			piece->currentPoint->setEmpty();
		}
		pointBelow->currentState = (piece->color == Piece::Color::WHITE) ? FramePoint::State::WHITE : FramePoint::State::BLACK;
		piece->currentPoint = pointBelow;

		// If the piece formed a three-in-a-row mill, the player can remove opponent's pieces.
		piecesLeftToRemove = getNumberOfMillsFormedByPiece(piece);
		if(piecesLeftToRemove == 0) {
			endTurn();
		} else {
			board->spawnMILL(piece->getPosition());
			board->setSubtitle(Board::INSTR_REMOVE_PIECE);
			highlightRemoveablePieces();
		}

		return true;
	}

	return false;
}
bool Game::removePiece(Piece* piece) {
	if(!isPieceRemoveable(piece)) return false;

	if(piece->color == Piece::Color::WHITE) {
		--whitePiecesOnBoard;
	} else {
		--blackPiecesOnBoard;
	}

	// Destroys the piece and removes it from the board.
	piece->currentPoint->setEmpty();
	piece->currentPoint = nullptr;
	piece->removeFromParentAndCleanup(true);

	if(--piecesLeftToRemove == 0) endTurn();
	return true;
}
void Game::highlightPossibleMoves(Piece* piece, bool flag) {
	for(int i = 0; i < NUM_OF_FRAMES; i++) {
		for(int j = 0; j < POINTS_PER_FRAME; j++) {
			if(framePoints[i][j]->isEmpty() && (piece->canFly || !piece->hasBeenPlaced() || arePointsAdjacent(piece->currentPoint, framePoints[i][j]))) {
				framePoints[i][j]->setHighlight(flag);
			}
		}
	}
}
void Game::highlightRemoveablePieces() {
	// This is called when a three-in-a-row mill has been formed.
	// If multiple mills were formed at once, this method will be called again after all of the pieces are unhighlighted.
	int iStart = 0;
	if(isWhitesTurn) {
		iStart = 1;
	}
	for(int i = iStart; i < NUM_OF_PIECES; i += 2) {
		if(pieces[i] && pieces[i]->hasBeenPlaced() && isPieceRemoveable(pieces[i])) {
			pieces[i]->setHighlight(true);
		}
	}
}
void Game::unhighlightAllPieces() {
	// This is called after removing a highlighted piece from the board.
	for(int i = 0; i < NUM_OF_PIECES; i++) {
		if(pieces[i] && pieces[i]->hasBeenPlaced()) {
			pieces[i]->setHighlight(false);
		}
	}
}

FramePoint* Game::getPointBelowPiece(Piece* piece) {
	for(int i = 0; i < NUM_OF_FRAMES; i++) {
		for(int j = 0; j < POINTS_PER_FRAME; j++) {
			FramePoint* point = this->framePoints[i][j];

			if(point->getBoundingBox().intersectsRect(piece->getBoundingBox())) {
				return point;
			}
		}
	}
	return nullptr;
}
int Game::getNumberOfMillsFormedByPiece(Piece* piece) {
	if(!piece->hasBeenPlaced()) return 0;
	FramePoint::State checkState = piece->currentPoint->currentState;

	int numberOfMills = 0;

	int i = piece->currentPoint->indexI;
	int j = piece->currentPoint->indexJ;

	if(j % 2) { //// Middle Intersection:
		// Check adjacent intersections on common frame.
		if(this->framePoints[i][j - 1]->currentState == checkState &&
		   this->framePoints[i][(j + 1) % POINTS_PER_FRAME]->currentState == checkState) {
			numberOfMills++;
		}
		// Check intersections on common bridge.
		if(this->framePoints[(i - 1 + NUM_OF_FRAMES) % NUM_OF_FRAMES][j]->currentState == checkState &&
		   this->framePoints[(i + 1) % NUM_OF_FRAMES][j]->currentState == checkState) {
			numberOfMills++;
		}
	} else { //// Corner Intersection:
		// Check intersections on counterclockwise side of the frame.
		if(this->framePoints[i][(j - 1 + POINTS_PER_FRAME) % POINTS_PER_FRAME]->currentState == checkState &&
		   this->framePoints[i][(j - 2 + POINTS_PER_FRAME) % POINTS_PER_FRAME]->currentState == checkState) {
			numberOfMills++;
		}
		// Check intersections on clockwise side of the frame.
		if(this->framePoints[i][j + 1]->currentState == checkState &&
		   this->framePoints[i][(j + 2) % POINTS_PER_FRAME]->currentState == checkState) {
			numberOfMills++;
		}
	}
	return numberOfMills;
}
bool Game::existsOpponentPieceOutsideMill() {
	int iStart = 0;
	if(isWhitesTurn) {
		iStart = 1;
	}
	for(int i = iStart; i < NUM_OF_PIECES; i += 2) {
		if(pieces[i] && pieces[i]->hasBeenPlaced() && getNumberOfMillsFormedByPiece(pieces[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool Game::arePointsAdjacent(FramePoint* previousPoint, FramePoint* nextPoint) {
	// Points which are adjacent will differ by one step, either on the frame or bridge.
	if(previousPoint == nextPoint) return false;

	int iPrevious = previousPoint->indexI;
	int iNext = nextPoint->indexI;
	int jPrevious = previousPoint->indexJ;
	int jNext = nextPoint->indexJ;

	bool adjacentOnFrame = (iPrevious == iNext && (abs(jPrevious - jNext) == 1 || abs(jPrevious - jNext) == POINTS_PER_FRAME - 1));
	bool adjacentOnBridge = (jPrevious % 2 && jPrevious == jNext && abs(iPrevious - iNext) == 1);
	if(adjacentOnFrame || adjacentOnBridge) {
		return true;
	}
}
bool Game::isPieceMoveable(Piece* piece) {
	// Piece can be dragged if it hasn't been placed on the board yet.
	// Piece can be moved only after every piece had been placed on the board.
	return (allPiecesPlaced() && !isPieceStuck(piece) || !piece->hasBeenPlaced());
}
bool Game::isPieceStuck(Piece* piece) {
	// Piece cannot be stuck if it can fly or hasn't been placed yet.
	if(piece->canFly || !piece->hasBeenPlaced()) return false;

	int i = piece->currentPoint->indexI;
	int j = piece->currentPoint->indexJ;

	if(j % 2) {
		// Check for clear adjacent intersections on common frame.
		if(framePoints[i][j - 1]->isEmpty() || framePoints[i][(j + 1) % POINTS_PER_FRAME]->isEmpty()) {
			return false;
		}
		// Check for clear adjacent intersections on common bridge.
		if((i > 0 && framePoints[i - 1][j]->isEmpty()) || (i + 1 < NUM_OF_FRAMES && framePoints[i + 1][j]->isEmpty())) {
			return false;
		}
	} else {
		// Check for clear intersections on both corner sides of the frame.
		if(framePoints[i][(j - 1 + POINTS_PER_FRAME) % POINTS_PER_FRAME]->isEmpty() || framePoints[i][j + 1]->isEmpty()) {
			return false;
		}
	}
	return true;
}
bool Game::isPieceRemoveable(Piece* piece) {
	// Piece can only be removed if:
	bool piecesCanBeRemoved = piecesLeftToRemove > 0;				// the instructions allow it
	bool pieceIsPlaced = piece->hasBeenPlaced();					// it's on the board
	bool isPieceInMill = getNumberOfMillsFormedByPiece(piece) > 0;	// is not a part of a three-in-a-row mill...
	bool allPiecesInMill = !existsOpponentPieceOutsideMill();		// ...unless there are no other options.

	if(piecesCanBeRemoved && pieceIsPlaced) {
		if(!isPieceInMill || isPieceInMill && allPiecesInMill) {
			return true;
		}
	}
	return false;
}
