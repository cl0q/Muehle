#include "GameManager.h"

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

        if (position < 0 || position >= boardManager->cells.size()) {
            std::cout << "Invalid Input. Please enter a number between 0 and 23!" << std::endl;
            continue;
        }

        break;
    }
    
    return position;
}

int GameManager::checkedSetStone(Player* p) {
    while (true) {
        int choice = getUserInput(0, 23);
        if (boardManager->setStone(choice, p)) {
            return choice;
        } else {
            std::cout << "Invalid input, try again!" << std::endl;
        }
    }
}

void GameManager::runPhaseOne() {
    for (int i = 0; i < 9; i++) {
        boardManager->displayBoard();
        std::cout << "Player " << p1->name << " 's turn. " << std::endl;
        int choice = checkedSetStone(p1);

        if (ruleEngine->isMillFormed(choice, p1->symbol)) {
            std::cout << "You formed a mill!" << std::endl;
            destroyStone(p1, p2);
            ruleEngine->canPlayerJump(p2);
        }
        
        boardManager->displayBoard();
        std::cout << "Player " << p2->name << " 's turn." << std::endl;
        choice = checkedSetStone(p2);

        if (ruleEngine->isMillFormed(choice, p2->symbol)) {
            destroyStone(p2, p1);
            ruleEngine->canPlayerJump(p1);
        }

    }
}

void GameManager::runPhaseTwo(Player* p) {
    std::cout << "Player " << p->name << " 's turn. Which Stone should move: " << std::endl;
    int from = getUserInput(0, 23);
    std::cout << "Where do you wanna place it:" << std::endl;
    int to = getUserInput(0, 23);
    boardManager->moveStone(from, to, p);
    if (ruleEngine->isMillFormed(to, p->symbol)) {
            destroyStone(p1, p2);
            ruleEngine->canPlayerJump(p2);
    }
}

void GameManager::runPhaseThree() {
    
}

void GameManager::run() {
    runPhaseOne();
    int test = 10;
    while(test > 0) {
        boardManager->displayBoard();
        runPhaseTwo(p1);

        if (ruleEngine->isGameOver()) {
            break;
        }
        boardManager->displayBoard();
        runPhaseTwo(p2);

        if (ruleEngine->isGameOver()) {
            isRunning = false;
        }
        test--;
    }
}

void GameManager::saveGame() {

}

void GameManager::loadGame() {

}

void GameManager::destroyStone(Player* killer, Player* victim)
{
    std::cout << killer->name << ", which Player do you want to destroy?" << std::endl;

    int choice;
    do {
        choice = getUserInput(0, 23);
        if (boardManager->cells[choice] != victim->symbol || !ruleEngine->isAllowedToKill(choice)) {
            std::cout << "Invalid input, please try again!" << std::endl;
        }
    } while (boardManager->cells[choice] != victim->symbol || !ruleEngine->isAllowedToKill(choice));

    boardManager->removeStone(choice, victim);

}