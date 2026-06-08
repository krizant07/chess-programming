#pragma once

#include "fen-parser.h"
#include "game.h"

#include <notcurses/notcurses.h>

class Application {
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
  int m_height{};
  int m_width{};
};
