#include "game.h"
int main() {
    Game game { constants::startingFenString };

    game.debugPrint();
    return 0;
}
