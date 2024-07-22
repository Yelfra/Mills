#pragma once

#include "cocos2d.h"

class Board;
class FramePoint;
class Piece;

/*
Game handles most of the Mills' logic. It's interacted with through Board GUI elements or Pieces.
*/
class Game {
private:
	//// Pointers to Pieces and FramePoints:
	std::vector<Piece*> pieces;
	std::vector<std::vector<FramePoint*>> framePoints;

	//// Game Variables:
	bool gameStarted = false;
	bool gameInProgress = false;
	bool isWhitesTurn = true;
	int piecesPlaced = 0;
	int whitePiecesOnBoard = 0;
	int blackPiecesOnBoard = 0;
	int piecesLeftToRemove = 0;

	//// Init:
	// Initialize and add framePoints to the board.
	void initFramePoints();
	// Initialize and add pieces to the board.
	void initPieces();
	// Initialize game UI.
	void initUI();

	// Returns a pointer to the FramePoint below the Piece that is being dragged.
	FramePoint* getPointBelowPiece(Piece* piece);
	// Returns number of three-in-a-row mills the given piece forms in its position.
	int getNumberOfMillsFormedByPiece(Piece* piece);
	// Checks whether there is an opponent's piece that isn't part of a mill.
	bool existsOpponentPieceOutsideMill();

public:
	/* CONSTANTS */
	static constexpr int NUM_OF_FRAMES = 3;
	static constexpr int POINTS_PER_FRAME = 8;
	static constexpr int NUM_OF_PIECES = 18;

	Board* board;

	//// Constructor and Destructor:
	Game(Board* board);
	~Game();
	
	/* GAME METHODS */
	// Starts the game.
	void start();
	// Ends the game, given results.
	void end(bool whiteWins, bool byReducing);

	// Checks whether its the given piece's turn.
	bool isTurnOf(Piece* piece);
	// Checks whether the player has opponent's pieces to remove.
	bool hasPiecesToRemove();
	// Checks whether all 18 pieces have been placed.
	bool allPiecesPlaced();

	// Ends turn.
	void endTurn();
	// Checks whether either player has less than 3 pieces on the board left.
	bool checkWinByReducing();
	// Checks whether either player has any moves left.
	bool checkWinByBlocking();
	// Allows the player to pull a piece off the stack.
	void spawnPiece();
	// Checks whether flying conditions (3 pieces left) is satisfied and gives wings.
	void checkFlyingCondition();

	// Called from Piece when the player selects it.
	bool selectPiece(Piece* piece);
	// Called from Piece when the player drops it.
	bool dropPiece(Piece* piece);
	// Called when the player selects a piece to remove it.
	bool removePiece(Piece* piece);

	// Called when moving pieces to highlight available points to move to.
	void highlightPossibleMoves(Piece* piece, bool flag);
	// Called when removing pieces to highlight those that can be removed.
	void highlightRemoveablePieces();
	// Called after a piece had been removed.
	void unhighlightAllPieces();

	// Checks whether two given points are next to each other on the frame or bridge.
	bool arePointsAdjacent(FramePoint* previousPoint, FramePoint* nextPoint);
	// Checks whether the piece is able to be dragged and dropped.
	bool isPieceMoveable(Piece* piece);
	// Checks whether the piece can be moved to any available point on the grid.
	bool isPieceStuck(Piece* piece);
	// Checks whether the piece can be removed from the board.
	bool isPieceRemoveable(Piece* piece);
};