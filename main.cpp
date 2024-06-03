#include <iostream>
#include <sstream>
#include "Game.h"
#include "Pieces.h"

int main() {
    bool controller = false;
    bool hasGameStarted = false;
    Color color = WHITE;
    Color color2 = BLACK;
    Game game; 
    while (!controller) {

        if (hasGameStarted) {
            if (game.isCheckmate(&color)) {
                std::cout << "Game over. Black won." << std::endl;
                controller = true;
                break;
            }
            else if (game.isCheckmate(&color2)) {
                std::cout << "Game over. White won." << std::endl;
                controller = true;
                break;
            }

            if (game.isStalemate(&color) || game.isStalemate(&color2)) {
                std::cout << "Game over. Stalemate." << std::endl;
                controller = true;
                break;
            }
        }

        std::cout << "Enter command (N to start new game, M to make move, U to undo move, S to save game, L to load game, Q to exit): ";
        char command;
        std::cin >> command;

        switch (command) {
        case 'N': {
            std::string p1Name, p2Name;
            std::cout << "Enter Player 1's name: " << std::endl;
            std::cin >> p1Name;
            std::cout << "Enter Player 2's name: " << std::endl;
            std::cin >> p2Name;

            game.startNewGame(p1Name, p2Name);
            hasGameStarted = true;
            break;
        }
        case 'M':
            if (!hasGameStarted) {

                std::cout << "Invalid command." << std::endl;
            }
            else {
                game.makeMove(); 
            }
            break;
        case 'U': {
            if (!hasGameStarted) {
                std::cout << "Invalid command." << std::endl;
            }
            else {
                game.undoMakeMove(); 
            }
            break;
        }
        case 'S': {
            std::string fileName;
            std::cout << "Enter File's name: " << std::endl;
            std::cin >> fileName;

            game.saveGame(fileName);
            break;
        }
        case 'L': {
            break;
        }
        case 'Q':
            controller = true;
            break;
        default:
            std::cout << "Invalid command." << std::endl;
        }
    }
}