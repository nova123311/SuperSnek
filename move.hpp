// Filename: move.hpp
// Author: Francis Dinh
// Date: October 14, 2016

#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

// constants for flags
enum FlagType {
    QUIET_MOVE = 0,
    DOUBLE_PAWN_PUSH = 1,
    KING_CASTLE = 2,
    QUEEN_CASTLE = 3,
    CAPTURE = 4,
    EP_CAPTURE = 5,
    KNIGHT_PROMOTION = 8,
    BISHOP_PROMOTION = 9,
    ROOK_PROMOTION = 10,
    QUEEN_PROMOTION = 11,
    KNIGHT_PROMOTION_CAPTURE = 12,
    BISHOP_PROMOTION_CAPTURE = 13,
    ROOK_PROMOTION_CAPTURE = 14,
    QUEEN_PROMOTION_CAPTURE = 15
};

class Move {
private:

    // move internally represented as at least 32 bit
    uint_fast32_t move;

public:

    // Default constructor
    Move() : move(0) {}

    // Construct move object with specified origin, target, and flag
    Move(unsigned origin, unsigned target, unsigned flag) : move(0) {
        move |= (origin << 16) | (target << 8) | flag;
    }

    // Get origin square
    unsigned getOrigin() {
        return (move & 0xff0000) >> 16;
    }

    // Get target square
    unsigned getTarget() {
        return (move & 0xff00) >> 8;
    }

    // Get flag
    unsigned getFlag() {
        return move & 0xff;
    }
};

#endif
