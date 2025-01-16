#include <iostream>
#include "BoardManager/BoardManager.h"
#include "Logger/Logger.h"
#include "GameManager/GameManager.h"
#include "Player/Player.h"

// Globale Logger-Instanz
// Logger logger(LogLevel::ERROR, "game.log");

int main() {
    // Instanziiere Manager
    Player* player1 = new Player(1, 'X');
    Player* player2 = new Player(2, 'O');

    BoardManager* boardManager = new BoardManager;
    
    
    RuleEngine* ruleEngine = new RuleEngine{boardManager, player1, player2};
    GameManager gameManager(player1, player2, ruleEngine, boardManager);

    // Terminal leeren und Hauptmenü anzeigen

    if (player1 == nullptr || player2 == nullptr || boardManager == nullptr || ruleEngine == nullptr) {
        std::cout << "Alarm hier ist ein nullpointer!!!" << std::endl;
    }

    std::cout << "Game starts:" << std::endl;
    gameManager.run();

    return 0;
}