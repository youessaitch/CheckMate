// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 03/05/20

// File: Chess.cpp

// Includes
#include "Game.h"	// Header file for this class
#include <iomanip>
#include <sstream>
#include <string>
#include <regex>
#include <fstream>
#include <algorithm>

// Tell compiler these objects exist in main
extern board game_board;
extern bool load;

// constructor
game::game() 
{
    first_round = true;
    in_check = false;
    game_over = false;
    requesting_available_moves = false;
    player_number = 0;
    difficulty_level = 0;
    white_captures = 0;
    black_captures = 0;
    move_counter = 0;
    number_of_legal_moves = 0;
    player_turn = colour::white;  // white goes first

    // flag if player can perform an en passant
    white_can_en_passant = false;
    black_can_en_passant = false;

    // flag if last move was a special move
    en_passant = false;
    castling = false;
    pawn_promotion = false;
    double_pawn = false;

    // store position of pieces to be en passant captured
    white_piece_pos_to_be_en_passant_captured = { 0,0 };
    black_piece_pos_to_be_en_passant_captured = { 0,0 };
}

// destructor to clear containers
game::~game()
{
    // clear all containers
    white_captured.clear();
    black_captured.clear();
    rounds.clear();
    save_castling.clear();
    save_en_passant.clear();
    save_double_pawn.clear();
    save_pawn_promotion.clear();
    save_white_can_en_passant.clear();
    save_black_can_en_passant.clear();
    save_white_piece_pos_to_be_en_passant_captured.clear();
    save_black_piece_pos_to_be_en_passant_captured.clear();
    record_double_step.clear();
    record_rook_castling.clear();
    record_king_castling.clear();

    game_board.reset_board();  // reset the board
}

// get the player action
bool game::start(const int& number_of_players, const int& difficulty)
{
    player_number = number_of_players;  // set the player number
    difficulty_level = difficulty;  // set the difficulty

    // if requested to load game
    if (load) {

        // get the file name
        std::string file_name;
        std::cout << "\nEnter the file to be loaded (no extension):";
        std::getline(std::cin, file_name);
        file_name += ".dat";

        load_game(file_name);  // load the game
        load = false;
    }

    // main game loop
    do{    
        game_board.print();  // print the game board
        current_state_of_game();  // print info about the game     

        // get the player action
        std::string action;
        std::cout << "(N)ew Game, (L)oad Game, (S)ave Game, (M)ove, (U)ndo, (Q)uit, (A)vailable moves, (H)elp\n";
        do {
            std::cout << "Enter an action:";
            std::getline(std::cin, action);
            to_lower_case(action);
            if (action != "n" && action != "l" && action != "s" && action != "m" && action != "u" && action != "q" && action != "h" && action != "a") {
                std::cerr << "Error: enter 'N' = New Game, 'L' = Load Game, 'S' = Save Game, 'M' = Move, 'U' = Undo, 'Q' = Quit, 'A' = Available moves, 'H' = Help\n";
            }
            if (action == "u" && move_counter == 0) std::cerr << "Error: there are no moves to undo\n";

        } while ((action == "u" && move_counter == 0) || (action != "n" && action != "l" && action != "s" && action != "m" 
            && action != "u" && action != "q" && action != "a" && action != "h"));

        // if requesting to display all available moves
        if (action == "a") {

            requesting_available_moves = true;

            std::vector<std::string> store_available_moves;
            store_available_moves = available_moves();

            // print out all the allowed moves if there are any 
            int counter{ 0 };
            if (number_of_legal_moves == 0) {
                std::cout << "There are no available moves for " << player_turn << ":";
            } else {
                std::cout << "\nThere are " << number_of_legal_moves << " legal moves available for " << player_turn << ":\n";
                for (auto available_moves_it = store_available_moves.begin(); available_moves_it < store_available_moves.end(); available_moves_it++) {
                    std::cout << std::setw(5) << "[" << *available_moves_it << "]";
                    counter++;
                    if (counter % 5 == 0) {
                        std::cout << "\n";
                    }
                }
            }
            std::cout << std::string(2, '\n');

            requesting_available_moves = false;
        }

        // if requesting to move, get the next move then alternate the turn
        else if (action == "m") {
            next_move();
            move_counter++;
            alternate_turn();
            std::cout << std::string(1, '\n');
        }

        // if requesting to quit
        else if (action == "q") {

            // confirm the action
            std::cout << "Are you sure you want to quit? (y/n):";
            if (confirm_action()) return false;
        }

        // if requesting to save the game
        else if (action == "s") {
        
            // get the file name
            std::string file_name;
            std::cout << "\nPlease enter the file name to save your game:";
            std::getline(std::cin, file_name);
            file_name += ".dat";  // saving to a .dat file

            save_game(file_name);
        }

        // if requesting to undo a move
        else if (action == "u") {

            if (first_round) std::cerr << "\nError: you have not made a move yet\n\n";
            else undo_move();

            // for a single player move, need undo both the player and computer move
            if (player_number == 1) undo_move();

            move_counter--;
        }

        // if requesting to load another game
        else if (action == "l") {

            // confirm the action
            std::cout << "Are you sure you want to load another game? (y/n):";
            if (confirm_action()) {
                std::cout << std::string(1, '\n');
                load = true;
                return true;
            }
        }

        // if requesting a new game
        else if (action == "n") {

            // confirm the action
            std::cout << "Are you sure you want to start a new game? (y/n):";
            if (confirm_action()) {
                std::cout << "\nNew Game:\n";
                return true;
            }
        }

        // if requesting to quit
        else if (action == "q") {
            
            // confirm the action
            std::cout << "Are you sure you want to quit? (y/n):";
            if(confirm_action()) return false;
        }

        // if requesting help, print the instructions again
        else if (action == "h") instructions();

        first_round = false;  // no longer first round

        // if it's a single player game, make the computer move
        if (number_of_players == 1 && action == "m") {

            if (is_game_over()) break;  // test game is not over

            double get_best_move = find_best_move(difficulty, 9999);  // find the best move

            // extract the inital and final positions
            std::size_t move_seperator{ best_move.find("-") };
            std::string initial_pos = best_move.substr(0, move_seperator);
            std::string final_pos = best_move.substr(move_seperator + 1);

            make_move(initial_pos, final_pos);  // make the move
            alternate_turn();  // alternate back to player turn
        }

    } while (!is_game_over());

    // if game is over, print the final game_board
    std::cout << "\nFinal game board:\n";
    game_board.print(); 

    // ask the user if they want another game 
    std::string another_game;
    std::cout << "Would you like another game? (y/n):";

    // validate input
    if (confirm_action()) {
        std::cout << "\nNew Game:\n";
        return true;
    }
    else return false;
}

// print the introduction
void game::introduction() const
{
    std::cout << "Welcome to OOP C++ Project - Chess!\n";
    instructions();
    std::cout << "Press enter to start the game";
    std::cin.ignore();
}

// displays the game instructions
void game::instructions() const
{
    std::cout << "\nInstructions:\n"
        << "Pieces are designated by W or B for colour, followed by a letter to denote the piece type:\n"
        << "    pawn = 'P', rook = 'R', knight = 'N', bishop = 'B', queen = 'Q' and king = 'K'\n"
        << "White always goes first\n"
        << "All advanced chess rules are in play, i.e double pawn advance, en passant, castling, pawn promotion.\n"
        << "Upon pressing enter you will be asked if you wish to play a one or two player game.\n"
        << "If a single player game is chosen you will be asked to select the difficulty: 'E' = easy, 'M' = medium, 'H' = hard\n"
        << "At the start of each turn you will be prompted for an 'action', type:\n"
        << "    'N' = start a new game,\n"
        << "    'L' = load a previously saved game,\n"
        << "    'S' = save the current game,\n"
        << "    'M' = make the next move (enter two co-ordinates; a letter (column) followed by a number (row) e.g \"A4\"),\n"
        << "    'U' = undo the last move,\n"
        << "    'Q' = quit the game,\n"
        << "    'A' = moves available,\n"
        << "    'H' = display these instructions again\n"
        << "The undo ability can be used in both game modes as many times as you like\n"
        << "You can only load a game in the same mode it was saved in, i.e cannot load a single player game in two player mode.\n\n";

}

// make a move
bool game::make_move(std::string& initial_pos_input, std::string& final_pos_input)
{
    // convert the string inputs to algebraic notation and test if it's a legal move
    position initial_pos{ convert_to_algebraic_notation(initial_pos_input) };
    position final_pos{ convert_to_algebraic_notation(final_pos_input) };

    // if piece is a pawn, rook or king, record their current ability to perform a special move incase the move attempt fails and we have to reset it
    std::shared_ptr<base_piece> piece{ game_board.get_piece(initial_pos) };
    char piece_type{ piece->get_type() };
    if (piece_type == 'P') {
        std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(piece);
        record_double_step.push_back(pawn_piece->get_double_step());
    } else if (piece_type == 'K') {
        std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(piece);
        record_king_castling.push_back(king_piece->get_castling());
    } else if (piece_type == 'R') {
        std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(piece);
        record_rook_castling.push_back(rook_piece->get_castling());
    }

    // reset the special move flags
    en_passant = false;
    castling = false;
    pawn_promotion = false;
    double_pawn = false;

    // perform a castling move if possible
    if (game_board.castling_valid(initial_pos, final_pos)) {

        // save info about en passant move
        if (player_turn == colour::white) {
            save_white_can_en_passant.push_back(white_can_en_passant);
            save_black_piece_pos_to_be_en_passant_captured.push_back(black_piece_pos_to_be_en_passant_captured);
            white_can_en_passant = false;
            black_piece_pos_to_be_en_passant_captured = { 0,0 };
        } else {
            save_black_can_en_passant.push_back(black_can_en_passant);
            save_white_piece_pos_to_be_en_passant_captured.push_back(white_piece_pos_to_be_en_passant_captured);
            black_can_en_passant = false;
            white_piece_pos_to_be_en_passant_captured = { 0,0 };
        }

        castling = true;  //  a castle move happened

        // record info about special moves
        save_castling.push_back(castling);
        save_double_pawn.push_back(double_pawn);
        save_en_passant.push_back(en_passant);
        save_pawn_promotion.push_back(pawn_promotion);

        // no captured pieces
        if (player_turn == colour::white) white_captured.push_back(nullptr);
        else if (player_turn == colour::black) black_captured.push_back(nullptr);

        log_move(initial_pos_input, final_pos_input);  // save the move 
        game_board.castling(initial_pos, final_pos);  // perform the castling move
        return true;
    }

    // test if player can en passant
    else if ((player_turn == colour::white && white_can_en_passant && game_board.en_passant_valid(initial_pos, final_pos, black_piece_pos_to_be_en_passant_captured)) 
        || (player_turn == colour::black && black_can_en_passant && game_board.en_passant_valid(initial_pos, final_pos, white_piece_pos_to_be_en_passant_captured))) {

        std::cout << "\nEn passant!\n";

        // save info about en passant move
        if (player_turn == colour::white) {
            save_white_can_en_passant.push_back(white_can_en_passant);
            save_black_piece_pos_to_be_en_passant_captured.push_back(black_piece_pos_to_be_en_passant_captured);
            white_can_en_passant = false;
            black_piece_pos_to_be_en_passant_captured = { 0,0 };
        } else {
            save_black_can_en_passant.push_back(black_can_en_passant);
            save_white_piece_pos_to_be_en_passant_captured.push_back(white_piece_pos_to_be_en_passant_captured);
            black_can_en_passant = false;
            white_piece_pos_to_be_en_passant_captured = { 0,0 };
        }

        // log the capture
        if (player_turn == colour::white) {
            position captured_piece_pos{ final_pos.x, initial_pos.y };
            white_captured.push_back(game_board.get_piece(captured_piece_pos));
        }
        if (player_turn == colour::black) {
            position captured_piece_pos{ final_pos.x, initial_pos.y };
            black_captured.push_back(game_board.get_piece(captured_piece_pos));
        }

        en_passant = true;  // en passant move happened

        // record info about special moves
        save_castling.push_back(castling);
        save_double_pawn.push_back(double_pawn);
        save_en_passant.push_back(en_passant);
        save_pawn_promotion.push_back(pawn_promotion);

        log_move(initial_pos_input, final_pos_input);  // save the move 
        game_board.en_passant(initial_pos, final_pos);  // perform en passant move

        return true;
    }

    // regular move
    // test if the move obeys rules for that piece
    else if (game_board.is_move_legal(initial_pos, final_pos)) {

        // test if there is an obstructing piece 
        bool obstructing_piece = game_board.obstructing_piece(initial_pos, final_pos);

        if (!obstructing_piece) {

            // test if the player is moving into check
            bool moving_into_check = game_board.moving_into_check(initial_pos, final_pos);
            if (!moving_into_check) {

                // store any captured pieces
                if (game_board.get_piece(final_pos) != nullptr) {
                    if (player_turn == colour::white) {
                        white_captures++;
                        white_captured.push_back(game_board.get_piece(final_pos));
                    } else if (player_turn == colour::black) {
                        black_captures++;
                        black_captured.push_back(game_board.get_piece(final_pos));
                    }
                }
                // if no captures, clear last capture
                else {
                    if (player_turn == colour::white) white_captured.push_back(nullptr);
                    else if (player_turn == colour::black) black_captured.push_back(nullptr);
                }

                // save info about en passant state to undo this move
                if (player_turn == colour::white) {
                    save_white_can_en_passant.push_back(white_can_en_passant);
                    save_black_piece_pos_to_be_en_passant_captured.push_back(black_piece_pos_to_be_en_passant_captured);
                } else {
                    save_black_can_en_passant.push_back(black_can_en_passant);
                    save_white_piece_pos_to_be_en_passant_captured.push_back(white_piece_pos_to_be_en_passant_captured);
                }

                // if the current move is a double pawn move, the opposite colour can en passant next turn
                if (game_board.double_pawn_move_valid(initial_pos, final_pos)) {
                    double_pawn = true;
                    if (player_turn == colour::white) {
                        black_can_en_passant = true;
                        white_piece_pos_to_be_en_passant_captured = final_pos;
                    } else {
                        white_can_en_passant = true;
                        black_piece_pos_to_be_en_passant_captured = final_pos;
                    }
                }
                // if it isn't a double pawn move, no en passant can happen next turn
                else {
                    if (player_turn == colour::white) {
                        black_can_en_passant = false;
                        white_piece_pos_to_be_en_passant_captured = { 0,0 };
                    } else {
                        white_can_en_passant = false;
                        black_piece_pos_to_be_en_passant_captured = { 0,0 };
                    }
                }

                game_board.move_piece(initial_pos, final_pos);  // make the move

                // test for a pawn promoation
                if (!requesting_available_moves && game_board.pawn_promotion_valid(final_pos)) {

                    pawn_promotion = true;
                    std::string promoted_piece_type;

                    // player promotion
                    if (player_number == 2 || (player_number == 1 && player_turn == colour::white)) {
                        std::cout << "\nPawn promotion!\nWould you like to promote your pawn? (y/n):";
                        if (confirm_action()) {
                            // get promoted piece
                            promoted_piece_type = game_board.pawn_promoation(final_pos, player_turn, player_number);
                            std::shared_ptr<base_piece> promoted_piece{ game_board.get_piece(final_pos) };

                            // save promoted piece ability to special move
                            if (promoted_piece_type == "P") {
                                std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(promoted_piece);
                                record_double_step.push_back(pawn_piece->get_double_step());
                            } else if (promoted_piece_type == "K") {
                                std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(promoted_piece);
                                record_king_castling.push_back(king_piece->get_castling());
                            } else if (promoted_piece_type == "R") {
                                std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(promoted_piece);
                                record_rook_castling.push_back(rook_piece->get_castling());
                            }
                        }
                    }
                    // computer promotion
                    else {
                        // get promoted piece
                        promoted_piece_type = game_board.pawn_promoation(final_pos, player_turn, player_number);
                        std::shared_ptr<base_piece> promoted_piece{ game_board.get_piece(final_pos) };

                        // save promoted piece ability to special move
                        if (promoted_piece_type == "P") {
                            std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(promoted_piece);
                            record_double_step.push_back(pawn_piece->get_double_step());
                        } else if (promoted_piece_type == "K") {
                            std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(promoted_piece);
                            record_king_castling.push_back(king_piece->get_castling());
                        } else if (promoted_piece_type == "R") {
                            std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(promoted_piece);
                            record_rook_castling.push_back(rook_piece->get_castling());
                        }
                    }

                }

                log_move(initial_pos_input, final_pos_input);  // save the move 

                // save all info about special moves
                save_castling.push_back(castling);
                save_double_pawn.push_back(double_pawn);
                save_en_passant.push_back(en_passant);
                save_pawn_promotion.push_back(pawn_promotion);

                return true;
            } else {

                if (in_check) std::cerr << "Error: you must move out of check\n";  // if already in check, must move out
                else std::cerr << "Error: you cannot move into check\n";  // if moving into check
            }
        }
        else std::cerr << "Error: there is a another piece in the way\n";
    }
    else game_board.custom_statement(initial_pos);  // no valid move available

    // move failed, so reset pieces ability to perform special moves
    if (piece_type == 'P') {
        std::shared_ptr<pawn> pawn_piece = std::dynamic_pointer_cast<pawn>(piece);
        bool reset_double_step = record_double_step.back();
        record_double_step.pop_back();
        pawn_piece->set_double_step(reset_double_step);
    } else if (piece_type == 'K') {
        std::shared_ptr<king> king_piece = std::dynamic_pointer_cast<king>(piece);
        bool reset_king_castling = record_king_castling.back();
        record_king_castling.pop_back();
        king_piece->set_castling(reset_king_castling);
    } else if (piece_type == 'R') {
        std::shared_ptr<rook> rook_piece = std::dynamic_pointer_cast<rook>(piece);
        bool reset_rook_castling = record_rook_castling.back();
        record_rook_castling.pop_back();
        rook_piece->set_castling(reset_rook_castling);
    }

    return false;
}

// check if a player has won
bool game::is_game_over()
{
    bool possible_move{ game_board.legal_move_available(player_turn, white_can_en_passant, black_can_en_passant,
        white_piece_pos_to_be_en_passant_captured, black_piece_pos_to_be_en_passant_captured) };

    // test if there is a possible move for the player
    if (!possible_move) {
        // if there is not test for a check mate
        if (game_board.is_in_check(player_turn)) {
            alternate_turn();
            if (player_turn == colour::white) std::cout << "\nCheckmate, white wins!\n";
            if (player_turn == colour::black) std::cout << "\nCheckmate, black wins!\n";
        }
        // otherwise it's a stalemate
        else {
            std::cout << "Stalemate!" << std::endl;
        }
        return true; // quit game
    }
    // if there is a possible move, test if player is currently in check
    else if (game_board.is_in_check(player_turn)) {
        if (player_turn == colour::white) std::cout << "\nBlack has placed white in check!\n\n";
        if (player_turn == colour::black) std::cout << "\nWhite has placed black in check!\n\n";
        in_check = true;
    }
    // if not in check
    else in_check = false;

    return false;
}

// validate the input is in algebraic notation
bool game::validate_input(const std::string& input) const
{
    bool validate_input{ false };

    // check length of input is correct
    if (input.length() != 2) {
        std::cerr << "Error: co-ord is not of the correct length" << std::endl;
        return validate_input;
    }

    // if correct length, check formating if correct i.e (letter,number)   
    std::stringstream input_stream{ input };  // convert to string stream  
    char stream_char_letter; char stream_char_number;  // characters to read from stringstream
    std::regex regex_pattern_numbers_all("[0-9]");  // regex pattern for all numbers
    std::regex regex_pattern_numbers_restricted("[1-8]");  // regex pattern for allowed numbers
    std::regex regex_pattern_letters_all("[a-z]");  // regex pattern for allowed letters
    std::regex regex_pattern_letters_restricted("[a-h]");  // regex pattern for all letters

    // loop over string and validate each character
    while (!input_stream.eof()) {
        input_stream >> stream_char_letter;
        std::string single_string_letter(1, stream_char_letter);
        if (std::regex_match(single_string_letter, regex_pattern_letters_all)) {
            input_stream >> stream_char_number;
            std::string single_string_number(1, stream_char_number);
            if (std::regex_match(single_string_number, regex_pattern_numbers_all)) {

                // if position is valid
                if (std::regex_match(single_string_letter, regex_pattern_letters_restricted) 
                    && std::regex_match(single_string_number, regex_pattern_numbers_restricted)) {
                    validate_input = true;
                    return validate_input;
                }

                // if position out of board range 
                else {
                    std::cerr << "Error: board position is out of bounds" << std::endl;
                    return validate_input;
                }
            }
        }
    }

    std::cerr << "Error: input is not formatted correctly, please input a letter followed by a number" << std::endl;  // if formatting is incorrect
    return validate_input;
}

// confirm the action has been inputted correctly
bool game::confirm_action() const
{
    // confirm the action
    std::string input;
    do {
        std::getline(std::cin, input);
        to_lower_case(input);  // not case sensitive
        if (input != "y" && input != "n") {
            std::cerr << "Error: please input 'Y' for another game or 'N' to quit:";
        }
    } while (input != "y" && input != "n");

    if (input == "y") return true;
    else return false;
}

// save the game to a .dat file
void game::save_game(const std::string& file_name) 
{
    // open the file for writing
    std::ofstream output_file(file_name);
    if (output_file.is_open()) {

        // write the date and time of the save
        auto now = std::chrono::system_clock::now();
        output_file << "[Chess console] Saved at: " << std::time_t(&now) << "\n";

        // record the number of players in the game
        auto number_of_players = std::to_string(player_number);
        output_file << number_of_players << "\n";

        // write all the moves  to the file
        for (size_t i{ 0 }; i < rounds.size(); i++)
        {
            output_file << rounds[i].white_move.c_str() << "|" << rounds[i].black_move.c_str() << "\n";
        }

        output_file.close();  // close the file
        std::cout << "Game saved as " << file_name << "\n\n";
    }
    // if opening file fails
    else {
        std::cerr << "Error: file could not be created, save failed\n\n";
    }
}

// load a game from a .dat file
void game::load_game(const std::string& file_name) 
{
    // open the file for reading
    std::ifstream input_file(file_name);
    if (input_file.is_open()) {

        std::string line;  // reading in each line
        std::getline(input_file, line);  // skip reading header line

        // check the game is for the correct number of players
        std::getline(input_file, line);
        if (line == std::to_string(player_number)) {

            // read in each line of the file, seperated by a carriage return
            while (std::getline(input_file, line, '\n')) {

                // there might be one or two moves in the line
                std::string loaded_move[4];

                // define the seperators
                std::size_t move_seperator{ line.find("-") };
                std::size_t player_seperator{ line.find("|") };

                // read in white move
                loaded_move[0] = line.substr(0, move_seperator);
                loaded_move[1] = line.substr(move_seperator + 1, player_seperator - 3);

                // try to read in black move
                bool no_black_move{ false };
                try {
                    loaded_move[2] = line.substr(player_seperator + 1, move_seperator);
                    loaded_move[3] = line.substr(move_seperator + 7);
                }
                // if no black move
                catch (const std::out_of_range&) {
                    no_black_move = true;
                }

                // validate the white move
                if (validate_input(loaded_move[0]) && validate_input(loaded_move[1])) {

                    make_move(loaded_move[0], loaded_move[1]);  // make the move
                    first_round = false;  // one move made so no longer first round
                    move_counter++;
                    alternate_turn();

                    // if there are black moves
                    if (!no_black_move) {

                        // validate black move
                        if (validate_input(loaded_move[2]) && validate_input(loaded_move[3])) {
                            make_move(loaded_move[2], loaded_move[3]);  // make the move
                            move_counter++;
                            alternate_turn();
                        }
                        // invalid black moves
                        else {
                            std::cerr << "Error: file contains invalid black moves\n\n" << std::endl;
                            exit(1);
                        }
                    }
                }
                // invalid white moves
                else {
                    std::cerr << "Error: file contains invalid white moves\n\n" << std::endl;
                    exit(1);
                }
            }

        input_file.close();  // close the file
        std::cout << "Game loaded successfully " << file_name << "\n\n";

        } else std::cerr << "Error: cannot load a " << line << " player game for " << player_number << " player\n\n";
    }
    // if opening file fails
    else {
        std::cerr << "Error: file could not be opened, load failed\n\n";
    }
}

// switch the player turn
void game::alternate_turn()
{
    player_turn = (player_turn == colour::white) ? colour::black : colour::white;
}

// print the current state of the game
void game::current_state_of_game() const
{
    // if at least one round has been played
    if (rounds.size() > 0) {

        // print recent moves
        std::cout << "Last moves (white | black):\n";
        size_t moves = rounds.size();  // number of moves made
        size_t moves_to_show = moves >= 5 ? 5 : moves;  // show a max 5 past moves
        std::string space = "";
        while (moves_to_show--)
        {
            if (moves < 10) space = " ";  // Add an extra hardspace to allign the numbers that are smaller than 10
            std::cout << space << moves << std::string(5, '.') << rounds[moves - 1].white_move.c_str() << " | " << rounds[moves - 1].black_move.c_str() << "\n";
            moves--;
        }
        std::cout << "\n";
    }

    // output captured pieces
    if (white_captures > 0 || black_captures > 0) {

        std::cout << std::string(35, '-') << "\n";

        // white captures
        std::cout << "WHITE captured: ";
        for (auto captured_vector_it = white_captured.begin(); captured_vector_it < white_captured.end(); captured_vector_it++) {
            if (*captured_vector_it != nullptr) std::cout << (*captured_vector_it)->get_type() << " ";
        }
        std::cout << "\n";

        // black captures
        std::cout << "BLACK captured: ";
        for (auto captured_vector_it = black_captured.begin(); captured_vector_it < black_captured.end(); captured_vector_it++) {
            if (*captured_vector_it != nullptr) std::cout << (*captured_vector_it)->get_type() << " ";
        }
        std::cout << "\n";

        std::cout << std::string(35, '-') << "\n";
    }

    // display current players turn
    if (player_turn == colour::white) std::cout << "Whites' turn\n\n";
    else std::cout << "Blacks' turn\n\n";
}

// get the next move from the user
void game::next_move()
{
    std::string initial_pos_input;
    std::string final_pos_input;

    // validate the move
    bool valid_move{ false };
    do {
        // validate initial postion
        bool valid_input_initial{ false };
        do {
            // get initial co-ords
            std::cout << "\nEnter co-ords of piece to move (letter, number):";
            std::getline(std::cin, initial_pos_input);
            to_lower_case(initial_pos_input);  // non-case sensitive

            // test for valid board position
            if (validate_input(initial_pos_input)) {
                position initial_pos{ convert_to_algebraic_notation(initial_pos_input) };  // convert to chess notation

                // test for correct coloured piece at initial position
                if (game_board.get_piece(initial_pos) == nullptr) {
                    std::cerr << "Error: no piece at this position\n";
                    valid_input_initial = false;
                } else if (game_board.get_piece(initial_pos)->get_colour() != player_turn) {
                    std::cerr << "Error: wrong colour piece selected\n";
                    valid_input_initial = false;
                } else valid_input_initial = true;

                // if initial input is valid
                if (valid_input_initial) {

                    // validate final position
                    bool valid_input_final{ false };
                    do {
                        // get final co-ords
                        std::cout << "Enter co-ords of position to move to (letter, number):";
                        std::getline(std::cin, final_pos_input);
                        to_lower_case(final_pos_input);  // non-case sensitive

                        if (initial_pos_input == final_pos_input) std::cerr << "Error: please select different co-ordinates\n";  // need different intitial and final co-ords

                        // test for valid board position
                        else if (validate_input(final_pos_input)) {

                            // check not trying to capture own piece
                            position final_pos{ convert_to_algebraic_notation(final_pos_input) };  // convert to chess notation
                            if (game_board.get_piece(final_pos) != nullptr && game_board.get_piece(final_pos)->get_colour() == player_turn)
                                std::cerr << "Error: you cannot capture your own piece\n";

                            else valid_input_final = true;  // final input is valid
                        }
                    } while (!valid_input_final);
                }
            }
        } while (!valid_input_initial);

        valid_move = make_move(initial_pos_input, final_pos_input);  // attempt to make the move
    } while (!valid_move);
}

// undo the last move made
void game::undo_move() 
{
    alternate_turn();

    // get the last round
    round last_round;
    last_round = rounds.back();
    rounds.pop_back();

    // get the last move
    std::string last_move;
    if (player_turn == colour::white) last_move = last_round.white_move;
    else {
        last_move = last_round.black_move;
        // for a black move we need to add back on the valid white move also contained in the round we removed
        last_round.black_move = "";
        rounds.push_back(last_round);   
    }

    // get initial and final positions
    std::string initial_pos_string{ last_move.substr(0, 2) };
    std::string final_pos_string{ last_move.substr(3) };

    // convert to algebraic notation
    position initial_pos{ convert_to_algebraic_notation(initial_pos_string) };
    position final_pos{ convert_to_algebraic_notation(final_pos_string) };

    // get the player piece to reset and the potential opponent captured piece to replace
    std::shared_ptr<base_piece> player_piece{ game_board.get_piece(final_pos) };
    std::shared_ptr<base_piece> captured_piece;
    if (player_turn == colour::white) {
        captured_piece = white_captured.back(); 
        white_captured.pop_back();  
    } else if (player_turn == colour::black) {
        captured_piece = black_captured.back();
        black_captured.pop_back(); 
    }

    // get info about special moves
    castling = save_castling.back();
    save_castling.pop_back();
    en_passant = save_en_passant.back();
    save_en_passant.pop_back();
    pawn_promotion = save_pawn_promotion.back();
    save_pawn_promotion.pop_back();
    double_pawn = save_double_pawn.back();
    save_double_pawn.pop_back();

    // initialise special move booleans to false
    bool double_step_status_before_move{ false };
    bool king_castling_status_before_move{ false };
    bool rook_castling_status_before_move{ false };

    // overwrite them if relevant, if not they won't be uesd anyway
    char piece_type{ player_piece->get_type() };
    if (piece_type == 'P') {
        double_step_status_before_move = record_double_step.back();
        record_double_step.pop_back();
    } else if (piece_type == 'K') {
        king_castling_status_before_move = record_king_castling.back();
        record_king_castling.pop_back();
    } else if (piece_type == 'R') {
        rook_castling_status_before_move = record_rook_castling.back();
        record_rook_castling.pop_back();
    }

    // undo the move
    game_board.undo_move(initial_pos, final_pos, player_piece, captured_piece, en_passant, castling, pawn_promotion, double_pawn, player_turn,
        double_step_status_before_move, king_castling_status_before_move, rook_castling_status_before_move);

    // restore the en_passant flags
    if (player_turn == colour::white) {
        white_can_en_passant = save_white_can_en_passant.back();
        save_white_can_en_passant.pop_back();
        black_piece_pos_to_be_en_passant_captured = save_black_piece_pos_to_be_en_passant_captured.back();
        save_black_piece_pos_to_be_en_passant_captured.pop_back();
    } else {
        black_can_en_passant = save_black_can_en_passant.back();
        save_black_can_en_passant.pop_back();
        white_piece_pos_to_be_en_passant_captured = save_white_piece_pos_to_be_en_passant_captured.back();
        save_white_piece_pos_to_be_en_passant_captured.pop_back();
    }

    // if undoing a capture, need to remove it from the captured pieces vector
    if (captured_piece != nullptr) {
        char captured_piece_type = captured_piece->get_type();

        // iterate through vectors and remove the captured piece
        if (player_turn == colour::white) { 
            white_captures--;
            for (auto captured_vector_it = white_captured.begin(); captured_vector_it < white_captured.end(); captured_vector_it++) {
                if (*captured_vector_it == captured_piece) {
                    captured_vector_it = white_captured.erase(captured_vector_it);
                    break;
                }
            }
        } else {   
            black_captures--;
            for (auto captured_vector_it = black_captured.begin(); captured_vector_it < black_captured.end(); captured_vector_it++) {
                if (*captured_vector_it == captured_piece) {
                    captured_vector_it = black_captured.erase(captured_vector_it);
                    break;
                }
            }
       }    
    }
}

// make input non-case sensitive
void game::to_lower_case(std::string& input) const
{
    for (size_t i{ 0 }; i < input.length(); i++) input[i] = tolower(input[i]);
}

// record the last move
void game::log_move(const std::string& initial_pos, const std::string& final_pos)
{
    std::string save_move{ initial_pos + "-" + final_pos };

    // if it's a white move
    if (player_turn == colour::white) {
        // create a new round, add the move, leave the black_move empty
        round last_round;
        last_round.white_move = save_move;
        last_round.black_move = "";
        rounds.push_back(last_round);
    }
    // if it's a black move
    else {
        // get the last round and update it
        round last_round{ rounds[rounds.size() - 1] };
        last_round.black_move = save_move;

        // Remove the last round and put it back, now with the black move
        rounds.pop_back();
        rounds.push_back(last_round);
    }
}

// list all players available moves
std::vector<std::string> game::available_moves()
{
    position piece_pos{ 0,0 };
    position move_to_pos{ 0,0 };
    number_of_legal_moves = 0;
    std::vector<std::string> store_available_moves;

    // loop over board to find all player pieces
    for (size_t x{ 0 }; x < 8; x++) {
        piece_pos.x = x;
        for (size_t y{ 0 }; y < 8; y++) {
            piece_pos.y = y;

            std::shared_ptr<base_piece> player_piece{ game_board.get_piece(piece_pos) };

            // if piece exists and is the appropriate colour
            if (player_piece != nullptr && player_piece->get_colour() == player_turn) {

                // loop over board to test all moves for that piece
                for (size_t x{ 0 }; x < 8; x++) {
                    move_to_pos.x = x;
                    for (size_t y{ 0 }; y < 8; y++) {
                        move_to_pos.y = y;

                        // convert the positions to strings
                        std::string initial_pos; std::string final_pos;
                        initial_pos = pos_to_string(piece_pos);
                        final_pos = pos_to_string(move_to_pos);

                        // set failstate of ouput to suppress it while running checks for legal moves
                        std::cout.setstate(std::ios_base::failbit);
                        std::cerr.setstate(std::ios_base::failbit);

                        // if there is a legal move, save it then undo it

                        if (initial_pos == "c7" && final_pos == "b8") {
                            int x{0};
                            x++;
                        }

                        if (make_move(initial_pos, final_pos)) {
                            std::string save_move{ initial_pos + "-" + final_pos };
                            store_available_moves.push_back(save_move);
                            number_of_legal_moves++;
                            alternate_turn();
                            undo_move();
                        }

                        // re-enable output
                        std::cout.clear();
                        std::cerr.clear();
                    }
                }
            }
        }
    }

    return store_available_moves;
}

// convert input to algebraic notation
position game::convert_to_algebraic_notation(std::string& input) const
{
    position input_pos;
    input_pos.x = (int)input[0] - 97;  // cast letter char to an int and subtract 97 to make acii
    input_pos.y = atoi(&input[1]) - 1;  // cast number char to an int and subtract 1 so it's based at 0
    return input_pos;
}

// convert positions to strings
std::string game::pos_to_string(const position& pos) const
{
    std::string pos_string;

    // convert the x co-ord
    if (pos.x == 0) pos_string += "a";
    else if (pos.x == 1) pos_string += "b";
    else if (pos.x == 2) pos_string += "c";
    else if (pos.x == 3) pos_string += "d";
    else if (pos.x == 4) pos_string += "e";
    else if (pos.x == 5) pos_string += "f";
    else if (pos.x == 6) pos_string += "g";
    else if (pos.x == 7) pos_string += "h";

    // convert the y co-ord
    if (pos.y == 0) pos_string += "1";
    else if (pos.y == 1) pos_string += "2";
    else if (pos.y == 2) pos_string += "3";
    else if (pos.y== 3) pos_string += "4";
    else if (pos.y == 4) pos_string += "5";
    else if (pos.y == 5) pos_string += "6";
    else if (pos.y == 6) pos_string += "7";
    else if (pos.y == 7) pos_string += "8";

    return pos_string;
}

// function to find the best move for the computer
double game::find_best_move(const int& probe_level, const double& previous_evaluation) 
{
    // if we're at the top of the minmax tree, clear the best move
    if (probe_level == difficulty_level) best_move.clear();

    double best_value;
    if (player_turn == colour::white) best_value = -9999;  // maximimising white player
    else best_value = 9999;  // minimising black player

    double find_best_value{ 0 };

    std::vector<std::string> store_available_moves;
    store_available_moves = available_moves();  // get all available moves

    // loop through all available moves
    for (auto available_moves_it = store_available_moves.begin(); available_moves_it < store_available_moves.end(); available_moves_it++) {

        double board_value{ 0 };  // initialise a value we will attempt to maximise

        // extract the inital and final positions
        std::string current_move = *available_moves_it;
        std::size_t move_seperator{ current_move.find("-") };
        std::string initial_pos = current_move.substr(0, move_seperator);
        std::string final_pos = current_move.substr(move_seperator + 1);

        // set failstate of ouput to suppress it while running checks for legal moves
        std::cout.setstate(std::ios_base::failbit);
        std::cerr.setstate(std::ios_base::failbit);

        make_move(initial_pos, final_pos); 
        board_value = evaluate_board();  // evalute board after move has been made

        // if evaluation at this level is at least as good as the previous
        if ((player_turn == colour::black && board_value <= previous_evaluation) || (player_turn == colour::white && board_value >= previous_evaluation)) {
            
            // if not at bottom level, go one level deeper
            if (probe_level != 1) {
                alternate_turn();
                find_best_value = find_best_move(probe_level - 1, board_value);
                alternate_turn();

                // if new move is a better move, update our best move
                if ((player_turn == colour::black && find_best_value < best_value) || (player_turn == colour::white && find_best_value > best_value)) {
                    best_value = board_value;
                    if (probe_level == difficulty_level) best_move = current_move;  // if at top of minmax tree, also set the move 
                }
            } else if ((player_turn == colour::black && board_value < best_value) || player_turn == colour::white && board_value > best_value) {
                best_value = board_value;
                if (probe_level == difficulty_level) best_move = current_move;  // if at top of minmax tree, also set the move
            }
        }

        // undo the move
        alternate_turn();
        undo_move();

        // re-enable output
        std::cout.clear();
        std::cerr.clear();
    }

    store_available_moves.clear();  // clear the vector

    if (probe_level == difficulty_level) {
        if (player_turn == colour::white) alternate_turn();  // reset to correct turn
        return 0;
    } else return best_value;
}

// determine the value of the board state
double game::evaluate_board() const
{
    double board_value{ 0 };
    position current_position;
    std::shared_ptr<base_piece> piece;

    // loop over game board
    for (size_t row{ 0 }; row < 8; row++) {
        for (size_t col{ 0 }; col < 8; col++) {

            // if piece exists at current position, increment the board value
            current_position.x = col, current_position.y = row;
            piece = game_board.get_piece(current_position);
            if (piece != nullptr) board_value += evalute_piece(piece, current_position);
        }
    }

    return board_value;
}

// return the value of a piece
double game::evalute_piece(const std::shared_ptr<base_piece>& piece, const position& piece_pos) const
{
    double evaluate_piece{ 0 };
    char piece_type{ piece->get_type() };
    colour piece_colour{ piece->get_colour() };

    // assign a weight to each piece
    if (piece_type == 'P') evaluate_piece = 10;
    else if (piece_type == 'R') evaluate_piece = 50;
    else if (piece_type == 'N') evaluate_piece = 30;
    else if (piece_type == 'B') evaluate_piece = 30;
    else if (piece_type == 'Q') evaluate_piece = 90;
    else if (piece_type == 'K') evaluate_piece = 900;

    evaluate_piece += evaluate_position(piece_type, piece_pos, piece_colour);  // evalaute board position

    return piece_colour == colour:: white ? evaluate_piece : -evaluate_piece;  // maximising white, minimising black
}

// adjust evaluation to take into account the positions of the pieces
double game::evaluate_position(const char& piece_type, const position& piece_pos, const colour& piece_colour) const
{
    // white pawn position evaluation array
    double white_pawn_pos_eval[8][8]{
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
        {5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
        {1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
        {0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
        {0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
        {0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
        {0.5,  1.0, 1.0,  -2.0, -2.0,  1.0,  1.0,  0.5},
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}
    };

    // pawn position evaluation array
    double black_pawn_pos_eval[8][8]{
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
        {0.5,  1.0, 1.0,  -2.0, -2.0,  1.0,  1.0,  0.5},
        {0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
        {0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
        {0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
        {1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
        {5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}
    };

    // white king position evaluation array
    double white_king_pos_eval[8][8]{
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
        {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
        {2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
        {2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}
    };

    // black king position evaluation array
    double black_king_pos_eval[8][8]{
        {2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0},
        {2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
        {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
        {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
        {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
    };
        
    // white rook position evaluation array
    double white_rook_pos_eval[8][8]{
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
        {0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {0.0,   0.0, 0.0,  0.5,  0.5,  0.0,  0.0,  0.0}
    };

    // black rook position evaluation array
    double black_rook_pos_eval[8][8]{
        {0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
        {0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
        {0.0,   0.0, 0.0,  0.5,  0.5,  0.0,  0.0,  0.0}
    };

    // white bishop position evaluation array
    double white_bishop_pos_eval[8][8]{
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
        {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
        {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
        {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
        {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
        {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
        {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}
    };

    // black bishop position evaluation array
    double black_bishop_pos_eval[8][8]{
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
        {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
        {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
        {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
        {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
        {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
        {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
        {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}
    };

    // queen position evaluation array
    double queen_pos_eval[8][8]{
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
        {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
        {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
        {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
        {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
        {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
        {-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
        {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}
    };

    // knight position evaluation array
    double knight_pos_eval [8][8]{
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
        {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
        {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
        {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
        {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
        {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
        {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
        {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}
    };

    if (piece_type == 'P') return piece_colour == colour::white ? white_pawn_pos_eval[piece_pos.x][piece_pos.y] : black_pawn_pos_eval[piece_pos.x][piece_pos.y];
    else if (piece_type == 'R') return piece_colour == colour::white ? white_rook_pos_eval[piece_pos.x][piece_pos.y] : black_rook_pos_eval[piece_pos.x][piece_pos.y];
    else if (piece_type == 'B') return piece_colour == colour::white ? white_bishop_pos_eval[piece_pos.x][piece_pos.y] : black_bishop_pos_eval[piece_pos.x][piece_pos.y];
    else if (piece_type == 'K') return piece_colour == colour::white ? white_king_pos_eval[piece_pos.x][piece_pos.y] : black_king_pos_eval[piece_pos.x][piece_pos.y];
    else if (piece_type == 'N') return knight_pos_eval[piece_pos.x][piece_pos.y];
    else return queen_pos_eval[piece_pos.x][piece_pos.y];
}
