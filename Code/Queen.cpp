// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Queen.cpp

// Includes
#include "Queen.h"	// Header file for this class
#include "Board.h"  // Board class

extern board game_board;  // tell compiler that a board object exists

// constructor for the queen
queen::queen(colour piece_col, position piece_pos)
{
	this->type = 'Q';
	this->col = piece_col;
	this->pos = piece_pos;
}

// check the requested queen move is valid:
//	queen can move an arbitrary number of spaces in any direction
bool queen::valid_move(const position& move_to_pos)
{
	bool valid_move{ false };

	// check for straight line move of arbitary length
	if (((move_to_pos.x == pos.x && move_to_pos.y > pos.y)
		|| (move_to_pos.x > pos.x && move_to_pos.y == pos.y)
		|| (move_to_pos.x == pos.x && pos.y > move_to_pos.y)
		|| (pos.x > move_to_pos.x && move_to_pos.y == pos.y))
		&& game_board.get_piece(move_to_pos) == nullptr) {
		
		return !valid_move;
	}

	// check for diagonal move of arbitrary length
	else if (((move_to_pos.x - pos.x == move_to_pos.y - pos.y)
		|| (pos.x - move_to_pos.x == pos.y - move_to_pos.y)
		|| (move_to_pos.x - pos.x == pos.y - move_to_pos.y)
		|| (pos.x - move_to_pos.x == move_to_pos.y - pos.y))
		&& game_board.get_piece(move_to_pos) == nullptr) {
		
		return !valid_move;
	}

	// check for capture of opposite colour piece
	else if ((((move_to_pos.x == pos.x && move_to_pos.y > pos.y)
		|| (move_to_pos.x > pos.x && move_to_pos.y == pos.y)
		|| (move_to_pos.x == pos.x && pos.y > move_to_pos.y)
		|| (pos.x > move_to_pos.x && move_to_pos.y == pos.y))
		|| ((move_to_pos.x - pos.x == move_to_pos.y - pos.y)
		|| (pos.x - move_to_pos.x == pos.y - move_to_pos.y)
		|| (move_to_pos.x - pos.x == pos.y - move_to_pos.y)
		|| (pos.x - move_to_pos.x == move_to_pos.y - pos.y)))
		&& game_board.get_piece(move_to_pos) != nullptr && game_board.get_piece(move_to_pos)->get_colour() != col) {
	
		return !valid_move;
	}

	return valid_move;
}