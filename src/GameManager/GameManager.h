//
// Created by Oliver Ilczuk on 06.01.25.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BoardManager/BoardManager.h"
#include <array>
#include <string>

class GameManager {

private:
    BoardManager& board_manager;

    bool isMovingStone = false;
    int movingStoneFrom = -1;
    bool pressedEnter = false;
    int currentPhase = 0; // 0 = Set, 1 = Move, 2 = Jump
    int latestKeyInput = 0;

    std::string iconPlayer1 = "X";
    std::string iconPlayer2 = "O";

public:
    // Constructor
    explicit GameManager(BoardManager& board_manager) : board_manager(board_manager) {}

    // Input Handling
    int getKey();
    static int getUserInput(int min, int max);
    static char getch();

    // Game Phases
    int setPhase(BoardManager& boardManager, int currentCell);
    int movePhase(BoardManager& boardManager, int currentCell);
    int jumpPhase(BoardManager& boardManager, int currentCell);

    // Game Logic
    void start();
    void startNewGame();
    void gameLoop();
    void handleSettingsMenu();

    // Board and Phase Management
    void printBoard(const BoardManager& boardManager, BoardManager::CellState currentPlayer, int currentCell);
    void printStatus(BoardManager& boardManager, int currentCell);

    std::string printPhase();

    // Utility Functions
    static void pause();
    static void clearScreen();
    static bool random50Percent();
    int randomPlayerStart() const;

    // Mill and Move Validation
    bool isPlayerAllowedToMoveAnywhere(BoardManager::CellState currentPlayer);
    bool hasLegalMoves(BoardManager& boardManager, BoardManager::CellState currentPlayer);
    bool checkMill(int& currentCell, BoardManager::CellState currentPlayer);

    // Save and Load
    void saveGame(const std::string& filename);
    void loadGame(const std::string& saveFile);
    static bool isSaveFileAvailable();

    // Stone Count
    std::array<int, 2> getPlacedStones();
    void dekrementStoneAfterDelete();
};

#endif // GAMEMANAGER_H
