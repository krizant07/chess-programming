#include "rendering/plane.h"

Plane::Plane() = default;

Plane::Plane(ncplane* parent) {
  int parentHeight{static_cast<int>(ncplane_dim_y(parent))};
  int parentWidth{static_cast<int>(ncplane_dim_x(parent))};
  // clang-format off
  ncplane_options board_opts{256, // hidden by default (offscreen)
                             256,
                             static_cast<unsigned int>(parentHeight),
                             static_cast<unsigned int>(parentWidth),
                             nullptr, nullptr, nullptr, 0, 0, 0};
  // clang-format on
  m_plane = ncplane_create(parent, &board_opts);
}

void Plane::show() {
  ncplane_move_yx(m_plane, 0, 0);
}

void Plane::hide() {
  ncplane_move_rel(m_plane, 256, 256);
}

void Plane::top() {
  ncplane_move_family_top(m_plane);
}
