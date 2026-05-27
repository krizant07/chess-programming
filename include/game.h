#pragma once

#include <bitset>

#include "board.h"
#include "piece-list.h"
#include "move-list.h"

class Game {
public:
    Game();

    void debugPrint();

    friend class FenParser;
private:
    Board m_board {};
    bool m_whiteTurn { false };
    std::bitset<4> m_canCastleKQkq {};
    int m_enPassant { -1 };
    int m_halfMoveClock {};
    int m_fullMoveCount {};

    PieceList m_whiteList {};
    PieceList m_blackList {};

    void generatePseudoLegalPawnMoves(MoveList&, std::byte, std::byte, uint8_t);
    MoveList generatePseudoLegal();
};
