#include <iostream>

#include "board.h"
#include "constants.h"

Board::Board() {
  for (auto i : constants::board64)
    m_board[i] = constants::EMPTY_SQUARE;
}

bool Board::isPieceAtIndex(std::byte piece, uint8_t index) const {
  return (m_board[index] & std::byte{0x07}) == (piece & std::byte{0x07});
}

bool Board::isColorAtIndex(std::byte color, uint8_t index) const {
  return (m_board[index] & std::byte{0x80}) == color;
}

bool Board::isPieceOfColorAtIndex(std::byte color, uint8_t index) const {
  return (!isPieceAtIndex(constants::EMPTY_SQUARE, index) && !isPieceAtIndex(constants::SENTINAL, index) &&
          isColorAtIndex(color, index));
}

bool Board::isAtPawnHomeRankOfColor(std::byte color, uint8_t index) const {
  if (color == pieces::WHITE)
    return (constants::board64[H1] < index) && (index < constants::board64[A3]);
  else
    return (constants::board64[H6] < index) && (index < constants::board64[A8]);
}

std::byte& Board::operator[](uint8_t i) {
  return m_board[i];
}

std::byte Board::at(uint8_t i) const {
  return m_board[i];
}

void Board::debugPrint() const {
  for (int i{0}; i < m_board.size(); ++i) {
    if ((i % 10) == 0)
      std::cout << '\n';
    pieces::print(m_board[i]);
  }
}
