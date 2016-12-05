/*
 * Filename: search.hpp
 * Author: Francis Dinh
 * Date: December 4, 2016
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "board.h"
#include "eval.hpp"
#include "utility.hpp"

Move& search(Board& b) {
    
    // generate list of moves
    std::vector<Move> list;
    b.genMoves(list);

    int min = eval(b), max = eval(b);
    int minIndex = 0, maxIndex = 0;
    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            minIndex = maxIndex = i;
            b.undoMove();
            break;
        }
    }

    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            int result = eval(b);

            if (result < min) {
                min = result;
                minIndex = i;
            }

            if (result >= max) {
                max = result;
                maxIndex = i;
            }
            b.undoMove();
        }
    }

    if (b.getWhiteToMove()) {
        std::cout << formatMove(list[maxIndex]) << std::endl;
        b.makeMove(list[maxIndex]);
        return list[maxIndex];
    }
    else {
        std::cout << formatMove(list[minIndex]) << std::endl;
        b.makeMove(list[minIndex]);
        return list[minIndex];
    }
}

#endif
