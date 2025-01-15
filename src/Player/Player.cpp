#include "./Player.h"


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

int Player::decreaseTotalStones()
{
    totalStones--;
    return totalStones;
}