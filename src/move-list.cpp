#include "move-list.h"
#include "constants.h"

MoveList::MoveList() {};

std::size_t MoveList::size() const {
  return m_list.size();
}

void MoveList::appendMove(std::byte piece, uint8_t initialSquare, int8_t offset, uint8_t flags) {
  m_list.emplace_back(static_cast<unsigned int>(initialSquare), static_cast<unsigned int>(initialSquare + offset),
                      static_cast<unsigned int>(flags), static_cast<unsigned int>(piece),
                      static_cast<unsigned int>(constants::EMPTY_SQUARE));
}

void MoveList::appendCapture(std::byte piece, std::byte captured, uint8_t initialSquare, int8_t offset, uint8_t flags) {
  m_list.emplace_back(static_cast<unsigned int>(initialSquare), static_cast<unsigned int>(initialSquare + offset),
                      static_cast<unsigned int>(flags), static_cast<unsigned int>(piece),
                      static_cast<unsigned int>(captured));
}
