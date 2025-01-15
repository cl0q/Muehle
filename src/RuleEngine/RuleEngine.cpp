#include "RuleEngine.h"
#include "../BoardManager/BoardManager.h"
#include "../Logger/Logger.h"
#include "../Player/Player.h"
#include <algorithm>
#include <vector>

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

bool RuleEngine::isMillFormed(int position, int identifier) {

    if (position < 0 || position >= board_manager->cells.size()) {
        logger.log(LogLevel::ERROR, "isMillFormed: Position " + std::to_string(position) + " is out of bounds.");
        return false;
    }

    if (millHelper(board_manager->horizontalNeighbors, board_manager->cells, position, identifier)) {
        return true;
    }

    if (millHelper(board_manager->verticalNeighbors, board_manager->cells, position, identifier)) {
        return true;
    }
    
    return false;
}

bool RuleEngine::canPlayerJump(Player* p) {
    if (p->getTotalStones() == 3) {
        std::cout << p->name << " can jump now!!!" << std::endl;
        p->canJump = true;
        return true;
    } else {
        return false;
    }
}

bool RuleEngine::isGameOver() {
    if (player1->getTotalStones() < 3) {
        std::cout << player2->name << " has won!! Congratulations." << std::endl;
        return true;
    }

    if (player2->getTotalStones() < 3) {
        std::cout << player1->name << " has won!!! Congratulations." << std::endl;
        return true;
    }
    return false;
}
