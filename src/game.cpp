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

void Game::generatePseudoLegalPawnMoves(MoveList& moves, std::byte color, std::byte enemy, uint8_t initialSquare) {
    int sign { (color == constants::WHITE) ? 1 : -1 };
    int frontOffset { sign * 10 };
    int captureLeftOffset { sign * 11 };
    int captureRightOffset { sign * 9 };
    if (m_board.isAtPawnHomeRankOfColor(enemy, initialSquare)) { // is promotable
        if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + frontOffset)) {
            moves.appendMove(constants::PAWN, initialSquare, frontOffset, 0b1000);
            moves.appendMove(constants::PAWN, initialSquare, frontOffset, 0b1001);
            moves.appendMove(constants::PAWN, initialSquare, frontOffset, 0b1010);
            moves.appendMove(constants::PAWN, initialSquare, frontOffset, 0b1011);
        }
        if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureRightOffset], initialSquare, captureRightOffset, 0b1100);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureRightOffset], initialSquare, captureRightOffset, 0b1101);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureRightOffset], initialSquare, captureRightOffset, 0b1110);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureRightOffset], initialSquare, captureRightOffset, 0b1111);
        }
        if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureLeftOffset], initialSquare, captureLeftOffset, 0b1100);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureLeftOffset], initialSquare, captureLeftOffset, 0b1101);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureLeftOffset], initialSquare, captureLeftOffset, 0b1110);
            moves.appendCapture(constants::PAWN, m_board[initialSquare + captureLeftOffset], initialSquare, captureLeftOffset, 0b1111);
        }

        return; // There should be no more legal moves for a pawn if its promotable
    }
    if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + frontOffset)) {
        moves.appendMove(constants::PAWN, initialSquare, frontOffset);
        if ((m_board.isAtPawnHomeRankOfColor(color, initialSquare)) && 
            (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + (frontOffset * 2)))) {
            moves.appendMove(constants::PAWN, initialSquare, frontOffset * 2, 0b0001);
        }
    }
    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
        moves.appendCapture(constants::PAWN, m_board[initialSquare + captureRightOffset], initialSquare, captureRightOffset);    
    }
    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
        moves.appendCapture(constants::PAWN, m_board[initialSquare + captureLeftOffset], initialSquare, captureLeftOffset);
    }
    if (m_enPassant == -1)
        return;
    
    if (constants::board64[m_enPassant] == (initialSquare + captureRightOffset)) {
        moves.appendCapture(constants::PAWN, constants::PAWN, initialSquare, captureRightOffset, 0b0101);
        return;
    }
    if (constants::board64[m_enPassant] == (initialSquare + captureLeftOffset))
        moves.appendCapture(constants::PAWN, constants::PAWN, initialSquare, captureLeftOffset, 0b0101);
}

MoveList Game::generatePseudoLegal() {
    MoveList moves {};
    const PieceList& list { m_whiteTurn ? m_whiteList : m_blackList };
    std::byte color { m_whiteTurn ? constants::WHITE : constants::BLACK };
    std::byte enemy { color ^ std::byte{0x80} };

    for (int i {0}; i < list.size(); ++i) {
        uint8_t initialSquare { constants::board64[list[i]] };
        uint8_t pieceInt { static_cast<uint8_t>(m_board[initialSquare] & std::byte{0x07}) };

        if (m_board.isPieceAtIndex(constants::PAWN, initialSquare)) {
            generatePseudoLegalPawnMoves(moves, color, enemy, initialSquare);
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
                    if (m_board.isColorAtIndex(color, move)) {
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
