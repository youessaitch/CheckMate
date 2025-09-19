// Object Orientated Programming in C++

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
