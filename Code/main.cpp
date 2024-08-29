// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: main.cpp

// Description
//	Implemenation of a chess game
//	The program makes use of key OOP principles e.g abstraction, inheritance, encapsulation and polymorphism
//	Advanced chess rules are in play i.e castling, en passant, double pawn move and pawn promotion
//  The program has additional quality of life features such as save/load game, undo move or list all available moves for your colour
//  There is both a one and two player mode
//  The single player mode makes use of a fairly sophisticated chess AI whose difficulty can be set at the beginning of the game

// Includes
#include "Game.h" // Chess game class

// Global variables
board game_board;
bool load{false}; // to test for a load game scenario

// Function declaration
int number_of_players();
int select_difficulty();

// Begin main program
int main()
{

	int number_of_games{0};
	int player_number;
	int difficulty;

	// loop for multiple games
	bool another_game;
	do
	{
		game chess; // instantiate a chess game

		// only print intro on the first game
		if (number_of_games == 0)
		{
			chess.introduction();
			std::cout << std::string(1, '\n');
		}

		player_number = number_of_players(); // get the number of players

		// if single player game, get the difficulty level
		if (player_number == 1)
			difficulty = select_difficulty();
		else
			difficulty = 1;

		another_game = chess.start(player_number, difficulty); // start the game
		number_of_games++;
	} while (another_game);

	std::cout << std::string(1, '\n');
	std::cout << "Thanks for playing!" << std::endl;

	return 0;
} // End of main program

// ask user if they want a one or two player game
int number_of_players()
{
	std::string number_of_players;
	int int_number_of_players;

	do
	{
		// get the number of players
		std::cout << "Number of players (1/2):";
		std::getline(std::cin, number_of_players);

		// check it is a valid integer
		if (number_of_players != "1" && number_of_players != "2")
		{
			std::cerr << "Error: Please enter '1' for single player or '2' for two players\n";
		}

	} while (number_of_players != "1" && number_of_players != "2");

	int_number_of_players = std::stoi(number_of_players); // convert string to integer

	if (int_number_of_players == 2)
		std::cout << "Player vs Player\n\n";
	else
		std::cout << "Player vs Computer\n\n";

	return int_number_of_players;
}

// ask the user for the difficulty level
int select_difficulty()
{
	std::string difficulty_str;
	int difficulty_int;

	do
	{
		// get the difficulty
		std::cout << "Difficulty level [(E)asy, (M)edium, (H)ard]:";
		std::getline(std::cin, difficulty_str);

		// make non-case sensitive
		for (size_t i{0}; i < difficulty_str.length(); i++)
		{
			difficulty_str[i] = tolower(difficulty_str[i]);
		}

		// check input is valid
		if (difficulty_str != "e" && difficulty_str != "m" && difficulty_str != "h")
		{
			std::cerr << "Error: Please enter 'E' = Easy, 'M' = Medium, 'H' = Hard\n";
		}

	} while (difficulty_str != "e" && difficulty_str != "m" && difficulty_str != "h");

	// convert to integer
	if (difficulty_str == "e")
	{
		difficulty_int = 1;
		std::cout << "Easy mode selected!\n\n";
	}
	else if (difficulty_str == "m")
	{
		difficulty_int = 2;
		std::cout << "Medium mode selected!\n\n";
	}
	else
	{
		difficulty_int = 3;
		std::cout << "Hard mode selected!\n\n";
	}

	return difficulty_int;
}