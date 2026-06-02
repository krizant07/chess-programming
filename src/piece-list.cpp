#include <iostream>

#include "piece-list.h"

PieceList::PieceList() {
  m_pieceList.fill(-1);
}

uint8_t PieceList::append(uint8_t boardIndex) {
  m_pieceList[m_size] = boardIndex;
  return m_size++; // return size then increment (readable but kinda weird)
}

uint8_t PieceList::remove(uint8_t index) {
  m_pieceList[index] = m_pieceList[--m_size];
  m_pieceList[m_size] = -1;
  return m_pieceList[index]; // return the index of the BoardArray, so we can update the index inside the Piece object
}

void PieceList::move(uint8_t index, uint8_t to) {
  std::cout << "yo.\n";
  std::cout << "index: " << int(index) << '\n';
  m_pieceList[index] = to;
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
