#include "piece-list.h"

PieceList::PieceList() {
  m_pieceList.fill(-1);
  m_indices.fill(-1);
}
void PieceList::append(uint8_t boardIndex) {
  if (boardIndex > 119)
    return;
  if (m_size >= m_pieceList.size())
    return;
  m_indices[boardIndex] = m_size;
  m_pieceList[m_size++] = boardIndex;
}

void PieceList::remove(uint8_t boardIndex) {
  if (boardIndex > 119)
    return;
  if (m_size == 0)
    return;
  if (m_indices[boardIndex] == -1)
    return;
  m_pieceList[m_indices[boardIndex]] = m_pieceList[--m_size];
  m_indices[m_pieceList[m_size]] = m_indices[boardIndex];
  m_indices[boardIndex] = -1;
}

uint8_t PieceList::size() const {
  return m_size;
}

int8_t PieceList::operator[](uint8_t i) const {
  return m_pieceList[i];
}
