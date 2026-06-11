#pragma once

#include "game.h"
#include "rendering/board-renderer.h"

class GameRenderer : public Plane {
public:
  GameRenderer();
  GameRenderer(ncplane*, const Game&);
  void render();
  void handleInput(const ncinput&, Game&);

private:
  BoardRenderer m_boardRenderer{};
};
