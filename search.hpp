/*
 * Filename: search.hpp
 * Author: Francis Dinh
 * Date: December 4, 2016
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <algorithm>
#include <climits>
#include "board.h"
#include "eval.hpp"
#include "utility.hpp"

/*
 * alpha beta minimax search
 */
int minimax(Board& b, int depth, int alpha, int beta, bool maxPlayer) {

    // evaluate leaf nodes
    if (depth == 0)
        return eval(b);

    // gen moves
    std::vector<Move> list;
    b.genMoves(list);

    // maximize results
    if (maxPlayer) {

        // depth first search
        for (size_t i = 0; i < list.size(); ++i) {
            if (b.makeMove(list[i])) {
                
                // get max of min children
                int score = minimax(b, depth - 1, alpha, beta, false);
                alpha = std::max(score, alpha);

                // prune branch if alpha >= beta
                if (alpha >= beta) {
                    b.undoMove();
                    break;
                }
        
                // move back up to parent node
                b.undoMove();
            }
        }
        return alpha;
    }

    // minimize results
    else {

        // depth first search
        for (size_t i = 0; i < list.size(); ++i) {
            if (b.makeMove(list[i])) {

                // get min of max children
                int score = minimax(b, depth - 1, alpha, beta, true);
                beta = std::min(score, beta);

                // prune branch if alpha >= beta
                if (alpha >= beta) {
                    b.undoMove();
                    break;
                }

                // move back up to parent node
                b.undoMove();
            }
        }
        return beta;
    }
}

/*
 * Search board to specified depth and make move
 */
Move search(Board& b, int depth) {
    Move m;
    std::vector<Move> list;

    if (b.getWhiteToMove()) {
        int maxScore = INT_MIN;
        b.genMoves(list);
        for (size_t i = 0; i < list.size(); ++i) {
            if (b.makeMove(list[i])) {
                int score = minimax(b, depth - 1, INT_MIN, INT_MAX, false);
                if (score > maxScore) {
                    maxScore = score;
                    m = list[i];
                }
                b.undoMove();
            }
        }
    }

    else {
        int minScore = INT_MAX;
        b.genMoves(list);
        for (size_t i = 0; i < list.size(); ++i) {
            if (b.makeMove(list[i])) {
                int score = minimax(b, depth - 1, INT_MIN, INT_MAX, true);
                if (score < minScore) {
                    minScore = score;
                    m = list[i];
                }
                b.undoMove();
            }
        }
    }

    return m;
}

#endif
