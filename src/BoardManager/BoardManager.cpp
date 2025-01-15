//
// Created by Oliver Ilczuk on 05.01.25.
//

#include "BoardManager.h"
#include "../Logger/Logger.h"
#include <algorithm>

extern Logger logger;

BoardManager::BoardManager() : cells(24, EMPTY) {
    initializeNeighbors();
}

bool BoardManager::setStone(int position, CellState state) {
    if (position < 0 || position >= cells.size()) {
        std::cerr << "setStone: Position out of bounds" << std::endl;
        return false;
    }
    if (cells[position] != EMPTY) {
        std::cerr << "setStone: Position already occupied" << std::endl;
        return false;
    }
    cells[position] = state;

    switchPlayer();
    return true;
}

bool BoardManager::isValidMove(int from, int to) {
    if (std::find(verticalNeighbors[from].begin(), verticalNeighbors[from].end(), to) != verticalNeighbors[from].end()
    || std::find(horizontalNeighbors[from].begin(), horizontalNeighbors[from].end(), to) != horizontalNeighbors[from].end()) {
        logger.log(LogLevel::DEBUG, "isValidMove: Move from " + std::to_string(from) +
                                    " to " + std::to_string(to) + " is valid.");
        return true;
    } else {
        logger.log(LogLevel::DEBUG, "isValidMove: Move from " + std::to_string(from) +
                                    " to " + std::to_string(to) + " is invalid.");
        return false;
    }
}

bool BoardManager::moveStone(int from, int to) {
    if (from < 0 || from >= cells.size() || to < 0 || to >= cells.size()) {
        logger.log(LogLevel::ERROR, "moveStone: Invalid indices. From: " + std::to_string(from) +
                                    ", To: " + std::to_string(to) + ".");
        return false;
    }

    if (cells[from] == BoardManager::EMPTY) {
        logger.log(LogLevel::WARNING, "moveStone: No stone at position " + std::to_string(from) + ".");
        return false;
    }

    if (cells[to] != BoardManager::EMPTY) {
        logger.log(LogLevel::WARNING, "moveStone: Position " + std::to_string(to) + " is already occupied.");
        return false;
    }

    if (!isValidMove(from, to)) {
        logger.log(LogLevel::WARNING, "moveStone: Move from " + std::to_string(from) +
                                      " to " + std::to_string(to) + " is not valid.");
        return false;
    }

    if (getCellState(cells[from]) != getCurrentPlayer()) {
        logger.log(LogLevel::WARNING, "moveStone: Player " + std::to_string(getCurrentPlayer()) +
                                      " is not allowed to move stone from position " + std::to_string(from) + ".");
        return false;
    }

    cells[to] = cells[from];
    cells[from] = EMPTY;

    logger.log(LogLevel::INFO, "moveStone: Player " + std::to_string(cells[to]) +
                               " moved stone from " + std::to_string(from) +
                               " to " + std::to_string(to) + ".");
    switchPlayer();
    return true;
}

bool BoardManager::removeStone(int at) {
    if (at < 0 || at >= cells.size()) {
        logger.log(LogLevel::ERROR, "removeStone: Position " + std::to_string(at) + " is out of bounds.");
        return false;
    }

    if (cells[at] == BoardManager::EMPTY) {
        logger.log(LogLevel::WARNING, "removeStone: No stone at position " + std::to_string(at) + " to remove.");
        return false;
    }

    cells[at] = EMPTY;
    logger.log(LogLevel::INFO, "removeStone: Stone removed from position " + std::to_string(at) + ".");
    switchPlayer();
    return true;
}

BoardManager::CellState BoardManager::getCurrentPlayer() const {
        if (currentPlayer == 0) {
            return PLAYER1;
        }
        else if (currentPlayer == 1) {
            return PLAYER2;
        }
        else {
            return INVALID;
        }
    }

int BoardManager::switchPlayer() {
    currentPlayer = currentPlayer == 1 ? 2 : 1;
    return currentPlayer;
}

std::vector<std::pair<int, BoardManager::CellState>> BoardManager::getNeighborsWithState(int position) const {
    std::vector<std::pair<int, CellState>> neighborsWithState;

    // Hole alle vertikalen Nachbarn
    if (verticalNeighbors.find(position) != verticalNeighbors.end()) {
        for (int neighbor : verticalNeighbors.at(position)) {
            neighborsWithState.emplace_back(neighbor, cells[neighbor]);
        }
    }

    // Hole alle horizontalen Nachbarn
    if (horizontalNeighbors.find(position) != horizontalNeighbors.end()) {
        for (int neighbor : horizontalNeighbors.at(position)) {
            // Prüfe, ob Nachbar schon hinzugefügt wurde (um Duplikate zu vermeiden)
            if (std::find_if(neighborsWithState.begin(), neighborsWithState.end(),
                             [neighbor](const std::pair<int, CellState>& p) { return p.first == neighbor; }) == neighborsWithState.end()) {
                neighborsWithState.emplace_back(neighbor, cells[neighbor]);
                             }
        }
    }

    return neighborsWithState;
}

BoardManager::CellState BoardManager::getCellState(int position) const {
    return cells[position];
}

const std::vector<BoardManager::CellState>& BoardManager::getCells() const {
    return cells;
}

void BoardManager::initializeNeighbors() {
    verticalNeighbors[0] = {9};
    verticalNeighbors[1] = {4};
    verticalNeighbors[2] = {14};
    verticalNeighbors[3] = {10};
    verticalNeighbors[4] = {1, 7};
    verticalNeighbors[5] = {13};
    verticalNeighbors[6] = {11};
    verticalNeighbors[7] = {4};
    verticalNeighbors[8] = {12};
    verticalNeighbors[9] = {0, 21};
    verticalNeighbors[10] = {3, 18};
    verticalNeighbors[11] = {6, 15};
    verticalNeighbors[12] = {8, 17};
    verticalNeighbors[13] = {5, 20};
    verticalNeighbors[14] = {2, 23};
    verticalNeighbors[15] = {11};
    verticalNeighbors[16] = {19};
    verticalNeighbors[17] = {12};
    verticalNeighbors[18] = {10};
    verticalNeighbors[19] = {16, 22};
    verticalNeighbors[20] = {13};
    verticalNeighbors[21] = {9};
    verticalNeighbors[22] = {19};
    verticalNeighbors[23] = {14};

    horizontalNeighbors[0] = {1};
    horizontalNeighbors[1] = {0, 2};
    horizontalNeighbors[2] = {1};
    horizontalNeighbors[3] = {4};
    horizontalNeighbors[4] = {3, 5};
    horizontalNeighbors[5] = {4};
    horizontalNeighbors[6] = {7};
    horizontalNeighbors[7] = {6, 8};
    horizontalNeighbors[8] = {7};
    horizontalNeighbors[9] = {10};
    horizontalNeighbors[10] = {9, 11};
    horizontalNeighbors[11] = {10};
    horizontalNeighbors[12] = {13};
    horizontalNeighbors[13] = {12, 14};
    horizontalNeighbors[14] = {13};
    horizontalNeighbors[15] = {16};
    horizontalNeighbors[16] = {15, 17};
    horizontalNeighbors[17] = {16};
    horizontalNeighbors[18] = {19};
    horizontalNeighbors[19] = {18, 20};
    horizontalNeighbors[20] = {19};
    horizontalNeighbors[21] = {22};
    horizontalNeighbors[22] = {21, 23};
    horizontalNeighbors[23] = {22};
}