// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Square.h

// Header guard
#ifndef _SQUARE_H
#define	_SQUARE_H

// Includes
#include <memory>
#include "BasePiece.h"  // Base piece class

class square 
{
private:
	std::shared_ptr<base_piece> chess_piece;  // each square contains a pointer to a chess piece
public:
	// constructors
	square() : chess_piece{ nullptr } {};
	~square() {};

	//public functions
	std::shared_ptr<base_piece> get_piece();
	void set_piece(std::shared_ptr<base_piece> piece);
	void clear(); 
};
#endif
