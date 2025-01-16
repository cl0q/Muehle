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
    int currentPhase = 0; // 0 = set, 1 = move, 2 = jump
    int latestKeyInput = 0;

    std::string iconPlayer1 = "X";
    std::string iconPlayer2 = "O";

public:
    GameManager(BoardManager& board_manager) : board_manager(board_manager) {};

    int getKey();

    int setPhase(BoardManager &boardManager, int currentCell);

    int movePhase(BoardManager &boardManager, int currentCell);

    int jumpPhase(BoardManager &boardManager, int currentCell);

    void printStatus(BoardManager &boardManager, int currentCell);

    std::array<int, 2> getPlacedStones();

    void dekrementStoneAfterDelete();

    bool isPlayerAllowedToMoveAnywhere(BoardManager::CellState currentPlayer);

    bool hasLegalMoves(BoardManager &boardManager, BoardManager::CellState currentPlayer);

    void printBoard(const BoardManager &boardManager, BoardManager::CellState currentPlayer, int currentCell);

    void gameLoop();

    static int getUserInput(int min, int max) ;

    static bool random50Percent();

    void start();

    int randomPlayerStart() const;

    static void clearScreen();

    static char getch();


    void startNewGame();
    void handleSettingsMenu();

    void saveGame(const std::string &filename);

    void loadGame(const std::string &saveFile);
    static bool isSaveFileAvailable();

    void printPhase();

    bool checkMill(int &currentCell, BoardManager::CellState currentPlayer);
};

#endif // GAMEMANAGER_H