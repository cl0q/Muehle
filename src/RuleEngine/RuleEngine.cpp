#include "RuleEngine.h"
#include "../BoardManager/BoardManager.h"
#include "../Logger/Logger.h"
#include "../Player/Player.h"
#include <algorithm>

extern Logger logger;

bool RuleEngine::millHelper(std::unordered_map<int, std::vector<int>> neighborList, std::vector<BoardManager::CellState> cells, int position, BoardManager::CellState state) {
    if (neighborList[position].size() == 2) {
        for (int cell : neighborList[position]) {
            if (cells[cell] != state) {
                return false;
            }
        }
        return true;
    } else {
        int neighbor = neighborList[position][0];
        if (cells[neighbor] == state) {
            for (int element : neighborList[neighbor]) {
                if (cells[element] != state) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
}

bool RuleEngine::isMillFormed(int position, BoardManager::CellState identifier) {

    if (position < 0 || position >= bm->cells.size()) {
        logger.log(LogLevel::ERROR, "isMillFormed: Position " + std::to_string(position) + " is out of bounds.");
        return false;
    }

    if (millHelper(bm->horizontalNeighbors, bm->cells, position, identifier)) {
        return true;
    }

    if (millHelper(bm->verticalNeighbors, bm->cells, position, identifier)) {
        return true;
    }
    
    return false;
}

bool RuleEngine::canPlayerJump(Player* p) {
    if (p->getTotalStones() == 3) {
        return true;
    } else {
        return false;
    }
}

bool RuleEngine::isGameOver() {
    if (player1->getTotalStones() < 3 || player2->getTotalStones() < 3) {
        return true;
    }
    return false;
}

bool RuleEngine::canGameContinue()
{

    if (isMillFormed) {
        return false;
    } 

    if (canPlayerJump(player1) || canPlayerJump(player2)) {
        return false;
    }

    if (isGameOver()) {
        return false;
    }

    return true;
}