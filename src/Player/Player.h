#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

class Player {
    int totalStones;
    int stonesPlayed;

    public:
    
    std::string name;
    char symbol;
    Player(int number, char symbol) : totalStones(9), stonesPlayed(0), symbol(symbol) {
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