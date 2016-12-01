/*
 * Filename: board.cpp
 * Author: Francis Dinh
 * Date: September 16, 2016
 */

#include <cctype>
#include <iostream>
#include "board.h"

/*
 * Constructor
 */
Board::Board(std::string fen) {
    setPosition(fen);
}

/*
 * Generate pseudolegal moves
 */
void Board::genMoves(std::vector<Move>& list, bool castle) {

    // functions to generate pieces
    void (Board::*genPiece[])(std::vector<Move>&, int){&Board::genPawn,
            &Board::genKnight, &Board::genBishop, &Board::genRook, 
            &Board::genQueen, &Board::genKing};

    // iterate through each real square
    for (int row = 0; row < 8; ++row) 
        for (int column = 0; column < 8; ++column) {
            int piece = position[row * 0x10 + column];
    
            if ((whiteToMove && piece > 0) || (!whiteToMove && piece < 0)) {
            }
        }
}

/*
 * Make a move
 */
bool Board::makeMove(Move& m) {
    
    // make the move
    position[m.getTarget()] = position[m.getOrigin()];
    position[m.getOrigin()] = 0;

    // switch colors
    whiteToMove = !whiteToMove;

    return true;
}

/*
 * Undo a move
 */
void Board::undoMove(Move& m) {

    // move back to prior position
    position[m.getOrigin()] = position[m.getTarget()];   
    position[m.getTarget()] = position[m.getCapture()];
    whiteToMove = !whiteToMove;
}

/*
 * Print board to console
 */
void Board::print() {

    // print top border of board
    std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;

    // iterate through every square on the board
    for (int row = 7; row >= 0; --row) {
        for (int column = 0; column < 8; ++column) {
            int piece = position[row * 16 + column];

            // print out pieces
            if (piece < 0) 
                std::cout << "|" << piece << " ";
            else 
                std::cout << "| " << piece << " ";
        }
        
        // go to next row
        std::cout << "|\n+---+---+---+---+---+---+---+---+" << std::endl;
    }
}

/*
 * Set game position/status
 */
void Board::setPosition(std::string fen) {
    std::string::iterator it = fen.begin();

    // piece placement
    int index = 0x70;
    while (!isspace(*it)) {

        // pieces
        if (*it == 'p' || *it == 'P')
            position[index++] = islower(*it) ? -1 : 1;
        else if (*it == 'n' || *it == 'N')
            position[index++] = islower(*it) ? -2 : 2;
        else if (*it == 'b' || *it == 'B')
            position[index++] = islower(*it) ? -3 : 3;
        else if (*it == 'r' || *it == 'R')
            position[index++] = islower(*it) ? -4 : 4;
        else if (*it == 'q' || *it == 'Q')
            position[index++] = islower(*it) ? -5 : 5;
        else if (*it == 'k' || *it == 'K')
            position[index++] = islower(*it) ? -6 : 6;

        // number of empty squares
        else if (isdigit(*it)) 
            for (int num = 0; num < *it - '0'; ++num)
                position[index++] = 0;

        // go to next row
        else if (*it == '/')
            index -= 0x18;
        ++it;
    }
    ++it;

    // active color
    whiteToMove = *it++ == 'w' ? true : false;
    ++it;

    // castling availability 
    for (int i = 0; i < 4; ++i)
        castle[i] = false;
    while (!isspace(*it)) {
        if (*it == 'K')
            castle[WHITE_KINGSIDE] = true;
        if (*it == 'Q')
            castle[WHITE_QUEENSIDE] = true;
        if (*it == 'k')
            castle[BLACK_KINGSIDE] = true;
        if (*it == 'q')
            castle[BLACK_QUEENSIDE] = true;
        ++it;
    }
    ++it;

    // enpassant target square
    enpassant = NO_ENPASSANT;
    if (*it != '-') {
        enpassant = (*it++ - 'a') + (*it++ - '1') * 0x10;
        ++it;
    }
    else
        it += 2;

    // halfmove clock
    halfmove = 0;
    while (!isspace(*it)) 
        halfmove = halfmove * 10 + *it++ - '0';
    ++it;

    // fullmove clock
    fullmove = 0;
    while (isdigit(*it)) 
        fullmove = fullmove * 10 + *it++ - '0';
}

/*
 * Generate pseudolegal moves for sliding pieces
 */
void Board::genSlidingPiece(std::vector<Move>& list, int origin,
        std::vector<int>& delta) {

    // use each offset
    for (int offset : delta) {
        int target = origin + offset;

        // add move only if target square is on board and empty
        while (!(target & 0x88) && position[target] == 0)  {
            Move m(origin, target, position[target], QUIET_MOVE);
            list.push_back(m);

            // update target
            target += offset;
        }

        // stop sliding once a piece is encountered
        if (!(target & 0x88) && 
                (position[origin] ^ position[target]) < 0) {
            Move m(origin, target, position[target], CAPTURE);
            list.push_back(m);
        }
    }
}

/*
 * Generate pseudolegal moves for nonsliding pieces
 */
void Board::genNonSlidingPiece(std::vector<Move>& list, int origin,
        std::vector<int>& delta) {

    // use each offset
    for (int offset : delta) {
        int target = origin + offset;
        
        // add move only if target square is on board and valid move
        if (!(target & 0x88)) {
            if (position[target] == 0) {
                Move m(origin, target, position[target], QUIET_MOVE);
                list.push_back(m);
            }
            else if ((position[origin] ^ position[target]) < 0) {
                Move m(origin, target, position[target], CAPTURE);
                list.push_back(m);
            }
        }
    }
}

/*
 * Generate pseudolegal moves for pawns
 */
std::vector<int> pawnOffset{0x10, 0xf, 0x11};
void Board::genPawn(std::vector<Move>& list, int origin) {

    // multiply offsets by modifier depending on player to move
    int modifier = whiteToMove ? 1 : -1;

    // single pawn push
    int target = origin + modifier * pawnOffset[0];
    if (position[target] == 0) {

        // promotions
        if ((target >= 0x70 && target <= 0x77) || 
                (target >= 0x0 && target <= 0x7)) 
            for (int i = 8; i <= 11; ++i) {
                Move m(origin, target, position[target], i);
                list.push_back(m);
            }
        else {
            Move m(origin, target, position[target], QUIET_MOVE);
            list.push_back(m);
        }

        // double pawn push
        target += (modifier * pawnOffset[0]);
        if (!(target & 0x88) && ((origin >= 0x10 && origin <= 0x17) ||
                (origin >= 0x60 && origin <= 0x67)) && position[target] == 0) {
            Move m(origin, target, position[target], DOUBLE_PAWN_PUSH);
            list.push_back(m);
        }
    }
    
    // pawn captures
    for (int i = 1; i < 3; ++i) {
        target = origin + modifier * pawnOffset[i];
        if ((position[target] != 0) && !(target & 0x88) && 
                (position[origin] ^ position[target]) < 0) {
        
            // promotions
            if ((target >= 0x70 && target <= 0x77) || 
                    (target >= 0x0 && target <= 0x7))
                for (int i = 12; i <= 15; ++i) {
                    Move m(origin, target, position[target], i);
                    list.push_back(m);
                }
            else {
                Move m(origin, target, position[target], CAPTURE);
                list.push_back(m);
            }
        }

        // enpassant
        if ((enpassant == target) && (position[target] == 0)) {
            Move m(origin, target, position[target], EP_CAPTURE);
            list.push_back(m);
        }
    }
}

/*
 * Generate pseudolegal moves for knights
 */
std::vector<int> knightOffset{0xe, 0x1f, 0x21, 0x12, -0xe, -0x1f, -0x21, -0x12};
void Board::genKnight(std::vector<Move>& list, int origin) {
    genNonSlidingPiece(list, origin, knightOffset);
}

/*
 * Generate pseudolegal moves for bishops
 */
std::vector<int> bishopOffset{0xf, 0x11, -0xf, -0x11};
void Board::genBishop(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, bishopOffset);
}

/*
 * Generate pseudolegal moves for rooks
 */
std::vector<int> rookOffset{0x10, 0x1, -0x10, -0x1};
void Board::genRook(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, rookOffset);
}

/*
 * Generate pseudolegal moves for queens
 */
std::vector<int> queenOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};
void Board::genQueen(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, queenOffset);
}

/*
 * Generate pseudolegal moves for kings
 */
std::vector<int> kingOffset{0xf, 0x10, 0x11, 0x1, -0xf, -0x10, -0x11, -0x1};
void Board::genKing(std::vector<Move>& list, int origin) {
    genNonSlidingPiece(list, origin, kingOffset);
}

/*
 * Generate castling moves
 */
void Board::genCastle(std::vector<Move>& list, int origin) {

    // ensure that king is not in check
    if (isAttacked(origin))
        return;

    // kingisde
    if ((whiteToMove && castle[WHITE_KINGSIDE]) || 
            (!whiteToMove && castle[BLACK_KINGSIDE])) {

        // squares between rook and king are empty and not attacked
        if (position[origin + 0x1] == 0 && position[origin + 0x2] == 0 &&
                !isAttacked(origin + 0x1) && !isAttacked(origin + 0x2)) {
            Move m(origin, origin + 0x2, 0, KING_CASTLE);
            list.push_back(m);
        }
    }   
    
    // queenside
    if ((whiteToMove && castle[WHITE_QUEENSIDE]) || 
            (!whiteToMove && castle[BLACK_QUEENSIDE])) {

        // squares between rook and king are empty and not attacked
        if (position[origin - 0x1] == 0 && position[origin - 0x2] == 0 &&
                !isAttacked(origin - 0x1) && !isAttacked(origin - 0x2)) {
            Move m(origin, origin - 0x2, 0, QUEEN_CASTLE);
            list.push_back(m);
        }
    }
}

/*
 * Is the square being attacked?
 */
bool Board::isAttacked(int square) {
    std::vector<Move> moveList;

    // temporarily switch sides
    whiteToMove = !whiteToMove;
    genMoves(moveList, false);
    whiteToMove = !whiteToMove;

    // check if move is in list
    for (size_t i = 0; i < moveList.size(); ++i) {
        if (moveList[i].isCapture() && (int)moveList[i].getTarget() == square) 
            return true;
    }
    return false;
}
