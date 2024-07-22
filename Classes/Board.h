#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Piece;
class FramePoint;

/*
Board plays the role of the GUI, acting as a platform for every displayed element.
Also provides helpful visual feedback (game juice) to player's actions.
*/
class Board : public cocos2d::Scene {
private:
    /* CONSTANTS */
    //// Stacks:
    static constexpr int STACK_SIZE = 9;
    static constexpr char* STACK_WHITE = "StackWhite.png";
    static constexpr char* STACK_BLACK = "StackBlack.png";
    //// Titles and Subtitles:
    static constexpr char* TITLE = "MILLS.png";
    static constexpr char* BY = "by.png";
    static constexpr char* NAME = "Yelfra.png";
    static constexpr char* MILL = "MILL.png";
    
    /* PIECE STACKS */
    //// Stack Components:
    cocos2d::Sprite* whiteStack = cocos2d::Sprite::create(STACK_WHITE, cocos2d::Rect(30, 0, 240, 80));
    cocos2d::Sprite* whiteStackTop = cocos2d::Sprite::create(STACKTOP_WHITE);
    cocos2d::Sprite* blackStack = cocos2d::Sprite::create(STACK_BLACK, cocos2d::Rect(30, 0, 240, 80));
    cocos2d::Sprite* blackStackTop = cocos2d::Sprite::create(STACKTOP_BLACK);

    cocos2d::Label* whiteLabel = cocos2d::Label::createWithSystemFont(std::to_string(STACK_SIZE), "Arial", 32);
    cocos2d::Label* blackLabel = cocos2d::Label::createWithSystemFont(std::to_string(STACK_SIZE), "Arial", 32);

    int whiteOnStack = STACK_SIZE;
    int blackOnStack = STACK_SIZE;

    //// Stack Methods:
    // Resets the piece stacks.
    void initStacks();
    // Updates visuals for the white piece stack and label.
    void updateWhiteStack();
    // Updates visuals for the black piece stack and label.
    void updateBlackStack();

    /* UI ELEMENTS */
    //// Titles and Subtitles:
    cocos2d::Sprite* titleSprite = cocos2d::Sprite::create(TITLE);
    cocos2d::Sprite* subtitleSprite = cocos2d::Sprite::create(BY);
    cocos2d::Sprite* nameTitleSprite = cocos2d::Sprite::create(NAME);
    cocos2d::Sprite* millSprite = cocos2d::Sprite::create(MILL);

public:
    /* FILENAME CONSTANTS */
    //// Pieces:
    static constexpr char* STACKTOP_WHITE = "StackTopWhite.png";
    static constexpr char* STACKTOP_BLACK = "StackTopBlack.png";

    static constexpr char* WHITE = "PieceWhite.png";
    static constexpr char* WHITE_HIGHLIGHT = "PieceWhiteHighlight.png";
    static constexpr char* WHITE_MOVE = "PieceWhiteMove.png";
    static constexpr char* BLACK = "PieceBlack.png";
    static constexpr char* BLACK_HIGHLIGHT = "PieceBlackHighlight.png";
    static constexpr char* BLACK_MOVE = "PieceBlackMove.png";

    static constexpr char* WHITE_FLY = "PieceWhiteFly.png";
    static constexpr char* BLACK_FLY = "PieceBlackFly.png";
    static constexpr char* WHITE_FLY_MOVE = "PieceWhiteFlyMove.png";
    static constexpr char* BLACK_FLY_MOVE = "PieceBlackFlyMove.png";
    static constexpr char* WHITE_FLY_HIGHLIGHT = "PieceWhiteFlyHighlight.png";
    static constexpr char* BLACK_FLY_HIGHLIGHT = "PieceBlackFlyHighlight.png";

    //// FramePoints:
    static constexpr char* FRAMEPOINT = "FramePoint.png";
    static constexpr char* FRAMEPOINT_HIGHLIGHT = "FramePointHighlight.png";

    //// Turn Indicators:
    static constexpr char* TURN_WHITE = "TurnWhite.png";
    static constexpr char* TURN_BLACK = "TurnBlack.png";
    
    //// Instructions:
    static constexpr char* INSTR_PLACE_PIECE = "InstructionPlacePiece.png";
    static constexpr char* INSTR_MOVE_PIECE = "InstructionMovePiece.png";
    static constexpr char* INSTR_REMOVE_PIECE = "InstructionRemovePiece.png";
    
    //// Titles:
    static constexpr char* WHITE_WINS = "WhiteWins.png";
    static constexpr char* BLACK_WINS = "BlackWins.png";
    //// Subtitles:
    static constexpr char* BY_REDUCING = "ByReducing.png";
    static constexpr char* BY_BLOCKING = "ByBlocking.png";

    //// Buttons:
    static constexpr char* BUTTON_START = "ButtonStart.png";
    static constexpr char* BUTTON_START_PRESSED = "ButtonStartPressed.png";
    static constexpr char* BUTTON_RESTART = "ButtonRestart.png";
    static constexpr char* BUTTON_RESTART_PRESSED = "ButtonRestartPressed.png";

    //// Dimensions:
    const cocos2d::Rect outsideFrame = cocos2d::Rect(80, 60, 1120, 840);
    
    /* UI ELEMENTS and METHODS */
    //// Elements:
    cocos2d::ui::Button* startButton = cocos2d::ui::Button::create(BUTTON_START, BUTTON_START_PRESSED);

    //// Init and Reset:
    static Board* create();
    virtual bool init();
    // Called when the game starts.
    void initGameUI();
    // Called when the game ends.
    void resetGameUI();

    //// Titles and Subtitles:
    // Sets the top title to given string.
    void setTitle(std::string title);
    // Sets the middle subtitle to given string.
    void setSubtitle(std::string title);
    // Shakes the top title for a short duration.
    void shakeTitle();
    // Pops MILL! in the given position, called on piece positions.
    void spawnMILL(cocos2d::Vec2 position);

    //// FramePoints and Pieces:
    // Puts an intersection/point on the board.
    void addFramePoint(FramePoint* point);
    // Puts a piece on the board.
    void addPiece(Piece* piece);

    //// Public Stack Methods:
    // Removes a white piece from the white piece stack.
    void popWhite();
    // Removes a black piece from the black piece stack.
    void popBlack();
    // Puts a white piece back on the white piece stack.
    void putWhite();
    // Puts a black piece back on the black piece stack.
    void putBlack();

    //// Feedback/Shake:
    // Returns a sequence to play with runAction() given a sprite (position is enough).
    static cocos2d::Sequence* shakeSprite(cocos2d::Sprite* sprite);
};