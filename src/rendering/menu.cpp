#include "rendering/menu.h"

Menu::Menu() = default;

Menu::Menu(ncplane* parent) : Plane(parent) {}

menuStrings::menuItem Menu::selection() const {
  return m_selection;
}

void Menu::next() {
  m_selection = static_cast<menuStrings::menuItem>(m_selection + 1);
  if (m_selection == menuStrings::menuItem::max_item) {
    m_selection = static_cast<menuStrings::menuItem>(0);
  }
}

void Menu::previous() {
  if (m_selection == 0) {
    m_selection = static_cast<menuStrings::menuItem>(menuStrings::menuItem::max_item - 1);
  } else {
    m_selection = static_cast<menuStrings::menuItem>(m_selection - 1);
  }
}

void Menu::render() {
  std::size_t i{0};
  for (; i < menuStrings::splashText.size(); ++i) {
    ncplane_putstr_yx(m_plane, i + 2, 4, menuStrings::splashText[i].data());
  }
  std::size_t y{};
  for (std::size_t j{0}; j < menuStrings::menuItem::max_item; ++j) {
    y = (j * 3) + i + 6;
    if (m_selection == j) {
      ncplane_putstr_yx(m_plane, y, 4, "> ");
      ncplane_putstr_yx(m_plane, y, 6, menuStrings::menuItems[j].data());
      continue;
    }
    ncplane_putstr_yx(m_plane, y, 4, menuStrings::menuItems[j].data());
    ncplane_putstr_yx(m_plane, y, 4 + menuStrings::menuItems[j].size(), "  ");
  }
}
