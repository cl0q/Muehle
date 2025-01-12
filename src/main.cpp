#include <iostream>
#include "./BoardManager/BoardManager.h"
#include "./Player/Player.h"
#include "./RuleEngine/RuleEngine.h"

int main() {
// Kleiner Test
    Player* p1 = new Player {1, 'X'};
    Player* p2 = new Player {2, 'O'};
    
    BoardManager* bm = new BoardManager();
    RuleEngine re {bm, p1, p2};

    for (int i = 0; i < 9; i++) {
        std::cout << p1->name << "'s turn. Select a position" << std::endl;
        int position;
        std::cin >> position;

        while (!bm->setStone(position, p1)) {
            std::cout << "Selection is invalid, please try again!" << std::endl;
            std::cin >> position;
        }

        if (re.isMillFormed(position, p1->symbol)) {
            std::cout << "Player 1 hat eine Muehle geformt!" << std::endl;
        }

        std::cout << p2->name << "'s turn. Select a position" << std::endl;
        std::cin >> position;

        while (!bm->setStone(position, p2)) {
            std::cout << "Selection is invalid, please try again!" << std::endl;
            std::cin >> position;
            std::cout << "Invalid Argument Exception was thrown. Try Again:" << std::endl;
        }

        if (re.isMillFormed(position, p2->symbol)) {
            std::cout << "Player 2 hat eine Muehle geformt!";
        }
    }
    std::cout << "Phase 1 beendet!" << std::endl;

    delete p1;
    delete p2;
    delete bm;
}