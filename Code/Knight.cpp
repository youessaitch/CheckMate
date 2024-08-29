// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Knight.cpp

// Includes
#include "Knight.h"	// Header file for this class
#include "Board.h"  // Board class

extern board game_board;  // tell compiler that a board object exists

// constructor for the knight
knight::knight(colour piece_col, position piece_pos)
{
	this->type = 'N';
	this->col = piece_col;
	this->pos = piece_pos;
}

// check the requested knight move is valid:
//	knights move in an L shape
bool knight::valid_move(const position& move_to_pos)
{
	bool valid_move{ false };

	// check for L shaped move
	if ((((move_to_pos.x - pos.x) == 2 && (move_to_pos.y - pos.y) == 1) || ((move_to_pos.x - pos.x) == 1 && (move_to_pos.y - pos.y) == 2)
		|| ((move_to_pos.x - pos.x) == 2 && (move_to_pos.y - pos.y) == -1) || ((move_to_pos.x - pos.x) == 1 && (move_to_pos.y - pos.y) == -2)
		|| ((move_to_pos.x - pos.x) == -2 && (move_to_pos.y - pos.y) == 1) || ((move_to_pos.x - pos.x) == -1 && (move_to_pos.y - pos.y) == 2)
		|| ((move_to_pos.x - pos.x) == -2 && (move_to_pos.y - pos.y) == -1) || ((move_to_pos.x - pos.x) == -1 && (move_to_pos.y - pos.y) == -2))
		&& game_board.get_piece(move_to_pos) == nullptr) {

		return !valid_move;
	}

	// check for a capture of opposite colour piece
	else if ((((move_to_pos.x - pos.x) == 2 && (move_to_pos.y - pos.y) == 1) || ((move_to_pos.x - pos.x) == 1 && (move_to_pos.y - pos.y) == 2)
		|| ((move_to_pos.x - pos.x) == 2 && (move_to_pos.y - pos.y) == -1) || ((move_to_pos.x - pos.x) == 1 && (move_to_pos.y - pos.y) == -2)
		|| ((move_to_pos.x - pos.x) == -2 && (move_to_pos.y - pos.y) == 1) || ((move_to_pos.x - pos.x) == -1 && (move_to_pos.y - pos.y) == 2)
		|| ((move_to_pos.x - pos.x) == -2 && (move_to_pos.y - pos.y) == -1) || ((move_to_pos.x - pos.x) == -1 && (move_to_pos.y - pos.y) == -2))
		&& game_board.get_piece(move_to_pos) != nullptr && game_board.get_piece(move_to_pos)->get_colour() != col) {

		return !valid_move;
	}

	return valid_move;
}
