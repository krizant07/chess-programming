#pragma once

#include <array>
#include <cstdint>

class PieceList {
public:
  PieceList();
  uint8_t size() const;

  uint8_t append(uint8_t);
  uint8_t remove(uint8_t);
  void move(uint8_t, uint8_t);

  void setMoveCount(uint8_t, int8_t);
  int8_t moveCount(uint8_t) const;

  int8_t operator[](uint8_t) const;

private:
  std::array<int8_t, 16> m_pieceList{};      // Stores indices into Board Array
  std::array<uint8_t, 16> m_moveListIndex{}; // Index into MoveList of Game object
  std::array<uint8_t, 16> m_moveCount{};     // Will be updated with pseudo move gen, used to index into the MoveList.
  uint8_t m_size{};
};
