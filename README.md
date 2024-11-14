# Mills
A first-round interview task was to develop a Nine Men's Morris (Mills) game using C++ and the Cocos2d-x (4.0) game engine.

## Description
Nine men's morris is a strategy board game for two players played on a board consisting of a grid with 24 intersections (or points). Each player has 9 pieces (or men), either black or white. Players try to form "mills" - 3 of their own pieces lined horizontally or vertically - allowing a player to remove an opponent's man from the game. The win goal is to reduce the opponent to two pieces (whereupon they can no longer form mills and thus are unable to win) or by leave them without a legal move.

The implemented game does its best to show the player how to play through simple instructions and highlighted pieces or points. The goal was to make the possible moves clear for the player from the get-go, without having them rely on tutorials.
This implementation is intended for two players (without the option of an AI opponent).

I had extra time to spice up the game with better player feedback ("game juice"), so I've added shake and other smaller effects.

## Technical
The game itself was developed in C++ using Visual Studio IDE. With a deadline of 10 days, my first 5 days were primarily dedicated to learning the intricacies of the Cocos2d-x game engine, and its limitations due to sparse documentation and deprecation. The other 5 days were spent on developing the game.
Apart from the code, I've done the visuals myself. Using Aseprite, I've created simple and clean assets in pixel art.

## How to Use
1. (Repository) Build and export the game. (Executable) Launch the game.
2. Press play.
