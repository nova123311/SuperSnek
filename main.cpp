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

    /*
    std::cout << perft(&b, 1) << std::endl;
    */

    /*
    // get moves
    std::vector<Move> list;
    b.genMoves(list, true);
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << std::hex << list[i].getTarget() << std::endl;
    }
    */

    /* 
    // take user moves
    while (true) {
        std::string move;

        // prompt for move
        std::cout << "Enter move: ";
        std::cin >> move;
        Move m = processMove(move);
        
        // end program
        if (!std::cin) {
            std::cout << "\nThanks for being super sneky :^)" << std::endl;
            break;
        }
    }
    */

    return 0;
}

/*
u64 perft(Board* b, int depth) {
    u64 nodes = 0;

    if (depth == 0)
        return 1;

    std::vector<Move> list;
    b->genMoves(list, true);
    for (size_t i = 0; i < list.size(); ++i) {
        b->makeMove(list[i]);
        nodes += perft(b, depth - 1);
        b->undoMove();
    }
    
    return nodes;
}
*/
