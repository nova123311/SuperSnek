/*
 * Filename: test.hpp
 * Author: Francis Dinh
 * Date: December 4, 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <cstdint>
#include <iostream>
#include "board.h"
#include "utility.hpp"

/*
 * Perform a perft on the board at depth
 */
uint64_t perft(Board* b, int depth) {
    uint64_t nodes = 0;

    if (depth == 0)
        return 1;

    std::vector<Move> list;
    b->genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b->makeMove(list[i])) {
            nodes += perft(b, depth - 1);
            b->undoMove();
        }
    }
    return nodes;
}

/*
 * Perform perft of depth d - 1 on each move generated
 */
void divide(Board* b, int depth) {
    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            std::cout << formatMove(list[i]) << " " << perft(&b, depth - 1) 
                    << std::endl;
            b.undoMove();
        }
    }
}

#endif
