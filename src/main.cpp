#include "constants.h"
#include "fen-parser.h"
#include "game.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <iostream>

Game buildGame(std::string_view fenStr) {
  FenParser parser{};
  Game game{};

  parser.setup(game, fenStr);
  return game;
}

ftxui::Element boardRank(bool black) {
  using namespace ftxui;
  std::vector<ftxui::Element> rank{};
  auto color1{black ? Color::Green : Color::White};
  auto color2{black ? Color::White : Color::Green};
  auto color{color1};

  for (int i{0}; i < 8; ++i) {
    if (i % 2 == 0) {
      color = color1;
    } else {
      color = color2;
    }

    rank.push_back(vbox({
        bgcolor(color, text("         ")),
        bgcolor(color, text("         ")),
        bgcolor(color, text("         ")),
        bgcolor(color, text("         ")),
        bgcolor(color, text("         ")),
    }));
  }

  return hbox({rank});
}

ftxui::Element chessBoard() {
  using namespace ftxui;
  std::vector<ftxui::Element> board{};
  bool black{true};
  for (int i{0}; i < 8; ++i) {
    board.push_back(boardRank(black));
    black = !black;
  }

  return vbox({board});
}

ftxui::Element document() {
  using namespace ftxui;
  return vbox({
      chessBoard() | center,
  });
}

auto screenCreate(ftxui::Element document) {
  using namespace ftxui;
  return Screen::Create(Dimension::Full(),       // Width
                        Dimension::Fit(document) // Height
  );
}

int main() {
  Game game{buildGame(constants::startingFenString)};

  MoveList moves{game.debugPrint()};

  int n{8};
  game.printMovesOfSquare(moves, static_cast<Square>(n));

  auto doc = document();
// Create a screen with full width and height fitting the document.
  auto screen = screenCreate(doc);
// Render the document onto the screen.
  Render(screen, doc);
// Print the screen to the console.
  screen.Print();

  return 0;
}
