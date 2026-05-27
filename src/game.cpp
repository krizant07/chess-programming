#include <iostream>
#include <cassert>

#include "game.h"
#include "constants.h"
#include "move-list.h"

Game::Game() {}

void Game::debugPrint() {
    std::cout << "Game Debug:";

    m_board.debugPrint();

    std::cout << '\n' << (m_whiteTurn ? "WHITE" : "BLACK") << " TURN\n";
    std::cout << "CAN CASTLE KQkq: " << m_canCastleKQkq << '\n';
    std::cout << "EN PASSANT SQUARE INDEX: ";
    if (m_enPassant == -1)
        std::cout << "NONE";
    else
        std::cout << m_enPassant;
    std::cout << '\n';
    std::cout << "HALF MOVE CLOCK: " << m_halfMoveClock << '\n';
    std::cout << "FULL MOVE COUNT: " << m_fullMoveCount << '\n';

    MoveList list = generatePseudoLegal();
    std::cout << "MOVE LIST SIZE: " << list.size() << '\n';
}

MoveList Game::generatePseudoLegal() {
    MoveList moves {};

    for (int i {0}; i < m_whiteList.size(); ++i) {
        uint8_t initialSquare { constants::board64[m_whiteList[i]] };
        uint8_t pieceInt { static_cast<uint8_t>(m_board[initialSquare] & std::byte{0x07}) };

        if (m_board.isPieceAtIndex(constants::PAWN, initialSquare)) {
            if (m_board.isAtPawnHomeRankOfColor(constants::BLACK, initialSquare)) { // is promotable
                if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + 10)) {
                    moves.appendMove(constants::PAWN, initialSquare, 10, 0b1000);
                    moves.appendMove(constants::PAWN, initialSquare, 10, 0b1001);
                    moves.appendMove(constants::PAWN, initialSquare, 10, 0b1010);
                    moves.appendMove(constants::PAWN, initialSquare, 10, 0b1011);
                }
                if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 9)) {
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9, 0b1100);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9, 0b1101);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9, 0b1110);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9, 0b1111);
                }
                if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 11)) {
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11, 0b1100);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11, 0b1101);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11, 0b1110);
                    moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11, 0b1111);
                }

                continue; // There should be no more legal moves for a pawn if its promotable
            }
            if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + 10)) {
                moves.appendMove(constants::PAWN, initialSquare, 10);
                if (m_board.isAtPawnHomeRankOfColor(constants::WHITE, initialSquare)) {
                    moves.appendMove(constants::PAWN, initialSquare, 10, 0b0001);
                }
            }
            if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 9)) {
                moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9);    
            }
            if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 11)) {
                moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11);
            }
            if (m_enPassant == -1)
                continue;
            
            if (constants::board64[m_enPassant] == (initialSquare + 9)) {
                moves.appendCapture(constants::PAWN, m_board[initialSquare + 9], initialSquare, 9, 0b0101);
                continue;
            }
            if (constants::board64[m_enPassant] == (initialSquare + 11))
                moves.appendCapture(constants::PAWN, m_board[initialSquare + 11], initialSquare, 11, 0b0101);
            continue;
        }

        for (int j {0}; j < constants::offset_count[pieceInt]; ++j) {
            for(int moveOffset { constants::offsets[pieceInt][j] }, move { initialSquare } ;;) {
                move += moveOffset;
                if (m_board.isPieceAtIndex(constants::SENTINAL, move)) {
                    break;
                }
                if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, move)) {
                    moves.appendMove(m_board[initialSquare], initialSquare, moveOffset);
                } else {
                    if (m_board.isColorAtIndex(constants::WHITE, move)) {
                        break;
                    }
                    moves.appendCapture(m_board[initialSquare], m_board[move], initialSquare, moveOffset);
                    break;
                }
                if (!constants::slide[pieceInt])
                   break; 
            }
        }
        
    }
    return moves;
}
