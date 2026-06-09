#include "rendering/board-renderer.h"
#include "constants.h"

BoardRenderer::BoardRenderer() = default;

BoardRenderer::BoardRenderer(ncplane* parent)
    : m_squareHeight{4}, m_squareWidth{9}, m_squareSelected{0},
      m_lightSquareChannel{(162u << 16u) + (205u << 8u) + 90u},
      m_darkSquareChannel{(255u << 16u) + (231u << 8u) + 186u} {
  int boardHeight{m_squareHeight * 8};
  int boardWidth{m_squareWidth * 8};
// clang-format off
  ncplane_options board_opts{(static_cast<int>(ncplane_dim_y(parent)) - boardHeight) / 2,
                             (static_cast<int>(ncplane_dim_x(parent)) - boardWidth) / 2,
                             static_cast<unsigned int>(boardHeight),
                             static_cast<unsigned int>(boardWidth),
                             nullptr, nullptr, nullptr, 0, 0, 0};

  m_plane = ncplane_create(parent, &board_opts);

  ncplane_options square_opts{0, 0,
                              static_cast<unsigned int>(m_squareHeight),
                              static_cast<unsigned int>(m_squareWidth),
                              nullptr, nullptr, nullptr, 0, 0, 0};
// clang-format on
  for (std::size_t i = 0; i < 64; ++i) {
    square_opts.y = (i / 8) * m_squareHeight;
    square_opts.x = (i % 8) * m_squareWidth;
    m_squarePlanes[i] = ncplane_create(m_plane, &square_opts);
  }
}

void BoardRenderer::render(bool whitePerspective, const PieceList& whiteList, const PieceList& blackList,
                           const Board& board) {
  renderSquares(whitePerspective);
  renderPieces(whitePerspective, whiteList, blackList, board);
}

void BoardRenderer::renderSquares(bool whitePerspective) {
  uint32_t lightDarkXor{m_darkSquareChannel ^ m_lightSquareChannel};
  [[maybe_unused]] uint32_t squareChannel{whitePerspective ? m_darkSquareChannel : m_lightSquareChannel};

  for (std::size_t i{0}; i < m_squarePlanes.size(); ++i) {
    if (i % 8 == 0)
      squareChannel ^= lightDarkXor;

    ncplane_set_bg_rgb(m_squarePlanes[i], squareChannel);
    for (int j{0}; j < m_squareHeight; ++j) {
      ncplane_putstr_yx(m_squarePlanes[i], j, 0, "         ");
    }

    squareChannel ^= lightDarkXor;
  }
}

void BoardRenderer::renderPieces(bool whitePerspective, const PieceList& whiteList, const PieceList& blackList,
                                 const Board& board) {
  int xorValue{whitePerspective ? 56 : 7};
  int asciiHeight{pieces::pieceASCII[0].size()};
  int asciiWidth{pieces::pieceASCII[0][0].size()};
  uint32_t whiteColor{(255 << 16) + (255 << 8) + 255};
  uint32_t blackColor{0};
  uint32_t outlineColor{0};

  renderList(whiteList, board, xorValue, asciiHeight, asciiWidth, whiteColor, outlineColor);
  renderList(blackList, board, xorValue, asciiHeight, asciiWidth, blackColor, outlineColor);
}

void BoardRenderer::renderList(const PieceList& pieceList, const Board& board, uint8_t xorValue, uint8_t asciiHeight,
                               uint8_t asciiWidth, uint32_t pieceColor, uint32_t outlineColor) {
  ncplane* plane{};
  uint8_t code{};
  for (std::size_t i{0}; i < pieceList.size(); ++i) {
    const Piece& piece{board.at(pieceList[i])};
    plane = m_squarePlanes[constants::board120[pieceList[i]] ^ xorValue];
    code = static_cast<uint8_t>(piece.code());
    for (int j{0}; j < asciiHeight; ++j) {
      ncplane_set_fg_rgb(plane, outlineColor);
      for (int k{0}; k < asciiWidth; ++k) {
        if (pieces::pieceASCII[code][j][k] == ' ')
          continue;
        if (k + 1 == asciiWidth) {
          ncplane_set_fg_rgb(plane, outlineColor);
        } else if (pieces::pieceASCII[code][j][k + 1] == ' ') {
          ncplane_set_fg_rgb(plane, outlineColor);
        }
        ncplane_putchar_yx(plane, j + m_squareHeight - asciiHeight, k + ((m_squareWidth - asciiWidth) / 2),
                           pieces::pieceASCII[code][j][k]);
        ncplane_set_fg_rgb(plane, pieceColor);
      }
    }
  }
}
