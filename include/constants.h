#include <array>
#include <cstdint>
#include <string_view>

/* 
 * bit 7
 * WHITE  0
 * BLACK  1
 *
 * bit 0-2
 * EMPTY  000
 * PAWN   001
 * ROOK   010
 * KNIGHT 011
 * BISHOP 100
 * QUEEN  101
 * KING   110
 *
 * SENTINAL 1111 1111
 *
 *FF FF FF FF FF FF FF FF FF FF
 *FF FF FF FF FF FF FF FF FF FF
 *FF 02 03 04 05 06 04 03 02 FF h1
 *FF 01 01 01 01 01 01 01 01 FF
 *FF XX XX XX XX XX XX XX XX FF
 *FF XX XX XX XX XX XX XX XX FF
 *FF XX XX XX XX XX XX XX XX FF
 *FF XX XX XX XX XX XX XX XX FF
 *FF 81 81 81 81 81 81 81 81 FF
 *FF 82 83 84 85 86 84 83 82 FF h8
 *FF FF FF FF FF FF FF FF FF FF
 *FF FF FF FF FF FF FF FF FF FF
     a8                   h8
*/

using BoardArray = std::array<std::byte, 120>;

namespace constants {
inline constexpr std::byte EMPTY    { 0b0000'0000 };
inline constexpr std::byte PAWN     { 0b0000'0001 };
inline constexpr std::byte ROOK     { 0b0000'0010 };
inline constexpr std::byte KNIGHT   { 0b0000'0011 };
inline constexpr std::byte BISHOP   { 0b0000'0100 };
inline constexpr std::byte QUEEN    { 0b0000'0101 };
inline constexpr std::byte KING     { 0b0000'0110 };
inline constexpr std::byte WHITE    { 0b0000'0000 };
inline constexpr std::byte BLACK    { 0b1000'0000 };
inline constexpr std::byte SENTINAL { 0b1111'1111 };
inline constexpr std::array<std::uint8_t, 64> board64 {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

inline constexpr BoardArray make_board(std::byte value) {
    BoardArray a{};
    for (auto& x : a) x = value;
    return a;
}

inline constexpr BoardArray empty_board { make_board(SENTINAL) };
inline constexpr std::string_view startingFenString { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
};
