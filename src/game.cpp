#include <iostream>
#include <cassert>

#include "game.h"

Game::Game(std::string_view fenStr) {
    int fenIndex { populateBoard(fenStr) };
    setWhiteTurn(fenStr, fenIndex);
    setCastlingRules(fenStr, fenIndex);
    setEnPassant(fenStr, fenIndex);
    setHalfMove(fenStr, fenIndex);
    setFullMove(fenStr, fenIndex);
}


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

int Game::populateBoard(std::string_view fenStr) {
    using constants::board64;

    int fenIndex { 0 };
    int boardIndex {};
    for(int i { 0 }; i < board64.size(); ++fenIndex) {
        if (fenStr[fenIndex] == '/')
            continue;
        if (isdigit(fenStr[fenIndex])) {
            i += fenStr[fenIndex] - '0';
            continue;
        }

        boardIndex = i ^ 56;       // fen string stores pieces with Big endian ranks and Little endian file - Ex:  a8 b8 c8 ... a7 b7 ... g1 h1
                                   // The board is stored as little endian ranks and files.                 - Ex:  a1 b1 c1 ... a2 b2 ... g8 h8
                                   // https://www.chessprogramming.org/Square_Mapping_Considerations#Endianness
                                   // squareIndexLittleEndianRank = squareIndexBigEndianRank    ^ 56;
        m_board[board64[boardIndex]] = charToPiece(fenStr[fenIndex]);
        if (islower(fenStr[fenIndex])) {
            m_board[board64[boardIndex]] |= constants::BLACK;
            m_blackList.append(m_board[board64[boardIndex]], boardIndex);
        } else {
            m_whiteList.append(m_board[board64[boardIndex]], boardIndex);
        }
        ++i;
    }
    return fenIndex+1;
}

void Game::setWhiteTurn(std::string_view fenStr, int& fenIndex) {
    if (fenStr.size() <= fenIndex) {
        m_whiteTurn = true;
        return ;
    }

    m_whiteTurn = (fenStr[fenIndex] == 'w');
    fenIndex += 2;
}

void Game::setCastlingRules(std::string_view fenStr, int& fenIndex) {
    while (fenIndex < fenStr.size()) {
        switch (fenStr[fenIndex]) {
        case 'K':
            m_canCastleKQkq.set(0, true);
            break;
        case 'Q':
            m_canCastleKQkq.set(1, true);
            break;
        case 'k':
            m_canCastleKQkq.set(2, true);
            break;
        case 'q':
            m_canCastleKQkq.set(3, true);
            break;
        case '-':
            fenIndex += 2;
            return;
        case ' ':
            ++fenIndex;
            return;
        default:
            assert(false && "Should never reach here");
            return;
        }
        ++fenIndex;
    }
}

void Game::setEnPassant(std::string_view fenStr, int& fenIndex) {
    int val { 0 };
    if (fenIndex >= (fenStr.size() + 1)) { // No fen str should ever have the final character '-' for the
        return;                             // en passant square. assuming there is 2 chars to read if
    }                                       // there is 1
    char c { fenStr[fenIndex] };
    if (c == '-') {
        fenIndex += 2;
        return;
    }
    if ('a' > c || c > 'h') {
        fenIndex += 2;
        return;
    }
    val += (c - 'a') * 8;
    ++fenIndex;
    if ('1' > c || c > '8') {
        ++fenIndex;
        return;
    }
    val += (c - '1');
    m_enPassant = val;
    fenIndex += 2;
}

void Game::setHalfMove(std::string_view fenStr, int& fenIndex) {
    m_halfMoveClock = extractInt(fenStr, fenIndex);
}

void Game::setFullMove(std::string_view fenStr, int& fenIndex) {
    m_fullMoveCount = extractInt(fenStr, fenIndex);
}

int Game::extractInt(std::string_view fenStr, int& fenIndex) {
    int ans {};
    char c {};
    while (fenIndex < fenStr.size()) {
        c = fenStr[fenIndex];
        if (c == ' ') {
            ++fenIndex;
            return ans;
        }
        else if ('0' > c || c > '9') {
            return ans;
        } else {
            ans = ans * 10 + (c - '0');
        }
        ++fenIndex;
    }

    return ans;
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
