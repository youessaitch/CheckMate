// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Bishop.h

// Header guard
#ifndef _BISHOP_H
#define	_BISHOP_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for bishop piece
class bishop : public base_piece
{
public:
	// constructors
	bishop(colour piece_col, position piece_pos);
	virtual ~bishop() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the bishop
};
#endif
