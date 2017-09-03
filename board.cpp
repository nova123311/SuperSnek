// Filename: board.cpp
// Author: Francis Dinh
// Date: September 16, 2016

#include <algorithm>
#include <cctype>
#include <iostream>
#include "board.h"
#include "movegen.h"

Board::Board(std::string fen) {
    setPosition(fen);

    // piece list
    for (int row = 0; row < 8; ++row) 
        for (int column = 0; column < 8; ++column) 
            if (position[row * 16 + column] != 0) 
                pieceList.push_back(row * 16 + column);
}

Board::Board(const Board& other) {
     
    // assign data of other to this board structure
    history = other.history;
    pieceList = other.pieceList;
    std::copy(other.position, other.position + 128, position);
    whiteToMove = other.whiteToMove;
    std::copy(other.castle, other.castle + 4, castle);
    enpassant = other.enpassant;
    halfmove = other.halfmove;
    fullmove = other.fullmove;
}

void Board::operator=(const Board& other) {
    *this = other;
}

void Board::genMoves(std::vector<Move>& list) {
    MoveGenerator generator(*this);
    generator.genMoves(list);
}

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
    
    // kingisde castle
    else if (m.getFlag() == KING_CASTLE) {
        position[m.getTarget() - 0x1] = position[m.getTarget() + 0x1];
        position[m.getTarget() + 0x1] = 0;
    }

    // queen side castle
    else if (m.getFlag() == QUEEN_CASTLE) {
        position[m.getTarget() + 0x1] = position[m.getTarget() - 0x2];
        position[m.getTarget() - 0x2] = 0;
    }

    // promotions
    else if (m.getFlag() >= KNIGHT_PROMOTION && 
            m.getFlag() <= QUEEN_PROMOTION_CAPTURE) {
        position[m.getTarget()] = position[m.getTarget()] < 0 ? 
                -(m.getFlag() % 4 + 2) : m.getFlag() % 4 + 2;
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

    // change side to move
    whiteToMove = !whiteToMove; 

    // check castling privileges
    if (position[0x4] != 6)
        castle[WHITE_KINGSIDE] = castle[WHITE_QUEENSIDE] = false;
    if (position[0x74] != -6)
        castle[BLACK_KINGSIDE] = castle[BLACK_QUEENSIDE] = false;
    if (position[0x0] != 4)
        castle[WHITE_QUEENSIDE] = false;
    if (position[0x7] != 4)
        castle[WHITE_KINGSIDE] = false;
    if (position[0x70] != -4)
        castle[BLACK_QUEENSIDE] = false;
    if (position[0x77] != -4)
        castle[BLACK_KINGSIDE] = false;

    // successful move
    return true;
}

void Board::undoMove() {
     
    // assign data of other to this board structure
    Board* other = history.back();
    pieceList = other->pieceList;
    std::copy(other->position, other->position + 128, position);
    whiteToMove = other->whiteToMove;
    std::copy(other->castle, other->castle + 4, castle);
    enpassant = other->enpassant;
    halfmove = other->halfmove;
    fullmove = other->fullmove;
    history.pop_back();

    //delete other
    delete other;
}

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

int* Board::getPosition() {
    return position;
}

bool Board::getWhiteToMove() {
    return whiteToMove;
}

std::vector<int> Board::getPieceList() {
    return pieceList;
}

bool Board::getCastle(CastleType castleType) {
    return castle[castleType];
}

int Board::getEnpassant() {
    return enpassant;
}

bool Board::isAttacked(int square) {

    // temporarily place a value corresponding with color on square
    int piece = position[square];
    position[square] = whiteToMove ? 1 : -1;

    // is attacked by pawn
    if (whiteToMove) {
        for (int i = 1; i < 3; ++i) {
            int target = square + pawnOffset[i];
            if (position[target] == -1)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
        }
    }
    else {
        for (int i = 1; i < 3; ++i) {
            int target = square - pawnOffset[i];
            if (position[target] == 1)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
        }
    }
    
    // is attacked by knight
    for (int delta : knightOffset) {
        int target = square + delta;
        if (position[target] == -2 || position[target] == 2)
            if ((position[square] ^ position[target]) < 0)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
    }

    // is attacked by bishop
    for (int delta : bishopOffset) {
        int target = square + delta;
        while (!(target & 0x88) && position[target] == 0) 
            target += delta;
        if (position[target] == -3 || position[target] == 3)
            if ((position[square] ^ position[target]) < 0)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
    }

    // is attacked by rook
    for (int delta : rookOffset) {
        int target = square + delta;
        while (!(target & 0x88) && position[target] == 0) 
            target += delta;
        if (position[target] == -4 || position[target] == 4)
            if ((position[square] ^ position[target]) < 0)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
    }

    // is attacked by queen
    for (int delta : queenOffset) {
        int target = square + delta;
        while (!(target & 0x88) && position[target] == 0) 
            target += delta;
        if (position[target] == -5 || position[target] == 5)
            if ((position[square] ^ position[target]) < 0)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
    }

    // is attacked by king
    for (int delta : kingOffset) {
        int target = square + delta;
        if (position[target] == -6 || position[target] == 6)
            if ((position[square] ^ position[target]) < 0)
                if (!(target & 0x88)) {
                    position[square] = piece;
                    return true;
                }
    }

    position[square] = piece;
    return false;
}
