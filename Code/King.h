// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: King.h

// Header guard
#ifndef _KING_H
#define	_KING_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for king piece
class king : public base_piece
{
private:
	bool castling;
public:
	// constructors
	king(colour piece_col, position piece_pos);
	virtual ~king() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the pawn

	// access functions
	bool get_castling() const;  // test if king is allowed to castle
	void set_castling(const bool& castling_status);  // set the castling status
};
#endif
