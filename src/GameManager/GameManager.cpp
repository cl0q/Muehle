//
// Created by Oliver Ilczuk on 06.01.25.
//

#include "./BoardManager/BoardManager.h"
#include "GameManager.h"
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include "./Logger/Logger.h"

extern Logger logger;

// ---------- GameManager Methoden ----------

void GameManager::start() {
    logger.log(LogLevel::INFO, "GameManager: Starting game manager.");

    logger.log(LogLevel::INFO, "GameManager: Displaying main menu.");
    std::cout << "\n\n\n";
    std::cout << "\t\t\tWillkommen bei Mühle!\n\n";
    std::cout << "\t\t1. Neues Spiel starten\n";
    std::cout << "\t\t2. Spiel laden\n";
    std::cout << "\t\t3. Einstellungen\n";

    int userInput = getUserInput(1, 3);
    if (userInput == 1) {
        startNewGame();
    } else if (userInput == 2) {
        if (isSaveFileAvailable()) {
            loadGame("game_save.dat");
        } else {
            logger.log(LogLevel::WARNING, "GameManager: No save file available.");
        }
    } else if (userInput == 3) {
        handleSettingsMenu();
    }
}

void GameManager::startNewGame() {
    logger.log(LogLevel::INFO, "GameManager: Starting new game.");
    clearScreen();

    int startingPlayer = randomPlayerStart();
    logger.log(LogLevel::INFO, "GameManager: Player " + std::to_string(startingPlayer) + " starts the game.");

    printBoard(this->board_manager, startingPlayer);
    gameLoop();
}

void GameManager::gameLoop() {
    logger.log(LogLevel::INFO, "GameManager: Entering game loop.");

    bool isRunning = true;

    while (isRunning) {
        printBoard(this->board_manager, this->board_manager.getCurrentPlayer());

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");

        // Platzhalter für zukünftige RuleEngine-Integration
        // Abbruchbedingung: Hier eine manuelle Eingabe simulieren
        char exitInput;
        std::cout << "Spiel beenden? (j/n): ";
        std::cin >> exitInput;

        if (exitInput == 'j' || exitInput == 'J') {
            isRunning = false;
        }
    }

    logger.log(LogLevel::INFO, "GameManager: Game loop ended.");
}

// ---------- Hilfsmethoden ----------

int GameManager::getUserInput(int min, int max) {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int input = -1;
    do {
        ch = getchar();
        if (ch >= '0' + min && ch <= '0' + max) {
            input = ch - '0';
        } else {
            logger.log(LogLevel::WARNING, "GameManager: Invalid input. Please try again.");
        }
    } while (input == -1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return input;
}

bool GameManager::random50Percent() {
    std::srand(std::time(nullptr));
    return std::rand() % 2 == 0;
}

int GameManager::randomPlayerStart() {
    return random50Percent() ? 1 : 2;
}

void GameManager::clearScreen() {
    std::cout << "\033[2J\033[H";
}


// ---------- Spielbrett und Einstellungen ----------

void GameManager::printBoard(const BoardManager& boardManager, int currentPlayer) {
    const std::string player1 = "{ X }";
    const std::string player2 = "{ O }";
    const std::string empty = "{   }";

    auto getCellRepresentation = [&](BoardManager::CellState state) -> std::string {
        switch (state) {
            case BoardManager::PLAYER1: return player1;
            case BoardManager::PLAYER2: return player2;
            case BoardManager::EMPTY: return empty;
            default: return empty;
        }
    };

    const auto& cells = boardManager.cells;

    std::cout << "\033[2J\033[H"; // Bildschirm löschen und Cursor zurücksetzen

    logger.log(LogLevel::INFO, "GameManager: Drawing board.");
    std::cout << "\n";
    std::cout << "				  " << getCellRepresentation(cells[0]) << "--------------------" << getCellRepresentation(cells[1]) << "--------------------" << getCellRepresentation(cells[2]) << "\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |      " << getCellRepresentation(cells[3]) << "-----------" << getCellRepresentation(cells[4]) << "-----------" << getCellRepresentation(cells[5]) << "      |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(cells[6]) << "---" << getCellRepresentation(cells[7]) << "---" << getCellRepresentation(cells[8]) << "     |        |\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				  " << getCellRepresentation(cells[9]) << "    " << getCellRepresentation(cells[10]) << "   " << getCellRepresentation(cells[11]) << "           " << getCellRepresentation(cells[12]) << "   " << getCellRepresentation(cells[13]) << "    " << getCellRepresentation(cells[14]) << "\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(cells[15]) << "---" << getCellRepresentation(cells[16]) << "---" << getCellRepresentation(cells[17]) << "     |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |      " << getCellRepresentation(cells[18]) << "-----------" << getCellRepresentation(cells[19]) << "-----------" << getCellRepresentation(cells[20]) << "      |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				  " << getCellRepresentation(cells[21]) << "--------------------" << getCellRepresentation(cells[22]) << "--------------------" << getCellRepresentation(cells[23]) << "\n";
    std::cout << "\n\n\n";

    logger.log(LogLevel::INFO, "GameManager: Player " + std::to_string(currentPlayer) + " is now playing.");
}

void GameManager::handleSettingsMenu() {
    logger.log(LogLevel::INFO, "GameManager: Handling settings menu.");

    std::vector<std::string> options = {"1. PLAYER1", "2. PLAYER2"};
    int selectedPlayer = getUserInput(1, 2);

    if (selectedPlayer == 1) {
        logger.log(LogLevel::INFO, "GameManager: Modifying PLAYER1 settings.");
        // Hier könnten Änderungen an PLAYER1 implementiert werden
    } else if (selectedPlayer == 2) {
        logger.log(LogLevel::INFO, "GameManager: Modifying PLAYER2 settings.");
        // Hier könnten Änderungen an PLAYER2 implementiert werden
    }
}

bool GameManager::loadGame(const std::string& saveFile) {
    std::ifstream file(saveFile);
    if (!file.is_open()) {
        logger.log(LogLevel::ERROR, "GameManager: Failed to load game from " + saveFile);
        return false;
    }
    logger.log(LogLevel::INFO, "GameManager: Game loaded successfully from " + saveFile + ".");
    return true;
}

bool GameManager::isSaveFileAvailable() {
    std::ifstream file("game_save.dat");
    return file.is_open();
}