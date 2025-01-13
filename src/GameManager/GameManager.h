//
// Created by Oliver Ilczuk on 06.01.25.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BoardManager/BoardManager.h"

class GameManager {
private:
    BoardManager board_manager;

public:
    GameManager(BoardManager board_manager) {
        this->board_manager = board_manager;
    }

    static void printBoard(const BoardManager &boardManager, int currentPlayer);

    void gameLoop();

    static int getUserInput(int min, int max) ;

    static bool random50Percent();

    void start();

    static int randomPlayerStart();

    static void clearScreen();

    void startNewGame();
    static void handleSettingsMenu();
    static bool loadGame(const std::string& saveFile);
    static bool isSaveFileAvailable();
};

#endif // GAMEMANAGER_H