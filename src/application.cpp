#include "application.h"

#include "rendering/board-renderer.h"

#include <clocale>

Application::Application() {
  setlocale(LC_ALL, "");
  m_notCurses = notcurses_init(nullptr, stdout);
  m_stdPlane = notcurses_stdplane(m_notCurses);
  m_height = static_cast<int>(ncplane_dim_y(m_stdPlane));
  m_width = static_cast<int>(ncplane_dim_x(m_stdPlane));
}

Application::~Application() {
  notcurses_stop(m_notCurses);
}

void Application::run() {
  Game game{buildGame(constants::startingFenString)};

  MoveList moves{game.debugPrint()};

  int n{8};
  game.printMovesOfSquare(moves, static_cast<Square>(n));
  BoardRenderer board{BoardRenderer(m_stdPlane, m_height, m_width)};
  board.render(game.whiteTurn(), game.whiteList(), game.blackList(), game.board());
  for (;;) {
    if (notcurses_get(m_notCurses, &m_timeSpec, &m_notCursesInput)) {
      if (ncinput_shift_p(&m_notCursesInput)) {
        if (m_notCursesInput.utf8[0] == 'Q') {
          break;
        }
      }
    }
    notcurses_render(m_notCurses);
    clock_gettime(CLOCK_MONOTONIC, &m_timeSpec);
    m_timeSpec.tv_nsec += 100'000'000; // 100ms, 10fps
  }
}
