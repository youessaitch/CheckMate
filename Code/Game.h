// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Chess.h

// Header guard
#ifndef _CHESS_H
#define	_CHESS_H

// Includes
#include <deque>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Board.h"	
#include "Pawn.h"	
#include "Rook.h"
#include "Queen.h"
#include "Knight.h"	
#include "King.h"	
#include "Bishop.h"

class game
{
private:
	// Private variables
	colour player_turn;  // current player turn colour
	int player_number;
	int difficulty_level;
	int number_of_legal_moves;  // number of allowed moves for use in available moves function
	int move_counter;  // track number of moves
	int white_captures;  // number of pieces captured by white
	int black_captures;  // number of pieces captured by black
	bool in_check;  // if player is in check
	bool game_over;  // if game is over
	bool requesting_available_moves;  // player is requesting available moves
	bool first_round;  // if it's the first round of each game

	// if last move was a special move 
	bool en_passant;  
	bool castling;  
	bool pawn_promotion;  
	bool double_pawn; 

	// check if white or black can en passant
	bool white_can_en_passant;
	bool black_can_en_passant;

	// position of white and black pawns that can be en passant captured
	position white_piece_pos_to_be_en_passant_captured;
	position black_piece_pos_to_be_en_passant_captured;

	// Private functions
	bool make_move(std::string& initial_pos_input, std::string& final_pos_input);
	bool is_game_over();
	bool validate_input(const std::string& input) const;  // check player input is valid
	bool confirm_action() const;  // confirm the user inputted the correct action
	void save_game(const std::string& file_name);
	void load_game(const std::string& file_name);
	void alternate_turn();
	void current_state_of_game() const;  // print info about the game
	void next_move();  // get the next move from the player
	void undo_move();
	void to_lower_case(std::string& input) const;  // make input non-case sensitive
	void log_move(const std::string& initial_pos, const std::string& final_pos);  // record the last move
	std::vector<std::string> available_moves();  // list all available moves for the player
	position convert_to_algebraic_notation(std::string& input) const;  // convert input to chess notation
	std::string pos_to_string(const position& pos) const;  // convert a position to a string

	// save the moves of each round
	struct round
	{
		std::string white_move;
		std::string black_move;
	};

	// double ended queue to store the rounds
	std::deque<round> rounds;

	// save the captured pieces in vectors
	std::vector<std::shared_ptr<base_piece>> white_captured;
	std::vector<std::shared_ptr<base_piece>> black_captured;

	// save info about special moves
	std::vector<bool> save_castling;
	std::vector<bool> save_en_passant;
	std::vector<bool> save_double_pawn;
	std::vector<bool> save_pawn_promotion;
	std::vector<bool> save_white_can_en_passant;
	std::vector<bool> save_black_can_en_passant;
	std::vector<position> save_white_piece_pos_to_be_en_passant_captured;
	std::vector<position> save_black_piece_pos_to_be_en_passant_captured;
	std::vector<bool> record_double_step;
	std::vector<bool> record_rook_castling;
	std::vector<bool> record_king_castling;
	
	// Advanced AI private functions
	std::string best_move;
	double find_best_move(const int& probe_level, const double& previous_evaluation);  // find the best move for the AI
	double evaluate_board() const;  // calculate value of the board
	double evalute_piece(const std::shared_ptr<base_piece>& piece, const position& piece_pos) const;  // return the value of a piece
	double evaluate_position(const char& piece_type, const position& piece_pos, const colour& piece_colour) const;  // return the value of a position

public:

	// constructors
	game(); 
	~game();

	// public functions
	bool start(const int& number_of_players, const int& difficulty_level);
	void introduction() const;
	void instructions() const;
};

#endif
