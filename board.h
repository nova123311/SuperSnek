// Filename: board.h
// Author: Francis Dinh
// Date: September 16, 2016

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "move.hpp"

// castling indexes
const int WHITE_KINGSIDE = 0;   
const int WHITE_QUEENSIDE = 1;  
const int BLACK_KINGSIDE = 2;   
const int BLACK_QUEENSIDE = 3;  

const int NO_ENPASSANT = 1000;  // 1000 (off board) to represent no en passant

class Board {
private:

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

    // Generate sliding piece (bishop, rook, queen) moves
    void genSlidingPiece(std::vector<Move>& list, int origin,  
            std::vector<int>& delta);

    // Generate non sliding piece (knight, king) moves
    void genNonSlidingPiece(std::vector<Move>& list, int origin,
            std::vector<int>& delta);
    
    // Generate pseudolegal moves for the pieces
    void genPawn(std::vector<Move>& list, int origin);
    void genKnight(std::vector<Move>& list, int origin);
    void genBishop(std::vector<Move>& list, int origin);
    void genRook(std::vector<Move>& list, int origin);
    void genQueen(std::vector<Move>& list, int origin);
    void genKing(std::vector<Move>& list, int origin);
    void genCastle(std::vector<Move>& list, int origin);

    // Determine if a square is being attacked
    bool isAttacked(int square);

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
};

#endif
