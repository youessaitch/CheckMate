// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 04/05/20

// File: Board.h

// Header guard
#ifndef _BOARD_H
#define	_BOARD_H

// Includes
#include "Square.h" // Square class
#include "Game.h"   // Board class
#include "Pawn.h"	// Pawn class
#include "Rook.h"	// Rook class
#include "Queen.h"	// Queen class
#include "Knight.h"	// Knight class
#include "King.h"	// King class
#include "Bishop.h"	// Bishop class

class board
{
private:
	square game_board[8][8];

	// private functions
	bool within_board(const position& pos) const;  // check a position is within the game board
public:
	// constructors
	board();
	~board() {}

	// Public Functions
	void print();  // draw the game board
	void custom_statement(const position& piece_position); // output custom error statement for piece
	void move_piece(const position& initial_pos, const position& final_pos);
	void reset_board();
	std::string pawn_promoation(const position& pawn_position, const colour& player_colour, const int& player_number);  // perform a pawn promoation
	void undo_move(const position& initial_pos, const position& final_pos, std::shared_ptr<base_piece>& player_piece, 
		std::shared_ptr<base_piece>& captured_piece, const bool& en_passant, const bool& castling, const bool& pawn_promotion, 
		const bool& double_pawn, const colour& player_turn, const bool& double_pawn_status_before_move, 
		const bool& king_status_before_move, const bool& rook_status_before_move);
	void undo_rook_move(const position& rook_initial_pos, const position& rook_final_pos, std::shared_ptr<base_piece> player_piece);  // undo castling rook move
	void en_passant(const position& initial_pos, const position& final_pos);  // perform the en passant move
	void castling(const position& initial_pos, const position& final_pos);  // perform the castling move
	bool is_move_legal(const position& initial_pos, const position& final_pos); 
	bool is_in_check(const colour& player_colour); 
	bool legal_move_available(const colour& player_colour, const bool& white_can_en_passant, const bool& black_can_en_passant, 
		const position& white_piece_pos_to_be_en_passant_captured, const position& black_piece_pos_to_be_en_passant_captured);
	bool moving_into_check(const position& initial_pos, const position& final_pos); 
	bool obstructing_piece(const position& initial_pos, const position& final_pos);  // test if there is an obstructing piece on the move path
	bool double_pawn_move_valid(const position& initial_pos, const position& final_pos);  // test if a double pawn advance can be made
	bool pawn_promotion_valid(const position& final_pos);  // test if a pawn promotion can be made
	bool en_passant_valid(const position& initial_pos, const position& final_pos, const position& en_passant_piece_pos);  // test if an en passant can be made
	bool castling_valid(const position& initial_pos, const position& final_pos);  // test if a castling move can be made

	// access functions
	std::shared_ptr<base_piece> get_piece(const position& pos);
};
#endif