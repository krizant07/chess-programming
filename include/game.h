#pragma once

#include <bitset>

#include "board.h"
#include "move-list.h"
#include "piece-list.h"
#include "types.h"

class Game {
public:
  Game();

  MoveList debugPrint();

  void makeMove(Move);
  void printMovesOfSquare(MoveList&, enum Square);

  const Board& board() const;
  bool whiteTurn() const;
  const PieceList& whiteList() const;
  const PieceList& blackList() const;

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

  void printList(std::byte) const;

  void listRemoveHelper(PieceList&, uint8_t);
  void appendMoveHelper(MoveList&, uint8_t, int8_t,
                        uint8_t flags = 0b0000) const;         // Ts all needs to be in a move generator class.
  void appendCaptureHelper(MoveList&, uint8_t, int8_t, uint8_t flags = 0b0100) const;
  void appendPawnPromotion(MoveList&, uint8_t, int8_t) const;
  void appendPawnPromotionCapture(MoveList&, uint8_t, int8_t) const;
  uint8_t generatePseudoLegalPawnMoves(MoveList&, std::byte, std::byte, uint8_t) const;
  uint8_t handleCastleGeneration(MoveList&, uint8_t, uint8_t) const;

  MoveList generatePseudoLegal();
};
