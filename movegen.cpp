#include "movegen.h"

MoveGenerator::MoveGenerator(const Board& board) : board(board) {}

void MoveGenerator::setBoard(const Board& board) {
    this->board = board;
}

void MoveGenerator::genMoves(std::vector<Move>& list) {
    std::vector<int> pieceList = board.getPieceList();
    int* position = board.getPosition();
    bool whiteToMove = board.getWhiteToMove();

    // functions to generate pieces
    void (MoveGenerator::*genPiece[])(std::vector<Move>&, int){&MoveGenerator::genPawn,
            &MoveGenerator::genKnight, &MoveGenerator::genBishop, &MoveGenerator::genRook, 
            &MoveGenerator::genQueen, &MoveGenerator::genKing};

    // iterate through the piece list
    for (size_t i = 0; i < pieceList.size(); ++i) {
        int piece = position[pieceList[i]];
        if ((whiteToMove && piece > 0) || (!whiteToMove && piece < 0)) 
            (this->*genPiece[abs(piece) - 1])(list, pieceList[i]);

        // castling
        if ((whiteToMove && piece == 6) || (!whiteToMove && piece == -6))
            genCastle(list, pieceList[i]);
    }
}

void MoveGenerator::genSlidingPiece(std::vector<Move>& list, int origin, 
        std::vector<int>& delta) {
    int* position = board.getPosition();

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

void MoveGenerator::genNonSlidingPiece(std::vector<Move>& list, int origin,
        std::vector<int>& delta) {
    int* position = board.getPosition();

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

void MoveGenerator::genPawn(std::vector<Move>& list, int origin) {
    int* position = board.getPosition();

    // multiply offsets by modifier depending on player to move
    int modifier = board.getWhiteToMove() ? 1 : -1;

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
        if (((board.getWhiteToMove() && origin >= 0x10 && origin <= 0x17) ||
                (!board.getWhiteToMove() && origin >= 0x60 && origin <= 0x67)) 
                && position[target] == 0) {
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
        if ((board.getEnpassant() == target) && (position[target] == 0)) {
            Move m(origin, target, EP_CAPTURE);
            list.push_back(m);
        }
    }
}

void MoveGenerator::genKnight(std::vector<Move>& list, int origin) {
    genNonSlidingPiece(list, origin, knightOffset);
}

void MoveGenerator::genBishop(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, bishopOffset);
}

void MoveGenerator::genRook(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, rookOffset);
}

void MoveGenerator::genQueen(std::vector<Move>& list, int origin) {
    genSlidingPiece(list, origin, queenOffset);
}

void MoveGenerator::genKing(std::vector<Move>& list, int origin) {
    genNonSlidingPiece(list, origin, kingOffset);
}

void MoveGenerator::genCastle(std::vector<Move>& list, int origin) {
    int* position = board.getPosition();

    // check that king is not in check
    if (board.isAttacked(origin))
        return;

    // kingisde castling
    if (!board.isAttacked(origin + 0x1) && !board.isAttacked(origin + 0x2)) {
        if (position[origin + 0x1] == 0 && position[origin + 0x2] == 0) {
            if ((board.getWhiteToMove() && board.getCastle(WHITE_KINGSIDE)) ||
                    (!board.getWhiteToMove() && board.getCastle(BLACK_KINGSIDE))) {
                Move m(origin, origin + 0x2, KING_CASTLE);
                list.push_back(m);
            }
        }
    }

    // queenside castling
    if (!board.isAttacked(origin - 0x1) && !board.isAttacked(origin - 0x2)) {
        if (position[origin - 0x1] == 0 && position[origin - 0x2] == 0 &&
                position[origin - 0x3] == 0) {
            if ((board.getWhiteToMove() && board.getCastle(WHITE_QUEENSIDE)) ||
                    (!board.getWhiteToMove() && board.getCastle(BLACK_QUEENSIDE))) {
                Move m(origin, origin - 0x2, QUEEN_CASTLE);
                list.push_back(m);
            }
        }
    }
}

