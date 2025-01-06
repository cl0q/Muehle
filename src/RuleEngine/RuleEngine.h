#ifndef RULEENGINE_H
#define RULEENGINE_H

#include <iostream>

class RuleEngine
{
private:
    bool millHelper(std::unordered_map<int, std::vector<int>> neighborList, std::vector<CellState> cells, int position, CellState state);

public:

    BoardManager bm;

    RuleEngine(BoardManager boardManager) : bm(boardManager)
    {

    }

    ~RuleEngine()
    {
        std::cout << "RuleEngine destructor called. Resources freed." << std::endl;
    }

    bool isMillFormed(int position, CellState state);

    bool canPlayerJump();

    bool isGameOver();
};


#endif