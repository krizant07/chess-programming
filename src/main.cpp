#include "constants.h"
#include "fen-parser.h"
#include "game.h"

#include <iostream>

Game buildGame(std::string_view fenStr) {
  FenParser parser{};
  Game game{};

  parser.setup(game, fenStr);
  return game;
}

int main() {
  Game game{buildGame(constants::startingFenString)};

  MoveList moves{game.debugPrint()};

  int n{};
  std::cout << "Please pick a square number ( 0 - 63 ):";
  std::cin >> n;
  game.printMovesOfSquare(moves, static_cast<Square>(n));

  return 0;
}
