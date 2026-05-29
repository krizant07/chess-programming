#pragma once

#include "constants.h"

class Board {
public:
  Board();

  int populate(std::string_view);

  bool isPieceAtIndex(std::byte, uint8_t) const;
  bool isColorAtIndex(std::byte, uint8_t) const;
  bool isPieceOfColorAtIndex(std::byte, uint8_t) const;
  bool isAtPawnHomeRankOfColor(std::byte, uint8_t) const;

  std::byte& operator[](uint8_t);

  std::byte at(uint8_t) const;

  void debugPrint() const;

private:
  BoardArray m_board{constants::sentinal_board};
};
