// Filename: utility.hpp
// Author: Francis Dinh
// Date: December 4, 2016

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "board.h"

// Parse an input and perform move on board
bool parseMove(Board& b, std::string input) {

    // get origin and target squares
    unsigned origin = (input[0] - 'a') + 16 * (input[1] - '1');
    unsigned target = (input[2] - 'a') + 16 * (input[3] - '1');

    // is move a promotion
    unsigned promotion = 0;
    if (input.size() == 5) {
        if (input[4] == 'n')
            promotion = 2; 
        else if (input[4] == 'b')
            promotion = 3;
        else if (input[4] == 'r')
            promotion = 4;
        else if (input[4] == 'q')
            promotion = 5;
    }

    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {

        // ensure origin and target are same
        if (origin == list[i].getOrigin() && target == list[i].getTarget()) {

            // regular move
            if (input.size() == 4) {
                if (b.makeMove(list[i]))
                    return true;
                else {
                    b.undoMove();
                    return false;
                }
            }

            // promotion
            else if (input.size() == 5) {
                if (list[i].getFlag() % 4 + 2 == promotion) {
                    if (b.makeMove(list[i]))
                        return true;
                    else {
                        b.undoMove();
                        return false;
                    }
                }
            }
        }
    }

    // move not legal
    return false;
}

// Format a move as a string 
std::string formatMove(Move& m) {
    int columnStart = m.getOrigin() & 7;
    int columnEnd = m.getTarget() & 7;
    int rowStart = m.getOrigin() >> 4;
    int rowEnd = m.getTarget() >> 4;

    std::string move;
    move.push_back('a' + columnStart);
    move.push_back('1' + rowStart);
    move.push_back('a' + columnEnd);
    move.push_back('1' + rowEnd);

    // promotions?
    if (m.getFlag() >= 8) {
        int piece = m.getFlag() % 4 + 2;
        if (piece == 2)
            move.push_back('n');
        else if (piece == 3)
            move.push_back('b');
        else if (piece == 4)
            move.push_back('r');
        else if (piece == 5) 
            move.push_back('q');
    }

    return move;
}

#endif
