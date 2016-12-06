/*
 * Filename: search.hpp
 * Author: Francis Dinh
 * Date: December 4, 2016
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <climits>
#include "board.h"
#include "eval.hpp"
#include "utility.hpp"

/*
 * negamax algorithm
 */
int negamax(Board& b, int depth) {

    // return evaluation of leaf node
    if (depth == 0)
        return eval(b);
    
    // perform a depth first search
    int max = INT_MIN;
    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            int score = -negamax(b, depth - 1);
            if (score > max)
                max = score;
            b.undoMove();
        }
    }

    return max;
}

/*
 * Search board to specified depth and make move
 */
Move search(Board& b, int depth) {
    Move m;

    int max = INT_MIN;
    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            int score = -negamax(b, depth - 1);
            if (score > max) {
                max = score;
                m = list[i];
            }
            b.undoMove();
        }
    }

    return m;
}

#endif
