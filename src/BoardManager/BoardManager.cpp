//
// Created by Oliver Ilczuk on 05.01.25.
//

#include "BoardManager.h"
#include "../Logger/Logger.h"

extern Logger logger;

BoardManager::BoardManager() : cells(24, EMPTY) {
    initializeNeighbors();
}


bool BoardManager::setStone(int position, CellState state) {
    if (position < 0 || position >= cells.size()) {
        logger.log(LogLevel::ERROR, "setStone: Position out of bounds");
        return false;
    }
    if (cells[position] != EMPTY) {
        logger.log(LogLevel::WARNING, "setStone: Position already occupied");
        return false;
    }

    cells[position] = state;

    if (state == PLAYER1) {
        if (HasStonesLeft_PLAYER1 == 0) {
            logger.log(LogLevel::ERROR, "setStone: No stones left for PLAYER1.");
            return false;
        }
        HasStonesLeft_PLAYER1--;
    } else if (state == PLAYER2) {
        if (HasStonesLeft_PLAYER2 == 0) {
            logger.log(LogLevel::ERROR, "setStone: No stones left for PLAYER2.");
            return false;
        }
        HasStonesLeft_PLAYER2--;
    }

    logger.log(LogLevel::INFO, "setStone: Player " + enumToString(state) + " placed a stone at position " + std::to_string(position));

    if (!checkMill(position, state)) {
        switchPlayer();
    } else {
        millHasBeenFormed = true;
        isDeletingStone = true;
        logger.log(LogLevel::INFO, "setStone: Mill formed at position " + std::to_string(position));
    }

    return true;
}

bool BoardManager::isValidMove(int from, int to) {
    if (std::find(verticalNeighbors[from].begin(), verticalNeighbors[from].end(), to) != verticalNeighbors[from].end() ||
        std::find(horizontalNeighbors[from].begin(), horizontalNeighbors[from].end(), to) != horizontalNeighbors[from].end()) {
        return true;
    }
    if ((getCurrentPlayer() == PLAYER1 && isAllowedToMoveAnywhere_Player1) ||
        (getCurrentPlayer() == PLAYER2 && isAllowedToMoveAnywhere_Player2)) {
        return true;
    }

    logger.log(LogLevel::DEBUG, "isValidMove: Move from " + std::to_string(from) + " to " + std::to_string(to) + " is invalid.");
    return false;
}

bool BoardManager::moveStone(int from, int to) {
    if (from < 0 || from >= cells.size() || to < 0 || to >= cells.size()) {
        logger.log(LogLevel::ERROR, "moveStone: Invalid indices. From: " + std::to_string(from) + ", To: " + std::to_string(to));
        return false;
    }

    if (cells[from] == EMPTY) {
        logger.log(LogLevel::WARNING, "moveStone: No stone at position " + std::to_string(from));
        return false;
    }

    if (cells[to] != EMPTY) {
        logger.log(LogLevel::WARNING, "moveStone: Position " + std::to_string(to) + " is already occupied.");
        return false;
    }

    if (!isValidMove(from, to)) {
        logger.log(LogLevel::WARNING, "moveStone: Move from " + std::to_string(from) + " to " + std::to_string(to) + " is not valid.");
        return false;
    }

    cells[to] = cells[from];
    cells[from] = EMPTY;

    logger.log(LogLevel::INFO, "moveStone: Player " + enumToString(getCellState(to)) + " moved stone from " + std::to_string(from) + " to " + std::to_string(to));

    if (!checkMill(to, getCellState(to))) {
        switchPlayer();
    } else {
        millHasBeenFormed = true;
        isDeletingStone = true;
        logger.log(LogLevel::INFO, "moveStone: Mill formed at position " + std::to_string(to));
    }

    return true;
}

bool BoardManager::removeStone(int at) {
    if (at < 0 || at >= cells.size()) {
        logger.log(LogLevel::ERROR, "removeStone: Position " + std::to_string(at) + " is out of bounds.");
        return false;
    }

    if (cells[at] == EMPTY) {
        logger.log(LogLevel::WARNING, "removeStone: No stone at position " + std::to_string(at) + " to remove.");
        return false;
    }

    cells[at] = EMPTY;
    logger.log(LogLevel::INFO, "removeStone: Stone removed from position " + std::to_string(at));

    if (millHasBeenFormed) {
        millHasBeenFormed = false;
    }
    switchPlayer();

    return true;
}

BoardManager::CellState BoardManager::getCurrentPlayer() const {
    return currentPlayer == 0 ? PLAYER1 : PLAYER2;
}

BoardManager::CellState BoardManager::getOppositePlayer() const {
    return currentPlayer == 0 ? PLAYER2 : PLAYER1;
}

void BoardManager::switchPlayer() {
    currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
}

std::vector<std::pair<int, BoardManager::CellState>> BoardManager::getNeighborsWithState(int position) const {
    std::vector<std::pair<int, CellState>> neighborsWithState;

    if (verticalNeighbors.find(position) != verticalNeighbors.end()) {
        for (int neighbor : verticalNeighbors.at(position)) {
            neighborsWithState.emplace_back(neighbor, cells[neighbor]);
        }
    }

    if (horizontalNeighbors.find(position) != horizontalNeighbors.end()) {
        for (int neighbor : horizontalNeighbors.at(position)) {
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

void BoardManager::initializeNeighbors() {
    verticalNeighbors = {
        {0, {9}}, {1, {4}}, {2, {14}}, {3, {10}}, {4, {1, 7}}, {5, {13}},
        {6, {11}}, {7, {4}}, {8, {12}}, {9, {0, 21}}, {10, {3, 18}},
        {11, {6, 15}}, {12, {8, 17}}, {13, {5, 20}}, {14, {2, 23}},
        {15, {11}}, {16, {19}}, {17, {12}}, {18, {10}}, {19, {16, 22}},
        {20, {13}}, {21, {9}}, {22, {19}}, {23, {14}}
    };

    horizontalNeighbors = {
        {0, {1}}, {1, {0, 2}}, {2, {1}}, {3, {4}}, {4, {3, 5}}, {5, {4}},
        {6, {7}}, {7, {6, 8}}, {8, {7}}, {9, {10}}, {10, {9, 11}}, {11, {10}},
        {12, {13}}, {13, {12, 14}}, {14, {13}}, {15, {16}}, {16, {15, 17}},
        {17, {16}}, {18, {19}}, {19, {18, 20}}, {20, {19}}, {21, {22}},
        {22, {21, 23}}, {23, {22}}
    };
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