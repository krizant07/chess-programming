#pragma once

#include <bitset>

#include "board.h"
#include "move-list.h"
#include "piece-list.h"

class Game {
public:
  Game();

  void debugPrint() const;

  friend class FenParser;

private:
  Board m_board{};
  bool m_whiteTurn{false};
  std::bitset<4> m_canCastleKQkq{};
  int m_enPassant{-1};
  int m_halfMoveClock{};
  int m_fullMoveCount{};

  PieceList m_whiteList{};
  PieceList m_blackList{};

  void appendMoveHelper(MoveList&, uint8_t, int8_t, uint8_t = 0b0000) const;
  void appendCaptureHelper(MoveList&, uint8_t, int8_t, uint8_t = 0b0100) const;
  void appendPawnPromotion(MoveList&, uint8_t, int8_t) const;
  void appendPawnPromotionCapture(MoveList&, uint8_t, int8_t) const;
  void generatePseudoLegalPawnMoves(MoveList&, std::byte, std::byte, uint8_t) const;
  MoveList generatePseudoLegal() const;

  void makeMove(Move);
};
