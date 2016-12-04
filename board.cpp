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

    // piece list
    for (int row = 0; row < 8; ++row) 
        for (int column = 0; column < 8; ++column) 
            if (position[row * 16 + column] != 0) 
                pieceList.push_back(row * 16 + column);
}

/*
 * Copy constructor
 */
Board::Board(const Board& other) {
     
    // assign data of other to this board structure
    for (size_t i = 0; i < other.history.size(); ++i)
        history.push_back(other.history[i]);
    for (size_t i = 0; i < other.pieceList.size(); ++i)
        pieceList.push_back(other.pieceList[i]);
    for (int i = 0; i < 128; ++i)
        position[i] = other.position[i];
    whiteToMove = other.whiteToMove;
    for (int i = 0; i < 4; ++i)
        castle[i] = other.castle[i];
    enpassant = other.enpassant;
    halfmove = other.halfmove;
    fullmove = other.fullmove;
}

/*
 * Generate pseudolegal moves
 */
void Board::genMoves(std::vector<Move>& list, bool castle) {

    // functions to generate pieces
    void (Board::*genPiece[])(std::vector<Move>&, int){&Board::genPawn,
            &Board::genKnight, &Board::genBishop, &Board::genRook, 
            &Board::genQueen, &Board::genKing};

    // iterate through the piece list
    for (size_t i = 0; i < pieceList.size(); ++i) {
        int piece = position[pieceList[i]];
        if ((whiteToMove && piece > 0) || (!whiteToMove && piece < 0)) 
            (this->*genPiece[abs(piece) - 1])(list, pieceList[i]);
    }
}

/*
 * Make a move on the board
 */
bool Board::makeMove(Move& m) {

    // add current board to history
    Board* b = new Board(*this);
    history.push_back(b);

    // make move
    position[m.getTarget()] = position[m.getOrigin()];
    position[m.getOrigin()] = 0;
    enpassant = NO_ENPASSANT;

    // enpassant
    if (m.getFlag() == EP_CAPTURE) {
        if (whiteToMove)
            position[m.getTarget() - 0x10] = 0;
        else
            position[m.getTarget() + 0x10] = 0;
    }

    // double pawn push
    else if (m.getFlag() == DOUBLE_PAWN_PUSH) {
        if (whiteToMove)
            enpassant = m.getOrigin() + 0x10;
        else
            enpassant = m.getOrigin() - 0x10;
    }

    // update piece list
    pieceList.clear();
    for (int row = 0; row < 8; ++row) 
        for (int column = 0; column < 8; ++column)
            if (position[row * 16 + column] != 0)
                pieceList.push_back(row * 16 + column);

    // check if king is in check
    for (size_t i = 0; i < pieceList.size(); ++i) {
        int piece = position[pieceList[i]];
        if ((whiteToMove && piece == 6) || (!whiteToMove && piece == -6)) {
            if (isAttacked(pieceList[i])) {
                undoMove();
                return false;
            }
        }
    }

    // change game state as necessary
    whiteToMove = !whiteToMove; 

    // successful move
    return true;
}

/*
 * Revert the board to previous position in history
 */
void Board::undoMove() {
     
    // assign data of other to this board structure
    Board* other = history.back();
    pieceList.clear();
    for (size_t i = 0; i < other->pieceList.size(); ++i)
        pieceList.push_back(other->pieceList[i]);
    for (int i = 0; i < 128; ++i)
        position[i] = other->position[i];
    whiteToMove = other->whiteToMove;
    for (int i = 0; i < 4; ++i)
        castle[i] = other->castle[i];
    enpassant = other->enpassant;
    halfmove = other->halfmove;
    fullmove = other->fullmove;
    history.pop_back();

    //delete other
    delete other;
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
            Move m(origin, target, QUIET_MOVE);
            list.push_back(m);

            // update target
            target += offset;
        }

        // stop sliding once a piece is encountered
        if (!(target & 0x88) && 
                (position[origin] ^ position[target]) < 0) {
            Move m(origin, target, CAPTURE);
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
                Move m(origin, target, QUIET_MOVE);
                list.push_back(m);
            }
            else if ((position[origin] ^ position[target]) < 0) {
                Move m(origin, target, CAPTURE);
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
                Move m(origin, target, i);
                list.push_back(m);
            }
        else {
            Move m(origin, target, QUIET_MOVE);
            list.push_back(m);
        }

        // double pawn push
        target += (modifier * pawnOffset[0]);
        if (!(target & 0x88) && ((origin >= 0x10 && origin <= 0x17) ||
                (origin >= 0x60 && origin <= 0x67)) && position[target] == 0) {
            Move m(origin, target, DOUBLE_PAWN_PUSH);
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
                    Move m(origin, target, i);
                    list.push_back(m);
                }
            else {
                Move m(origin, target, CAPTURE);
                list.push_back(m);
            }
        }

        // enpassant
        if ((enpassant == target) && (position[target] == 0)) {
            Move m(origin, target, EP_CAPTURE);
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
 * Determine if a square is attacked
 */
bool Board::isAttacked(int square) {
    std::vector<Move> moveList;
    whiteToMove = !whiteToMove;
    genMoves(moveList, false);
    whiteToMove = !whiteToMove;
    for (size_t i = 0; i < moveList.size(); ++i) 
        if (moveList[i].isCapture() && (int)moveList[i].getTarget() == square)
            return true;
    return false;
}
