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

    bool millHasBeenFormed = false;
    bool isDeletingStone = false;
    bool isAllowedToMoveAnywhere_Player1 = false;
    bool isAllowedToMoveAnywhere_Player2 = false;

    u_int HasStonesLeft_PLAYER1 = 9;
    u_int HasStonesLeft_PLAYER2 = 9;

    std::string enumToString(CellState cell_state);

    bool setStone(int position, CellState player);

    bool isValidMove(int from, int to);

    bool moveStone(int from, int to);

    bool removeStone(int at);

    BoardManager::CellState getCurrentPlayer() const;

    CellState getOppositePlayer() const;

    void switchPlayer();

    std::vector<std::pair<int, CellState>> getNeighborsWithState(int position) const;

    CellState getCellState(int position) const;

    const std::vector<BoardManager::CellState> &getCells() const;

    void initializeNeighbors();

    bool checkMill(int &currentCell, CellState currentPlayer);

    void setCurrentPlayer(int currentPlayer);

private:
    int currentPlayer = 1;
};

#endif //BOARDMANAGER_H
