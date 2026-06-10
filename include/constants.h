#pragma once
#include <array>
#include <bitset>
#include <string_view>

// clang-format off

namespace constants {
  inline constexpr std::array board64 {
      21, 22, 23, 24, 25, 26, 27, 28,
      31, 32, 33, 34, 35, 36, 37, 38,
      41, 42, 43, 44, 45, 46, 47, 48,
      51, 52, 53, 54, 55, 56, 57, 58,
      61, 62, 63, 64, 65, 66, 67, 68,
      71, 72, 73, 74, 75, 76, 77, 78,
      81, 82, 83, 84, 85, 86, 87, 88,
      91, 92, 93, 94, 95, 96, 97, 98
  };

  inline constexpr std::array board120 {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
    -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  };

  inline constexpr std::string_view startingFenString{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
}; // namespace constants

namespace pieces {
  using namespace std::literals::string_view_literals;

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

  inline constexpr std::array offset_count { 8, 4, 4, 8, 8 }; // Knight has 8 squares to look at per square,
                                                                          // bishop/rook 4, king/queen 8
  inline constexpr std::array offsets = {
      std::array{ -21, -19,-12, -8, 8, 12, 19, 21 }, /* KNIGHT */
      std::array{ -11,  -9,  9, 11, 0,  0,  0,  0 }, /* BISHOP */
      std::array{ -10,  -1,  1, 10, 0,  0,  0,  0 }, /* ROOK */
      std::array{ -11, -10, -9, -1, 1,  9, 10, 11 }, /* QUEEN */
      std::array{ -11, -10, -9, -1, 1,  9, 10, 11 }  /* KING */
  };

  inline constexpr std::array pieceASCII {
    std::array{" (\"\\ "sv,
               " )#\' "sv,
               "/###\\"sv},
    std::array{" (V) "sv,
               " )#( "sv,
               "/###\\"sv},
    std::array{" [-] "sv,
               " |#| "sv,
               "/###\\"sv},
    std::array{" \\^/ "sv,
               " )#( "sv,
               "/###\\"sv},
    std::array{" \\+/ "sv,
               " )#( "sv,
               "/###\\"sv},
    std::array{"  _  "sv,
               " (#) "sv,
               "/###\\"sv},
    std::array{"     "sv,
               "     "sv,
               "     "sv},
    std::array{"XXXXX"sv,
               "XXXXX"sv,
               "XXXXX"sv}
  };
}; // namespace pieces

namespace menuStrings {
  using namespace std::literals::string_view_literals;

  constexpr inline std::array splashText {
  "              ,,    ,,                    ,,"sv,
  "  .g8\"\"\"bgd `7MM    db        .g8\"\"\"bgd `7MM"sv,
  ".dP'     `M   MM            .dP'     `M   MM"sv,
  "dM'       `   MM  `7MM      dM'       `   MMpMMMb.   .gP\"Ya  ,pP\"Ybd ,pP\"Ybd"sv,
  "MM            MM    MM      MM            MM    MM  ,M'   Yb 8I   `\" 8I   `\""sv,
  "MM.           MM    MM      MM.           MM    MM  8M\"\"\"\"\"\" `YMMMa. `YMMMa."sv,
  "`Mb.     ,'   MM    MM      `Mb.     ,'   MM    MM  YM.    , L.   I8 L.   I8"sv,
  "  `\"bmmmd'  .JMML..JMML.      `\"bmmmd'  .JMML  JMML. `Mbmmd' M9mmmP' M9mmmP'"sv
  };

  constexpr inline std::array menuItems {
    "Play"sv,
    "Settings"sv,
    "Quit"sv
  };

  enum menuItem : unsigned int {
    play,
    settings,
    quit,
    max_item
  };

  static_assert((menuItems.size() == max_item) && "Yooooo you got some messed up stuff in your constants");
};
