#pragma once

#include <cstddef>
#include <cstdint>

class Piece {
public:
  Piece();

  void print() const;

  std::byte code() const;         // Piece code, knight, bishop, etc, as defined in constants.h
  std::byte color() const;        // Color, white or black, also in constants.h
  uint8_t pieceListIndex() const; // Index into respective pieceList

  void setCode(std::byte);
  void setCode(char);
  void setColor(std::byte);
  void setPieceListIndex(uint8_t);

private:
  std::byte m_piece{};
};
