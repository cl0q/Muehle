//
// Created by Oliver Ilczuk on 06.01.25.
//

#include "../BoardManager/BoardManager.h"
#include "../Player/Player.h"
#include "../RuleEngine/RuleEngine.h"
#include "./GameManager.h"
#include <fstream>
#include <vector>
/*
    #include <termios.h>
    #include <unistd.h>
*/

#include "./Logger/Logger.h"

extern Logger logger;

// ---------- GameManager Methoden ----------

void GameManager::start() {
    logger.log(LogLevel::INFO, "GameManager: Starting game manager.");

    logger.log(LogLevel::INFO, "GameManager: Displaying main menu.");
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

    gameLoop();
}

void GameManager::gameLoop() {
    logger.log(LogLevel::INFO, "GameManager: Entering game loop.");

    bool isRunning = true;

    int currentCell = 0;
    displayBoard(board_manager.cells);
    runPhaseOne();

    while (isRunning) {
        logger.log(LogLevel::INFO, "GameManager: Entering second Phase");
        displayBoard(board_manager.cells);
        runPhaseTwo(p1);
        if (rule_engine->isGameOver()) {
            break;
        }
        runPhaseTwo(p2);
        


        isRunning = rule_engine->isGameOver();
    }

    logger.log(LogLevel::INFO, "GameManager: Game loop ended.");
}

void GameManager::runPhaseOne() {
    for (int i = 0; i < 9; i++) {
        std::cout << "Player " << p1->name << " 's turn. " << std::endl;
        int choice = getUserInput(0, 23);
        board_manager.setStone(choice, p1->identifier);
        if (rule_engine->isMillFormed(choice, p1->identifier)) {
            destroyStone(p1, p2, choice);
        }
        
        displayBoard(board_manager.cells);

        std::cout << "Player " << p2->name << " 's turn." << std::endl;
        choice = getUserInput(0, 23);
        board_manager.setStone(choice, p2->identifier);
        if (rule_engine->isMillFormed(choice, p2->identifier)) {
            destroyStone(p2, p1, choice);
        }

        displayBoard(board_manager.cells);
    }
}

void GameManager::runPhaseTwo(Player* p) {
    std::cout << "Player " << p->name << " 's turn. Which Stone should move: " << std::endl;
    int from = getUserInput(0, 23);
    std::cout << "Where do you wanna place it:" << std::endl;
    int to = getUserInput(0, 23);
    board_manager.moveStone(from, to, p);
    if (rule_engine->isMillFormed(to, p->identifier)) {
            destroyStone(p1, p2, to);
        }
}

void GameManager::destroyStone(Player* killer, Player* victim, int position) {
    if (rule_engine->isMillFormed(position, killer->identifier)) {
        std::cout << killer->name << "Wich position do you want to eleminate:" << std::endl;
        int aim = getUserInput(0, 23);
        board_manager.removeStone(aim, victim);        
    }
}

// ---------- Hilfsmethoden ----------

bool isNumber(std::string input) {
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}


int GameManager::getUserInput(int min, int max) {
    int position;
    std::string input;

    while (true) {
        std::cout << "Please enter a position:" << std::endl;
        std::cin >> input;

        if (!isNumber(input)) {
            std::cout << "Invalid Input. Please enter a number!" << std::endl;
            continue;
        }

        position = std::stoi(input);

        if (position < min || position > max) {
            std::cout << "Invalid Input. Please enter a number between 0 and 3!" << std::endl;
            continue;
        }

        break;
    }
    
    return position;
}

bool GameManager::random50Percent() {
    std::srand(std::time(nullptr));
    return std::rand() % 2 == 0;
}

int GameManager::randomPlayerStart() {
    return random50Percent() ? 0 : 1;
}

void GameManager::clearScreen() {
    std::cout << "\033[2J\033[H";
}

// ---------- Spielbrett und Einstellungen ----------

void GameManager::displayBoard(std::vector<BoardManager::CellState> cells) {
    auto getSymbol = [](BoardManager::CellState state) -> std::string {
        switch (state) {
            case BoardManager::CellState::PLAYER1: return "X";
            case BoardManager::CellState::PLAYER2: return "O";
            case BoardManager::CellState::EMPTY:   return ".";
        }
        return "?"; // Fallback für ungültige Zustände
    };

    // Spielfeld anzeigen (Indexzuordnung folgt einem typischen Mühlespielfeld)
    std::cout << "\n";
    std::cout << " " << getSymbol(cells[0]) << "-----------" << getSymbol(cells[1]) << "-----------" << getSymbol(cells[2]) << "\n";
    std::cout << " |           |           |\n";
    std::cout << " |   " << getSymbol(cells[8]) << "-------" << getSymbol(cells[9]) << "-------" << getSymbol(cells[10]) << "   |\n";
    std::cout << " |   |       |       |   |\n";
    std::cout << " |   |   " << getSymbol(cells[16]) << "---" << getSymbol(cells[17]) << "---" << getSymbol(cells[18]) << "   |   |\n";
    std::cout << " |   |       |       |   |\n";
    std::cout << " |   " << getSymbol(cells[11]) << "-------" << getSymbol(cells[12]) << "-------" << getSymbol(cells[13]) << "   |\n";
    std::cout << " |           |           |\n";
    std::cout << " " << getSymbol(cells[3]) << "-----------" << getSymbol(cells[4]) << "-----------" << getSymbol(cells[5]) << "\n";
    std::cout << "\n";
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