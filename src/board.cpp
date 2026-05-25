#include <iostream>

#include "board.h"
#include "constants.h"

Board::Board() {
    for(auto i : constants::board64)
        m_board[i] = constants::EMPTY_SQUARE;
}

bool Board::isPieceAtIndex(std::byte piece, uint8_t index) const {
    return (m_board[index] & std::byte{0x07}) == piece;
}

bool Board::isColorAtIndex(std::byte color, uint8_t index) const {
    return (m_board[index] & std::byte{0x80}) == color;
}

bool Board::isPieceOfColorAtIndex(std::byte color, uint8_t index) const {
    return (!isPieceAtIndex(constants::EMPTY_SQUARE, index) && 
            !isPieceAtIndex(constants::SENTINAL, index) && 
            isColorAtIndex(color, index));
}

std::byte& Board::operator[](uint8_t i) {
    return m_board[i];
}

void Board::debugPrint() const {
    for(int i {0}; i < m_board.size(); ++i) {
        if ((i % 10) == 0)
            std::cout << '\n';
        switch (m_board[i]) {
        case constants::SENTINAL:
            std::cout << "FF ";
            break;
        case constants::EMPTY_SQUARE:
            std::cout << "XX ";
            break;
        // White pieces
        case constants::WHITE | constants::PAWN:
            std::cout << "WP ";
            break;
        case constants::WHITE | constants::ROOK:
            std::cout << "WR ";
            break;
        case constants::WHITE | constants::KNIGHT:
            std::cout << "WN ";
            break;
        case constants::WHITE | constants::BISHOP:
            std::cout << "WB ";
            break;
        case constants::WHITE | constants::QUEEN:
            std::cout << "WQ ";
            break;
        case constants::WHITE | constants::KING:
            std::cout << "WK ";
            break;
        // Black pieces
        case constants::BLACK | constants::PAWN:
            std::cout << "BP ";
            break;
        case constants::BLACK | constants::ROOK:
            std::cout << "BR ";
            break;
        case constants::BLACK | constants::KNIGHT:
            std::cout << "BN ";
            break;
        case constants::BLACK | constants::BISHOP:
            std::cout << "BB ";
            break;
        case constants::BLACK | constants::QUEEN:
            std::cout << "BQ ";
            break;
        case constants::BLACK | constants::KING:
            std::cout << "BK ";
            break;
        default:
            std::cout << "?? ";
            break;
        }
    }
}
