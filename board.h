// Filename: board.h
// Author: Francis Dinh
// Date: September 16, 2016

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "move.hpp"

// castling indexes
enum CastleType {
    WHITE_KINGSIDE,
    WHITE_QUEENSIDE,
    BLACK_KINGSIDE,
    BLACK_QUEENSIDE
};

const int NO_ENPASSANT = 1000;  // 1000 (off board) to represent no en passant

class Board {
private:
    std::vector<int> pawnOffset{0x10, 0xf, 0x11};
    std::vector<int> knightOffset{0xe, 0x1f, 0x21, 0x12, -0xe, -0x1f, -0x21, -0x12};
    std::vector<int> bishopOffset{0xf, 0x11, -0xf, -0x11};
    std::vector<int> rookOffset{0x10, 0x1, -0x10, -0x1};
    std::vector<int> queenOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};
    std::vector<int> kingOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};

    // History of positions prior to current
    std::vector<Board*> history;

    // Piece list maintains locations of the pieces on the board
    std::vector<int> pieceList;

    // Game position is represented as an array of 128 ints
    int position[128];

    // Game state representation
    bool whiteToMove;
    bool castle[4];
    int enpassant;
    int halfmove;
    int fullmove;

public:
    
    // Constructor
    Board(std::string fen);

    // Copy contstructor
    Board(const Board& other);

    // Assignment operator
    void operator=(const Board& other);

    // Generate all pseudolegal moves for side to move
    void genMoves(std::vector<Move>& list);

    // For making and undoing moves on the board
    bool makeMove(Move& m);
    void undoMove();

    // Print a representation of the board to the console
    void print();

    // Set the game position and state given a fen string
    void setPosition(std::string fen);

    // Return the position
    int* getPosition();

    // Get piece list
    std::vector<int> getPieceList();

    // Is it white to move
    bool getWhiteToMove();

    // Determine whether a certain castling type is possible
    bool getCastle(CastleType castleType);

    // Get enpassant target square if any and is NO_ENPASSANT if not
    int getEnpassant();

    // Determine if a square is being attacked
    bool isAttacked(int square);
};

#endif
