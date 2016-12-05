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

//std::string formatMove(Move& m);
bool parseMove(Board& b, std::string input);

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

bool parseMove(Board& b, std::string input) {

    unsigned origin = (input[0] - 'a') + 16 * (input[1] - '1');
    unsigned target = (input[2] - 'a') + 16 * (input[3] - '1');

    std::vector<Move> list;
    b.genMoves(list);
    for (size_t i = 0; i < list.size(); ++i) {
        if (origin == list[i].getOrigin() && target == list[i].getTarget()) {
            if (b.makeMove(list[i]))
                return true;
            else {
                b.undoMove();   
                return false;
            }
        }
    }

    return false;
}

/*
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
*/
