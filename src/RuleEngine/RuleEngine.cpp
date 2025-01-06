#include "RuleEngine.h"
#include "./BoardManager/BoardManager.h"
#include "./Logger/Logger.h"
#include <algorithm>

extern Logger logger;

bool RuleEngine::millHelper(std::unordered_map<int, std::vector<int>> neighborList, std::vector<CellState> cells, int position, CellState state) {
    if (neighborList[position].size() == 2) {
        for (int cell : neighborList[position]) {
            if (cells[cell] != state) {
                return false;
            }
        }
        return true;
    } else {
        if (int neighbor = neighborList[position][0] == state) {
            for (int element : neighborList[neighbor]) {
                if (cells[element] != state) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool RuleEngine::isMillFormed(int position, CellState state) {

    if (position < 0 || position >= bm.cells.size()) {
        logger.log(LogLevel::ERROR, "isMillFormed: Position " + std::to_string(position) + " is out of bounds.");
        return false;
    }

    if (millHelper(bm.horizontalNeighbors, bm.cells, position, state)) {
        return true;
    }

    if (millHelper(bm.verticalNeighbors, bm.cells, position, state)) {
        return true;
    }
    
    return false;
}

bool RuleEngine::canPlayerJump() {

}

bool RuleEngine::isGameOver() {

}