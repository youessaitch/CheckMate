// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Pawn.h

// Header guard
#ifndef _PAWN_H
#define	_PAWN_H

// Includes
#include "BasePiece.h"  // Base piece class

// derived class for pawn piece
class pawn : public base_piece 
{
private:	
	bool double_step;  // check whether a double step advance is available
public:
	// constructors
	pawn(colour piece_col, position piece_pos);
	virtual ~pawn() {};

	// public functions
	bool valid_move(const position& move_to_pos);  // check move is valid for the pawn
	void set_double_step(const bool& double_step_status);  // set the double advance status of the piece
	bool get_double_step() const;  // return double step status
};
#endif