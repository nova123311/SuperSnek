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
    Board b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

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

    return 0;
}
