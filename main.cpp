/*
 * Filename: main.cpp
 * Author: Francis Dinh
 * Date: September 16, 2016
 */

#include <iostream>
#include <string>

#include "board.h"
#include "move.hpp"
#include "search.hpp"
#include "utility.hpp"
#include "test.hpp"

/*
 *
 */
int main() {

    // set board to initial position
    Board b("r1bqk2r/ppp1bpp1/4pn1p/3p4/2PP4/2NBP1N1/PP3PPP/R2QK2R b KQkq - 0 1");

    // main game loop
    std::string input;
    while (true) {
        std::cin >> input;

        // perform user move if legal
        if (parseMove(b, input)) {

            // engine searches to depth 
            Move m = search(b, 5);
            b.makeMove(m);
            std::cout << formatMove(m) << std::endl;
        }
        else {
            std::cerr << "Incorrect input" << std::endl;
        }
    }

    return 0;
}
