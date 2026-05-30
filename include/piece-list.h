#pragma once

#include <array>
#include <cstdint>

class PieceList {
public:
  PieceList();
  void append(uint8_t);
  void remove(uint8_t);
  uint8_t size() const;

  int8_t at(uint8_t) const;
  int8_t operator[](uint8_t) const;

private:
  std::array<int8_t, 32> m_pieceList{}; // Stores indices into Board Array
  std::array<int8_t, 120> m_indices{}; // Stores indices into pieceLists indexed with board index
  uint8_t m_size{};
};
