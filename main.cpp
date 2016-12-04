/*
 * Filename: main.cpp
 * Author: Francis Dinh
 * Date: September 16, 2016
 */

#include <iostream>
#include <string>

#include "board.h"
#include "move.hpp"

typedef unsigned long long u64;
u64 perft(Board* b, int depth);

/*
 *
 */
int main() {

    // set board to initial position
    Board b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // perft tests
    std::cout << perft(&b, 4) << std::endl;
    return 0;
}

u64 perft(Board* b, int depth) {
    u64 nodes = 0;

    if (depth == 0)
        return 1;

    std::vector<Move> list;
    b->genMoves(list, true);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b->makeMove(list[i])) {
            nodes += perft(b, depth - 1);
            b->undoMove();
        }
    }
    
    return nodes;
}
