#include "Pieces.h"
#include "Board.h"
#include "Game.h"
#include <iostream>
#include <sstream>

Piece::Piece(Color color, Type type) : type(type), color(color) {}

Piece::~Piece() = default;

void Piece::setPosition(std::pair<int, int> positon) {
	this->position = position;
}

std::pair<int, int> Piece::getPosition() {
	return this->position;
}

void Piece::setColor(Color color) {
	this->color = color;
}

Color Piece::getColor() {
	return this->color;
}

void Piece::setType(Type type) {
	this->type = type;
}

Type Piece::getType() {
	return this->type;
}

Pawn::Pawn(Color color) : Piece(color, PAWN) { }

Pawn::~Pawn() { }

bool Pawn::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int new_x = ending_position.first;
	int new_y = ending_position.second;

	int x = starting_position.first;
	int y = starting_position.second;

	int direction = this->getColor() == WHITE ? 1 : -1;

	if (new_y == y && new_x == x + direction) {
		if (board.getPiece(new_x, new_y) == nullptr) {
			
			return true;
		}
	}

	if ((x == 1 || x == 6) && new_x == x + 2 * direction) {
		if (board.getPiece(new_x, new_y) == nullptr) {
			return true;
		}
	}

	if (abs(new_y - y) == 1 && new_x == x + direction) {
		if (board.getPiece(new_x, new_y) != nullptr && board.getPiece(new_x, new_y)->getColor() != this->getColor()) {
			return true;
		}
	}

	if (isEnPassantPossible(starting_position, ending_position, board)) {
		return true;
	}

	return false;
}

bool Pawn::isEnPassantPossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int new_x = ending_position.first;
	int new_y = ending_position.second;

	int x = starting_position.first;
	int y = starting_position.second;

	int direction = this->getColor() == WHITE ? 1 : -1;

	Round *lastRound = Game::getRounds().back();

	std::pair<int, int> firstMove, lastMove;

	std::stringstream ss(lastRound->move);
	std::string startMove, endMove;

	ss >> startMove;
	ss >> endMove;

	firstMove.first = startMove[1] - '0' - 1;
	firstMove.second = startMove[0] - 'a';

	lastMove.first = endMove[1] - '0' - 1;
	lastMove.second = endMove[0] - 'a';

	if (board.getPiece(lastMove.first, lastMove.second) != nullptr &&
		board.getPiece(lastMove.first, lastMove.second)->getType() == PAWN) {
		if (abs(firstMove.first - lastMove.first) == 2 && lastMove.first == x && lastMove.second == new_y) {
			board.setNull(lastMove);
			return true;
		}
	}
	return false;
}

char Pawn::getSymbol() {
	return this->getColor() == WHITE ? 'P' : 'p';
}

Rook::Rook(Color color) : Piece(color, ROOK) { }

Rook::~Rook() { }

bool Rook::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int new_x = ending_position.first;
	int new_y = ending_position.second;

	int x = starting_position.first;
	int y = starting_position.second;

	if (new_x == x && new_y != y) {
		if (board.getPiece(new_x, new_y) == nullptr || board.getPiece(new_x, new_y)->getColor() != this->getColor()) {
			return true;
		}
	}

	if (new_y == y && new_x != x) {
		if (board.getPiece(new_x, new_y) == nullptr || board.getPiece(new_x, new_y)->getColor() != this->getColor()) {
			return true;
		}
	}

	return false;
}

char Rook::getSymbol() {
	return this->getColor() == WHITE ? 'R' : 'r';
}

void Rook::setHasMoved(bool moved) {
	hasMoved = moved;
}

bool Rook::getHasMoved() {
	return hasMoved;
}

Knight::Knight(Color color) : Piece(color, KNIGHT) { }

Knight::~Knight() { }

bool Knight::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int x_diff = abs(ending_position.first - starting_position.first);
	int y_diff = abs(ending_position.second - starting_position.second);

	if ((x_diff == 1 && y_diff == 2) || (x_diff == 2 && y_diff == 1)) {
		if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
			board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
			return true;
		}	
	}

	return false;
}

char Knight::getSymbol() {
	return this->getColor() == WHITE ? 'N' : 'n';
}

Bishop::Bishop(Color color) : Piece(color, BISHOP) { }

Bishop::~Bishop() { }

bool Bishop::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int x_diff = abs(ending_position.first - starting_position.first);
	int y_diff = abs(ending_position.second - starting_position.second);
	
	if (x_diff == y_diff) {
		if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
			board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
			return true;
		}
	}

	return false;
}

char Bishop::getSymbol() {
	return this->getColor() == WHITE ? 'B' : 'b';
}

Queen::Queen(Color color) : Piece(color, QUEEN) { }

Queen::~Queen() { }

bool Queen::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
		int new_x = ending_position.first;
		int new_y = ending_position.second;

		int x = starting_position.first;
		int y = starting_position.second;

		int x_diff = abs(new_x - x);
		int y_diff = abs(new_y - y);

		if (x_diff == y_diff) {
			if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
				board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
				return true;
			}
		}

		if (new_x == x && new_y != y) {
			if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
				board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
				return true;
			}
		}

		if (new_y == y && new_x != x) {
			if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
				board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
				return true;
			}
		}

		return false;
}

char Queen::getSymbol() {
	return this->getColor() == WHITE ? 'Q' : 'q';
}

King::King(Color color) : Piece(color, KING) { }

King::~King() { }

bool King::isMovePossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board) {
	int new_x = ending_position.first;
	int new_y = ending_position.second;

	int x = starting_position.first;
	int y = starting_position.second;

	int x_diff = abs(new_x - x);
	int y_diff = abs(new_y - y);

	if ((x_diff <= 1) && (y_diff <= 1)) {
		if (board.getPiece(ending_position.first, ending_position.second) == nullptr ||
			board.getPiece(ending_position.first, ending_position.second)->getColor() != this->getColor()) {
			return true;
		}
	}

	return false;
}

char King::getSymbol() {
	return this->getColor() == WHITE ? 'K' : 'k';
}

void King::setHasMoved(bool moved) {
	hasMoved = moved;
}

bool King::getHasMoved() {
	return hasMoved;
}