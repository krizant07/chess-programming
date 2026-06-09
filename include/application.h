#pragma once

#include "fen-parser.h"
#include "rendering/game-renderer.h"
#include "rendering/menu.h"

class Application {
  enum Mode { menu, game, settings, max_mode };

public:
  Application();
  ~Application();

  void run();

private:
  static Game buildGame(std::string_view fenStr) {
    FenParser parser{};
    Game game{};

    parser.setup(game, fenStr);
    return game;
  }

  notcurses* m_notCurses{};
  ncplane* m_stdPlane{};
  ncinput m_notCursesInput{};
  timespec m_timeSpec{};

  Mode m_mode{};

  Game m_game{};

  Menu m_menu{};
  GameRenderer m_gameRenderer{};
};
