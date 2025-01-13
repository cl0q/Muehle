//
// Created by Oliver Ilczuk on 05.01.25.
//

#include <iostream>
#include "BoardManager/BoardManager.h"
#include "Logger/Logger.h"
#include "GameManager/GameManager.h"
#include "UIManager/UIManager.h"
#include "InputHandler/InputHandler.h"
#include "PlayerManager/PlayerManager.h"

// Globale Logger-Instanz
Logger logger(LogLevel::ERROR, "game.log");

int main() {
    // Instanziiere Manager
    BoardManager boardManager;
    GameManager gameManager(boardManager);
    UIManager uiManager;
    InputHandler inputHandler;

    // Spieleroptionen initialisieren
    PlayerManager playerManager;

    // Terminal leeren und Hauptmenü anzeigen
    uiManager.clearScreen();
    logger.log(LogLevel::DEBUG, "Game initialized.");

    gameManager.start();

    return 0;
}