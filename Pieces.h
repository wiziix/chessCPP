#ifndef PIECE_H
#define PIECE_H

#include <vector>

enum Color {
	NONE,
	WHITE,
	BLACK
};

enum Type {
	EMPTY,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

class Board;

class Piece {
public:
	Piece(Color color, Type type);
	virtual ~Piece();
	virtual bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board &board) = 0;
	void setPosition(std::pair<int, int> positon);
	std::pair<int, int> getPosition();
	void setColor(Color color);
	Color getColor();
	void setType(Type type);
	Type getType();
	virtual char getSymbol() = 0;

private:
	Type type;
	Color color;
	std::pair<int, int> position;
};

class Pawn : public Piece {
public:
	Pawn(Color color);
	~Pawn();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board &board) override;
	bool isEnPassantPossible(std::pair<int, int> starting_position, std::pair<int, int> ending_position, Board& board);
	char getSymbol() override;
};

class Rook : public Piece {
public:
	Rook(Color color);
	~Rook();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board& board) override;
	char getSymbol() override;
	void setHasMoved(bool moved);
	bool getHasMoved();
private:
	bool hasMoved = false;
};

class Knight : public Piece {
public:
	Knight(Color color);
	~Knight();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board& board) override;
	char getSymbol() override;
};

class Bishop : public Piece {
public:
	Bishop(Color color);
	~Bishop();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board& board) override;
	char getSymbol() override;
};

class Queen : public Piece {
public:
	Queen(Color color);
	~Queen();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board& board) override;
	char getSymbol() override;
};

class King : public Piece {
public:
	King(Color color);
	~King();
	bool isMovePossible(std::pair<int, int> start_position, std::pair<int, int> end_position, Board& board) override;
	char getSymbol() override;
	void setHasMoved(bool moved);
	bool getHasMoved();
private:
	bool hasMoved = false;
};
#endif 
