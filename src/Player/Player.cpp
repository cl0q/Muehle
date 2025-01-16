#include "Player.h"

int Player::place_stone()
{
    stonesPlayed++;
    return stonesPlayed;
}

int Player::getTotalStones()
{
    return totalStones;
}

int Player::getStonesPlayed()
{
    return stonesPlayed;
}

void Player::decreaseTotalStones() {
    totalStones--;
    if (totalStones == 3) {
        std::cout << name << " is allowed to jump!" << std::endl;
    }
}