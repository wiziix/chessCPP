#include "Player.h"
#include "Pieces.h"
#include<iostream>

Player::Player(std::string name, Color color) : name(name), color(color) { }

Color Player::getColor() {
	return color;
}

std::string Player::getName() {
	return name;
}

void Player::setColor(Color c) {
	color = c;
}

void Player::setName(std::string n) {
	name = n;
}


