#include "application.h"
#include "constants.h"

#include <clocale>

Application::Application() {
  setlocale(LC_ALL, "");
  m_notCurses = notcurses_init(nullptr, stdout);
  m_stdPlane = notcurses_stdplane(m_notCurses);
  m_gameRenderer = GameRenderer(m_stdPlane);
  m_menu = Menu(m_stdPlane);

  m_gameRenderer.show();
  m_gameRenderer.top();
}

Application::~Application() {
  notcurses_stop(m_notCurses);
}

void Application::run() {
  m_game = buildGame(constants::startingFenString);

  MoveList moves{m_game.debugPrint()};

  int n{8};
  m_game.printMovesOfSquare(moves, static_cast<Square>(n));
  for (;;) {
    if (notcurses_get(m_notCurses, &m_timeSpec, &m_notCursesInput)) {
      if (ncinput_shift_p(&m_notCursesInput)) {
        if (m_notCursesInput.utf8[0] == 'Q') {
          break;
        }
      }
    }
    m_gameRenderer.render(m_game);
    notcurses_render(m_notCurses);
    clock_gettime(CLOCK_MONOTONIC, &m_timeSpec);
    m_timeSpec.tv_nsec += 100'000'000; // 100ms, 10fps
  }
}
