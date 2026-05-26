#include "constants.h"
#include "game.h"
#include "fen-parser.h"

Game buildGame(std::string_view fenStr) {
    FenParser parser {};
    Game game {};

    parser.setup(game, fenStr);
    return game;
}

int main() {
    Game game { buildGame(constants::startingFenString) };
    
    game.debugPrint();
    return 0;
}
