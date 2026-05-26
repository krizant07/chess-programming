#include <string_view>

#include "game.h"

class FenParser {
public:
    FenParser();

    void setup(Game&, std::string_view);

private:
    int extractInt();
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

    void populateBoard(Game&);
    void setWhiteTurn(Game&);
    void setCastlingRules(Game&);
    void setEnPassant(Game&);
    void setHalfMove(Game&);
    void setFullMove(Game&);

    std::string_view m_fenStr {};
    int m_fenIndex {};
};
