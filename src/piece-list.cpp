#include <iostream>
#include "piece-list.h"
#include "constants.h"

PieceList::PieceList() {}
void PieceList::append(std::byte piece, uint8_t boardIndex) {
    if (boardIndex > 63)
        return;
    if (m_size >= m_pieceList.size())
        return;
    m_indices[boardIndex] = m_size;
    m_pieceList[m_size++] = piece;
}

void PieceList::debugPrint() {
    for (int i {0}; i < m_size; ++i) {
        switch (m_pieceList[i] & std::byte{0x07}) {
        case constants::PAWN:
            std::cout << "Pawn ";
            break;
        case constants::ROOK:
            std::cout << "Rook ";
            break;
        case constants::KNIGHT:
            std::cout << "Knight ";
            break;
        case constants::BISHOP:
            std::cout << "Bishop ";
            break;
        case constants::QUEEN:
            std::cout << "Queen ";
            break;
        case constants::KING:
            std::cout << "King ";
            break;
        default:
            std::cout << "Unknown ";
            break;
        }
    }
}
