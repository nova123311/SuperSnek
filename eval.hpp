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
    
    // calculate material
    int sum = 0;
    int* position = b.getPosition();
    std::vector<int> pieceList = b.getPieceList();
    for (int index : pieceList) {
        int piece = position[index];
        if (piece == -1 || piece == 1)
            sum += (piece < 0 ? -100 : 100);
        else if (piece == -2 || piece == 2)
            sum += (piece < 0 ? -300 : 300);
        else if (piece == -3 || piece == 3)
            sum += (piece < 0 ? -300 : 300);
        else if (piece == -4 || piece == 4)
            sum += (piece < 0 ? -500 : 500);
        else if (piece == -5 || piece == 5)
            sum += (piece < 0 ? -900 : 900);
        else if (piece == -6 || piece == 6)
            sum += (piece < 0 ? -100000 : 100000);
    }

    // calculate mobility
    std::vector<Move> list;
    b.genMoves(list);
    int mobility = b.getWhiteToMove() ? list.size() : -list.size();

    return sum + mobility;
}

#endif 
