/*
 * Filename: move.hpp
 * Author: Francis Dinh
 * Date: October 14, 2016
 */

#ifndef MOVE_HPP
#define MOVE_HPP

// constants for flags
const int QUIET_MOVE = 0;
const int DOUBLE_PAWN_PUSH = 1;
const int KING_CASTLE = 2;
const int QUEEN_CASTLE = 3;
const int CAPTURE = 4;
const int EP_CAPTURE = 5;
const int KNIGHT_PROMOTION = 8;
const int BISHOP_PROMOTION = 9;
const int ROOK_PROMOTION = 10;
const int QUEEN_PROMOTION = 11;
const int KNIGHT_PROMOTION_CAPTURE = 12;
const int BISHOP_PROMOTION_CAPTURE = 13;
const int ROOK_PROMOTION_CAPTURE = 14;
const int QUEEN_PROMOTION_CAPTURE = 15;

class Move {
private:

    /* 
     * Moves are internally represented as 32 bit int with each 
     * flag (origin, target, captured piece, move flag) represented as
     * some bits.
     */
    unsigned move;

public:

    /* 
     * Construct move object with specified flags
     * Max sizes
     *      origin = 127 (7 bits)
     *      target = 127 (7 bits)
     *      capture = 15 (1 bit for sign 3 bit for piece)
     *      flag = 2^14-1 (14 bits)
     */
    Move(unsigned origin, unsigned target, int capture, unsigned flag) {
        move = 0;
        move |= (origin << 25) | (target << 18) | ((capture & 0xF) << 14) | flag;
    }

    /*
     * Get origin square
     */
    unsigned getOrigin() {
        return (move & 0xFE000000) >> 25;
    }

    /*
     * Get target square
     */
    unsigned getTarget() {
        return (move & 0x1FC0000) >> 18;
    }

    /*
     * Get captured piece
     */
    int getCapture() {
        int capturedPiece = (move & 0x3C000) >> 14;
        if ((capturedPiece & 0x8) == 0x8)
            capturedPiece |= 0xFFFFFFF0;
        return capturedPiece < 0 ? -(~capturedPiece + 1) : capturedPiece;
    }

    /*
     * Get flag
     */
    unsigned getFlag() {
        return move & 0x3FFF;
    }

    /*
     * Is the move a capture?
     */
    bool isCapture() {
        return move & 0x4;
    }
};

#endif
