#include "GameManager.h"

bool isNumber(std::string input) {
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

int GameManager::getUserInput(int min, int max, Player* p, bool isPhaseOne, bool isFirstSelection) {
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
            std::cout << "Invalid Input. Please enter a number between " << min << " and " << max << "!" << std::endl;
            continue;
        }

            if (isPhaseOne) {
                if (boardManager->cells[position] != '.') {
                    std::cout << "Invalid Input. Choose an empty Field!" << std::endl;
                    continue;
                }
            }

            if (isFirstSelection) {
                if (boardManager->cells[position] != p->symbol) {
                    std::cout << "Invalid Input. Choose a field you occupied!" << std::endl;
                    continue;
                }
            } else if (boardManager->cells[position] != '.') {
                std::cout << "Invalid Input. Choose a free field!" << std::endl;
                continue;
            }
        break;
    }
    
    return position;
}

void GameManager::runPhaseOne() {
    for (int i = 0; i < 9; i++) {
        boardManager->displayBoard();
        std::cout << "Player " << p1->name << " 's turn. " << std::endl;
        int choice = getUserInput(0, 23, p1, true, false);
        boardManager->setStone(choice, p1);

        if (ruleEngine->isMillFormed(choice, p1->symbol)) {
            destroyStone(p1, p2);
            ruleEngine->canPlayerJump(p2);
        }
        
        boardManager->displayBoard();
        std::cout << "Player " << p2->name << " 's turn." << std::endl;
        choice = getUserInput(0, 23, p2, true, false);
        boardManager->setStone(choice, p2);

        if (ruleEngine->isMillFormed(choice, p2->symbol)) {
            destroyStone(p2, p1);
            ruleEngine->canPlayerJump(p1);
        }
    }
}

void GameManager::runPhaseTwo(Player* p) {
    std::cout << p->name << p->getTotalStones() << p->canJump;
    if (ruleEngine->isAbleToMove(p)) {
        std::cout << "Player " << p->name << " 's turn. Which Stone should move: " << std::endl;
        int from;
        do
        {
            from = getUserInput(0, 23, p, false, true);

        } while (!ruleEngine->canStoneMove(from, p));

        std::cout << "Where do you wanna place it:" << std::endl;
        int to;
        do
        {
            to = getUserInput(0, 23, p, false, false);
        } while (!boardManager->isValidMove(from, to, p));
        
        if (p->canJump) {
            boardManager->jumpwithStone(from, to, p);
        } else {
            boardManager->moveStone(from, to, p);
        }

        if (ruleEngine->isMillFormed(to, p->symbol)) {
            Player* otherPlayer;
            if (p == p1) {
                otherPlayer = p2;
            } else {
                otherPlayer = p1;
            }
            destroyStone(p, otherPlayer);
            ruleEngine->canPlayerJump(p2);
        }
    } else {std::cout << p->name << " cant move";}
}

void GameManager::runPhaseThree() {
    
}

void GameManager::run() {
    runPhaseOne();
    bool isRunning = true;
    std::cout << "Now Entering Phase Two" << std::endl;
    while(isRunning) {
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
    }
}

void GameManager::saveGame() {

}

void GameManager::loadGame() {

}

int GameManager::getDestroyInput(int min, int max, Player* victim) {
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
            std::cout << "Invalid Input. Please enter a number between " << min << " and " << max << "!" << std::endl;
            continue;
        }

        if (boardManager->cells[position] != victim->symbol || !ruleEngine->isAllowedToKill(position)) {
            std::cout << victim->symbol << " und " << boardManager->cells[position] << std::endl;
            bool kill = ruleEngine->isAllowedToKill(position);
            std::cout << kill << std::endl;
            continue;

        }

        break;
    }
    return position;
}

void GameManager::destroyStone(Player* killer, Player* victim)
{
    std::cout << "You scored a Mill!" << std::endl;
    std::cout << killer->name << ", which Player do you want to destroy?" << std::endl;

    int choice;
    choice = getDestroyInput(0, 23, victim);
    boardManager->removeStone(choice, victim);
}