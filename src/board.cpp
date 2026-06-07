#include <iostream>

#include "board.h"
#include "constants.h"

Board::Board() {
  for (auto& i : m_board)
    i.setCode(pieces::SENTINAL);
  for (auto i : constants::board64)
    m_board[i].setCode(pieces::EMPTY);
}

bool Board::isPieceAtIndex(std::byte piece, uint8_t index) const {
  return m_board[index].code() == (piece & std::byte{0x07});
}

bool Board::isColorAtIndex(std::byte color, uint8_t index) const {
  return m_board[index].color() == (color & std::byte{0x08});
}

bool Board::isPieceOfColorAtIndex(std::byte color, uint8_t index) const {
  return (!isPieceAtIndex(pieces::EMPTY, index) && !isPieceAtIndex(pieces::SENTINAL, index) &&
          isColorAtIndex(color, index));
}

bool Board::isAtPawnHomeRankOfColor(std::byte color, uint8_t index) const {
  if (color == pieces::WHITE)
    return (constants::board64[H1] < index) && (index < constants::board64[A3]);
  else
    return (constants::board64[H6] < index) && (index < constants::board64[A8]);
}

Piece& Board::operator[](uint8_t i) {
  return m_board[i];
}

const Piece& Board::at(uint8_t i) const {
  return m_board[i];
}

void Board::debugPrint() const {
  for (std::size_t i{0}; i < m_board.size(); ++i) {
    if ((i % 10) == 0)
      std::cout << '\n';
    m_board[i].print();
  }
}
