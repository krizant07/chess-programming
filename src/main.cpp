#include "constants.h"
#include "fen-parser.h"
#include "game.h"

Game buildGame(std::string_view fenStr) {
  FenParser parser{};
  Game game{};

  parser.setup(game, fenStr);
  return game;
}

int main() {
  Game game{buildGame(constants::startingFenString)};

  MoveList moves{game.debugPrint()};

  std::size_t n{};
  std::cout << "Hey chud pick a move number (there is NO validation, please be nice): ";
  std::cin >> n;

  game.makeMove(moves[n]);

  game.debugPrint();

  return 0;
}
