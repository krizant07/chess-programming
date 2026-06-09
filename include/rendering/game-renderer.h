#pragma once

#include "game.h"
#include "rendering/board-renderer.h"

class GameRenderer : public Plane {
public:
  GameRenderer();
  GameRenderer(ncplane*);
  void render(const Game&);
  void handleInput(const ncinput&, Game&);

private:
  BoardRenderer m_boardRenderer{};
};
