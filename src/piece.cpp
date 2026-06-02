#include "piece.h"

#include "constants.h"

#include <cstddef>
#include <iostream>

Piece::Piece() {};

Piece::Piece(std::byte data) : m_piece{data} {};

std::byte Piece::code() const {
  return m_piece & (std::byte{0b0111});
}

std::byte Piece::color() const {
  return m_piece & (std::byte{0b1000});
}

uint8_t Piece::pieceListIndex() const {
  return static_cast<uint8_t>(m_piece >> 4);
}

void Piece::setCode(std::byte code) {
  m_piece = (m_piece & std::byte{0b1111'1000}) | code;
}

void Piece::setCode(char c) {
  std::byte code{};
  switch (tolower(c)) {
  case 'p': code = pieces::PAWN; break;
  case 'r': code = pieces::ROOK; break;
  case 'n': code = pieces::KNIGHT; break;
  case 'b': code = pieces::BISHOP; break;
  case 'q': code = pieces::QUEEN; break;
  case 'k': code = pieces::KING; break;
  default: code = pieces::EMPTY; break;
  }

  m_piece = (m_piece & std::byte{0b1111'1000}) | code;
}

void Piece::setColor(std::byte color) {
  m_piece = (m_piece & std::byte{0b1111'0111}) | color;
}

void Piece::setPieceListIndex(uint8_t index) {
  m_piece = (m_piece & std::byte{0b0000'1111}) | static_cast<std::byte>(index) << 4;
}

void Piece::print() const {
  std::byte pieceCode{code()};
  if (pieceCode == (pieces::SENTINAL & std::byte{0x07})) {
    std::cout << "XX ";
    return;
  }
  if (pieceCode == pieces::EMPTY) {
    std::cout << "** ";
    return;
  }
  switch (color()) {
  case pieces::WHITE: std::cout << 'W'; break;
  case pieces::BLACK: std::cout << 'B'; break;
  default: std::cout << '?';
  }
  switch (pieceCode) {
  case pieces::KNIGHT: std::cout << "N "; break;
  case pieces::BISHOP: std::cout << "B "; break;
  case pieces::ROOK: std::cout << "R "; break;
  case pieces::QUEEN: std::cout << "Q "; break;
  case pieces::KING: std::cout << "K "; break;
  case pieces::PAWN: std::cout << "P "; break;
  default: std::cout << "? "; break;
  }
}
