#pragma once

#include "plane.h"

#include "constants.h"

class Menu : public Plane {
public:
  Menu();
  Menu(ncplane*);

  menuStrings::menuItem selection() const;
  void next();
  void previous();

  void render();

private:
  enum menuStrings::menuItem m_selection{};
};
