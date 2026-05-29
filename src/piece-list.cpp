#include "piece-list.h"

PieceList::PieceList() {}
void PieceList::append(std::byte piece, uint8_t boardIndex) {
  if (boardIndex > 63)
    return;
  if (m_size >= m_pieceList.size())
    return;
  m_indices[boardIndex] = m_size;
  m_pieceList[m_size++] = boardIndex;
}

uint8_t PieceList::size() const {
  return m_size;
}

uint8_t PieceList::capacity() const {
  return m_pieceList.size();
}

uint8_t PieceList::operator[](int i) const {
  return m_pieceList[i];
}
