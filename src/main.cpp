#include "constants.h"
#include "fen-parser.h"
#include "game.h"

#include <notcurses/notcurses.h>

Game buildGame(std::string_view fenStr) {
  FenParser parser{};
  Game game{};

  parser.setup(game, fenStr);
  return game;
}

int main() {
  setlocale(LC_ALL, "");
  notcurses* nc{notcurses_init(nullptr, stdout)};
  ncplane* std_plane{notcurses_stdplane(nc)};
  int width{static_cast<int>(ncplane_dim_x(std_plane))};    // using int for consistency,
  int height{static_cast<int>(ncplane_dim_y(std_plane))};   // should obviously never exceed +2.1b
  constexpr int square_width{9};
  constexpr int square_height{4};
  constexpr int board_width = square_width * 8;
  constexpr int board_height = square_height * 8;
  ncplane_options board_opts{(height - board_height) / 2,
                             (width - board_width) / 2,
                             board_height,
                             board_width,
                             nullptr,
                             nullptr,
                             nullptr,
                             0,
                             0,
                             0};

  ncplane* board{ncplane_create(std_plane, &board_opts)};
  std::vector<ncplane*> squares{};
  ncplane_options square_opts{0, 0, square_height, square_width, nullptr, nullptr, nullptr, 0, 0, 0};

  uint32_t dark_square_channel{(162u << 16u) + (205u << 8u) + 90u};
  uint32_t light_square_channel{(255u << 16u) + (231u << 8u) + 186u};

  uint32_t light_dark_xor{dark_square_channel ^ light_square_channel};

  uint32_t square_channel = dark_square_channel;

  uint32_t piece_outline{0};

  uint8_t asciiHeight{3};
  uint8_t asciiWidth{5};

  timespec ts{};
  ncinput ni{};

  Game game{buildGame(constants::startingFenString)};

  MoveList moves{game.debugPrint()};

  int n{8};
  game.printMovesOfSquare(moves, static_cast<Square>(n));
  const Board& gameBoard{game.board()};

  for (int i = 0; i < 64; ++i) {
    if (i % 8 == 0)
      square_channel ^= light_dark_xor;

    square_opts.x = (i % 8) * square_width;
    square_opts.y = (i / 8) * square_height;
    squares.push_back(ncplane_create(board, &square_opts));
    ncplane_set_bg_rgb(squares[i], square_channel);
    square_channel ^= light_dark_xor;
  }
  ncplane* plane{};

  for (std::size_t i{0}; i < squares.size(); ++i) {
    plane = squares[i];
    for (std::size_t j{0}; j < square_height; ++j) {
      ncplane_putstr_yx(plane, j, 0, "         ");
    }

    const Piece& piece{gameBoard.at(constants::board64[i ^ 56])};

    uint8_t pieceCode{static_cast<uint8_t>(piece.code())};
    uint32_t pieceColor{piece.color() == pieces::WHITE ? (255u << 16u) + (255u << 8u) + 255u : 0};

    for (std::size_t j{0}; j < asciiHeight; ++j) {
      bool outline{true};
      for (std::size_t k{0}; k < asciiWidth; ++k) {
        if (pieces::pieceASCII[pieceCode][j][k] == ' ')
          continue;
        if (!outline) {
          if (k + 1 == asciiWidth) {
            outline = true;
          } else if (pieces::pieceASCII[pieceCode][j][k + 1] == ' ') {
            outline = true;
          }
        }
        if (outline)
          ncplane_set_fg_rgb(plane, piece_outline);
        else
          ncplane_set_fg_rgb(plane, pieceColor);
        ncplane_putchar_yx(plane, j + square_height - asciiHeight, k + ((square_width - asciiWidth) / 2),
                           pieces::pieceASCII[pieceCode][j][k]);
        outline = false;
      }
    }
  }

  for (;;) {
    if (notcurses_get(nc, &ts, &ni)) {
      if (ncinput_shift_p(&ni)) {
        if (ni.utf8[0] == 'Q') {
          break;
        }
      }
    }
    notcurses_render(nc);
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts.tv_nsec += 100'000'000; // 100ms, 10fps
  }

  notcurses_stop(nc);

  return 0;
}
