//
// Created by Oliver Ilczuk on 05.01.25.
//

#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include "../Player/Player.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <unordered_map>
#include <vector>

class BoardManager {
public:
    std::vector<char> cells;
    std::unordered_map<int, std::vector<int>> verticalNeighbors;
    std::unordered_map<int, std::vector<int>> horizontalNeighbors;

    BoardManager() : cells(24, '.' ){
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

    ~BoardManager() {
        std::cout << "BoardManager destructor called. Resources freed." << std::endl;
    }

    bool setStone(int position, Player* p);

    bool isValidMove(int from, int to);

    bool moveStone(int from, int to);

    bool removeStone(int at);
};

#endif //BOARDMANAGER_H
