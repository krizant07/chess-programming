#include "rendering/board-renderer.h"
#include "constants.h"

BoardRenderer::BoardRenderer() = default;
BoardRenderer::BoardRenderer(ncplane* parent, bool whitePerspective, const PieceList& whiteList,
                             const PieceList& blackList, const Board& board)
    : m_squareHeight{4}, m_squareWidth{9}, m_whitePerspective{whitePerspective},
      m_lightSquareChannel{m_whitePerspective ? ((255u << 16u) + (231u << 8u) + 186u)
                                              : ((162u << 16u) + (205u << 8u) + 90u)},
      m_darkSquareChannel{m_whitePerspective ? ((162u << 16u) + (205u << 8u) + 90u)
                                             : ((255u << 16u) + (231u << 8u) + 186u)},
      m_xorValue{m_whitePerspective ? 56u : 7u} {
  int boardHeight{m_squareHeight * 8};
  int boardWidth{m_squareWidth * 8};
// clang-format off
  ncplane_options board_opts{(static_cast<int>(ncplane_dim_y(parent)) - boardHeight) / 2,
                             (static_cast<int>(ncplane_dim_x(parent)) - boardWidth) / 2,
                             static_cast<unsigned int>(boardHeight),
                             static_cast<unsigned int>(boardWidth),
                             nullptr, nullptr, nullptr, 0, 0, 0};
// clang-format on
  m_plane = ncplane_create(parent, &board_opts);

  render(whiteList, blackList, board);
}

void BoardRenderer::render(const PieceList& whiteList, const PieceList& blackList, const Board& board) {
  renderSquares();
  renderSelector();
  renderPieces(whiteList, blackList, board);
}

void BoardRenderer::renderSquares() {
  uint32_t lightDarkXor{m_darkSquareChannel ^ m_lightSquareChannel};
  uint32_t squareChannel{m_whitePerspective ? m_darkSquareChannel : m_lightSquareChannel};

  for (std::size_t i{0}; i < 8; ++i) {
    squareChannel ^= lightDarkXor;
    for (std::size_t j{0}; j < 8; ++j) {
      ncplane_set_bg_rgb(m_plane, squareChannel);
      for (int k{0}; k < m_squareHeight; ++k) {
        ncplane_putstr_yx(m_plane, (i * m_squareHeight) + k, j * m_squareWidth, "         ");
      }
      squareChannel ^= lightDarkXor;
    }
  }
}

void BoardRenderer::renderPieces(const PieceList& whiteList, const PieceList& blackList, const Board& board) {
  uint32_t whiteColor{(255 << 16) + (255 << 8) + 255};
  uint32_t blackColor{0};
  uint32_t outlineColor{0};

  renderList(whiteList, board, whiteColor, outlineColor);
  renderList(blackList, board, blackColor, outlineColor);
}

void BoardRenderer::renderList(const PieceList& pieceList, const Board& board, uint32_t pieceColor,
                               uint32_t outlineColor) {
  unsigned int asciiHeight{pieces::pieceASCII[0].size()};
  unsigned int asciiWidth{pieces::pieceASCII[0][0].size()};
  for (std::size_t i{0}; i < pieceList.size(); ++i) {
    unsigned int boardIndex{static_cast<unsigned int>(constants::board120[pieceList[i]])};
    unsigned int boardIndexXor{boardIndex ^ m_xorValue};
    unsigned int y{boardIndexXor / 8};
    unsigned int x{boardIndexXor % 8};
// clang-format off
    ncplane_options piece_opts{
        static_cast<int>((y * m_squareHeight) + (((m_squareHeight - asciiHeight) + 2 - 1) / 2)),
        static_cast<int>((x * m_squareWidth) + (((m_squareWidth - asciiWidth) + 2 - 1) / 2)),
        asciiHeight, asciiWidth, nullptr, nullptr, nullptr, 0, 0, 0};
// clang-format on
    ncplane* plane{ncplane_create(m_plane, &piece_opts)};
    uint8_t code{static_cast<uint8_t>(board.at(pieceList[i]).code())};
    uint64_t channels{};
    ncchannels_set_bg_alpha(&channels, NCALPHA_TRANSPARENT);
    ncplane_set_base(plane, " ", 0, channels);
    for (std::size_t j{0}; j < asciiHeight; ++j) {
      ncplane_set_fg_rgb(plane, outlineColor);
      for (std::size_t k{0}; k < asciiWidth; ++k) {
        if (pieces::pieceASCII[code][j][k] == ' ') {
          continue;
        }
        if (k + 1 == asciiWidth) {
          ncplane_set_fg_rgb(plane, outlineColor);
        } else if (pieces::pieceASCII[code][j][k + 1] == ' ') {
          ncplane_set_fg_rgb(plane, outlineColor);
        }
        ncplane_putchar_yx(plane, j, k, pieces::pieceASCII[code][j][k]);
        ncplane_set_fg_rgb(plane, pieceColor);
      }
    }
    m_pieces.push_back(plane);
  }
}

void BoardRenderer::renderSelector() {
  unsigned int boardIndex{m_whitePerspective ? constants::E1 : constants::E8};
  unsigned int boardIndexXor{boardIndex ^ m_xorValue};
  unsigned int y{boardIndexXor / 8};
  unsigned int x{boardIndexXor % 8};
// clang-format off
  ncplane_options board_opts{static_cast<int>(m_squareHeight * y),
                             static_cast<int>(m_squareWidth * x),
                             static_cast<unsigned int>(m_squareHeight),
                             static_cast<unsigned int>(m_squareWidth),
                             nullptr,nullptr,nullptr,0,0,0};
// clang-format on
  m_selector = ncplane_create(m_plane, &board_opts);
  uint64_t channels{};
  ncchannels_set_bg_alpha(&channels, NCALPHA_TRANSPARENT);
  ncchannels_set_fg_alpha(&channels, NCALPHA_TRANSPARENT);
  ncplane_set_base(m_selector, " ", 0, channels);

  ncplane_set_fg_rgb8(m_selector, 255, 0, 0);

  for (int i{0}; i < m_squareHeight; ++i) {
    ncplane_putchar_yx(m_selector, i, 0, '*');
    ncplane_putchar_yx(m_selector, i, m_squareWidth - 1, '*');
  }
  ncplane_putstr_yx(m_selector, 0, 1, "*******"); // 7 wide bc 9-2=7 (this is dumb to hardcode)
  ncplane_putstr_yx(m_selector, m_squareHeight - 1, 1, "*******");
}
