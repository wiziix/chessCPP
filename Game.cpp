#include "Game.h"
#include "Board.h"
#include "Pieces.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <deque>


Game::~Game() = default;

void Game::startNewGame(std::string p1_name, std::string p2_name) {

	std::cout << "Starting a new game..." << std::endl;

	// Initialize players
	p1 = new Player(p1_name, WHITE);
	p2 = new Player(p2_name, BLACK);
	
	// Initialize board
	board = new Board();
	rnd = new Round;

	std::cout << "Player " << p1->getName() << "'s turn" << std::endl;
	board->printBoard(isWhiteTurn);
}

void Game::makeMove() {

	int firstMove[2], lastMove[2];
	std::string start_move, ending_move;
	std::regex reg("^[a-h][1-8]$");
	std::smatch match;

	// Prompt the user for the starting square
	std::cout << "Enter starting square (e.g. e2): ";
	std::cin >> start_move;

	// Prompt the user for the ending square
	std::cout << "Enter ending square (e.g. e4): ";
	std::cin >> ending_move;

	// Check if the entered moves are valid using regex
	if (regex_search(start_move, match, reg) && regex_search(ending_move, match, reg)) {
		// Convert start and end positions from string to board indices
		firstMove[0] = start_move[0] - 'a';
		firstMove[1] = start_move[1] - '1';
		std::pair<int, int> firstVect = std::make_pair(firstMove[1], firstMove[0]);

		lastMove[0] = ending_move[0] - 'a';
		lastMove[1] = ending_move[1] - '1';
		std::pair<int, int> lastVect = std::make_pair(lastMove[1], lastMove[0]);

		Piece* p = board->getPiece(firstMove[1], firstMove[0]);
		Color turn = isWhiteTurn ? WHITE : BLACK;

		// Check if the starting position is valid and if the piece belongs to the current player
		if (p == nullptr || p->getColor() != turn) {
			std::cout << "Invalid piece!" << std::endl;
		}
		else {
			// Check if the starting piece is a KING
			if (p->getType() == KING) {
				Piece* r = board->getPiece(lastMove[1], lastMove[0]);

				// Check if the ending piece is valid and if it is a ROOK
				if (r != nullptr && r->getType() == ROOK) {
					// Check if castling is possible
					if (isCastlingPossible(firstVect, lastVect)) {
						// Perform castling and add new round to round deque
						board->Castle(firstVect, lastVect);
						rnd->number++;
						rnd->move = start_move + " " + ending_move;
						rounds.push_back(rnd);

						// Switch turns
						isWhiteTurn = !isWhiteTurn;

						std::cout << "Player " << (isWhiteTurn ? p1->getName() : p2->getName()) << "'s turn" << std::endl;
						board->printBoard(isWhiteTurn);

						checkGameState(&turn);

						return;
					}
				}
			}

			// Check if the move is valid and if the path is clear
			if (p->isMovePossible(firstVect, lastVect, *board) && board->isPathClear(firstVect, lastVect)) {
				board->movePiece(firstVect, lastVect);
				if (canPromotePawn(lastVect)) {
					int choice;

					std::cout << "Pawn can be promoted." << std::endl;
					std::cout << "Choose a piece: 1. Rook, 2. Knight, 3. Bishop, 4. Queen" << std::endl;

					std::cin >> choice;

					Type type;

					switch (choice) {
					case 1:
						type = ROOK;
						board->promotePawn(lastVect, &type, &turn);
						break;

					case 2:
						type = KNIGHT;
						board->promotePawn(lastVect, &type, &turn);
						break;

					case 3:
						type = BISHOP;
						board->promotePawn(lastVect, &type, &turn);
						break;
					case 4:
						type = QUEEN;
						board->promotePawn(lastVect, &type, &turn);
						break;
					}
				}

				rnd->number++;
				rnd->move = start_move + " " + ending_move;
				rounds.push_back(rnd);

				// Switch turns
				isWhiteTurn = !isWhiteTurn;

				std::cout << "Player " << (isWhiteTurn ? p1->getName() : p2->getName()) << "'s turn" << std::endl;
				board->printBoard(isWhiteTurn);

				checkGameState(&turn);
			}
			else {
				std::cout << "Invalid move!" << std::endl;
			}
		}
	}
	else {
		std::cout << "Enter a valid move!" << std::endl;
	}
}


void Game::undoMakeMove() {
	//Get the latest round from the round deque
	Round* rnd = getRounds().back();

	// Define pairs for the initial and final positions of the move
	std::pair<int, int> firstMove, lastMove;


	// Parse the move string from the round to extract start and end positions
	std::stringstream ss(rnd->move);
	std::string startMove, endMove;

	ss >> startMove;
	ss >> endMove;

	// Convert start and end position from string to board indices
	firstMove.first = startMove[1] - '0' - 1;
	firstMove.second = startMove[0] - 'a';

	lastMove.first = endMove[1] - '0' - 1;
	lastMove.second = endMove[0] - 'a';

	// Undo the move on the board
	board->undoMove(firstMove, lastMove);

	// Print board after undoing the move
	board->printBoard(isWhiteTurn);
}


std::pair<int,int> Game::findKing(Color *color) {
	// Loop through the chessboard
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Get the piece at the current square (i, j)
			Piece* piece = board->getPiece(i,j);
			// Check if the square is not emtpy, the piece is of the opposing color and if it's a KING
			if (piece != nullptr && piece->getColor() == *color && piece->getType() == KING) {
				// If the king is found, return its position
				return std::make_pair(i, j);
			}
		}
	}
	// If the king is not found on the board, throw an exception
	throw std::runtime_error("King not found on the board");

}


bool Game::isKingChecked(Color* kingColor) {
	// Find the position of the king
	std::pair<int, int> kingPosition = findKing(kingColor);

	// Loop through the chessboard
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Get the piece at the current square (i, j)
			auto piece = board->getPiece(i, j);

			// Check if the square is not empty and the piece is of the opposing color
			if (piece != nullptr && piece->getColor() != *kingColor) {
				// Check if the piece can move to the king's position and if the path is clear
				if (piece->isMovePossible({ i, j }, kingPosition, *board) && board->isPathClear({ i,j }, kingPosition)) {
					// If an opposing piece can move to the king's position and the path is clear, the king is in check
					return true;
				}
			}
		}
	}

	// If no opposing piece can move to the king's position, the king is not in check
	return false;
}


bool Game::isStalemate(Color *color) {
	// Check if the king is in check
	if (isKingChecked(color)) {
		return false;
	}

	// Check if the player has any possible moves
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece* piece = board->getPiece(i, j);
			if (piece != nullptr && piece->getColor() == *color) {
				std::pair<int, int> piecePosition = std::make_pair(i, j);
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						std::pair<int, int> newPosition = std::make_pair(k, l);
						if (piece->isMovePossible(piecePosition, newPosition, *board) &&
							board->isPathClear(piecePosition, newPosition)) {
							// Simulate the move
							board->movePiece(piecePosition, newPosition);

							bool isStillChecked = isKingChecked(color);

							// Revert the move
							board->undoMove(piecePosition, newPosition);

							// If the move does not leave the king in check, it's not a stalemate
							if (!isStillChecked) {
								return false;
							}
						}
					}
				}
			}
		}
	}
	// If no possible moves and not in check, it's a stalemate
	return true;
}


bool Game::isCheckmate(Color* color) {
	// If the king is not in check, it's not checkmate
	if (!isKingChecked(color)) {
		return false;
	}

	// Find the king's position
	std::pair<int, int> kingPosition = findKing(color);

	// Check if any move can get the king out of check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece* piece = board->getPiece(i, j);
			if (piece != nullptr && piece->getColor() == *color) {
				std::pair<int, int> piecePosition = std::make_pair(i, j);

				// Try moving this piece to every other square on the board
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						std::pair<int, int> newPosition = std::make_pair(k, l);

						// Check if the move is possible for this piece
						if (piece->isMovePossible(piecePosition, newPosition, *board)) {
							// Check if the move gets the king out of check
							board->movePiece(piecePosition, newPosition);

							board->undoMove(piecePosition, newPosition);


							if (!isKingChecked(color)) {
								return false;
							}
							
						}
					}
				}
			}
		}
	}
	// If no moves can get the king out of check, it's checkmate
	return true;

}

void Game::saveGame(std::string fileName) {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file for saving." << std::endl;
		return;
	}

	/*for (int i = 0;i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board->getPiece(i, j) != nullptr) {
				
			}
		}
	}*/

	//Write the rounds and the moves made so far
	for (Round* rnd : rounds) {
		file.write((char*)&rnd->move, sizeof(rnd->move));
	}

	file.write(p1->getName().c_str(), p1->getName().size() + 1);
	file.write(p2->getName().c_str(), p2->getName().size() + 1);

	file.close();
}

std::deque<Round*> Game::rounds;

std::deque<Round*> Game::getRounds() {
	return rounds;
}

bool Game::isCastlingPossible(std::pair<int, int> start, std::pair<int, int> end) {
	int startRank = start.first;
	int startFile = start.second;
	int endRank = end.first;
	int endFile = end.second;

	// Determine the color of the player making the move
	Color playerColor = (startRank == 0) ? WHITE : BLACK;

	// Check if the piece being moved is a king
	if (board->getPiece(startRank, startFile) != nullptr && board->getPiece(startRank, startFile)->getType() == KING) {
		// Determine if it's a kingside or queenside castling
		bool kingSide = (endFile > startFile);

		// Retrieve the king and rook pieces
		King* king = dynamic_cast<King*>(board->getPiece(startRank, startFile));
		Rook* rook = dynamic_cast<Rook*>(board->getPiece(startRank, (kingSide) ? 7 : 0));

		// Check if the king and the rook involved haven't moved yet
		if (king->getHasMoved() == 0 && rook->getHasMoved() == 0) {

			// Check if there are no pieces between the king and the rook
			int direction = (kingSide) ? 1 : -1;
			int rookFile = (kingSide) ? 7 : 0;
			for (int file = startFile + direction; file != rookFile; file += direction) {
				if (board->getPiece(startRank, file) != nullptr) {
					return false; // Piece blocking castling path
				}
			}

			// Simulate opponent moves and check if any target the squares the king passes through
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
					auto piece = board->getPiece(i, j);
					if (piece != nullptr && piece->getColor() != playerColor) {

						for (int file = startFile; file != endFile; file += direction) {
							// Simulate opponent piece moving to the file
							if (piece->isMovePossible({ i, j }, { startRank, file }, *board)) {

								return false; // Square under attack
							}
						}
					}
				}
			}

			return true; // All conditions met for castling
		}
	}

	return false; // Castling not possible for this move
}

bool Game::canPromotePawn(std::pair<int, int> end) {
	Piece* piece = board->getPiece(end.first, end.second);

	// Check if the piece exists and is a pawn
	if (piece->getType() != PAWN) {
		return false;
	}

	// Check if the pawn is on the last rank for its color
	if ((piece->getColor() == WHITE && end.first == 7) ||
		(piece->getColor() == BLACK && end.first == 0)) {
		return true;
	}

	return false;
}

void Game::checkGameState(Color *color) {
	if (isCheckmate(color)) {
		std::cout << "Checkmate! " << (isWhiteTurn ? p2->getName() : p1->getName()) << " wins!" << std::endl;
	}
	else if (isStalemate(color)) {
		std::cout << "Stalemate! The game is a draw." << std::endl;
	}
}