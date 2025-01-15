//
// Created by Oliver Ilczuk on 06.01.25.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../BoardManager/BoardManager.h"
#include "../Player/Player.h"
#include "../RuleEngine/RuleEngine.h"
#include <vector>

class GameManager {
private:
    BoardManager& board_manager;
    RuleEngine* rule_engine;
    Player* p1;
    Player* p2;
    int movingStoneFrom = -1;
    void runPhaseOne();
    void runPhaseTwo(Player* p);

public:
    GameManager(BoardManager& board_manager) : board_manager(board_manager) {};

    void displayBoard(std::vector<BoardManager::CellState> cells);

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
    void destroyStone(Player* killer, Player* victim, int position);
};

#endif // GAMEMANAGER_H