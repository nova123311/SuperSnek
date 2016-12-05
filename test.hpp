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
    b->genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (b->makeMove(list[i])) {
            std::cout << formatMove(list[i]) << " " << perft(b, depth - 1) 
                    << std::endl;
            b->undoMove();
        }
    }
}

/*
 * Perform perft test on set positions and expected values
 */
void testSuite() {

    /* TESTING */
    Board test1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << "Position 1" << std::endl;
    for (int i = 1; i <= 7; ++i)
        std::cout << perft(&test1, i) << std::endl;
    std::cout << std::endl;

    Board test2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    std::cout << "Position 2" <<std::endl;
    for (int i = 1; i <= 5; ++i)
        std::cout << perft(&test2, i) << std::endl;
    std::cout << std::endl;

    Board test3("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    std::cout << "Position 3" <<std::endl;
    for (int i = 1; i <= 7; ++i)
        std::cout << perft(&test3, i) << std::endl;
    std::cout << std::endl;
   
    Board test4("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    std::cout << "Position 4" <<std::endl;
    for (int i = 1; i <= 6; ++i)
        std::cout << perft(&test4, i) << std::endl;
    std::cout << std::endl;

    Board test5("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    std::cout << "Position 5" <<std::endl;
    for (int i = 1; i <= 5; ++i)
        std::cout << perft(&test5, i) << std::endl;
    std::cout << std::endl;

    Board test6("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    std::cout << "Position 6" <<std::endl;
    for (int i = 1; i <= 6; ++i)
        std::cout << perft(&test6, i) << std::endl;
    std::cout << std::endl;
}

#endif
