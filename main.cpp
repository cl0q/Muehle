#include <iostream>
#include <map>
#include <vector>

// Jeder Spieler besitzt zu Beginn 9 Spielsteine
const int MAX_PIECES = 9;

enum Symbols {
    Player1 = 'X',
    Player2 = 'O'
};

const std::vector<std::vector<std::string>> muehlen = {
    {"A1", "D1", "G1"}, {"B2", "D2", "F2"}, {"C3", "D3", "E3"},
    {"A4", "B4", "C4"}, {"E4", "F4", "G4"}, {"C5", "D5", "E5"},
    {"B6", "D6", "F6"}, {"A7", "D7", "G7"}, {"A1", "A4", "A7"},
    {"G1", "G4", "G7"}, {"B2", "B4", "B6"}, {"C3", "C4", "C5"}, 
    {"D1", "D2", "D3"}, {"D5", "D6", "D7"}, {"E3", "E4", "E5"}, 
    {"F2", "F4", "F6"}
};

class Player {
    std::string name;
    char symbol;
    int piecesLeft = MAX_PIECES;
    int piecesPlaced = 0;

    bool placePiece(std::string position);

    public:

    Player(char playerSymbol) {
        symbol = playerSymbol;
        std::cin >> name;
    }

    int getpiecesPlaced() {return piecesPlaced;}
    void decreasePieces() {this->piecesLeft--;}
    void makeMove();
    bool validateMuehle();
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

bool validateInput(std::string input) {
    if (field.find(input) == field.end()) {
        return false;
    } else { 
        return true;
    }
}

std::string getInput() {
    std::string input1;
    std::cin >> input1;

    while (!validateInput(input1)) {
        std::cout << "Invalid Input! Please try again:" << std::endl;
        std::cin >> input1;
    }
    
    return input1;
}

bool Player::placePiece(std::string position)
{
    if (field[position] == '.') {
        field[position] = symbol;
        piecesPlaced++;
        return true;
    } else {
        std::cout << "Move is not possible!" << std::endl;
        return false;
    }
}

void Player::makeMove()
{
    std::cout << "It's " << name << "'s turn" << std::endl;
    std::string choice1 = getInput();
    
    while (!placePiece(choice1)) {
        choice1 = getInput();
    }
    showField();
    validateMuehle();
}

bool Player::validateMuehle()
{
    for (std::vector element : muehlen) {
        if (field[element[0]] == symbol && field[element[1]] == symbol && field[element[2]] == symbol) {
            std::cout << "Spieler " << name << " scored a Mühle!" << std::endl
            << "Which piece do you want to remove?" << std::endl;

            std::string piece;
            std::cin >> piece;

            char otherPlayer;
            
            if (symbol == Player1) {
                otherPlayer = Player2;
            } else {
                otherPlayer = Player1;
            }

            while (!validateInput(piece) || field[piece] != otherPlayer) {
                std::cout << "Your choice is not possible please try again: " << std::endl;
                std::cin >> piece;
            }

            field[piece] = '.';
            // To Do: piecesLeft des anderen Spielers um 1 verringern

            return true;
        }
    }
    return false;
}

int main() {
    std::cout << "Enter Name for Player 1: " << std::endl;
    Player p1(Player1);

    std::cout << "Enter Name for Player 2: " << std::endl;
    Player p2(Player2);

    showField();

    // Phase 1 Beginn
    while (p1.getpiecesPlaced() < 3 || p2.getpiecesPlaced() < 3)
    {
        p1.makeMove();
        std::cout << std::endl;
        p2.makeMove();
    } 
}