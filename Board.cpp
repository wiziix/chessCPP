#include "Board.h"
#include "Pieces.h"
#include <iostream>
#include <vector>

Board::Board() {
	board.resize(8, std::vector<Piece*>(8, nullptr));

    //Placing white pawns
    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(WHITE);
        board[1][i]->setPosition({ 1, i });
    }

    //Placing white pieces
    board[0][0] = new Rook(WHITE);
    board[0][0]->setPosition({ 0, 0 });
    board[0][1] = new Knight(WHITE);
    board[0][1]->setPosition({ 0, 1 });
    board[0][2] = new Bishop(WHITE);
    board[0][2]->setPosition({ 0, 2 });
    board[0][3] = new Queen(WHITE);
    board[0][3]->setPosition({ 0, 3 });
    board[0][4] = new King(WHITE);
    board[0][4]->setPosition({ 0, 4 });
    board[0][5] = new Bishop(WHITE);
    board[0][5]->setPosition({ 0, 5 });
    board[0][6] = new Knight(WHITE);
    board[0][6]->setPosition({ 0, 6 });
    board[0][7] = new Rook(WHITE);
    board[0][7]->setPosition({ 0, 7 });

    //Placing black pawns
    for (int i = 0; i < 8; i++) {
        board[6][i] = new Pawn(BLACK);
        board[6][i]->setPosition({ 1, i });
    }

    //Placing black pieces
    board[7][0] = new Rook(BLACK);
    board[7][0]->setPosition({ 7, 0 });
    board[7][1] = new Knight(BLACK);
    board[7][1]->setPosition({ 7, 1 });
    board[7][2] = new Bishop(BLACK);
    board[7][2]->setPosition({ 7, 2 });
    board[7][3] = new Queen(BLACK);
    board[7][3]->setPosition({ 7, 3 });
    board[7][4] = new King(BLACK);
    board[7][4]->setPosition({ 7, 4 });
    board[7][5] = new Bishop(BLACK);
    board[7][5]->setPosition({ 7, 5 });
    board[7][6] = new Knight(BLACK);
    board[7][6]->setPosition({ 7, 6 });
    board[7][7] = new Rook(BLACK);
    board[7][7]->setPosition({ 7, 7 });
}


Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
            board[i][j] = nullptr;
        }
    }
}

void Board::printBoard(bool isWhiteTurn) { 
    std::cout << "   a b c d e f g h" << std::endl;
    std::cout << std::endl;
   if (!isWhiteTurn) {
        
        std::cout << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << i + 1 << " " << " ";
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == nullptr) {
                    std::cout << '#' << " ";
                }
                else {
                    std::cout << board[i][j]->getSymbol() << " ";
                }              
            }
            std::cout << std::endl;
        }
    }
    else {
        for (int i = 7; i >= 0; i--) {
            std::cout << i + 1 << " " << " ";
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == nullptr) {
                    std::cout << '#' << " ";
                }
                else {
                    std::cout << board[i][j]->getSymbol() << " ";
                }
            }
            std::cout << std::endl;
        }
    }  
}

bool Board::isPathClear(std::pair<int, int> start, std::pair<int, int> end) {
    Piece* piece = board[start.first][start.second];
    Type type = piece->getType();

    switch (type) {
    case 1:
        return true;
        break;
    case 2:
        // Check if the move is vertical
        if (start.first == end.first) {
            int step = (start.second < end.second) ? 1 : -1;
            for (int i = start.second + step; i != end.second; i += step) {
                if (board[start.first][i] != nullptr) {
                    return false;
                }
            }
        }
        // Check if the move is horizontal
        else if (start.second == end.second) {
            int step = (start.first < end.first) ? 1 : -1;
            for (int i = start.first + step; i != end.first; i += step) {
                if (board[i][start.second] != nullptr) {
                    return false;
                }
            }
        }
        else {
            return false;
        }
        return true;
    case 3:
        return true;
        break;
    case 4: {
        int x_diff = abs(end.first - start.first);
        int y_diff = abs(end.second - start.second);

        if (x_diff == y_diff) {
            int x_step = (start.first < end.first) ? 1 : -1;
            int y_step = (start.second < end.second) ? 1 : -1;
            int i = start.first + x_step;
            int j = start.second + y_step;

            while (i != end.first) {
                if (board[i][j] != nullptr) {
                    return false;
                }
                i += x_step;
                j += y_step;
            }
        }
        return true;
        break;
    }
    case 5: {
        int x_diff = abs(end.first - start.first);
        int y_diff = abs(end.second - start.second);

        if (start.first == end.first) {
            int step = (start.second < end.second) ? 1 : -1;
            for (int i = start.second + step; i < end.second; i += step) {
                if (board[start.first][i] != nullptr) {
                    return false;
                }
            }
        }

        if (start.second == end.second) {
            int step = (start.first < end.first) ? 1 : -1;
            for (int i = start.first + step; i < end.first; i += step) {
                if (board[i][start.second] != nullptr) {
                    return false;
                }
            }
        }

        if (x_diff == y_diff) {
            int x_step = (start.first < end.first) ? 1 : -1;
            int y_step = (start.second < end.second) ? 1 : -1;
            int i = start.first + x_step;
            int j = start.second + y_step;

            while (i != end.first) {
                if (board[i][j] != nullptr) {
                    return false;
                }
                i += x_step;
                j += y_step;
            }
        }
        return true;
        break;
    }
    case 6:
        return true;
        break;
    }
    
    return true;
}

void Board::movePiece(std::pair<int ,int> start, std::pair<int, int> end) {

    board[end.first][end.second] = getPiece(start.first, start.second);
    board[start.first][start.second] = nullptr;
}

void Board::undoMove(std::pair<int, int> firstMove, std::pair<int, int> lastMove) {
    movePiece(lastMove, firstMove);
}

Piece* Board::getPiece(int x, int y) {
    return board[x][y];
}

void Board::promotePawn(std::pair<int, int> position, Type* newPieceType, Color* color) {
    Piece* newPiece = getPiece(position.first, position.second);

    switch (*newPieceType) {
    case 5:
        board[position.first][position.second] = new Queen(*color);
        break;
    case 4:
        board[position.first][position.second] = new Bishop(*color);
        break;
    case 3:
        board[position.first][position.second] = new Knight(*color);
        break;
    case 2:
        board[position.first][position.second] = new Rook(*color);
        break;
    default:
        std::cout << "Invalid piece type" << std::endl;
    }
    return;
}

void Board::setNull(std::pair<int, int> position) {
    board[position.first][position.second] = nullptr;
}

void Board::Castle(std::pair<int, int> start, std::pair<int, int> end) {
    int kingDirection = (end.second > start.second) ? 2 : -2;

    board[end.first][start.second + kingDirection] = getPiece(start.first, start.second);
    board[start.first][start.second] = nullptr;


    int direction = (end.second > start.second) ? 1 : -1;
    board[end.first][start.second  + direction] = getPiece(end.first, end.second);
    board[end.first][end.second] = nullptr;
}