#include <iostream>
#include <map>

// Jeder Spieler besitzt zu Beginn 9 Spielsteine
const int MAX_PIECES = 9;

class Player {
    char symbol;
    int piecesLeft = MAX_PIECES;
    int piecesPlaced = 0;

    public:
    Player(char playerSymbol) {
        symbol = playerSymbol;
    }

    void decreasePieces() {
        this->piecesLeft--;
    }

    void placePiece(std::string position);
};

std::map<std::string, char> field = {
    {"A1", '.'}, {"D1", '.'}, {"G1", '.'},
    {"B2", '.'}, {"D2", '.'}, {"F2", '.'},
    {"C3", '.'}, {"D3", '.'}, {"E3", '.'},
    {"A4", '.'}, {"B4", '.'}, {"C4", '.'},
    {"E4", '.'}, {"F4", '.'}, {"G4", '.'},
    {"C5", '.'}, {"D5", '.'}, {"E5", '.'},
    {"B6", '.'}, {"D6", '.'}, {"F6", '.'},
    {"A7", '.'}, {"D7", '.'}, {"G7", '.'}
};

void Player::placePiece(std::string position)
{
    field[position] = symbol;
    piecesPlaced++;
}

void showField() {
    std::cout << field["A1"] << "-----------" << field["D1"] << "-----------" << field["G1"] << std::endl;
    std::cout << "|           |           |" << std::endl;
    std::cout << "|   " << field["B2"] << "-------" << field["D2"] << "-------" << field["F2"] << "   |" << std::endl;
    std::cout << "|   |       |       |   |" << std::endl;
    std::cout << "|   |   " << field["C3"] << "---" << field["D3"] << "---" << field["E3"] << "   |   |" << std::endl;
    std::cout << field["A4"] << "---" << field["B4"] << "---" << field["C4"] << "       " << field["E4"] << "---" << field["F4"] << "---" << field["G4"] << std::endl;
    std::cout << "|   |   " << field["C5"] << "---" << field["D5"] << "---" << field["E5"] << "   |   |" << std::endl;
    std::cout << "|   " << field["B6"] << "-------" << field["D6"] << "-------" << field["F6"] << "   |" << std::endl;
    std::cout << "|           |           |" << std::endl;
    std::cout << field["A7"] << "-----------" << field["D7"] << "-----------" << field["G7"] << std::endl;
}

int main() {
    Player p1('X');
    p1.placePiece("A1");
    showField();
}