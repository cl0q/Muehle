//
// Created by Oliver Ilczuk on 14.01.25.
//

#include "InputHandler.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <algorithm>

// ANSI-Farben
#define RESET "\033[0m"
#define HIGHLIGHT "\033[43m"
#define SELECTED "\033[42m"

// Funktion zum Einlesen von Pfeiltasten (ANSI-Codes)
int getArrowKey() {
    struct termios oldt, newt;
    int key;

    // Terminal in den Rohmodus schalten
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Lesen der Eingabe
    key = getchar();
    if (key == '\033') { // Escape-Sequenz
        getchar();       // '[' ignorieren
        key = getchar(); // Richtungskey
    }

    // Terminal zurücksetzen
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return key;
}

InputHandler::InputHandler(BoardManager* boardManager)
    : boardManager(boardManager), currentPosition(0), selectedPosition(-1), isMovingStone(false) {}

// Startet die interaktive Eingabe
void InputHandler::startInteractiveInput(Player* currentPlayer) {
    int key;
    drawBoard();

    while (true) {
        key = getArrowKey(); // Liest die Pfeiltaste
        if (key == 'q') break; // Beenden

        handleInput(key, currentPlayer);
        drawBoard();
    }
}

// Zeichnet das Spielfeld
void InputHandler::drawBoard() {
    std::cout << "\033[2J\033[H"; // Clear Screen und Cursor zurücksetzen

    const auto& cells = boardManager->getCells();

    auto getCellRepresentation = [&](int index) -> std::string {
        std::string cellContent;
        BoardManager::CellState state = cells[index];

        // Hervorhebung je nach Status
        if (index == currentPosition) {
            cellContent = HIGHLIGHT;
        } else if (index == selectedPosition) {
            cellContent = SELECTED;
        }

        // Zelleninhalt bestimmen
        switch (state) {
            case BoardManager::CellState::PLAYER1:
                cellContent += "{ X }";
                break;
            case BoardManager::CellState::PLAYER2:
                cellContent += "{ O }";
                break;
            case BoardManager::CellState::EMPTY:
                cellContent += "{   }";
                break;
            default:
                cellContent += "{   }";
                break;
        }

        cellContent += RESET; // ANSI-Farben zurücksetzen
        return cellContent;
    };

    // Spielfeld zeichnen mit unverändertem Layout
    std::cout << "\n";
    std::cout << "				  " << getCellRepresentation(0) << "--------------------" << getCellRepresentation(1) << "--------------------" << getCellRepresentation(2) << "\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |      " << getCellRepresentation(3) << "-----------" << getCellRepresentation(4) << "-----------" << getCellRepresentation(5) << "      |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(6) << "---" << getCellRepresentation(7) << "---" << getCellRepresentation(8) << "     |        |\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				  " << getCellRepresentation(9) << "    " << getCellRepresentation(10) << "   " << getCellRepresentation(11) << "           " << getCellRepresentation(12) << "   " << getCellRepresentation(13) << "    " << getCellRepresentation(14) << "\n";
    std::cout << "				    |        |       |               |       |        |\n";
    std::cout << "				    |        |     " << getCellRepresentation(15) << "---" << getCellRepresentation(16) << "---" << getCellRepresentation(17) << "     |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |        |               |               |        |\n";
    std::cout << "				    |      " << getCellRepresentation(18) << "-----------" << getCellRepresentation(19) << "-----------" << getCellRepresentation(20) << "      |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				    |                        |                        |\n";
    std::cout << "				  " << getCellRepresentation(21) << "--------------------" << getCellRepresentation(22) << "--------------------" << getCellRepresentation(23) << "\n";
    std::cout << "\n\n\n";
}

// Highlightet eine Zelle mit der angegebenen Farbe
void InputHandler::highlightCell(int position, const std::string& color) {
    const auto& cells = boardManager->getCells();
    std::cout << color << "{" << cells[position] << "}" << RESET << " ";
}

// Verarbeitet Benutzereingaben
void InputHandler::handleInput(int key, Player* currentPlayer) {
    const auto& neighbors = boardManager->getNeighbors(currentPosition);

    int newPosition = currentPosition;
    if (key == 'A') { // Pfeil nach oben
        for (int neighbor : neighbors) {
            if (neighbor < currentPosition) {
                newPosition = neighbor;
                break;
            }
        }
    } else if (key == 'B') { // Pfeil nach unten
        for (int neighbor : neighbors) {
            if (neighbor > currentPosition) {
                newPosition = neighbor;
                break;
            }
        }
    } else if (key == 'C') { // Pfeil nach rechts
        for (int neighbor : neighbors) {
            if (neighbor > currentPosition) {
                newPosition = neighbor;
                break;
            }
        }
    } else if (key == 'D') { // Pfeil nach links
        for (int neighbor : neighbors) {
            if (neighbor < currentPosition) {
                newPosition = neighbor;
                break;
            }
        }
    } else if (key == ' ') { // Space: Stein setzen
        if (boardManager->getCellState(currentPosition) == BoardManager::CellState::EMPTY) {
            boardManager->setStone(currentPosition, currentPlayer);
        } else {
            std::cout << "Warning: Invalid action. Cannot place a stone here.\n";
        }
    } else if (key == '\r') { // Enter: Stein bewegen
        if (!isMovingStone) {
            if (boardManager->getCellState(currentPosition) == currentPlayer->symbol) {
                isMovingStone = true;
                selectedPosition = currentPosition;
            } else {
                std::cout << "Warning: Invalid action. Not your stone.\n";
            }
        } else {
            if (isValidMove(selectedPosition, currentPosition)) {
                boardManager->moveStone(selectedPosition, currentPosition);
                isMovingStone = false;
                selectedPosition = -1;
            } else {
                std::cout << "Warning: Invalid move.\n";
            }
        }
    }

    if (isValidMove(currentPosition, newPosition)) {
        currentPosition = newPosition;
    }
}

// Prüft, ob eine Bewegung valide ist
bool InputHandler::isValidMove(int from, int to) {
    const auto& neighbors = boardManager->getNeighbors(from);
    return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}
