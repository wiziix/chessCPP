#ifndef BOARD_H
#define BOARD_H
#include <map>
#include<vector>

class Piece;

enum Type;

enum Color;

class Board {
public:
	Board();
	~Board();
	void printBoard(bool isWhiteTurn);
	bool isPathClear(std::pair<int, int> start, std::pair<int, int> end);
	Piece* getPiece(int x, int y);
	void movePiece(std::pair<int, int> start, std::pair<int, int> end);
	void undoMove(std::pair<int, int> firstMove, std::pair<int, int> lastMove);
	void promotePawn(std::pair<int, int> position, Type *newPiece, Color *color);
	void setNull(std::pair<int, int> position);
	void Castle(std::pair<int, int> start, std::pair<int, int> end);
private:
	std::vector<std::vector<Piece*>> board;
};

#endif