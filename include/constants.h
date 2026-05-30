#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>
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
inline constexpr std::byte EMPTY_SQUARE { 0b0000'0110 };
inline constexpr std::byte SENTINAL     { 0b1111'1111 };

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

inline constexpr BoardArray make_board(std::byte value) {
  BoardArray a{};
  for (auto& x : a)
    x = value;
  return a;
}

inline constexpr BoardArray sentinal_board{make_board(SENTINAL)};
inline constexpr std::string_view startingFenString{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
}; // namespace constants

namespace pieces {
using Piece = std::byte;
// clang-format off
inline constexpr Piece KNIGHT { 0b0000'0000 };
inline constexpr Piece BISHOP { 0b0000'0001 };
inline constexpr Piece ROOK   { 0b0000'0010 };
inline constexpr Piece QUEEN  { 0b0000'0011 };
inline constexpr Piece KING   { 0b0000'0100 };
inline constexpr Piece PAWN   { 0b0000'0101 };
inline constexpr Piece WHITE  { 0b0000'0000 };
inline constexpr Piece BLACK  { 0b1000'0000 };

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

inline void print(Piece piece) {
  switch (piece) {
  case constants::SENTINAL: std::cout << "FF "; break;
  case constants::EMPTY_SQUARE: std::cout << "XX "; break;
  // White Pieces
  case WHITE | PAWN: std::cout << "WP "; break;
  case WHITE | ROOK: std::cout << "WR "; break;
  case WHITE | KNIGHT: std::cout << "WN "; break;
  case WHITE | BISHOP: std::cout << "WB "; break;
  case WHITE | QUEEN: std::cout << "WQ "; break;
  case WHITE | KING: std::cout << "WK "; break;
  // Black Pieces
  case BLACK | PAWN: std::cout << "BP "; break;
  case BLACK | ROOK: std::cout << "BR "; break;
  case BLACK | KNIGHT: std::cout << "BN "; break;
  case BLACK | BISHOP: std::cout << "BB "; break;
  case BLACK | QUEEN: std::cout << "BQ "; break;
  case BLACK | KING: std::cout << "BK "; break;
  default: std::cout << "?? "; break;
  }
}

inline constexpr Piece charToPiece(char c) {
  switch (tolower(c)) {
  case 'p': return PAWN;
  case 'r': return ROOK;
  case 'n': return KNIGHT;
  case 'b': return BISHOP;
  case 'q': return QUEEN;
  case 'k': return KING;
  default: return constants::EMPTY_SQUARE;
  }
}

inline constexpr Piece codeToPiece(uint8_t code) {
  switch (static_cast<std::byte>((code & 0x07))) {
  case (KNIGHT & std::byte{0x07}): return KNIGHT;
  case (BISHOP & std::byte{0x07}): return BISHOP;
  case (ROOK & std::byte{0x07}): return ROOK;
  case (QUEEN & std::byte{0x07}): return QUEEN;
  case (KING & std::byte{0x07}): return KING;
  case (PAWN & std::byte{0x07}): return PAWN;
  default: return constants::EMPTY_SQUARE;
  }
}
}; // namespace pieces
