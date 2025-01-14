#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../BoardManager/BoardManager.h"

extern Logger logger;

class Player {
    int totalStones;
    int stonesPlayed;

    public:
    
    std::string name;
    BoardManager::CellState identifier;

    Player(int number, BoardManager::CellState identifier) : totalStones(9), stonesPlayed(0), identifier(identifier) {
        name = std::to_string(identifier);
    };
    
    int place_stone();
    int getTotalStones();
    int getStonesPlayed();

    ~Player() {
        logger.log(LogLevel::DEBUG, "Player " + name + " destructor called. Resources freed.");

    }
};

#endif