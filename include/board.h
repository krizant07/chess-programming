#pragma once

#include "constants.h"

class Board {
public:
  Board();

  bool isPieceAtIndex(std::byte, uint8_t) const;
  bool isColorAtIndex(std::byte, uint8_t) const;
  bool isPieceOfColorAtIndex(std::byte, uint8_t) const;
  bool isAtPawnHomeRankOfColor(std::byte, uint8_t) const;

  Piece& operator[](uint8_t);

  const Piece& at(uint8_t) const;

  void debugPrint() const;

private:
  BoardArray m_board{};
};
