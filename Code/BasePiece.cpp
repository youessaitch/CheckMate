

// File: BasePiece.cpp

// Includes
#include "BasePiece.h"	// Base piece class

// print a piece
void base_piece::print() const
{
	if (col == colour::black) std::cout << "B";
	if (col == colour::white) std::cout << "W";
	std::cout << type;
}

// set position of a piece
void base_piece::set_position(const position& pos) 
{
	this->pos = pos;
}

// return the position of a piece
position base_piece::get_position() const
{
	return pos;
}

// return the colour of a piece
colour base_piece::get_colour() const
{
	return col;
}

// return the piece type
char base_piece::get_type() const
{
	return type;
}

// overload comparison operator
bool operator==(const position& pos_1, const position& pos_2) 
{
	if ((pos_1.x == pos_2.x) && (pos_1.y == pos_2.y)) return true;
	else return false;
}

// overload insertion operator for colours
std::ostream& operator<<(std::ostream& os, const colour& player_colour) 
{
	if (player_colour == colour::white)  os << "white";
	else if (player_colour == colour::black)  os << "black";

	return os;
}

// overload insertion operator for positions
std::ostream& operator<<(std::ostream& os, const position& pos) 
{
	if (pos.x == 0) os << "A";
	else if (pos.x == 1) os << "B";
	else if (pos.x == 2) os << "C";
	else if (pos.x == 3) os << "D";
	else if (pos.x == 4) os << "E";
	else if (pos.x == 5) os << "F";
	else if (pos.x == 6) os << "G";
	else if (pos.x == 7) os << "H";

	if (pos.y == 0) os << "1";
	else if (pos.y == 1) os << "2";
	else if (pos.y == 2) os << "3";
	else if (pos.y == 3) os << "4";
	else if (pos.y == 4) os << "5";
	else if (pos.y == 5) os << "6";
	else if (pos.y == 6) os << "7";
	else if (pos.y == 7) os << "8";

	return os;
}

