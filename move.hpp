/*
 * Filename: move.hpp
 * Author: Francis Dinh
 * Date: October 14, 2016
 */

#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

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
     * Moves are internally represented as 16 bit variable
     * with format (origin, target, flag)
     */
    uint16_t move;

public:

    /* 
     * Construct move object with specified origin, target, and flag
     */
    Move(unsigned origin, unsigned target, unsigned flag) {
        move = 0;
        origin = (origin + (origin & 7)) >> 1; 
        target = (target + (target & 7)) >> 1;
        move |= (origin << 10) | (target << 4) | flag;
    }

    /*
     * Get origin square
     */
    unsigned getOrigin() {
        unsigned origin = (move & 0xFC00) >> 10;
        return origin + (origin & ~7);
    }

    /*
     * Get target square
     */
    unsigned getTarget() {
        unsigned target = (move & 0x3F0) >> 4;
        return target + (target & ~7);
    }

    /*
     * Get flag
     */
    unsigned getFlag() {
        return move & 0xF;
    }
};

#endif
