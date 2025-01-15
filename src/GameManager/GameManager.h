//
// Created by Oliver Ilczuk on 06.01.25.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BoardManager/BoardManager.h"

class GameManager {
private:
    BoardManager& board_manager;
    bool isMovingStone;
    int movingStoneFrom;
    bool pressedEnter;

public:
    GameManager(BoardManager& board_manager) : board_manager(board_manager) {};

    int moveCursor(BoardManager &boardManager, int currentCell);

    void printStatus(BoardManager &boardManager, int currentCell);

    int getPlacedStones();

    void printBoard(const BoardManager &boardManager, BoardManager::CellState currentPlayer, int currentCell);

    void gameLoop();

    static int getUserInput(int min, int max) ;

    static bool random50Percent();

    void start();

    int randomPlayerStart() const;

    static void clearScreen();

    static char getch();


    void startNewGame();
    static void handleSettingsMenu();
    static bool loadGame(const std::string& saveFile);
    static bool isSaveFileAvailable();

    bool checkMill(int &currentCell, BoardManager::CellState currentPlayer);
};

#endif // GAMEMANAGER_H