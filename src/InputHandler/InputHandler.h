//
// Created by Oliver Ilczuk on 14.01.25.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../BoardManager/BoardManager.h"
#include "../Player/Player.h"

class InputHandler {
public:
    InputHandler(BoardManager* boardManager);

    // Startet die interaktive Eingabe
    void startInteractiveInput(Player* currentPlayer);

private:
    BoardManager* boardManager; // Referenz auf das Spielfeld
    int currentPosition;        // Aktuelle Position des Cursors
    int selectedPosition;       // Position eines selektierten Steins (-1, wenn kein Stein ausgewählt)
    bool isMovingStone;         // Status, ob ein Stein bewegt wird

    // Zeichnet das Spielfeld mit Hervorhebung
    void drawBoard();

    // Verarbeitet Benutzeraktionen (Pfeiltasten, Space, Enter)
    void handleInput(int key, Player* currentPlayer);

    // Highlight für die aktuelle Zelle
    void highlightCell(int position, const std::string& color);

    // Überprüft, ob ein Zug valide ist
    bool isValidMove(int from, int to);
};

#endif // INPUTHANDLER_H
