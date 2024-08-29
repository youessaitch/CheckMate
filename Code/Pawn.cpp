// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Pawn.cpp

// Includes
#include "Pawn.h"	// Header file for this class
#include "Board.h"  // Board class

// tell compiler that these objects exist
extern board game_board;

// constructor for the pawn
pawn::pawn(colour piece_col, position piece_pos) 
{
	this->type = 'P';
	this->col = piece_col;
	this->pos = piece_pos;
	this->double_step = true;
}

// check the requested pawn move is valid:
//	pawns can move forward one space, except on their first move where they can double step
//	they perform one space diagonal captures
//	cannot move backwards
bool pawn::valid_move(const position& move_to_pos)
{
	bool valid_move{ false };

	int move_one_space{ 1 }; int move_two_space{ 2 };  // white pieces moving up the board
	if (col == colour::black) { move_one_space = -1; move_two_space = -2; }  // black pieces moving down the board

	// check for valid move of one space forward
	if (move_to_pos.y == pos.y + move_one_space && move_to_pos.x == pos.x && game_board.get_piece(move_to_pos) == nullptr) {

		this->double_step = false;
		return !valid_move;
	}

	// check for double step move
	else if (double_step && move_to_pos.y == pos.y + move_two_space && move_to_pos.x == pos.x && game_board.get_piece(move_to_pos) == nullptr) {	
		
		this->double_step = false;
		return !valid_move;
	}

	// check for diagonal capture of opposite colour pieces 
	else if (move_to_pos.y == pos.y + move_one_space && (move_to_pos.x == pos.x + move_one_space || move_to_pos.x == pos.x - move_one_space) 
		&& game_board.get_piece(move_to_pos) != nullptr && game_board.get_piece(move_to_pos)->get_colour() != col) {

		this->double_step = false;
		return !valid_move;
	}

	return valid_move;
}

// set the double advance status
void pawn::set_double_step(const bool& double_step_status) 
{
	double_step = double_step_status;
}

// return the double step status
bool pawn::get_double_step() const
{
	return double_step;
}