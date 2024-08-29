// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Board.cpp

// Includes
#include "Board.h"	// Header file for this class

board::board() 
{
	reset_board();
}

// draw the game board
void board::print()
{
	const int square_width = 4;
	const int square_height = 3;

	// loop to print first part of board
	for (size_t row{ 0 }; row < 8 * square_height; row++) {
	
		size_t square_row = row / square_height;

		// print the side of the board with numbering
		if (row % 3 == 1) std::cout << "-" << 8 - square_row << "-";
		else std::cout << "---";

		// print the game board squares
		for (size_t col{ 0 }; col < 8 * square_width; col++) {
		
			size_t square_col = col / square_width;
			std::shared_ptr<base_piece> current_piece = game_board[square_col][7 - square_row].get_piece();

			if ((row % 3) == 1 && ((col % 4) == 1 || (col % 4) == 2) && current_piece != nullptr) {
				if ((col % 4) == 1) {
					current_piece->print();
				}
			} else{
					if ((square_row + square_col) % 2 == 1) std::cout << '*';
					else std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}

	// print the bottom of the board with lettering
	for (size_t row{ 0 }; row < square_height; row++) {
		if (row % 3 == 1) {
			std::cout << "---";
			for (size_t col{ 0 }; col < 8 * square_width; col++) {
				size_t square_col = col / square_width;
				if (col % 4 == 1) {
					if (square_col == 0) std::cout << "a";
					if (square_col == 1) std::cout << "b";
					if (square_col == 2) std::cout << "c";
					if (square_col == 3) std::cout << "d";
					if (square_col == 4) std::cout << "e";
					if (square_col == 5) std::cout << "f";
					if (square_col == 6) std::cout << "g";
					if (square_col == 7) std::cout << "h";
				} else std::cout << "-";
			}	
			std::cout << std::endl;
		} else {
			for (size_t col{ 0 }; col < 9 * square_width - 1; col++) std::cout << "-";	
			std::cout << std::endl;
		}
	}
}

//  return a custom error statement for the piece
void board::custom_statement(const position& piece_pos)
{
	// get piece and type
	std::shared_ptr<base_piece> player_piece{ game_board[piece_pos.x][piece_pos.y].get_piece() };
	std::string piece_type{ player_piece->get_type() };

	// output custom statement
	if (piece_type == "P") std::cerr << "Error: invalid pawn move\n";
	if (piece_type == "R") std::cerr << "Error: invalid rook move\n";
	if (piece_type == "N") std::cerr << "Error: invalid knight move\n";
	if (piece_type == "B") std::cerr << "Error: invalid bishop move\n";
	if (piece_type == "K") std::cerr << "Error: invalid king move\n";
	if (piece_type == "Q") std::cerr << "Error: invalid queen move\n";
}

// move the piece from inital to final position
void board::move_piece(const position& initial_pos, const position& final_pos)
{
	std::shared_ptr<base_piece> piece_to_move{ game_board[initial_pos.x][initial_pos.y].get_piece() };  // get the piece from the initial position
	piece_to_move->set_position(final_pos);  // change position of the piece
	game_board[final_pos.x][final_pos.y].set_piece(piece_to_move);  // update final position on board to contain the piece
	game_board[initial_pos.x][initial_pos.y].set_piece(nullptr);  // delete piece from its initial position on board
}

// function to reset the board for a new game
void board::reset_board()
{
	// initialise all pointers to null 
	for (size_t row{ 0 }; row < 8; row++) {
		for (size_t col{ 0 }; col < 8; col++) {
			position null_piece{ row,col };
			game_board[null_piece.x][null_piece.y].set_piece(nullptr);
		}
	}

	// pawns
	for (size_t column{ 0 }; column < 8; column++) {

		// white pawns
		position white_pawn_pos{ column, 1 };
		game_board[white_pawn_pos.x][white_pawn_pos.y].set_piece(std::shared_ptr<base_piece>(new pawn(colour::white, white_pawn_pos)));

		// black pawns
		position black_pawn_pos{ column, 6 };
		game_board[black_pawn_pos.x][black_pawn_pos.y].set_piece(std::shared_ptr<base_piece>(new pawn(colour::black, black_pawn_pos)));
	}

	// white rooks
	position white_rook_pos_1{ 0,0 };
	game_board[white_rook_pos_1.x][white_rook_pos_1.y].set_piece(std::shared_ptr<base_piece>(new rook(colour::white, white_rook_pos_1)));
	position white_rook_pos_2{ 7,0 };
	game_board[white_rook_pos_2.x][white_rook_pos_2.y].set_piece(std::shared_ptr<base_piece>(new rook(colour::white, white_rook_pos_2)));

	// black rooks
	position black_rook_pos_1{ 0,7 };
	game_board[black_rook_pos_1.x][black_rook_pos_1.y].set_piece(std::shared_ptr<base_piece>(new rook(colour::black, black_rook_pos_1)));
	position black_rook_pos_2{ 7,7 };
	game_board[black_rook_pos_2.x][black_rook_pos_2.y].set_piece(std::shared_ptr<base_piece>(new rook(colour::black, black_rook_pos_2)));

	// white knights
	position white_knight_pos_1{ 1,0 };
	game_board[white_knight_pos_1.x][white_knight_pos_1.y].set_piece(std::shared_ptr<base_piece>(new knight(colour::white, white_knight_pos_1)));
	position white_knight_pos_2{ 6,0 };
	game_board[white_knight_pos_2.x][white_knight_pos_2.y].set_piece(std::shared_ptr<base_piece>(new knight(colour::white, white_knight_pos_2)));

	// black knights
	position black_knight_pos_1{ 1,7 };
	game_board[black_knight_pos_1.x][black_knight_pos_1.y].set_piece(std::shared_ptr<base_piece>(new knight(colour::black, black_knight_pos_1)));
	position black_knight_pos_2{ 6,7 };
	game_board[black_knight_pos_2.x][black_knight_pos_2.y].set_piece(std::shared_ptr<base_piece>(new knight(colour::black, black_knight_pos_2)));

	// white bishops
	position white_bishop_pos_1{ 2,0 };
	game_board[white_bishop_pos_1.x][white_bishop_pos_1.y].set_piece(std::shared_ptr<base_piece>(new bishop(colour::white, white_bishop_pos_1)));
	position white_bishop_pos_2{ 5,0 };
	game_board[white_bishop_pos_2.x][white_bishop_pos_2.y].set_piece(std::shared_ptr<base_piece>(new bishop(colour::white, white_bishop_pos_2)));

	// black bishops
	position black_bishop_pos_1{ 2,7 };
	game_board[black_bishop_pos_1.x][black_bishop_pos_1.y].set_piece(std::shared_ptr<base_piece>(new bishop(colour::black, black_bishop_pos_1)));
	position black_bishop_pos_2{ 5,7 };
	game_board[black_bishop_pos_2.x][black_bishop_pos_2.y].set_piece(std::shared_ptr<base_piece>(new bishop(colour::black, black_bishop_pos_2)));

	// white king
	position white_king_pos{ 4,0 };
	game_board[white_king_pos.x][white_king_pos.y].set_piece(std::shared_ptr<base_piece>(new king(colour::white, white_king_pos)));

	// black king
	position black_king_pos{ 4,7 };
	game_board[black_king_pos.x][black_king_pos.y].set_piece(std::shared_ptr<base_piece>(new king(colour::black, black_king_pos)));

	// white queen
	position white_queen_pos{ 3,0 };
	game_board[white_queen_pos.x][white_queen_pos.y].set_piece(std::shared_ptr<base_piece>(new queen(colour::white, white_queen_pos)));

	// black queen
	position black_queen_pos{ 3,7 };
	game_board[black_queen_pos.x][black_queen_pos.y].set_piece(std::shared_ptr<base_piece>(new queen(colour::black, black_queen_pos)));
}

// perform a pawn promoation
std::string board::pawn_promoation(const position& pawn_position, const colour& player_colour, const int& player_number)
{
	// get the piece to promote to from the player
	std::string promoted_piece;

	// for a two player game or single player game on the players turn, ask for the piece to promote to
	if (player_number == 2 || (player_number == 1 && player_colour == colour::white)) {
		std::cout << "Please enter the piece type you wish to promote to:";
		do {
			std::getline(std::cin, promoted_piece);
			if (promoted_piece != "Q" && promoted_piece != "N" && promoted_piece != "B" && promoted_piece != "R") {
				std::cerr << "Error: please enter 'Q' = Queen, 'N' = Knight, 'B' = Bishop, 'R' = Rook:";
			}
		} while (promoted_piece != "Q" && promoted_piece != "N" && promoted_piece != "B" && promoted_piece != "R");
	}

	// for the computers pawn promotion, choose at random
	else {
		// get a random number 1-4
		srand((unsigned)time(0));
		int rand_num = (rand() % 4) + 1;

		// convert that number to a piece
		if (rand_num == 1) promoted_piece = "Q";
		else if (rand_num == 2) promoted_piece = "N";
		else if (rand_num == 3) promoted_piece = "B";
		else if (rand_num == 4) promoted_piece = "R";
	}


	game_board[pawn_position.x][pawn_position.y].set_piece(nullptr);  // delete the pawn

	// replace with the appropriate piece
	if (promoted_piece == "Q") game_board[pawn_position.x][pawn_position.y].set_piece(std::shared_ptr<base_piece>(new queen(player_colour, pawn_position)));
	else if (promoted_piece == "N") game_board[pawn_position.x][pawn_position.y].set_piece(std::shared_ptr<base_piece>(new knight(player_colour, pawn_position)));
	else if (promoted_piece == "B") game_board[pawn_position.x][pawn_position.y].set_piece(std::shared_ptr<base_piece>(new bishop(player_colour, pawn_position)));
	else if (promoted_piece == "R") game_board[pawn_position.x][pawn_position.y].set_piece(std::shared_ptr<base_piece>(new rook(player_colour, pawn_position)));

	return promoted_piece;
}

// undo the most recent move
void board::undo_move(const position& initial_pos, const position& final_pos, std::shared_ptr<base_piece>& player_piece, std::shared_ptr<base_piece>& captured_piece,
	const bool& en_passant, const bool& castling, const bool& pawn_promotion, const bool& double_pawn, const colour& player_turn,
	const bool& double_step_status_before_move, const bool& king_castling_status_before_move, const bool& rook_castling_status_before_move)
{
	// if undoing an en passant move
	if (en_passant) {

		// undo the player piece move
		player_piece->set_position(initial_pos);
		game_board[initial_pos.x][initial_pos.y].set_piece(player_piece);
		game_board[final_pos.x][final_pos.y].set_piece(nullptr);

		// reset the captured pawn
		position en_passant_captured_pawn{ final_pos.x, initial_pos.y };
		captured_piece->set_position(en_passant_captured_pawn);
		game_board[en_passant_captured_pawn.x][en_passant_captured_pawn.y].set_piece(captured_piece);
	}

	// if undoing a castling move
	else if (castling) {

		// undo the king move
		player_piece->set_position(initial_pos);
		game_board[initial_pos.x][initial_pos.y].set_piece(player_piece);
		game_board[final_pos.x][final_pos.y].set_piece(nullptr);
		std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece);  // dynamic down cast to a derived class pointer
		king_piece->set_castling(true);  // allow king to castle again

		// white castle
		if (final_pos.y == 0) {

			// king side
			if (final_pos.x == 6) {

				// undo the rook move
				position rook_initial_pos{ 5,0 };
				position rook_final_pos{ 7,0 };
				undo_rook_move(rook_initial_pos, rook_final_pos, player_piece);
			}
			// queen side
			else if (final_pos.x == 2) {

				// undo the rook move
				position rook_initial_pos{ 3,0 };
				position rook_final_pos{ 0,0 };
				undo_rook_move(rook_initial_pos, rook_final_pos, player_piece);
			}
		}

		// black castle
		else if (final_pos.y == 7) {

			// king side
			if (final_pos.x == 6) {

				// undo the rook move
				position rook_initial_pos{ 5,7 };
				position rook_final_pos{ 7,7 };
				undo_rook_move(rook_initial_pos, rook_final_pos, player_piece);
			}
			// queen side
			else if (final_pos.x == 2) {

				// undo the rook move
				position rook_initial_pos{ 3,7 };
				position rook_final_pos{ 0,7 };
				undo_rook_move(rook_initial_pos, rook_final_pos, player_piece);
			}
		}
	}

	// undo pawn promotion move
	else if (pawn_promotion) {

		game_board[final_pos.x][final_pos.y].set_piece(nullptr);  // delete the promoted piece

		// if there was a capture, reset the captured piece
		if (captured_piece != nullptr) {
			captured_piece->set_position(final_pos);
			game_board[final_pos.x][final_pos.y].set_piece(captured_piece);
		}

		// create a new pawn at the initial pos
		game_board[initial_pos.x][initial_pos.y].set_piece(std::shared_ptr<base_piece>(new pawn(player_turn, initial_pos)));
		std::shared_ptr<base_piece> reset_pawn{ get_piece(initial_pos) };
		std::shared_ptr<pawn> reset_pawn_double_step = std::dynamic_pointer_cast<pawn>(reset_pawn);  // dynamic down cast to a derived class pointer
		reset_pawn_double_step->set_double_step(false);  // don't allow new pawn to double advance
	}

	// undoing a regular move
	else {

		// undo the player piece move
		player_piece->set_position(initial_pos);
		game_board[initial_pos.x][initial_pos.y].set_piece(player_piece);
		game_board[final_pos.x][final_pos.y].set_piece(nullptr);

		// if a king or rook, restore their ability to special move
		char piece_type{ player_piece->get_type() };
		if (piece_type == 'P') {
			std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(player_piece);
			pawn_piece->set_double_step(double_step_status_before_move);
		}
		else if (piece_type == 'K') {
			std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece);
			king_piece->set_castling(king_castling_status_before_move);
		}
		else if (piece_type == 'R') {
			std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece);
			rook_piece->set_castling(rook_castling_status_before_move);
		}

		// if there was a captured piece, reset it
		if (captured_piece != nullptr) {
			captured_piece->set_position(final_pos);
			game_board[final_pos.x][final_pos.y].set_piece(captured_piece);
		}
	}
}

// undo the rook move
void board::undo_rook_move(const position& rook_initial_pos, const position& rook_final_pos, std::shared_ptr<base_piece> player_piece)
{
	// get the rook and reset its position
	std::shared_ptr<base_piece> rook_piece_reset_pos{ get_piece(rook_initial_pos) };
	rook_piece_reset_pos->set_position(rook_final_pos);
	game_board[rook_final_pos.x][rook_final_pos.y].set_piece(rook_piece_reset_pos);
	game_board[rook_initial_pos.x][rook_initial_pos.y].set_piece(nullptr);

	std::shared_ptr<rook> rook_piece_reset_castle = std::dynamic_pointer_cast<rook>(rook_piece_reset_pos);  // dynamic down cast to a derived class pointer
	rook_piece_reset_castle->set_castling(true);  // allow rook to castle again
}

// perform the en passant move
void board::en_passant(const position& initial_pos, const position& final_pos)
{
	// get the relevant pieces and player colour
	std::shared_ptr<base_piece> player_pawn{ get_piece(initial_pos) };
	colour player_colour{ player_pawn->get_colour() };
	std::shared_ptr<base_piece> opponent_pawn{ get_piece(final_pos) };

	// move the player piece
	player_pawn->set_position(final_pos);
	game_board[final_pos.x][final_pos.y].set_piece(player_pawn);
	game_board[initial_pos.x][initial_pos.y].set_piece(nullptr);

	// remove the opponents pawn
	game_board[final_pos.x][initial_pos.y].set_piece(nullptr);
}

// perform the castling move
void board::castling(const position& initial_pos, const position& final_pos)
{
	position rook_initial_pos;
	position rook_final_pos;

	// get player piece and colour
	std::shared_ptr<base_piece> player_piece_one{ get_piece(initial_pos) };
	colour player_colour = player_piece_one->get_colour();

	std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece_one);  // dynamic down cast to a derived class pointer

	// queen side castle
	if (final_pos.x == 2) {

		// get the appropriate coloured rook
		if (player_colour == colour::white) { rook_initial_pos = { 0,0 }; rook_final_pos = { 3,0 }; }  // white castle
		else { rook_initial_pos = { 0,7 }; rook_final_pos = { 3,7 }; }  // black castle
		std::shared_ptr<base_piece> player_piece_two{ get_piece(rook_initial_pos) };
		std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece_two);  // dynamic down cast to a derived class pointer

		// move the king
		king_piece->set_position(final_pos);
		game_board[final_pos.x][final_pos.y].set_piece(king_piece);
		game_board[initial_pos.x][initial_pos.y].set_piece(nullptr);

		// move the rook
		rook_piece->set_position(rook_final_pos);
		game_board[rook_final_pos.x][rook_final_pos.y].set_piece(rook_piece);
		game_board[rook_initial_pos.x][rook_initial_pos.y].set_piece(nullptr);

		// cannot castle again
		king_piece->set_castling(false);
		rook_piece->set_castling(false);

		std::cout << "\nCastling complete!\n";
	}

	// king side castle
	else if (final_pos.x == 6) {

		// get the appropriate coloured rook and test if it can castle
		if (player_colour == colour::white) { rook_initial_pos = { 7,0 }; rook_final_pos = { 5,0 }; }  // white castle
		else { rook_initial_pos = { 7,7 }; rook_final_pos = { 5,7 }; }  // black castle
		std::shared_ptr<base_piece> player_piece_two{ get_piece(rook_initial_pos) };
		std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece_two);  // dynamic down cast to a derived class pointer

		// move the king
		king_piece->set_position(final_pos);
		game_board[final_pos.x][final_pos.y].set_piece(king_piece);
		game_board[initial_pos.x][initial_pos.y].set_piece(nullptr);

		// move the rook
		rook_piece->set_position(rook_final_pos);
		game_board[rook_final_pos.x][rook_final_pos.y].set_piece(rook_piece);
		game_board[rook_initial_pos.x][rook_initial_pos.y].set_piece(nullptr);

		// cannot castle again
		king_piece->set_castling(false);
		rook_piece->set_castling(false);

		std::cout << "\nCastling complete!\n";
	}
}

// test if the move is legal
bool board::is_move_legal(const position& initial_pos, const position& final_pos)
{
	bool move_piece{ false };

	// check initial and final position not equal
	if (initial_pos.x != final_pos.x || initial_pos.y != final_pos.y) {

		std::shared_ptr<base_piece> piece_to_move{ game_board[initial_pos.x][initial_pos.y].get_piece() };  // get the piece from the initial position

		// check that a piece exists in the initial location, and the final position is a valid move within the board limits
		if (piece_to_move != nullptr && piece_to_move->valid_move(final_pos) && within_board(final_pos)) {
			return !move_piece;
		}
	}

	return move_piece;
}

// test whether the player is in check
bool board::is_in_check(const colour& player_colour)
{
	bool check{ false };

	// find the position of the king players king
	position king_pos{ 0,0 };
	for (size_t row{ 0 }; row < 8; row++) {
		for (size_t col{ 0 }; col < 8; col++) {
			std::shared_ptr<base_piece> king_search{ game_board[col][row].get_piece() };
			if (king_search != nullptr && king_search->get_colour() == player_colour && king_search->get_type() == 'K') {
				king_pos.x = col; king_pos.y = row;
				goto endloop1;
			}
		}
	}
endloop1:

	// run through opponents pieces and see if any can capture the king
	for (size_t row{ 0 }; row < 8; row++) {
		for (size_t col{ 0 }; col < 8; col++) {
			position opponent_pos{ col,row };
			std::shared_ptr<base_piece> opponent_piece{ game_board[col][row].get_piece() };
			if (opponent_piece != nullptr && opponent_piece->get_colour() != player_colour) {
				if (opponent_piece->valid_move(king_pos)) {
					position opponent_piece_pos{ col,row };
					if (!board::obstructing_piece(opponent_piece_pos, king_pos)) {
						return !check;
					}
				}
			}
		}
	}

	return check;
}

// test if the player has a legal move available
bool board::legal_move_available(const colour& player_colour, const bool& white_can_en_passant, const bool& black_can_en_passant,
	const position& white_piece_pos_to_be_en_passant_captured, const position& black_piece_pos_to_be_en_passant_captured)
{
	bool legal_move_available{ false };
	char piece_type;

	// record special move status of pieces
	bool castling_king;
	bool castling_rook;
	bool double_pawn_step;

	// run through all pieces on board
	for (size_t row{ 0 }; row < 8; row++) {
		for (size_t col{ 0 }; col < 8; col++) {
			position piece_pos{ col, row };
			std::shared_ptr<base_piece> player_piece{ game_board[col][row].get_piece() };

			// if a piece exists that belongs to the player
			if (player_piece != nullptr && player_piece->get_colour() == player_colour) {

				piece_type = player_piece->get_type();

				// if piece is a pawn, rook, or king, save it's current status to perform special moves
				if (piece_type == 'P') {
					std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(player_piece);
					double_pawn_step = pawn_piece->get_double_step();
				}
				else if (piece_type == 'R') {
					std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece);
					castling_rook = rook_piece->get_castling();
				}
				else if (piece_type == 'K') {
					std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece);
					castling_king = king_piece->get_castling();
				}

				// run through all possible moves and check if there is a valid one
				for (size_t move_row{ 0 }; move_row < 8; move_row++) {
					for (size_t move_col{ 0 }; move_col < 8; move_col++) {
						position test_move{ move_col,move_row };

						// test for special moves
						if (castling_valid(piece_pos, test_move) || double_pawn_move_valid(piece_pos, test_move)
							|| (white_can_en_passant && en_passant_valid(piece_pos, test_move, black_piece_pos_to_be_en_passant_captured))
							|| (black_can_en_passant && en_passant_valid(piece_pos, test_move, white_piece_pos_to_be_en_passant_captured))) {
							return !legal_move_available;
						}

						// regular moves
						else if (player_piece->valid_move(test_move)) {
							position player_piece_pos{ player_piece->get_position() };
							if (!board::obstructing_piece(player_piece_pos, test_move)) {
								if (!board::moving_into_check(player_piece_pos, test_move)) {

									// if the piece is a pawn, rook or king, need to reset their ability to perform special moves
									if (player_piece->get_type() == 'P') {
										std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(player_piece);
										pawn_piece->set_double_step(double_pawn_step);
									}
									else if (player_piece->get_type() == 'K') {
										std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece);
										king_piece->set_castling(castling_king);
									}
									else if (player_piece->get_type() == 'R') {
										std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece);
										rook_piece->set_castling(castling_rook);
									}

									return !legal_move_available;
								}
							}
						}
					}
				}
			}
		}
	}

	return legal_move_available;
}

// test whether the player is moving into check
bool board::moving_into_check(const position& initial_pos, const position& final_pos)
{
	bool moving_into_check{ false };
	bool capture{ false };

	// get piece player wants to move
	std::shared_ptr<base_piece> player_piece{ game_board[initial_pos.x][initial_pos.y].get_piece() };
	colour player_colour{ player_piece->get_colour() };

	// record the potential opponent piece at the final position
	std::shared_ptr<base_piece> opponent_piece{ game_board[final_pos.x][final_pos.y].get_piece() };
	if (opponent_piece != nullptr) capture = true;

	// make the move
	player_piece->set_position(final_pos);
	game_board[final_pos.x][final_pos.y].set_piece(player_piece);
	game_board[initial_pos.x][initial_pos.y].set_piece(nullptr);

	// if move puts player in check
	if (is_in_check(player_colour)) {

		// undo the move
		player_piece->set_position(initial_pos);
		game_board[initial_pos.x][initial_pos.y].set_piece(player_piece);
		if (capture) game_board[final_pos.x][final_pos.y].set_piece(opponent_piece);
		else game_board[final_pos.x][final_pos.y].set_piece(nullptr);

		moving_into_check = true;
		return moving_into_check;
	}

	// undo the move
	player_piece->set_position(initial_pos);
	game_board[initial_pos.x][initial_pos.y].set_piece(player_piece);
	if (capture) game_board[final_pos.x][final_pos.y].set_piece(opponent_piece);
	else game_board[final_pos.x][final_pos.y].set_piece(nullptr);

	return moving_into_check;
}

// test whether there is a piece between the initial and final position
bool board::obstructing_piece(const position& initial_pos, const position& final_pos)
{
	bool obstructing_piece{ false };

	// if a one space move, cannot be an obstructing piece
	if ((final_pos.x == initial_pos.x && final_pos.y == initial_pos.y + 1)
		|| (final_pos.x == initial_pos.x + 1 && final_pos.y == initial_pos.y + 1)
		|| (final_pos.x == initial_pos.x + 1 && final_pos.y == initial_pos.y)
		|| (final_pos.x == initial_pos.x + 1 && final_pos.y == initial_pos.y - 1)
		|| (final_pos.x == initial_pos.x && final_pos.y == initial_pos.y - 1)
		|| (final_pos.x == initial_pos.x - 1 && final_pos.y == initial_pos.y - 1)
		|| (final_pos.x == initial_pos.x - 1 && final_pos.y == initial_pos.y)
		|| (final_pos.x == initial_pos.x - 1 && final_pos.y == initial_pos.y + 1)) {

		return obstructing_piece;
	}

	std::shared_ptr<base_piece> player_piece{ game_board[initial_pos.x][initial_pos.y].get_piece() };  // get player piece
	char piece_type = player_piece->get_type();

	// only the knight can jump over pieces
	if (piece_type == 'N') return obstructing_piece;

	// if the move is vertical
	if (final_pos.x == initial_pos.x) {
		// check for any piece between the initial and final position
		if (final_pos.y > initial_pos.y) {
			for (size_t i{ initial_pos.y + 1 }; i <= final_pos.y; i++) {
				std::shared_ptr<base_piece> potential_piece{ game_board[initial_pos.x][i].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
			}
		}
		else {
			for (size_t i{ final_pos.y }; i <= initial_pos.y - 1; i++) {
				std::shared_ptr<base_piece> potential_piece{ game_board[initial_pos.x][i].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
			}
		}
	}

	// if move is horizontal
	else if (final_pos.y == initial_pos.y) {

		if (final_pos.x > initial_pos.x) {

			// check for any piece between the initial and final position
			for (size_t i{ initial_pos.x + 1 }; i <= final_pos.x; i++) {
				std::shared_ptr<base_piece> potential_piece{ game_board[i][initial_pos.y].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
			}
		}
		else {
			// check for any piece between the initial and final position
			for (size_t i{ final_pos.x }; i <= initial_pos.x - 1; i++) {
				std::shared_ptr<base_piece> potential_piece{ game_board[i][initial_pos.y].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
			}
		}
	}

	// if the move is diagonal
	else {

		// check for any piece between the initial and final position for the four different diagonal moves
		size_t row; size_t col;

		// forward right diagonal
		if (final_pos.y > initial_pos.y&& final_pos.x > initial_pos.x) {
			row = initial_pos.y + 1; col = initial_pos.x + 1;
			do {
				std::shared_ptr<base_piece> potential_piece{ game_board[col][row].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
				row++; col++;
			} while (row != final_pos.y);
		}
		// forward left diagonal
		else if (final_pos.y > initial_pos.y&& final_pos.x < initial_pos.x) {
			row = initial_pos.y + 1; col = initial_pos.x - 1;
			do {
				std::shared_ptr<base_piece> potential_piece{ game_board[col][row].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
				row++; col--;
			} while (row != final_pos.y);
		}
		// back left diagonal
		else if (final_pos.y < initial_pos.y && final_pos.x < initial_pos.x) {
			row = initial_pos.y - 1; col = initial_pos.x - 1;
			do {
				std::shared_ptr<base_piece> potential_piece{ game_board[col][row].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
				row--; col--;
			} while (row != final_pos.y);
		}
		// back right diagonal
		else {
			row = initial_pos.y - 1; col = initial_pos.x + 1;
			do {
				std::shared_ptr<base_piece> potential_piece{ game_board[col][row].get_piece() };
				if (potential_piece != nullptr) return !obstructing_piece;
				row--; col++;
			} while (row != final_pos.y);
		}
	}

	return obstructing_piece;
}

// test for a double pawn move
bool board::double_pawn_move_valid(const position& initial_pos, const position& final_pos)
{
	bool double_pawn_move{ false };

	// get piece and colour
	std::shared_ptr<base_piece> pawn_piece{ get_piece(initial_pos) };
	colour player_colour{ pawn_piece->get_colour() };

	// if piece is a pawn
	if (pawn_piece->get_type() == 'P') {

		// test white double pawn move
		if (player_colour == colour::white) {
			if (initial_pos.x == final_pos.x && initial_pos.y + 2 == final_pos.y
				&& !obstructing_piece(initial_pos, final_pos) && !moving_into_check(initial_pos, final_pos)) {
				return !double_pawn_move;
			}
		}
		// test black double pawn move
		else {
			if (initial_pos.x == final_pos.x && initial_pos.y - 2 == final_pos.y
				&& !obstructing_piece(initial_pos, final_pos) && !moving_into_check(initial_pos, final_pos)) {
				return !double_pawn_move;
			}
		}
	}

	return double_pawn_move;
}

// test for pawn promotion move
bool board::pawn_promotion_valid(const position& final_pos)
{
	bool pawn_promotion{ false };

	// get info about moving piece
	std::shared_ptr<base_piece> player_piece{ get_piece(final_pos) };
	colour player_colour{ player_piece->get_colour() };

	if (player_piece->get_type() == 'P') {

		// white promotion
		if (player_colour == colour::white) {
			if (final_pos.y == 7) return !pawn_promotion;
		}
		// black promotion
		else {
			if (final_pos.y == 0) return !pawn_promotion;
		}
	}

	return pawn_promotion;
}

// test for an en passant move
bool board::en_passant_valid(const position& initial_pos, const position& final_pos, const position& en_passant_piece_pos)
{
	bool en_passant{ false };

	// get piece and colour
	std::shared_ptr<base_piece> pawn_piece{ get_piece(initial_pos) };
	colour player_colour{ pawn_piece->get_colour() };

	// if piece is a pawn
	if (pawn_piece->get_type() == 'P') {

		int move_one_space{ 1 }; int move_two_space{ 2 };  // white pieces moving up the board
		if (player_colour == colour::black) { move_one_space = -1; move_two_space = -2; }  // black pieces moving down the board

		// diagonal move into an empty space and not moving into check
		if (final_pos.y == initial_pos.y + move_one_space && (final_pos.x == initial_pos.x + move_one_space || final_pos.x == initial_pos.x - move_one_space)
			&& get_piece(final_pos) == nullptr && !moving_into_check(initial_pos, final_pos)) {

			// test if the position we could en passant capture contains the opposite pawn piece that just made a double advance
			position en_passant_capture_postion{ final_pos.x, initial_pos.y };
			if (en_passant_capture_postion == en_passant_piece_pos) return !en_passant;
		}
	}

	return en_passant;
}

// test if player is attempting a castling move
bool board::castling_valid(const position& initial_pos, const position& final_pos)
{
	bool castling{ false };
	position rook_initial_pos;
	position rook_final_pos;
	position intermediate_king_pos;

	// get player piece and colour
	std::shared_ptr<base_piece> player_piece_one{ get_piece(initial_pos) };
	colour player_colour = player_piece_one->get_colour();

	// if players piece is the king
	if (player_piece_one->get_type() == 'K') {

		std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(player_piece_one);  // dynamic down cast to a derived class pointer

		// if the king is allowed to castle
		if (king_piece->get_castling()) {

			// queen side castle
			if (final_pos.x == 2 && initial_pos.y == final_pos.y) {

				// get the appropriate coloured rook and test if it can castle
				if (player_colour == colour::white) { rook_initial_pos = { 0,0 }; rook_final_pos = { 3,0 }; intermediate_king_pos = { 3,0 }; }  // white castle
				else { rook_initial_pos = { 0,7 }; rook_final_pos = { 3,7 }; intermediate_king_pos = { 3,7 }; }  // black castle
				std::shared_ptr<base_piece> player_piece_two{ get_piece(rook_initial_pos) };
				std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece_two);  // dynamic down cast to a derived class pointer
				if (rook_piece != nullptr && rook_piece->get_type() == 'R' && rook_piece->get_castling()) {

					// test for no obstructing pieces
					if (!obstructing_piece(initial_pos, final_pos)) {

						// test king doesn't move into or over a check locked position
						if (!moving_into_check(initial_pos, intermediate_king_pos) && !moving_into_check(initial_pos, final_pos)) {
							return !castling;
						}
					}
				}
			}
			// king side castle
			else if (final_pos.x == 6 && initial_pos.y == final_pos.y) {

				// get the appropriate coloured rook and test if it can castle
				if (player_colour == colour::white) { rook_initial_pos = { 7,0 }; rook_final_pos = { 5,0 }; intermediate_king_pos = { 5,0 }; }  // white castle
				else { rook_initial_pos = { 7,7 }; rook_final_pos = { 5,7 }; intermediate_king_pos = { 5,7 }; }  // black castle
				std::shared_ptr<base_piece> player_piece_two{ get_piece(rook_initial_pos) };
				std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(player_piece_two);  // dynamic down cast to a derived class pointer
				if (rook_piece != nullptr && rook_piece->get_type() == 'R' && rook_piece->get_castling()) {

					// test for no obstructing pieces
					if (!obstructing_piece(initial_pos, final_pos)) {

						// test king doesn't move into or over a check locked position
						if (!moving_into_check(initial_pos, intermediate_king_pos) && !moving_into_check(initial_pos, final_pos)) {
							return !castling;
						}
					}
				}
			}
		}
	}

	return castling;
}

// return the piece at a given position on the board
std::shared_ptr<base_piece> board::get_piece(const position& pos)
{
	return game_board[pos.x][pos.y].get_piece();
}

// check if the piece is within the game board
bool board::within_board(const position& pos) const
{
	bool within_board{ false };

	// check position is within board range
	if (pos.x < 8 && pos.x >= 0 && pos.y < 8 && pos.y >= 0) return !within_board;

	return within_board;
}