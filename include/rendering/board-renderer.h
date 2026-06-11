#pragma once

#include "board.h"
#include "piece-list.h"
#include "plane.h"
#include <vector>

class BoardRenderer : public Plane {
public:
  BoardRenderer();
  BoardRenderer(ncplane*, bool, const PieceList&, const PieceList&, const Board&);

  void render(const PieceList&, const PieceList&, const Board&);

private:
  void renderSquares();
  void renderPieces(const PieceList&, const PieceList&, const Board&);
  void renderList(const PieceList&, const Board&, uint32_t, uint32_t);
  void renderSelector();

  int m_squareHeight{};
  int m_squareWidth{};
  bool m_whitePerspective{};
  uint32_t m_lightSquareChannel{};
  uint32_t m_darkSquareChannel{};
  unsigned int m_xorValue{};
  int m_squareSelected{};
  bool m_selected{};

  std::vector<ncplane*> m_pieces{};
  ncplane* m_selector{};
};
