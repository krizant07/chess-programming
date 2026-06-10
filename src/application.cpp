#include "application.h"
#include "constants.h"

#include <cassert>
#include <clocale>

Application::Application() {
  setlocale(LC_ALL, "");
  m_notCurses = notcurses_init(nullptr, stdout);
  m_stdPlane = notcurses_stdplane(m_notCurses);
  m_running = true;
  m_mode = Mode::menu;

  m_menu = Menu(m_stdPlane);
  m_gameRenderer = GameRenderer(m_stdPlane);

  m_menu.show();
  m_menu.top();
}

Application::~Application() {
  notcurses_stop(m_notCurses);
}

void Application::render() {
  switch (m_mode) {
  case menu: m_menu.render(); break;
  case game: m_gameRenderer.render(m_game); break;
  case settings: break;
  default: assert(false && "NO!!!!");
  }
  notcurses_render(m_notCurses);
}

void Application::handleInput() {
  if (m_notCursesInput.evtype != NCTYPE_PRESS) {
    return;
  }
  char input{m_notCursesInput.utf8[0]};
  uint32_t id{m_notCursesInput.id};
  if (ncinput_ctrl_p(&m_notCursesInput)) {
    if (input == 'Q') {
      m_running = false;
    }
  }

  switch (m_mode) {
  case menu:
    if (input == 'j' || id == NCKEY_DOWN) {
      m_menu.next();
    }
    if (input == 'k' || id == NCKEY_UP) {
      m_menu.previous();
    }
    if (id == NCKEY_ENTER) {
      using enum menuStrings::menuItem;
      switch (m_menu.selection()) {
      case play: break;
      case settings: break;
      case quit: m_running = false; break;
      default: assert(false && "NO!!!!");
      }
    }
    break;
  case game: break;
  default: assert(false && "NO!!!!");
  }
}

void Application::run() {
  m_game = buildGame(constants::startingFenString);

  while (m_running) {
    if (notcurses_get(m_notCurses, &m_timeSpec, &m_notCursesInput)) {
      handleInput();
    }
    render();
    clock_gettime(CLOCK_MONOTONIC, &m_timeSpec);
    m_timeSpec.tv_nsec += 100'000'000; // 100ms, 10fps
  }
}
