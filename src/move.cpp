#include "move.h"
#include "constants.h"

Move::Move(uint8_t from, uint8_t to, uint8_t flags, uint8_t fromPiece, uint8_t toPiece)
    : m_data{((toPiece & 7u) << 19) | ((fromPiece & 7u) << 16) | ((flags & 15u) << 12) | ((to & 63u) << 6) |
             (from & 63u)} {}

uint8_t Move::getFrom() const {
  return (m_data & 63u);
}

uint8_t Move::getTo() const {
  return ((m_data >> 6) & 63u);
}

uint8_t Move::getFlags() const {
  return ((m_data >> 12) & 15u);
}

std::byte Move::getFromPiece() const {
  return pieces::codeToPiece((m_data >> 16) & 7u);
}

std::byte Move::getToPiece() const {
  return pieces::codeToPiece((m_data >> 19) & 7u);
}
