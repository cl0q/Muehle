#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../BoardManager/BoardManager.h"

class Player {
    int totalStones;
    int stonesPlayed;

    public:
    
    std::string name;
    BoardManager::CellState identifier;

    Player(int number, BoardManager::CellState identifier) : totalStones(9), stonesPlayed(0), identifier(identifier) {
        std::cout << "Enter Name for Player " << number << std::endl;
        std::cin >> name;
    };
    
    int place_stone();
    int getTotalStones();
    int getStonesPlayed();

    ~Player() {
         std::cout << "Player " << name << " destructor called. Resources freed." << std::endl;
    }
};

#endif