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

void PieceList::move(uint8_t from, uint8_t to) {
  if (m_indices[from] == -1)
    return;
  m_pieceList[m_indices[from]] = to;
  m_indices[to] = m_indices[from];
  m_indices[from] = -1;
}

uint8_t PieceList::size() const {
  return m_size;
}

void PieceList::setMoveCount(uint8_t i, int8_t count) {
  m_moveCount[i] = count;
}

int8_t PieceList::moveCount(uint8_t i) const {
  return m_moveCount[i];
}

int8_t PieceList::operator[](uint8_t i) const {
  return m_pieceList[i];
}
