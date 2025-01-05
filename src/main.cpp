//
// Created by Oliver Ilczuk on 05.01.25.
//

#include <iostream>
#include "BoardManager/BoardManager.h"
#include "Logger/Logger.h"
// Globale Logger-Instanz
Logger logger(LogLevel::DEBUG, "game.log");

int main() {
    // BoardManager-Instanz erstellen
    BoardManager boardManager;

    // Spielfeld initialisieren
    //boardManager.initializeBoard();
    logger.log(LogLevel::INFO, "Game board initialized.");

    // Beispiel: Stein setzen
    if (boardManager.setStone(0, 1)) {
        logger.log(LogLevel::INFO, "Player 1 placed a stone at position 0.");
    } else {
        logger.log(LogLevel::ERROR, "Failed to place a stone for Player 1 at position 0.");
    }

    // Beispiel: Einen ungültigen Stein setzen
    if (!boardManager.setStone(0, 2)) {
        logger.log(LogLevel::WARNING, "Player 2 attempted to place a stone at an occupied position 0.");
    }

    // Beispiel: Stein bewegen
    if (boardManager.moveStone(0, 1)) {
        logger.log(LogLevel::INFO, "Player 1 moved a stone from position 0 to position 1.");
    } else {
        logger.log(LogLevel::ERROR, "Failed to move stone from 0 to 1.");
    }

    // Beispiel: Stein entfernen
    if (boardManager.removeStone(1)) {
        logger.log(LogLevel::INFO, "Player 1's stone removed from position 1.");
    } else {
        logger.log(LogLevel::ERROR, "Failed to remove stone from position 1.");
    }

    // Beenden des Spiels
    logger.log(LogLevel::INFO, "Game session ended.");
    return 0;
}