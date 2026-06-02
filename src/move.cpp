#include "move.h"

Move::Move() {}

Move::Move(uint8_t from, uint8_t to, uint8_t flags, uint8_t fromPiece, uint8_t toPiece)
    : m_data{((toPiece & 7u) << 21) | ((fromPiece & 7u) << 18) | ((flags & 15u) << 14) | ((to & 127u) << 7) |
             (from & 127u)} {}

uint8_t Move::from() const {
  return (m_data & 127u);
}

uint8_t Move::to() const {
  return ((m_data >> 7) & 127u);
}

uint8_t Move::flags() const {
  return ((m_data >> 14) & 15u);
}

std::byte Move::fromPiece() const {
  return static_cast<std::byte>((m_data >> 18) & 7u);
}

std::byte Move::toPiece() const {
  return static_cast<std::byte>((m_data >> 21) & 7u);
}

std::ostream& operator<<(std::ostream& cout, const Move& move) {
  return cout << "piece at square " << int(move.from()) << " to square " << int(move.to()) << ".\n";
}
