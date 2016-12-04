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
std::string formatMove(Move& m);
const int DEPTH = 4;

/*
 *
 */
int main() {

    // set board to initial position
    Board b("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    // perft tests
    /*
    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b.makeMove(list[i])) {
            std::cout << formatMove(list[i]) << " " << perft(&b, DEPTH - 1) << std::endl;
            b.undoMove();
        }
    }
    */

    for (int i = 1; i <= 6; ++i)
        std::cout << perft(&b, i) << std::endl;
    return 0;
}

u64 perft(Board* b, int depth) {
    u64 nodes = 0;

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

std::string formatMove(Move& m) {
    int columnStart = m.getOrigin() & 7;
    int columnEnd = m.getTarget() & 7;
    int rowStart = m.getOrigin() >> 4;
    int rowEnd = m.getTarget() >> 4;

    std::string suh;
    suh.push_back('a' + columnStart);
    suh.push_back('1' + rowStart);
    suh.push_back('a' + columnEnd);
    suh.push_back('1' + rowEnd);

    return suh;
}
