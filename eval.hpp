/*
 * Filename: eval.hpp
 * Author: Francis Dinh
 * Date: December 4, 2016
 */

#ifndef EVAL_H
#define EVAL_H

#include "board.h"

/*
 * Assign a score to the board 
 */
int eval(Board& b) {
    int sum = 0;
    int* position = b.getPosition();
    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            int piece = position[row * 16 + column];

            if (piece == -1 || piece == 1)
                sum += piece;
            else if (piece == -2 || piece == 2)
                sum += (piece < 0 ? -3 : 3);
            else if (piece == -3 || piece == 3)
                sum += (piece < 0 ? -3 : 3);
            else if (piece == -4 || piece == 4)
                sum += (piece < 0 ? -5 : 5);
            else if (piece == -5 || piece == 5)
                sum += (piece < 0 ? -9 : 9);
            else if (piece == -6 || piece == 6)
                sum += (piece < 0 ? -10000 : 10000);
        }
    }
    return b.getWhiteToMove() ? sum : -sum;
}

#endif 
