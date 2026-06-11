#include "rendering/game-renderer.h"

GameRenderer::GameRenderer() = default;

GameRenderer::GameRenderer(ncplane* parent, const Game& game) : Plane(parent) {
  m_boardRenderer = BoardRenderer(m_plane, true, game.whiteList(), game.blackList(), game.board());
}

void GameRenderer::render() {}

void GameRenderer::handleInput(const ncinput& notCursesInput, Game& game) {}
