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
    Board b("2r1r3/pp1n1k2/3B1p2/2pp1p1p/2P2P1P/6P1/PP2R1K1/4R3 w - - 1 8");
    std::cout << negamax(b, 6) << std::endl;

    /*
    // main game loop
    std::string input;
    while (true) {
        std::cin >> input;
        if (parseMove(b, input)) {
            search(b);
        }
        else {
            std::cerr << "Incorrect input" << std::endl;
        }
    }
    */

    return 0;
}
