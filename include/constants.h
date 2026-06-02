#pragma once

#include <bitset>
#include <cstdint>
#include <string_view>

#include "types.h"

/*
 * bit 7
 * WHITE  0
 * BLACK  1
 *
 * bit 0-2
 * KNIGHT            000
 * BISHOP            001
 * ROOK              010
 * QUEEN             011
 * KING              100
 * PAWN              101
 * EMPTY_SQUARE      110
 * SENTINAL & 0x07   111
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

// clang-format off
namespace constants {

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
// clang-format on
inline constexpr std::string_view startingFenString{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
}; // namespace constants

namespace pieces {
// clang-format off
inline constexpr std::byte KNIGHT   { 0b0000 };
inline constexpr std::byte BISHOP   { 0b0001 };
inline constexpr std::byte ROOK     { 0b0010 };
inline constexpr std::byte QUEEN    { 0b0011 };
inline constexpr std::byte KING     { 0b0100 };
inline constexpr std::byte PAWN     { 0b0101 };
inline constexpr std::byte EMPTY    { 0b0110 };
inline constexpr std::byte SENTINAL { 0b0111 };
inline constexpr std::byte WHITE    { 0b0000 };
inline constexpr std::byte BLACK    { 0b1000 };

inline constexpr std::bitset<5> slide { 0b0'1110 }; // Knight doesnt slide, bishop/rook/queen do slide, king doesnt

inline constexpr std::array<uint8_t, 5> offset_count { 8, 4, 4, 8, 8 }; // Knight has 8 squares to look at per square,
                                                                        // bishop/rook 4, king/queen 8
inline constexpr std::array<std::array<int8_t, 8>, 5> offsets = {{
    { -21, -19,-12, -8, 8, 12, 19, 21 }, /* KNIGHT */
    { -11,  -9,  9, 11, 0,  0,  0,  0 }, /* BISHOP */
    { -10,  -1,  1, 10, 0,  0,  0,  0 }, /* ROOK */
    { -11, -10, -9, -1, 1,  9, 10, 11 }, /* QUEEN */
    { -11, -10, -9, -1, 1,  9, 10, 11 }  /* KING */
}};
// clang-format on
}; // namespace pieces
