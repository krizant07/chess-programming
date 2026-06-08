#pragma once

#include "board.h"
#include "piece-list.h"

#include <array>
#include <notcurses/notcurses.h>

class BoardRenderer {
public:
  BoardRenderer(ncplane*, int, int);

  void render(bool, const PieceList&, const PieceList&, const Board&);

private:
  void renderSquares(bool);
  void renderPieces(bool, const PieceList&, const PieceList&, const Board&);
  void renderList(const PieceList&, const Board&, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);

  ncplane* m_plane{};
  std::array<ncplane*, 64> m_squarePlanes{};
  int m_squareHeight{};
  int m_squareWidth{};
  int m_squareSelected{};
  uint32_t m_lightSquareChannel{};
  uint32_t m_darkSquareChannel{};
};
