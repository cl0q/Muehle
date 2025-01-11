//
// Created by Oliver Ilczuk on 05.01.25.
//

#include "BoardManager.h"
#include "../Logger/Logger.h"
#include "../Player/Player.h"
#include <algorithm>

bool BoardManager::setStone(int position, Player* p) {
    if (position < 0 || position >= cells.size()) {
        logger.log(LogLevel::ERROR, "setStone: Position " + std::to_string(position) + " is out of bounds.");
        return false;
    }
    if (cells[position] != '.') {
        logger.log(LogLevel::WARNING, "setStone: Position " + std::to_string(position) + " is already occupied.");
        return false;
    }

    cells[position] = p->symbol;
    logger.log(LogLevel::INFO, "setStone: Player " + p->name +
                               " placed a stone at position " + std::to_string(position) + ".");
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

    if (cells[from] == '.') {
        logger.log(LogLevel::WARNING, "moveStone: No stone at position " + std::to_string(from) + ".");
        return false;
    }

    if (cells[to] != '.') {
        logger.log(LogLevel::WARNING, "moveStone: Position " + std::to_string(to) + " is already occupied.");
        return false;
    }

    if (!isValidMove(from, to)) {
        logger.log(LogLevel::WARNING, "moveStone: Move from " + std::to_string(from) +
                                      " to " + std::to_string(to) + " is not valid.");
        return false;
    }

    cells[to] = cells[from];
    cells[from] = '.';

    logger.log(LogLevel::INFO, "moveStone: Player " + std::to_string(cells[to]) +
                               " moved stone from " + std::to_string(from) +
                               " to " + std::to_string(to) + ".");
    return true;
}

bool BoardManager::removeStone(int at) {
    if (at < 0 || at >= cells.size()) {
        logger.log(LogLevel::ERROR, "removeStone: Position " + std::to_string(at) + " is out of bounds.");
        return false;
    }

    if (cells[at] == '.') {
        logger.log(LogLevel::WARNING, "removeStone: No stone at position " + std::to_string(at) + " to remove.");
        return false;
    }

    cells[at] = '.';
    logger.log(LogLevel::INFO, "removeStone: Stone removed from position " + std::to_string(at) + ".");
    return true;
}
