#ifndef GAME_H
#define GAME_H

#include "../Player/Player.h"
#include "../BoardManager/BoardManager.h"
#include "../RuleEngine/RuleEngine.h"

class GameManager {
    private:
    Player* p1;
    Player* p2;
    RuleEngine* ruleEngine;
    BoardManager* boardManager;

    void runPhaseOne();
    void runPhaseTwo(Player* p);
    void runPhaseThree();
    int getUserInput(int min, int max);
    int checkedSetStone(Player* p);

    public:
        GameManager(Player* p1, Player* p2, RuleEngine* ruleEngine, BoardManager* boardManager) : p1(p1), p2(p2), ruleEngine(ruleEngine), boardManager(boardManager){}

        bool isRunning = false;

        void run();
        void saveGame();
        void loadGame();
        void destroyStone(Player* killer, Player* victim);
        void displayBoard();
};

#endif