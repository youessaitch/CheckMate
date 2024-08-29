// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Rook.h

// Header guard
#ifndef _ROOK_H
#define	_ROOK_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for rook piece
class rook : public base_piece
{
private:
	bool castling;
public:
	// constructors
	rook(colour piece_col, position piece_pos);
	virtual ~rook() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the pawn

	// access functions
	bool get_castling() const;  // test if rook is allowed to castle
	void set_castling(const bool& castle_status);  // set the castling status
};
#endif
