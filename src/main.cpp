//
// Created by Oliver Ilczuk on 05.01.25.
//

#include <iostream>
#include "BoardManager/BoardManager.h"
#include "Logger/Logger.h"
#include "GameManager/GameManager.h"
#include "Player/Player.h"

// Globale Logger-Instanz
Logger logger(LogLevel::INFO, "game.log");

int main() {
    // Instanziiere Manager
    BoardManager boardManager;
    GameManager gameManager(boardManager);
    Player player1(0, BoardManager::PLAYER1);
    Player player2(1, BoardManager::PLAYER2);

    // Terminal leeren und Hauptmenü anzeigen
    GameManager::clearScreen();
    logger.log(LogLevel::DEBUG, "Game initialized.");

    gameManager.start();

    return 0;
}