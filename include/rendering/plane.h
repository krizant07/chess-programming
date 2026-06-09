#pragma once

#include <notcurses/notcurses.h>

class Plane {
public:
  Plane();
  Plane(ncplane*);
  void show();
  void hide();
  void top();

protected:
  ncplane* m_plane{};

private:
};
