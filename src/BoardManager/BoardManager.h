//
// Created by Oliver Ilczuk on 05.01.25.
//

#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <Logger/Logger.h>

class BoardManager {
public:
    enum CellState {EMPTY, PLAYER1, PLAYER2, INVALID};
    std::vector<CellState> cells;
    std::unordered_map<int, std::vector<int>> neighbors;

    BoardManager() : cells(24, EMPTY ) {
        neighbors[0] = {1, 9};
        neighbors[1] = {0, 2, 4};
        neighbors[2] = {1, 14};
        neighbors[3] = {4, 10};
        neighbors[4] = {1, 3, 5, 7};
        neighbors[5] = {4, 13};
        neighbors[6] = {7, 11};
        neighbors[7] = {4, 6, 8};
        neighbors[8] = {7, 12};
        neighbors[9] = {0, 10, 21};
        neighbors[10] = {3, 9, 11, 18};
        neighbors[11] = {6, 10, 15};
        neighbors[12] = {8, 13, 17};
        neighbors[13] = {5, 12, 14, 20};
        neighbors[14] = {2, 13, 23};
        neighbors[15] = {11, 16};
        neighbors[16] = {15, 17, 19};
        neighbors[17] = {12, 16};
        neighbors[18] = {10, 19};
        neighbors[19] = {16, 18, 20, 22};
        neighbors[20] = {13, 19};
        neighbors[21] = {9, 22};
        neighbors[22] = {19, 21, 23};
        neighbors[23] = {14, 22};
    }

    ~BoardManager() {
        extern Logger logger;
        logger.log(LogLevel::DEBUG, "~BoardManager called.");
    }

    bool setStone(int position, int player);
    bool isValidMove(int from, int to);
    bool moveStone(int from, int to);
    bool removeStone(int at);
    int getCurrentPlayer() const;

private:
    int currentPlayer = 1; // Spieler 1 beginnt
};

#endif //BOARDMANAGER_H
