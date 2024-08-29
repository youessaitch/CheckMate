// File: BasePiece.h

// Header guard
#ifndef _BASEPIECE_H
#define	_BASEPIECE_H

// Includes
#include <string>
#include <iostream>

// enumeration for piece colour
enum class colour { white, black };

// structure for position of piece
struct position {
	size_t x;
	size_t y;
};

// base class for base pieces
class base_piece 
{
protected:
	char type;
	colour col;
	position pos;
public:
	// constructors
	base_piece() : type { 0 }, col{ colour::white }, pos{ 0,0 } {}
	virtual ~base_piece() {};

	// public functions
	void print() const;  // draw the piece on the board
	void set_position(const position& pos);
	position get_position() const;
	colour get_colour() const;
	char get_type() const;
	virtual bool valid_move(const position& move_to_pos) = 0;  // pure virtual function to test if move of specific piece is valid
};

std::ostream& operator<<(std::ostream&, const colour& player_colour);  // overload insertion operator for colour
std::ostream& operator<<(std::ostream&, const position& pos);  // overload insertion operator for board positions
bool operator==(const position& pos_1, const position& pos_2);  // function to overload comparison operator for positions

#endif
