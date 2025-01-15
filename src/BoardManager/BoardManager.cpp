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

    logger.log(LogLevel::ERROR, "setStone: CurrentPlayer " + enumToString(getCurrentPlayer()) + " set stone at position " + std::to_string(position) + " for player " + enumToString(state) + ".");

    std::cout << "checkMill: " << checkMill(position, state) << std::endl;
    if (!checkMill(position, state)) {
        switchPlayer();
    } else {
        //TODO remove stone with backspace
    }

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

    if (getCellState(from) != getCurrentPlayer() ) {
        logger.log(LogLevel::WARNING, "moveStone:  Player: " + enumToString(getCurrentPlayer()) +
                                      " is not allowed to move stone from position " + std::to_string(from) +
                                      ".\n cellFrom: " + std::to_string(from) + " " + enumToString(getCellState(from)) +
                                      "\n cellTo: "  + std::to_string(to) + " " +enumToString(getCellState(to)) +
                                      ""
                                      );
        return false;
    }

    cells[to] = cells[from];
    cells[from] = EMPTY;

    logger.log(LogLevel::INFO, "moveStone: Player " + std::to_string(cells[to]) +
                               " moved stone from " + std::to_string(from) +
                               " to " + std::to_string(to) + ".\n cellFrom: " + enumToString(getCellState(from)) + "\n cellTo: " + enumToString(getCellState(to)));
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
            return EMPTY;
        }
    }

void BoardManager::switchPlayer() {
    if (currentPlayer == CellState::PLAYER1) {
        currentPlayer = CellState::PLAYER2;
    } else if (currentPlayer == CellState::PLAYER2) {
        currentPlayer = CellState::PLAYER1;
    } else {
        logger.log(LogLevel::ERROR, "switchPlayer: Invalid player " + std::to_string(currentPlayer) + ".");
    }
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

std::string BoardManager::enumToString(BoardManager::CellState state) {
    switch (state) {
        case BoardManager::CellState::PLAYER1: return "PLAYER1";
        case BoardManager::CellState::PLAYER2: return "PLAYER2";
        case BoardManager::CellState::EMPTY:   return "EMPTY";
        default: throw std::invalid_argument("Unknown CellState");
    }
}

void BoardManager::setCurrentPlayer(int currentPlayer) {
    currentPlayer = currentPlayer;
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

bool BoardManager::checkMill(int& currentCell, BoardManager::CellState currentPlayer) {

    if (currentCell == 0) {
        if (this->cells[0] == currentPlayer && this->cells[1] == currentPlayer && this->cells[2] == currentPlayer) { return true; }
        if (this->cells[0] == currentPlayer && this->cells[9] == currentPlayer && this->cells[21] == currentPlayer) { return true; }

    }

    if (currentCell == 1) {
        if (this->cells[0] == currentPlayer && this->cells[1] == currentPlayer && this->cells[2] == currentPlayer) { return true; }
        if (this->cells[1] == currentPlayer && this->cells[4] == currentPlayer && this->cells[7] == currentPlayer) { return true; }

    }

    if (currentCell == 2) {
        if (this->cells[0] == currentPlayer && this->cells[1] == currentPlayer && this->cells[2] == currentPlayer) { return true; }
        if (this->cells[2] == currentPlayer && this->cells[14] == currentPlayer && this->cells[23] == currentPlayer) { return true; }

    }

    if (currentCell == 3) {
        if (this->cells[3] == currentPlayer && this->cells[4] == currentPlayer && this->cells[5] == currentPlayer) { return true; }
        if (this->cells[3] == currentPlayer && this->cells[10] == currentPlayer && this->cells[18] == currentPlayer) { return true; }

    }

    if (currentCell == 4) {
        if (this->cells[3] == currentPlayer && this->cells[4] == currentPlayer && this->cells[5] == currentPlayer) { return true; }
        if (this->cells[1] == currentPlayer && this->cells[4] == currentPlayer && this->cells[7] == currentPlayer) { return true; }

    }

    if (currentCell == 5) {
        if (this->cells[3] == currentPlayer && this->cells[4] == currentPlayer && this->cells[5] == currentPlayer) { return true; }
    }

    if (currentCell == 6 ) {
        if (this->cells[6] == currentPlayer && this->cells[7] == currentPlayer && this->cells[8] == currentPlayer) { return true; }
        if (this->cells[6] == currentPlayer && this->cells[11] == currentPlayer && this->cells[15] == currentPlayer) { return true; }
    }

    if (currentCell == 7) {
        if (this->cells[6] == currentPlayer && this->cells[7] == currentPlayer && this->cells[8] == currentPlayer) { return true; }
        if (this->cells[1] == currentPlayer && this->cells[4] == currentPlayer && this->cells[7] == currentPlayer) { return true; }

    }

    if (currentCell == 8) {
        if (this->cells[9] == currentPlayer && this->cells[10] == currentPlayer && this->cells[11] == currentPlayer) { return true; }
        if (this->cells[8] == currentPlayer && this->cells[12] == currentPlayer && this->cells[17] == currentPlayer) { return true; }

    }

    if (currentCell == 9) {
        if (this->cells[9] == currentPlayer && this->cells[10] == currentPlayer && this->cells[11] == currentPlayer) { return true; }
        if (this->cells[0] == currentPlayer && this->cells[9] == currentPlayer && this->cells[21] == currentPlayer) { return true; }


    }

    if (currentCell == 10) {
        if (this->cells[9] == currentPlayer && this->cells[10] == currentPlayer && this->cells[11] == currentPlayer) { return true; }
        if (this->cells[3] == currentPlayer && this->cells[10] == currentPlayer && this->cells[18] == currentPlayer) { return true; }


    }if (currentCell == 11) {
        if (this->cells[9] == currentPlayer && this->cells[10] == currentPlayer && this->cells[11] == currentPlayer) { return true; }
        if (this->cells[6] == currentPlayer && this->cells[11] == currentPlayer && this->cells[15] == currentPlayer) { return true; }
    }

    if (currentCell == 12) {
        if (this->cells[12] == currentPlayer && this->cells[13] == currentPlayer && this->cells[14] == currentPlayer) { return true; }
        if (this->cells[8] == currentPlayer && this->cells[12] == currentPlayer && this->cells[17] == currentPlayer) { return true; }

    }
    if (currentCell == 13) {
        if (this->cells[12] == currentPlayer && this->cells[13] == currentPlayer && this->cells[14] == currentPlayer) { return true; }

    }
    if (currentCell == 14) {
        if (this->cells[12] == currentPlayer && this->cells[13] == currentPlayer && this->cells[14] == currentPlayer) { return true; }
        if (this->cells[2] == currentPlayer && this->cells[14] == currentPlayer && this->cells[23] == currentPlayer) { return true; }

    }

    if (currentCell == 15) {
        if (this->cells[15] == currentPlayer && this->cells[16] == currentPlayer && this->cells[17] == currentPlayer) { return true; }
        if (this->cells[6] == currentPlayer && this->cells[11] == currentPlayer && this->cells[15] == currentPlayer) { return true; }

    }

    if (currentCell == 16) {
        if (this->cells[15] == currentPlayer && this->cells[16] == currentPlayer && this->cells[17] == currentPlayer) { return true; }
        if (this->cells[16] == currentPlayer && this->cells[19] == currentPlayer && this->cells[22] == currentPlayer) { return true; }

    }

    if (currentCell == 17) {
        if (this->cells[15] == currentPlayer && this->cells[16] == currentPlayer && this->cells[17] == currentPlayer) { return true; }
        if (this->cells[8] == currentPlayer && this->cells[12] == currentPlayer && this->cells[17] == currentPlayer) { return true; }

    }

    if ( currentCell == 18) {
        if (this->cells[18] == currentPlayer && this->cells[19] == currentPlayer && this->cells[20] == currentPlayer) { return true; }
        if (this->cells[3] == currentPlayer && this->cells[10] == currentPlayer && this->cells[18] == currentPlayer) { return true; }

    }

    if ( currentCell == 19) {
        if (this->cells[18] == currentPlayer && this->cells[19] == currentPlayer && this->cells[20] == currentPlayer) { return true; }
        if (this->cells[16] == currentPlayer && this->cells[19] == currentPlayer && this->cells[22] == currentPlayer) { return true; }

    }

    if ( currentCell == 20) {
        if (this->cells[18] == currentPlayer && this->cells[19] == currentPlayer && this->cells[20] == currentPlayer) { return true; }

    }

    if (currentCell == 21) {
        if (this->cells[21] == currentPlayer && this->cells[22] == currentPlayer && this->cells[23] == currentPlayer) { return true; }
        if (this->cells[0] == currentPlayer && this->cells[9] == currentPlayer && this->cells[21] == currentPlayer) { return true; }

    }

    if (currentCell == 22) {
        if (this->cells[21] == currentPlayer && this->cells[22] == currentPlayer && this->cells[23] == currentPlayer) { return true; }
        if (this->cells[16] == currentPlayer && this->cells[19] == currentPlayer && this->cells[22] == currentPlayer) { return true; }

    }

    if (currentCell == 23) {
        if (this->cells[21] == currentPlayer && this->cells[22] == currentPlayer && this->cells[23] == currentPlayer) { return true; }
        if (this->cells[2] == currentPlayer && this->cells[14] == currentPlayer && this->cells[23] == currentPlayer) { return true; }

    }
    /*
    if (this->cells[0] == currentPlayer && this->cells[1] == currentPlayer && this->cells[2] == currentPlayer) { return true; }
    if (this->cells[3] == currentPlayer && this->cells[4] == currentPlayer && this->cells[5] == currentPlayer) { return true; }
    if (this->cells[9] == currentPlayer && this->cells[10] == currentPlayer && this->cells[11] == currentPlayer) { return true; }
    if (this->cells[12] == currentPlayer && this->cells[13] == currentPlayer && this->cells[14] == currentPlayer) { return true; }
    if (this->cells[15] == currentPlayer && this->cells[16] == currentPlayer && this->cells[17] == currentPlayer) { return true; }
    if (this->cells[18] == currentPlayer && this->cells[19] == currentPlayer && this->cells[20] == currentPlayer) { return true; }
    if (this->cells[21] == currentPlayer && this->cells[22] == currentPlayer && this->cells[23] == currentPlayer) { return true; }

    if (this->cells[0] == currentPlayer && this->cells[9] == currentPlayer && this->cells[21] == currentPlayer) { return true; }
    if (this->cells[3] == currentPlayer && this->cells[10] == currentPlayer && this->cells[18] == currentPlayer) { return true; }
    if (this->cells[6] == currentPlayer && this->cells[11] == currentPlayer && this->cells[15] == currentPlayer) { return true; }
    if (this->cells[1] == currentPlayer && this->cells[4] == currentPlayer && this->cells[7] == currentPlayer) { return true; }
    if (this->cells[16] == currentPlayer && this->cells[19] == currentPlayer && this->cells[22] == currentPlayer) { return true; }
    if (this->cells[8] == currentPlayer && this->cells[12] == currentPlayer && this->cells[17] == currentPlayer) { return true; }
    if (this->cells[2] == currentPlayer && this->cells[14] == currentPlayer && this->cells[23] == currentPlayer) { return true; }

 */
    return false;
}