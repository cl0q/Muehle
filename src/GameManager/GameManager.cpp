//
// Created by Oliver Ilczuk on 06.01.25.
//

#include "./BoardManager/BoardManager.h"
#include "GameManager.h"
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include "./Logger/Logger.h"
#include <sstream>
#include <iostream>

extern Logger logger;

// ---------- GameManager Methoden ----------

void GameManager::start() {
    logger.log(LogLevel::INFO, "GameManager: Starting game manager.");

    logger.log(LogLevel::INFO, "GameManager: Displaying main menu.");
    clearScreen();
    std::cout << "\t\t\tWillkommen bei Mühle!\n\n";
    std::cout << "\t\t1. Neues Spiel starten\n";
    std::cout << "\t\t2. Spiel laden\n";
    std::cout << "\t\t3. Einstellungen\n";

    int userInput = getUserInput(1, 3);
    if (userInput == 1) {
        startNewGame();
    } else if (userInput == 2) {
        if (isSaveFileAvailable()) {
            loadGame("save.muehle");
            gameLoop();

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
    movingStoneFrom = 24;
    bool pressedEnter = false;


    int currentCell = 0;
    while (this->board_manager.HasStonesLeft_PLAYER1 != 0 || this->board_manager.HasStonesLeft_PLAYER2 != 0) {
            printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
            currentCell = setPhase(this->board_manager, currentCell);

            logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");

            printPhase();
            this->saveGame("save.muehle");
    }
    currentPhase = 1;

    while (getPlacedStones()[0] > 3 && getPlacedStones()[1] > 3) {

        printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
        currentCell = movePhase(this->board_manager, currentCell);

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");

        printPhase();
        this->saveGame("save.muehle");

    }
    currentPhase = 2;

    while ((getPlacedStones()[0] > 2 && hasLegalMoves(this->board_manager, BoardManager::CellState::PLAYER1)) &&
           (getPlacedStones()[1] > 2 && hasLegalMoves(this->board_manager, BoardManager::CellState::PLAYER2))) {

        isPlayerAllowedToMoveAnywhere(this->board_manager.getCurrentPlayer());

        printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);
        currentCell = jumpPhase(this->board_manager, currentCell);

        logger.log(LogLevel::INFO, "GameManager: Waiting for the next move.");

        printPhase();
        this->saveGame("save.muehle");
    }

    printBoard(this->board_manager, this->board_manager.getCurrentPlayer(), currentCell);

    BoardManager::CellState winner = this->board_manager.getCurrentPlayer();

    std::cout << "[WINNER] : " << this->board_manager.enumToString(winner) << std::endl;

    logger.log(LogLevel::INFO, "GameManager: Game loop ended.");

    pause();
    start();
}

// ---------- Hilfsmethoden ----------

void GameManager::pause() {
    std::cout << "\nDrücke eine beliebige Taste, um fortzufahren...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

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

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    read(STDIN_FILENO, &key, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return static_cast<int>(key);
}

int GameManager::getKey() {
    int input = readKeyAsInt();
    if (input == 27) {
        input = readKeyAsInt();
        if (input == 91) {
            return readKeyAsInt();
        }
    }
    this->latestKeyInput = input;
    return input;
}

int GameManager::setPhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey();

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

                if (!boardManager.millHasBeenFormed) {
                    if (!isMovingStone) {
                        boardManager.setStone(currentCell, boardManager.getCurrentPlayer());
                        break;
                    } else if (isMovingStone && this->movingStoneFrom != -1) {
                        if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {
                            isMovingStone = false;
                            this->movingStoneFrom = -1;
                            break;
                        }
                        break;
                    }
                } else if (boardManager.millHasBeenFormed) {
                }
                break;
            case 10: // Enter
                if (!boardManager.millHasBeenFormed) {
                    if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                        if (!pressedEnter) {
                            pressedEnter = true;
                            this->isMovingStone = true;
                            this->movingStoneFrom = currentCell;

                            break;
                        }
                        if (pressedEnter && currentCell == this->movingStoneFrom) {
                            pressedEnter = false;
                            this->isMovingStone = false;
                            this->movingStoneFrom = -1;

                            break;
                        }
                    } else {
                        break;
                    }
                } else if (boardManager.millHasBeenFormed) {
                    break;
                }
            break;

            case 127: // Backspace
                if (boardManager.millHasBeenFormed) {
                    if (boardManager.getCellState(currentCell) != boardManager.getCurrentPlayer() && boardManager.getCellState(currentCell) != boardManager.EMPTY
                        && !(boardManager.checkMill(currentCell, boardManager.getOppositePlayer())) ) {
                        boardManager.removeStone(currentCell);
                        this->dekrementStoneAfterDelete();
                        break;
                    }
                    break;
                }
            break;
        }

        //printStatus(boardManager, currentCell);

        if (boardManager.isValidMove(currentCell, nextCell)) {
            return nextCell;
        } else {
            return nextCell;
        }
    }
}

int GameManager::movePhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey();
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

                if (!isMovingStone) {
                    break;
                } else if (isMovingStone && this->movingStoneFrom != -1) {
                    if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {
                    isMovingStone = false;
                    this->movingStoneFrom = -1;
                        break;
                    }
                    break;
                }
            case 10: // Enter
                if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                    if (!pressedEnter) {
                        pressedEnter = true;
                        this->isMovingStone = true;
                        this->movingStoneFrom = currentCell;

                        break;
                    }
                    if (pressedEnter && currentCell == this->movingStoneFrom) {

                        pressedEnter = false;
                        this->isMovingStone = false;
                        this->movingStoneFrom = -1;

                        break;
                    }
                }
            break;

            case 127: // Backspace
                if (boardManager.millHasBeenFormed) {
                    if (boardManager.getCellState(currentCell) != boardManager.getCurrentPlayer() && boardManager.getCellState(currentCell) != boardManager.EMPTY
                        && !(boardManager.checkMill(currentCell, boardManager.getOppositePlayer())) ) {
                        boardManager.removeStone(currentCell);
                        this->dekrementStoneAfterDelete();
                        break;
                        }
                    break;
                }
            break;

        }

        //printStatus(boardManager, currentCell);

        if (boardManager.isValidMove(currentCell, nextCell)) {
            return nextCell;
        } else {
            return nextCell;
        }
    }
}

int GameManager::jumpPhase(BoardManager& boardManager, int currentCell) {

    while (true) {
        int key = getKey();

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

                if (!isMovingStone) {
                    break;
                } else if (isMovingStone && this->movingStoneFrom != -1) {
                    if (boardManager.moveStone(this->movingStoneFrom, currentCell)) {

                    isMovingStone = false;
                    this->movingStoneFrom = -1;
                        break;
                    }
                    break;
                }
            case 10: // Enter
                if (boardManager.getCellState(currentCell) == boardManager.getCurrentPlayer()) {
                    if (!pressedEnter) {
                        pressedEnter = true;
                        this->isMovingStone = true;
                        this->movingStoneFrom = currentCell;

                        break;
                    }
                    if (pressedEnter && currentCell == this->movingStoneFrom) {

                        pressedEnter = false;
                        this->isMovingStone = false;
                        this->movingStoneFrom = -1;

                        break;
                    }
                }
            break;

            case 127: // Backspace
                if (boardManager.millHasBeenFormed) {
                    if (boardManager.getCellState(currentCell) != boardManager.getCurrentPlayer() && boardManager.getCellState(currentCell) != boardManager.EMPTY
                        && !(boardManager.checkMill(currentCell, boardManager.getOppositePlayer())) ) {
                        boardManager.removeStone(currentCell);
                        this->dekrementStoneAfterDelete();
                        break;
                        }
                    break;
                }
            break;
        }

        //printStatus(boardManager, currentCell);

        if (boardManager.isValidMove(currentCell, nextCell)) {
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
                    <<  "getPlacedStones_Player1 : " << getPlacedStones()[0] << std::endl
                    <<  "getPlacedStones_Player2 : " << getPlacedStones()[1] << std::endl
                    <<  "HasStonesLeft_PLAYER1 : " << this->board_manager.HasStonesLeft_PLAYER1 << std::endl
                    <<  "HasStonesLeft_PLAYER2 : " << this->board_manager.HasStonesLeft_PLAYER2 << std::endl
                    << "latest keyhit: " << latestKeyInput << std::endl
                    << "millHasBeenFormed: " << boardManager.millHasBeenFormed << std::endl
                    << "isDeletingStone: " << this->board_manager.isDeletingStone << std::endl
                    << "isAllowedToMoveAnywhere_Player1: " << this->board_manager.isAllowedToMoveAnywhere_Player1 << std::endl
                    << "isAllowedToMoveAnywhere_Player2: " << this->board_manager.isAllowedToMoveAnywhere_Player2 << std::endl
                    << "hasLegalMoves " << std::to_string(this->board_manager.getCurrentPlayer()) << " : " << hasLegalMoves(boardManager, this->board_manager.getCurrentPlayer()) << std::endl;
}

std::array<int, 2> GameManager::getPlacedStones() {
    int stonesOnBoard_PLAYER1 = 0;
    int stonesOnBoard_PLAYER2 = 0;
    std::array<int, 2> sumStonesOnBoard = {stonesOnBoard_PLAYER1, stonesOnBoard_PLAYER2};

    for (int i = 0; i < this->board_manager.getCells().size(); ++i) {
        if (this->board_manager.getCells()[i] == BoardManager::CellState::PLAYER1) {
            stonesOnBoard_PLAYER1++;
        } else if (this->board_manager.getCells()[i] == BoardManager::CellState::PLAYER2) {
            stonesOnBoard_PLAYER2++;
        }
    }

    sumStonesOnBoard[0] = stonesOnBoard_PLAYER1;
    sumStonesOnBoard[1] = stonesOnBoard_PLAYER2;
    return sumStonesOnBoard;
}

void GameManager::dekrementStoneAfterDelete() {
    if (this->board_manager.getCurrentPlayer() == BoardManager::CellState::PLAYER1) {
        this->getPlacedStones()[0]--;
    } else if (this->board_manager.getCurrentPlayer() == BoardManager::CellState::PLAYER2) {
        this->getPlacedStones()[1]--;
    } else {
    }
}


bool GameManager::isPlayerAllowedToMoveAnywhere(BoardManager::CellState currentPlayer) {
    if (currentPlayer == BoardManager::PLAYER1) {
        if (this->getPlacedStones()[0] <= 3) {
            this->board_manager.isAllowedToMoveAnywhere_Player1 = true;
            return true;
        }
    } else if (currentPlayer == BoardManager::PLAYER2) {
        if (this->getPlacedStones()[1] <= 3) {
            this->board_manager.isAllowedToMoveAnywhere_Player2 = true;
            return true;
        }
    }
        return false;

}

bool GameManager::hasLegalMoves(BoardManager& boardManager, BoardManager::CellState currentPlayer) {
    const auto& cells = boardManager.getCells();

    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i] != currentPlayer) {
            continue;
        }

        for (int neighbor : boardManager.horizontalNeighbors[i]) {
            if (cells[neighbor] == BoardManager::CellState::EMPTY) {
                return true;
            }
        }

        for (int neighbor : boardManager.verticalNeighbors[i]) {
            if (cells[neighbor] == BoardManager::CellState::EMPTY) {
                return true;
            }
        }
    }

    return false;
}


// ---------- Spielbrett und Einstellungen ----------

void GameManager::printBoard(const BoardManager& boardManager, BoardManager::CellState currentPlayer, int currentCell = -1) {
    const std::string player1 = "{ " + iconPlayer1 + " }";
    const std::string player2 = "{ " + iconPlayer2 + " }";
    const std::string empty = "{   }";
    const std::string highlighted = "\033[33m"; // ANSI escape code für rote Schrift
    const std::string beingMoved = "\033[36m"; // ANSI escape code für blaue Schrift
    const std::string currentIsSelected = "\033[33m\033[36m"; // Red text on blue background
    const std::string isDeleting = "\033[41m";
    const std::string cantDeleteOwn = "\033[47m";
    const std::string reset = "\033[0m";       // ANSI escape code für Reset

    auto getCellRepresentation = [&](int index, BoardManager::CellState state) -> std::string {
        std::string cellContent;

        // Setze die Zellenanzeige basierend auf dem Zustand
        switch (state) {
            case BoardManager::CellState::PLAYER1:
                cellContent = player1;
            break;
            case BoardManager::CellState::PLAYER2:
                cellContent = player2;
            break;
            case BoardManager::CellState::EMPTY:
                cellContent = empty;
            break;
            default:
                cellContent = empty;
            break;
        }

        if (index == this->movingStoneFrom && index == currentCell && this->pressedEnter) {
            return currentIsSelected + cellContent + reset;
        } else if (boardManager.millHasBeenFormed && boardManager.getCellState(index) == boardManager.getCurrentPlayer() && index == currentCell) {
            return cantDeleteOwn + cellContent + reset;
        } else if (boardManager.millHasBeenFormed && index == currentCell) {
            return isDeleting + cellContent + reset;
        } else if (index == this->movingStoneFrom) {
            return beingMoved + cellContent + reset;
        } else if (index == currentCell) {
            return highlighted + cellContent + reset;
        }
        return cellContent;
    };



    const auto& cells = boardManager.getCells();

    std::cout << "\033[2J\033[H"; // Bildschirm löschen und Cursor zurücksetzen

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
    std::cout << "\n\n";

    int stonesPlayer1 = boardManager.HasStonesLeft_PLAYER1;
    int stonesPlayer2 = boardManager.HasStonesLeft_PLAYER2;

    std::string player1Stones, player2Stones;
    for (int i = 0; i < stonesPlayer1; ++i) {
        player1Stones += iconPlayer1 + " ";
    }
    for (int i = 0; i < stonesPlayer2; ++i) {
        player2Stones += iconPlayer2 + " ";
    }

    int totalLength = 50;
    int padding = (totalLength - (player1Stones.size() + player2Stones.size() + 4)) / 2;

    std::cout << "\t\t\t\t      " << std::string(padding, ' ') << player1Stones << "|| " << player2Stones << "\n";

    std::cout << "\n\n";



    std::cout << "\t\tSpielphase: " + this->printPhase() + "\t\t **Am Zug: Spieler " << (currentPlayer == 1 ? player2 : player1) << "**\n";
    logger.log(LogLevel::INFO, "GameManager: Player " + this->board_manager.enumToString(currentPlayer) + " " + " is now playing.");
}

void GameManager::handleSettingsMenu() {
    while (true) {
        clearScreen();
        std::cout << "\n--- Einstellungen ---\n";
        std::cout << "1. Spieler 1 Symbol ändern (aktuell: " << this->iconPlayer1 << ")\n";
        std::cout << "2. Spieler 2 Symbol ändern (aktuell: " << this->iconPlayer2 << ")\n";
        std::cout << "3. Zurück zum Hauptmenü\n\n";
        std::cout << "Wähle eine Option: ";

        int userInput = getUserInput(1, 3);
        clearScreen();

        switch (userInput) {
            case 1: {
                std::cout << "Gib das neue Symbol für Spieler 1 ein: ";
                std::string newSymbol;
                std::cin >> newSymbol;

                if (newSymbol == this->iconPlayer2) {
                    std::cout << "Das Symbol darf nicht mit Spieler 2 übereinstimmen!\n";
                } else {
                    this->iconPlayer1 = newSymbol[0];
                    std::cout << "Symbol für Spieler 1 erfolgreich geändert zu: " << this->iconPlayer1 << "\n";
                }
                break;
            }
            case 2: {
                std::cout << "Gib das neue Symbol für Spieler 2 ein: ";
                std::string newSymbol;
                std::cin >> newSymbol;

                if (newSymbol == this->iconPlayer1) {
                    std::cout << "Das Symbol darf nicht mit Spieler 1 übereinstimmen!\n";
                } else {
                    this->iconPlayer2 = newSymbol[0];
                    std::cout << "Symbol für Spieler 2 erfolgreich geändert zu: " << this->iconPlayer2 << "\n";
                }
                break;
            }
            case 3:
                std::cout << "Zurück zum Hauptmenü...\n";
                start();
            default:
                std::cout << "Ungültige Option. Bitte erneut versuchen.\n";
                break;
        }

        std::cout << "\nDrücke Enter, um fortzufahren...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}


void GameManager::saveGame(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Fehler: Kann die Datei '" << filename << "' nicht öffnen.\n";
        return;
    }

    const auto& cells = this->board_manager.getCells();
    for (int i = 0; i < cells.size(); ++i) {
        outFile << i << "," << static_cast<int>(cells[i]) << "\n";
    }
    outFile << "playerStones," << this->board_manager.HasStonesLeft_PLAYER1 << "," << this->board_manager.HasStonesLeft_PLAYER2 << "\n";

    outFile.close();
    std::cout << "Spielstand erfolgreich in '" << filename << "' gespeichert.\n";
}

bool GameManager::isSaveFileAvailable() {
    std::ifstream file("save.muehle");
    return file.is_open();
}

void GameManager::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "Fehler: Kann die Datei '" << filename << "' nicht öffnen.\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string key, value1, value2;

        if (line.find("playerStones") != std::string::npos) {
            if (std::getline(iss, key, ',') &&
                std::getline(iss, value1, ',') &&
                std::getline(iss, value2)) {
                this->board_manager.HasStonesLeft_PLAYER1 = std::stoi(value1);
                this->board_manager.HasStonesLeft_PLAYER2 = std::stoi(value2);
                }
        } else {
            std::string cellIndexStr, cellStateStr;
            if (std::getline(iss, cellIndexStr, ',') && std::getline(iss, cellStateStr)) {
                int cellIndex = std::stoi(cellIndexStr);
                int cellState = std::stoi(cellStateStr);

                if (cellState == static_cast<int>(BoardManager::CellState::PLAYER1)) {
                    this->board_manager.setStone(cellIndex, BoardManager::CellState::PLAYER1);
                } else if (cellState == static_cast<int>(BoardManager::CellState::PLAYER2)) {
                    this->board_manager.setStone(cellIndex, BoardManager::CellState::PLAYER2);
                } else {
                    this->board_manager.setStone(cellIndex, BoardManager::CellState::EMPTY);
                }
            }
        }
    }

    inFile.close();
    std::cout << "Spielstand erfolgreich aus '" << filename << "' geladen.\n";
}


std::string GameManager::printPhase() {
    switch (this->currentPhase){
        case 0:
            return "Setzphase";
        case 1:
            return "Zugphase";
        case 2:
            return "Sprungphase";
    }
    return "Unbekannte Phase";
}
