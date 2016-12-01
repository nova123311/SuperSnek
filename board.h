/*
 * Filename: board.h
 * Author: Francis Dinh
 * Date: September 16, 2016
 */

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "move.hpp"
#include "piece_entry.h"

const int WHITE_KINGSIDE = 0;   // represent index of castling kingside white
const int WHITE_QUEENSIDE = 1;  // represent index of castling queenside white
const int BLACK_KINGSIDE = 2;   // represent index of castling kingside black
const int BLACK_QUEENSIDE = 3;  // represent index of castling queenside black

const int NO_ENPASSANT = 1000;  // 1000 (off board) to represent no en passant

class Board {
private:

    /*
     * For storing previous positions
     */
    std::vector<Board*> history;

    /*
     * Maintain a list of pieces and their locations on the board
     */
    std::vector<PieceEntry*> pieceList;

    /*
     * Game position is represented as an array of 128 ints
     */
    int position[128];

    /*
     * Game state representation
     */
    bool whiteToMove;
    bool castle[4];
    int enpassant;
    int halfmove;
    int fullmove;

    /*
     * Is square being attacked?
     */
    bool isAttacked(int square);

    /*
     * Generate sliding piece (bishop, rook, queen) moves
     */
    void genSlidingPiece(std::vector<Move>& list, int origin,  
            std::vector<int>& delta);

    /*
     * Generate non sliding piece (knight, king) moves
     */
    void genNonSlidingPiece(std::vector<Move>& list, int origin,
            std::vector<int>& delta);
    
    /*
     * Generate pseudolegal moves for the pieces
     */
    void genPawn(std::vector<Move>& list, int origin);
    void genKnight(std::vector<Move>& list, int origin);
    void genBishop(std::vector<Move>& list, int origin);
    void genRook(std::vector<Move>& list, int origin);
    void genQueen(std::vector<Move>& list, int origin);
    void genKing(std::vector<Move>& list, int origin);
    void genCastle(std::vector<Move>& list, int origin);

public:
    
    /*
     * Constructor that takes fen string
     */
    Board(std::string fen);

    /*
     * Generate all pseudolegal moves for side to move
     */
    void genMoves(std::vector<Move>& list, bool castle);

    /*
     * Make and undo moves
     */
    bool makeMove(Move& m);
    void undoMove(Move& m);

    /*
     * Print a representation of the board to the console
     */
    void print();

    /*
     * Set the game position and state given a fen string
     */
    void setPosition(std::string fen);
};

#endif
