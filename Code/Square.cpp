// Object Orientated Programming in C++
// Final Project
// Name: Christopher Robert Kitching
// Student ID: 10134621
// E-mail: christopher.kitching@student.manchester.ac.uk
// Date created: 24/03/20
// Last edited: 05/05/20

// File: Square.cpp

// Includes
#include <iostream>
#include "Square.h"	// Header file for this class

// return piece on a chosen square
std::shared_ptr<base_piece> square::get_piece()
{
	return chess_piece;
}

// set piece on a chosen square
void square::set_piece(std::shared_ptr<base_piece> piece)
{
	chess_piece = piece;
}

// clear a chosen square
void square::clear()
{
	chess_piece = nullptr;
}