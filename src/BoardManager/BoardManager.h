//
// Created by Oliver Ilczuk on 05.01.25.
//

#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include "../Logger/Logger.h"
#include <iostream>
#include <unordered_map>
#include <vector>

extern Logger logger;

class BoardManager {
public:

    enum CellState {PLAYER1, PLAYER2, EMPTY};

    std::vector<CellState> cells;
    std::unordered_map<int, std::vector<int>> verticalNeighbors;
    std::unordered_map<int, std::vector<int>> horizontalNeighbors;

    ~BoardManager() {
        logger.log(LogLevel::DEBUG, "BoardManager destructor called. Resources freed.");

    }

    BoardManager();

    std::string enumToString(CellState cell_state);

    bool setStone(int position, CellState player);

    bool isValidMove(int from, int to);

    bool moveStone(int from, int to);

    bool removeStone(int at);

    BoardManager::CellState getCurrentPlayer() const;

    void switchPlayer();

    std::vector<std::pair<int, CellState>> getNeighborsWithState(int position) const;

    CellState getCellState(int position) const;

    const std::vector<BoardManager::CellState> &getCells() const;

    void initializeNeighbors();

    void setCurrentPlayer(int currentPlayer);

private:
    int currentPlayer = 1;
};

#endif //BOARDMANAGER_H
