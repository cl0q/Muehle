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
        std::string position;
        std::cin >> position;
        bm->setStone(std::stoi(position), p1);

        std::cout << p2->name << "'s turn. Select a position" << std::endl;
        std::string selection;
        std::cin >> selection;
        bm->setStone(std::stoi(selection), p2);
    }
    std::cout << "Phase 1 beendet!" << std::endl;
}