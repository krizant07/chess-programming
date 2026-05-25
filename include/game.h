#pragma once

#include <string_view>
#include <bitset>
#include <vector>

#include "board.h"
#include "constants.h"
#include "piece-list.h"
#include "move.h"

class Game {
public:
    Game(std::string_view);
    
    void debugPrint();

    static std::byte charToPiece(char c) {
        switch (tolower(c)) {
        case 'p': return constants::PAWN;
        case 'r': return constants::ROOK;
        case 'n': return constants::KNIGHT;
        case 'b': return constants::BISHOP;
        case 'q': return constants::QUEEN;
        case 'k': return constants::KING;
        default: return constants::EMPTY_SQUARE;
        }
    }
private:
    Board m_board {};
    bool m_whiteTurn { false };
    std::bitset<4> m_canCastleKQkq {};
    int m_enPassant { -1 };
    int m_halfMoveClock {};
    int m_fullMoveCount {};

    PieceList m_whiteList {};
    PieceList m_blackList {};

    int populateBoard(std::string_view);
    void setWhiteTurn(std::string_view, int&);
    void setCastlingRules(std::string_view, int&);
    void setEnPassant(std::string_view, int&);
    void setHalfMove(std::string_view, int&);
    void setFullMove(std::string_view, int&);
    int extractInt(std::string_view, int&);
    std::vector<Move> generatePseudoLegal();
};
