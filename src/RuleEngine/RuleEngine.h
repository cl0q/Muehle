#ifndef RULEENGINE_H
#define RULEENGINE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Player/Player.h"
#include "../BoardManager/BoardManager.h"
#include "../RuleEngine/RuleEngine.h"

class RuleEngine
{
private:
    bool millHelper(std::unordered_map<int, std::vector<int>> neighborList, std::vector<BoardManager::CellState> cells, int position, BoardManager::CellState state);

public:

    BoardManager* board_manager;
    Player* player1;
    Player* player2;

    RuleEngine(BoardManager* boardManager, Player* p1, Player* p2) : board_manager(boardManager), player1(p1), player2(p2)
    {

    }

    ~RuleEngine()
    {
        std::cout << "RuleEngine destructor called. Resources freed." << std::endl;
    }

    bool isMillFormed(int position, BoardManager::CellState identifier);

    bool canPlayerJump(Player* p);

    bool isGameOver();

};


#endif