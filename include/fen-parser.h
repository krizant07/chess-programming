#pragma once

#include <string_view>

#include "game.h"

class FenParser {
public:
  FenParser();

  void setup(Game&, std::string_view);

private:
  int extractInt();

  void populateBoard(Game&);
  void setWhiteTurn(Game&);
  void setCastlingRules(Game&);
  void setEnPassant(Game&);
  void setHalfMove(Game&);
  void setFullMove(Game&);

  std::string_view m_fenStr{};
  std::size_t m_fenIndex{};
};
