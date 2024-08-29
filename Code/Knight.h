// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Knight.h

// Header guard
#ifndef _KNIGHT_H
#define	_KNIGHT_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for knight piece
class knight : public base_piece
{
public:
	// constructors
	knight(colour piece_col, position piece_pos);
	virtual ~knight() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the pawn
};
#endif
