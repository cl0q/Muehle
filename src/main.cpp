//
// Created by Oliver Ilczuk on 05.01.25.
//

#include <iostream>
#include "BoardManager/BoardManager.h"
#include "Logger/Logger.h"
#include "GameManager/GameManager.h"

// Globale Logger-Instanz
Logger logger(LogLevel::WARNING, "game.log");

int main() {
    // Instanziiere Manager
    BoardManager boardManager;
    GameManager gameManager(boardManager);

    // Terminal leeren und Hauptmenü anzeigen
    GameManager::clearScreen();
    logger.log(LogLevel::DEBUG, "Game initialized.");

    gameManager.start();

    return 0;
}