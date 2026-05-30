#include <cassert>
#include <iostream>

#include "fen-parser.h"

std::ostream& operator<<(std::ostream& os, std::byte b) {
  return os << std::bitset<8>(std::to_integer<int>(b));
}

void FenParser::setup(Game& game, std::string_view fenStr) {
  m_fenIndex = 0;
  m_fenStr = fenStr;
  populateBoard(game);
  setWhiteTurn(game);
  setCastlingRules(game);
  setEnPassant(game);
  setHalfMove(game);
  setFullMove(game);
}

FenParser::FenParser() {};

void FenParser::populateBoard(Game& game) {
  int8_t board64Index{};
  int8_t boardIndex{};
  for (int i{0}; i < constants::board64.size(); ++m_fenIndex) {
    char c{m_fenStr[m_fenIndex]};
    if (c == '/')
      continue;
    if (isdigit(c)) {
      i += c - '0';
      continue;
    }

    // fen string stores pieces with Big endian ranks and Little endian file - Ex:  a8 b8 c8 ... a7 b7 ... g1 h1
    // The board is stored as little endian ranks and files.                 - Ex:  a1 b1 c1 ... a2 b2 ... g8 h8
    // https://www.chessprogramming.org/Square_Mapping_Considerations#Endianness
    // squareIndexLittleEndianRank = squareIndexBigEndianRank    ^ 56;
    board64Index = i ^ 56;
    boardIndex = constants::board64[board64Index];

    std::byte& square{game.m_board[boardIndex]};
    square = pieces::charToPiece(c);
    if (islower(c)) {
      square |= pieces::BLACK;
      game.m_blackList.append(boardIndex);
    } else {
      game.m_whiteList.append(boardIndex);
    }
    ++i;
  }
  ++m_fenIndex;
}

void FenParser::setWhiteTurn(Game& game) {
  if (m_fenStr.size() <= m_fenIndex) {
    game.m_whiteTurn = true;
    return;
  }

  game.m_whiteTurn = (m_fenStr[m_fenIndex] == 'w');
  m_fenIndex += 2;
}

void FenParser::setCastlingRules(Game& game) {
  while (m_fenIndex < m_fenStr.size()) {
    switch (m_fenStr[m_fenIndex]) {
    case 'K': game.m_canCastleKQkq.set(0, true); break;
    case 'Q': game.m_canCastleKQkq.set(1, true); break;
    case 'k': game.m_canCastleKQkq.set(2, true); break;
    case 'q': game.m_canCastleKQkq.set(3, true); break;
    case '-': m_fenIndex += 2; return;
    case ' ': ++m_fenIndex; return;
    default: assert(false && "Should never reach here"); return;
    }
    ++m_fenIndex;
  }
}

void FenParser::setEnPassant(Game& game) {
  int val{0};
  if (m_fenIndex >= (m_fenStr.size() + 1)) { // No fen str should ever have the final character '-' for the
    return;                                  // en passant square. assuming there is 2 chars to read if
  }                                          // there is 1
  char c{m_fenStr[m_fenIndex]};
  if (c == '-') {
    m_fenIndex += 2;
    return;
  }
  if ('a' > c || c > 'h') {
    m_fenIndex += 2;
    return;
  }
  val += (c - 'a') * 8;
  ++m_fenIndex;
  c = m_fenStr[m_fenIndex];
  if ('1' > c || c > '8') {
    ++m_fenIndex;
    return;
  }
  val += (c - '1');
  game.m_enPassant = val;
  m_fenIndex += 2;
}

void FenParser::setHalfMove(Game& game) {
  game.m_halfMoveClock = extractInt();
}

void FenParser::setFullMove(Game& game) {
  game.m_fullMoveCount = extractInt();
}

int FenParser::extractInt() {
  int ans{};
  char c{};
  while (m_fenIndex < m_fenStr.size()) {
    c = m_fenStr[m_fenIndex];
    if (c == ' ') {
      ++m_fenIndex;
      return ans;
    } else if ('0' > c || c > '9') {
      return ans;
    } else {
      ans = ans * 10 + (c - '0');
    }
    ++m_fenIndex;
  }

  return ans;
}
