#include "RuleEngine.h"
#include "../BoardManager/BoardManager.h"
#include "../Logger/Logger.h"
#include "../Player/Player.h"
#include <algorithm>

extern Logger logger;

bool RuleEngine::millHelper(std::unordered_map<int, std::vector<int>> neighborList, std::vector<char> cells, int position, char symbol) {
    if (neighborList[position].size() == 2) {
        for (int cell : neighborList[position]) {
            if (cells[cell] != symbol) {
                return false;
            }
        }
        return true;
    } else {
        if (int neighbor = neighborList[position][0] == symbol) {
            for (int element : neighborList[neighbor]) {
                if (cells[element] != symbol) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool RuleEngine::isMillFormed(int position, char symbol) {

    if (position < 0 || position >= bm->cells.size()) {
        logger.log(LogLevel::ERROR, "isMillFormed: Position " + std::to_string(position) + " is out of bounds.");
        return false;
    }

    if (millHelper(bm->horizontalNeighbors, bm->cells, position, symbol)) {
        return true;
    }

    if (millHelper(bm->verticalNeighbors, bm->cells, position, symbol)) {
        return true;
    }
    
    return false;
}

bool RuleEngine::canPlayerJump(Player p) {
    if (p.getTotalStones() == 3) {
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