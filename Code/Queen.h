// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Queen.h

// Header guard
#ifndef _QUEEN_H
#define	_QUEEN_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for queen piece
class queen : public base_piece
{
public:
	// constructors
	queen(colour piece_col, position piece_pos);
	virtual ~queen() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the pawn
};
#endif
