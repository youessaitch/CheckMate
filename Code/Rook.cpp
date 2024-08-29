// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Rook.cpp

// Includes
#include "Rook.h"	// Header file for this class
#include "Board.h"  // Board class

extern board game_board;  // tell compiler that a board object exists

// constructor for the rook
rook::rook(colour piece_col, position piece_pos)
{
	this->type = 'R';
	this->col = piece_col;
	this->pos = piece_pos;
	castling = true;
}

// check the requested rook move is valid
//	rook can move an arbitrary number of spaces in a straight line
bool rook::valid_move(const position& move_to_pos)
{
	bool valid_move{ false };

	// check for straight line move of arbitrary length
	if (((move_to_pos.x == pos.x && (move_to_pos.y - pos.y) > 0)
		|| ((move_to_pos.x - pos.x) > 0 && move_to_pos.y == pos.y)
		|| (move_to_pos.x == pos.x && (pos.y - move_to_pos.y) > 0)
		|| ((pos.x - move_to_pos.x) > 0 && move_to_pos.y == pos.y))
		&& game_board.get_piece(move_to_pos) == nullptr) {

		castling = false;
		return !valid_move;
	}

	// check for capture of opposite colour piece
	else if (((move_to_pos.x == pos.x && (move_to_pos.y - pos.y) > 0)
		|| ((move_to_pos.x - pos.x) > 0 && move_to_pos.y == pos.y)
		|| (move_to_pos.x == pos.x && (pos.y - move_to_pos.y) > 0)
		|| ((pos.x - move_to_pos.x) > 0 && move_to_pos.y == pos.y))
		&& game_board.get_piece(move_to_pos) != nullptr && game_board.get_piece(move_to_pos)->get_colour() != col) {

		castling = false;
		return !valid_move;
	}

	return valid_move;
}

// test if the piece is allowed to castle
bool rook::get_castling() const
{
	return castling;
}

// set the castling status
void rook::set_castling(const bool& castle_status)
{
	castling = castle_status;
}