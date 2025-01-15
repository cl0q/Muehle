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
    logger.log(LogLevel::INFO, "GameManager: Player " + this->board_manager.enumToString(this->board_manager.getCurrentPlayer()) + " starts the game.");

    gameLoop();
}

void GameManager::gameLoop() {
    logger.log(LogLevel::INFO, "GameManager: Entering game loop.");

    bool isRunningSetPhase = true;
    bool isRunningMovePhase = false;
    bool isRunningJumpPhase = false;

    isMovingStone = false;
    movingStoneFrom = 24;
    bool pressedEnter = false;


    int currentCell = 0;

    while (isRunningSetPhase) {
        printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
        currentCell = setPhase(this->board_manager, currentCell);

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");

        printPhase();
        // Platzhalter für zukünftige RuleEngine-Integration
        // Abbruchbedingung: Hier eine manuelle Eingabe simulieren
    }

    while (isRunningMovePhase) {
        printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
        currentCell = movePhase(this->board_manager, currentCell);

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");
        printPhase();

        // Platzhalter für zukünftige RuleEngine-Integration
        // Abbruchbedingung: Hier eine manuelle Eingabe simulieren
    }

    while (isRunningJumpPhase) {
        printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
        currentCell = jumpPhase(this->board_manager, currentCell);

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");
        printPhase();

        // Platzhalter für zukünftige RuleEngine-Integration
        // Abbruchbedingung: Hier eine manuelle Eingabe simulieren
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

void GameManager::clearScreen() {
    std::cout << "\033[2J\033[H";
}

int readKeyAsInt() {
    struct termios oldt, newt;
    char key;

    // Aktuelle Terminal-Einstellungen speichern
    tcgetattr(STDIN_FILENO, &oldt);

    // Neue Terminal-Einstellungen setzen (Raw-Modus aktivieren)
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Canonical-Mode und Echo deaktivieren
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Lesen der Tastatureingabe
    read(STDIN_FILENO, &key, 1);

    // Alte Terminal-Einstellungen wiederherstellen
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return static_cast<int>(key);
}

int getKey() {
    int input = readKeyAsInt(); // Liest die erste Taste
    if (input == 27) { // Escape-Sequenz beginnt
        input = readKeyAsInt();
        if (input == 91) { // '[' erkannt
            return readKeyAsInt(); // Richtungswert
        }
    }
    return input; // Gibt den ASCII-Wert zurück
}


// Funktion zum Bewegen des Cursors basierend auf Tastatureingaben
int GameManager::setPhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey(); // Liest eine einzelne Taste ein

        // Debug: Welche Taste wurde gedrückt?
        if (key == 65) {
            std::cout << "Taste gedrückt: Oben\n";
        } else if (key == 66) {
            std::cout << "Taste gedrückt: Unten\n";
        } else if (key == 67) {
            std::cout << "Taste gedrückt: Rechts\n";
        } else if (key == 68) {
            std::cout << "Taste gedrückt: Links\n";
        } else if (key == 32) {
            std::cout << "Taste gedrückt: Leertaste\n";
        } else if (key == 10) {
            std::cout << "Taste gedrückt: Enter\n";
        }

        int nextCell = currentCell;
        switch (key) {
            case 65: // Oben
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 66: // Unten
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 68: // Links
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 67: // Rechts
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;

            case 32: // Leertaste
                pressedEnter = false;

            std::cout << "[SPACE HIT]" << std::endl;


                if (!isMovingStone) {
                    std::cout << "[SPACE SET] Leertaste gedrückt" << std::endl << "currentCell: " << currentCell << " belongs to : " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;

                    boardManager.setStone(currentCell, boardManager.getCurrentPlayer());
                    break;
                } else if (isMovingStone && this->movingStoneFrom != -1) {
                    std::cout << "[SPACE MOVING] movingStone from: " << this->movingStoneFrom << " to: " << currentCell << " , currentPlayer: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {
                    std::cout << "[SPACE MOVING] movedStone from : " << this->movingStoneFrom << " to : " << currentCell << " , currentPlayer" << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                    isMovingStone = false;
                    this->movingStoneFrom = -1;
                        break;
                    }
                    break;
                }
            case 10: // Enter
                if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                    std::cout << "[ENTER CELLSTATE BELONGS CORRECT TO] " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (!pressedEnter) {
                        pressedEnter = true;
                        std::cout << "[ENTER MOV] movingStone from: " << currentCell << " belongs to: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                        this->isMovingStone = true;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = currentCell;

                        break;
                    }
                    if (pressedEnter && currentCell == this->movingStoneFrom) {

                        pressedEnter = false;
                        this->isMovingStone = false;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = -1;

                        break;
                    }
                }
                std::cout << "[ENTER NOT MINE] cellstate at: " << currentCell << " is: " << boardManager.enumToString(boardManager.getCellState(currentCell)) << std::endl;


        }

        printStatus(boardManager, currentCell);

        // Überprüfen, ob die Bewegung gültig ist
        if (boardManager.isValidMove(currentCell, nextCell)) {
            std::cout << "Bewegung gültig: Zelle " << currentCell << " -> Zelle " << nextCell << "currentCell owner: " << boardManager.enumToString(boardManager.getCellState(currentCell))<< "\n";
            return nextCell;
        } else {
            return nextCell;
        }
    }
}

int GameManager::movePhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey(); // Liest eine einzelne Taste ein

        // Debug: Welche Taste wurde gedrückt?
        if (key == 65) {
            std::cout << "Taste gedrückt: Oben\n";
        } else if (key == 66) {
            std::cout << "Taste gedrückt: Unten\n";
        } else if (key == 67) {
            std::cout << "Taste gedrückt: Rechts\n";
        } else if (key == 68) {
            std::cout << "Taste gedrückt: Links\n";
        } else if (key == 32) {
            std::cout << "Taste gedrückt: Leertaste\n";
        } else if (key == 10) {
            std::cout << "Taste gedrückt: Enter\n";
        }

        int nextCell = currentCell;
        switch (key) {
            case 65: // Oben
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 66: // Unten
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 68: // Links
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 67: // Rechts
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;

            case 32: // Leertaste
                pressedEnter = false;

            std::cout << "[SPACE HIT]" << std::endl;


                if (!isMovingStone) {
                    std::cout << "[SPACE SET] Leertaste gedrückt" << std::endl << "currentCell: " << currentCell << " belongs to : " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;

                    boardManager.setStone(currentCell, boardManager.getCurrentPlayer());
                    break;
                } else if (isMovingStone && this->movingStoneFrom != -1) {
                    std::cout << "[SPACE MOVING] movingStone from: " << this->movingStoneFrom << " to: " << currentCell << " , currentPlayer: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {
                    std::cout << "[SPACE MOVING] movedStone from : " << this->movingStoneFrom << " to : " << currentCell << " , currentPlayer" << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                    isMovingStone = false;
                    this->movingStoneFrom = -1;
                        break;
                    }
                    break;
                }
            case 10: // Enter
                if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                    std::cout << "[ENTER CELLSTATE BELONGS CORRECT TO] " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (!pressedEnter) {
                        pressedEnter = true;
                        std::cout << "[ENTER MOV] movingStone from: " << currentCell << " belongs to: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                        this->isMovingStone = true;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = currentCell;

                        break;
                    }
                    if (pressedEnter && currentCell == this->movingStoneFrom) {

                        pressedEnter = false;
                        this->isMovingStone = false;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = -1;

                        break;
                    }
                }
                std::cout << "[ENTER NOT MINE] cellstate at: " << currentCell << " is: " << boardManager.enumToString(boardManager.getCellState(currentCell)) << std::endl;


        }

        printStatus(boardManager, currentCell);

        // Überprüfen, ob die Bewegung gültig ist
        if (boardManager.isValidMove(currentCell, nextCell)) {
            std::cout << "Bewegung gültig: Zelle " << currentCell << " -> Zelle " << nextCell << "currentCell owner: " << boardManager.enumToString(boardManager.getCellState(currentCell))<< "\n";
            return nextCell;
        } else {
            return nextCell;
        }
    }
}

int GameManager::jumpPhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey(); // Liest eine einzelne Taste ein

        // Debug: Welche Taste wurde gedrückt?
        if (key == 65) {
            std::cout << "Taste gedrückt: Oben\n";
        } else if (key == 66) {
            std::cout << "Taste gedrückt: Unten\n";
        } else if (key == 67) {
            std::cout << "Taste gedrückt: Rechts\n";
        } else if (key == 68) {
            std::cout << "Taste gedrückt: Links\n";
        } else if (key == 32) {
            std::cout << "Taste gedrückt: Leertaste\n";
        } else if (key == 10) {
            std::cout << "Taste gedrückt: Enter\n";
        }

        int nextCell = currentCell;
        switch (key) {
            case 65: // Oben
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 66: // Unten
                for (int neighbor : boardManager.verticalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 68: // Links
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor < currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;
            case 67: // Rechts
                for (int neighbor : boardManager.horizontalNeighbors[currentCell]) {
                    if (neighbor > currentCell) {
                        nextCell = neighbor;
                        break;
                    }
                }
                break;

            case 32: // Leertaste
                pressedEnter = false;

            std::cout << "[SPACE HIT]" << std::endl;


                if (!isMovingStone) {
                    std::cout << "[SPACE SET] Leertaste gedrückt" << std::endl << "currentCell: " << currentCell << " belongs to : " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;

                    boardManager.setStone(currentCell, boardManager.getCurrentPlayer());
                    break;
                } else if (isMovingStone && this->movingStoneFrom != -1) {
                    std::cout << "[SPACE MOVING] movingStone from: " << this->movingStoneFrom << " to: " << currentCell << " , currentPlayer: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {
                    std::cout << "[SPACE MOVING] movedStone from : " << this->movingStoneFrom << " to : " << currentCell << " , currentPlayer" << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                    isMovingStone = false;
                    this->movingStoneFrom = -1;
                        break;
                    }
                    break;
                }
            case 10: // Enter
                if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                    std::cout << "[ENTER CELLSTATE BELONGS CORRECT TO] " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;
                    if (!pressedEnter) {
                        pressedEnter = true;
                        std::cout << "[ENTER MOV] movingStone from: " << currentCell << " belongs to: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl;


                        this->isMovingStone = true;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = currentCell;

                        break;
                    }
                    if (pressedEnter && currentCell == this->movingStoneFrom) {

                        pressedEnter = false;
                        this->isMovingStone = false;
                        std::cout << "set isMovingStone: " << isMovingStone << std::endl;
                        this->movingStoneFrom = -1;

                        break;
                    }
                }
                std::cout << "[ENTER NOT MINE] cellstate at: " << currentCell << " is: " << boardManager.enumToString(boardManager.getCellState(currentCell)) << std::endl;


        }

        printStatus(boardManager, currentCell);

        // Überprüfen, ob die Bewegung gültig ist
        if (boardManager.isValidMove(currentCell, nextCell)) {
            std::cout << "Bewegung gültig: Zelle " << currentCell << " -> Zelle " << nextCell << "currentCell owner: " << boardManager.enumToString(boardManager.getCellState(currentCell))<< "\n";
            return nextCell;
        } else {
            return nextCell;
        }
    }
}

void GameManager::printStatus(BoardManager& boardManager, int currentCell) {
    std::cout << "isMovingStone: " << this->isMovingStone << std::endl
                        << "pressedEnter: " << this->pressedEnter << std::endl
                        << "currentCell: " << currentCell << std::endl
                        << "movingStoneFrom: " << this->movingStoneFrom << std::endl
                        << "currentPlayer: " << boardManager.enumToString(boardManager.getCurrentPlayer()) << std::endl
                    << "last cellState: " << boardManager.enumToString(boardManager.getCellState(currentCell)) << std::endl
                    <<  "sumStones : " << getPlacedStones() << std::endl;
}

int GameManager::getPlacedStones() {
    int sumStones = 0;

    for (int i = 0; i < this->board_manager.getCells().size(); ++i) {
        if (this->board_manager.getCells()[i] != BoardManager::CellState::EMPTY) {
            //std::cout << "sumStones index: " << i << " cellState: " << this->board_manager.enumToString(this->board_manager.getCells()[i]) << std::endl;
            sumStones++;
        }
    }
    return sumStones;
}

// ---------- Spielbrett und Einstellungen ----------

void GameManager::printBoard(const BoardManager& boardManager, BoardManager::CellState currentPlayer, int currentCell = -1) {
    const std::string player1 = "{ X }";
    const std::string player2 = "{ O }";
    const std::string empty = "{   }";
    const std::string highlighted = "\033[31m"; // ANSI escape code für rote Schrift
    const std::string beingMoved = "\033[44m"; // ANSI escape code für blaue Schrift
    const std::string currentIsSelected = "\033[31m\033[44m"; // Red text on blue background
    const std::string reset = "\033[0m";       // ANSI escape code für Reset

    auto getCellRepresentation = [&](int index, BoardManager::CellState state) -> std::string {
        std::string cellContent;
        switch (state) {
            case BoardManager::CellState::PLAYER1: cellContent = player1; break;
            case BoardManager::CellState::PLAYER2: cellContent = player2; break;
            case BoardManager::CellState::EMPTY: cellContent = empty; break;
            default: cellContent = empty; break;
        }
        if (index == this->movingStoneFrom && index == currentCell && this->pressedEnter) {
            return currentIsSelected + cellContent + reset;
        } else if (index == currentCell) {
            return highlighted + cellContent + reset;
        } else if (index == this->movingStoneFrom) {
            return beingMoved + cellContent + reset;
        }

        return cellContent;
    };

    const auto& cells = boardManager.getCells();

    for (int i = 0; i < cells.size(); ++i) {
        logger.log(LogLevel::DEBUG, "Cell " + std::to_string(i) + ": " + std::to_string(this->board_manager.cells[i]));
    }

    //std::cout << "\033[2J\033[H"; // Bildschirm löschen und Cursor zurücksetzen

    logger.log(LogLevel::INFO, "GameManager: Drawing board.");
    std::cout << "\n\n\n";
    std::cout << "				  " << getCellRepresentation(0, this->board_manager.cells[0]) << "--------------------" << getCellRepresentation(1, this->board_manager.cells[1]) << "--------------------" << getCellRepresentation(2, this->board_manager.cells[2]) << "\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |      " << getCellRepresentation(3, this->board_manager.cells[3]) << "-----------" << getCellRepresentation(4, this->board_manager.cells[4]) << "-----------" << getCellRepresentation(5, this->board_manager.cells[5]) << "      |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(6, this->board_manager.cells[6]) << "---" << getCellRepresentation(7, this->board_manager.cells[7]) << "---" << getCellRepresentation(8, this->board_manager.cells[8]) << "     |        |\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				  " << getCellRepresentation(9, this->board_manager.cells[9]) << "----" << getCellRepresentation(10, this->board_manager.cells[10]) << "---" << getCellRepresentation(11, this->board_manager.cells[11]) << "           " << getCellRepresentation(12, this->board_manager.cells[12]) << "---" << getCellRepresentation(13, this->board_manager.cells[13]) << "----" << getCellRepresentation(14, this->board_manager.cells[14]) << "\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(15, this->board_manager.cells[15]) << "---" << getCellRepresentation(16, this->board_manager.cells[16]) << "---" << getCellRepresentation(17, this->board_manager.cells[17]) << "     |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |      " << getCellRepresentation(18, this->board_manager.cells[18]) << "-----------" << getCellRepresentation(19, this->board_manager.cells[19]) << "-----------" << getCellRepresentation(20, this->board_manager.cells[20]) << "      |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				  " << getCellRepresentation(21, this->board_manager.cells[21]) << "--------------------" << getCellRepresentation(22, this->board_manager.cells[22]) << "--------------------" << getCellRepresentation(23, this->board_manager.cells[23]) << "\n";
    std::cout << "\n\n\n";
    std::cout << "\t\t\t\t\t\t **Am Zug: Spieler " << (currentPlayer == 1 ? player2 : player1) << "**\n";
    std::cout << "\t\t\t\t\t\t **currentPlayer: " << this->board_manager.enumToString(currentPlayer) << "**\n";


        std::cout << "\n\n\n";
    std::cout << "				  " << this->board_manager.enumToString(this->board_manager.cells[0]) << "--------------------" << this->board_manager.enumToString(this->board_manager.cells[1]) << "--------------------" << this->board_manager.enumToString(this->board_manager.cells[2]) << "\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |      " << this->board_manager.enumToString(this->board_manager.cells[3]) << "-----------" << this->board_manager.enumToString(this->board_manager.cells[4]) << "-----------" << this->board_manager.enumToString(this->board_manager.cells[5]) << "      |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |     " << this->board_manager.enumToString(this->board_manager.cells[6]) << "---" << this->board_manager.enumToString(this->board_manager.cells[7]) << "---" << this->board_manager.enumToString(this->board_manager.cells[8]) << "     |        |\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				  " << this->board_manager.enumToString(this->board_manager.cells[9]) << "----" << this->board_manager.enumToString(this->board_manager.cells[10]) << "---" << this->board_manager.enumToString(this->board_manager.cells[11]) << "           " << this->board_manager.enumToString(this->board_manager.cells[12]) << "---" << this->board_manager.enumToString(this->board_manager.cells[13]) << "----" << this->board_manager.enumToString(this->board_manager.cells[14]) << "\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				    |        |     " << this->board_manager.enumToString(this->board_manager.cells[15]) << "---" << this->board_manager.enumToString(this->board_manager.cells[16]) << "---" << this->board_manager.enumToString(this->board_manager.cells[17]) << "     |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |      " << this->board_manager.enumToString(this->board_manager.cells[18]) << "-----------" << this->board_manager.enumToString(this->board_manager.cells[19]) << "-----------" << this->board_manager.enumToString(this->board_manager.cells[20]) << "      |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				  " << this->board_manager.enumToString(this->board_manager.cells[21]) << "--------------------" << this->board_manager.enumToString(this->board_manager.cells[22]) << "--------------------" << this->board_manager.enumToString(this->board_manager.cells[23]) << "\n";
    std::cout << "\n\n\n";



    for (int i = 0; i < cells.size(); ++i) {
        logger.log(LogLevel::DEBUG, "cellState " + std::to_string(i) + ": " + this->board_manager.enumToString(this->board_manager.cells[i]));
    }

    logger.log(LogLevel::INFO, "GameManager: Player " + this->board_manager.enumToString(currentPlayer) + " " + " is now playing.");
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

void GameManager::printPhase() {
    switch (this->currentPhase){
        case 0:
            std::cout << "Setzphase" << std::endl;
            break;
        case 1:
            std::cout << "Zugphase" << std::endl;
            break;
        case 2:
            std::cout << "Sprungphase" << std::endl;
            break;
    }
}
