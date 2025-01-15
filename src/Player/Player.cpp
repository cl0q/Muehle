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
}