#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>

enum Color;

class Player {
public:
	Player(std::string name, Color color);
	std::string getName();
	Color getColor();
	void setName(std::string name);
	void setColor(Color color);

private:
	std::string name;
	Color color;
};

#endif 
