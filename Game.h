#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <deque>

enum Color;

struct Round {
	int number = 1;
	std::string move;

};
class Player;

class Board;

class Game {
public:
	~Game();
	void makeMove();
	void undoMakeMove();
	void saveGame(std::string fileName);
	void startNewGame(std::string p1_name, std::string p2_name);
	std::pair<int, int> findKing(Color *color);
	bool isKingChecked(Color *color);
	bool isStalemate(Color *color);
	bool isCheckmate(Color* color);
	bool isCastlingPossible(std::pair<int, int> start, std::pair<int, int> end);
	static std::deque<Round*> getRounds();
	bool canPromotePawn(std::pair<int, int> end);
	void checkGameState(Color *color);
private:
	static std::deque<Round*> rounds;
	Board *board;
	Round *rnd;
	Player* p1;
	Player* p2;
	bool isWhiteTurn = true;
};
#endif