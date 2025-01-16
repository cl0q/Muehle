#ifndef GAME_H
#define GAME_H

#include "../Player/Player.h"
#include "../BoardManager/BoardManager.h"
#include "../RuleEngine/RuleEngine.h"

class GameManager {
    public:
    Player* p1;
    Player* p2;
    RuleEngine* ruleEngine;
    BoardManager* boardManager;

    void runPhaseOne();
    void runPhaseTwo(Player* p);
    void runPhaseThree();
    int getUserInput(int min, int max, Player* p, bool isPhaseOne, bool isFirstSelection);

    public:
        GameManager(Player* p1, Player* p2, RuleEngine* ruleEngine, BoardManager* boardManager) : p1(p1), p2(p2), ruleEngine(ruleEngine), boardManager(boardManager){}

        bool isRunning = false;

        void run();
        void saveGame();
        void loadGame();
        void destroyStone(Player* killer, Player* victim);
        void displayBoard();
        int getDestroyInput(int min, int max, Player* killer);
};

#endif