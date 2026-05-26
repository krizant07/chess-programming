#include <iostream>
#include <cassert>

#include "game.h"
#include "constants.h"

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
}

std::vector<Move> Game::generatePseudoLegal() {
    std::vector<Move> moves {};

    for (int i {0}; i < m_whiteList.size(); ++i) {
        uint8_t initialSquare { constants::board64[m_whiteList[i]] };
        uint8_t pieceInt { static_cast<uint8_t>(m_board[initialSquare] & std::byte{0x07}) };

        if (m_board.isPieceAtIndex(constants::PAWN, initialSquare)) {
            if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + 10)) { 
                if ((constants::board64[7] < initialSquare) && 
                    (initialSquare < constants::board64[16]) && 
                    m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + 20)) {
                    
                    moves.emplace_back(
                        static_cast<unsigned int>(initialSquare),
                        static_cast<unsigned int>(initialSquare + 20),
                        0b0001,
                        static_cast<unsigned int>(m_board[initialSquare]),
                        static_cast<unsigned int>(constants::EMPTY_SQUARE)
                    );
                } 
                moves.emplace_back(
                    static_cast<unsigned int>(initialSquare),
                    static_cast<unsigned int>(initialSquare + 10),
                    0,
                    static_cast<unsigned int>(m_board[initialSquare]),
                    static_cast<unsigned int>(constants::EMPTY_SQUARE)
                );
            }
            if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 9)) {
                
            }
            if (m_board.isPieceOfColorAtIndex(constants::BLACK, initialSquare + 11)) {}
            //do pawn stuff
            continue;
        }

        for (int j {0}; j < constants::offset_count[pieceInt]; ++j) {
            for (int move { initialSquare } ;;) {
                move += constants::offsets[pieceInt][j];
                if (m_board.isPieceAtIndex(constants::SENTINAL, move))
                    break;
                if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, move)) {
                    moves.emplace_back(
                        static_cast<unsigned int>(initialSquare),
                        static_cast<unsigned int>(move),
                        0,
                        static_cast<unsigned int>(m_board[initialSquare]),
                        static_cast<unsigned int>(constants::EMPTY_SQUARE)
                    );
                } else {
                    if (m_board.isColorAtIndex(constants::WHITE, move))
                        break;
                    moves.emplace_back(
                        static_cast<unsigned int>(initialSquare),
                        static_cast<unsigned int>(move),
                        static_cast<unsigned int>(0b0100),
                        static_cast<unsigned int>(m_board[initialSquare]),
                        static_cast<unsigned int>(m_board[move])
                    );
                    break;
                }

                if (!constants::slide[pieceInt])
                   break; 
            }
        }
        
    }



    return moves;
}
