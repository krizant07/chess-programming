#pragma once

#include "plane.h"

class Menu : public Plane {
public:
  Menu();
  Menu(ncplane*);

  void render();

private:
};
