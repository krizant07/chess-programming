#pragma once

#include <cstddef>
#include <cstdint>

class Move {
public:
  Move(uint8_t from, uint8_t to, uint8_t flags, uint8_t fromPiece, uint8_t toPiece);

  uint8_t from() const;
  uint8_t to() const;
  uint8_t flags() const;
  std::byte fromPiece() const;
  std::byte toPiece() const;

private:
                          //                   FROM
                          //   UNUSED          PIECE CODE
                          //   |_______|    |__|
  unsigned int m_data{};  // 0b0000'0000'0000'0000'0000'0000'0000'0000
                          //            |__|    |___||_______||______|
                          //               TO      flags  to    from
                          //               PIECE
                          //               CODE
};
