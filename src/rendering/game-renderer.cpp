#include "rendering/game-renderer.h"

GameRenderer::GameRenderer() = default;

GameRenderer::GameRenderer(ncplane* parent) : Plane(parent) {
  m_boardRenderer = BoardRenderer(m_plane);
}

void GameRenderer::render(const Game& game) {
  m_boardRenderer.render(game.whiteTurn(), game.whiteList(), game.blackList(), game.board());
}

void GameRenderer::handleInput(const ncinput& notCursesInput, Game& game) {}
