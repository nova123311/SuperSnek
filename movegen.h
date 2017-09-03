#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "board.h"
#include "move.hpp"

class MoveGenerator {
private:
    Board board;

    std::vector<int> pawnOffset{0x10, 0xf, 0x11};
    std::vector<int> knightOffset{0xe, 0x1f, 0x21, 0x12, -0xe, -0x1f, -0x21, -0x12};
    std::vector<int> bishopOffset{0xf, 0x11, -0xf, -0x11};
    std::vector<int> rookOffset{0x10, 0x1, -0x10, -0x1};
    std::vector<int> queenOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};
    std::vector<int> kingOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};

public:

    // MoveGenerator composes a board upon which to generate moves
    MoveGenerator(const Board& board);

    // Set the board upon which to generate moves
    void setBoard(const Board& board);

    // Generate possible pseudolegal moves
    void genMoves(std::vector<Move>& list);

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
};

#endif
