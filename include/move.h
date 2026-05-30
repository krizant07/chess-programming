#pragma once

#include <cstddef>
#include <cstdint>

class Move {
public:
  Move(uint8_t from, uint8_t to, uint8_t flags, uint8_t fromPiece, uint8_t toPiece);

  uint8_t getFrom() const;
  uint8_t getTo() const;
  uint8_t getFlags() const;
  std::byte getFromPiece() const;
  std::byte getToPiece() const;

private:
                          //                   FROM
                          //   UNUSED          PIECE CODE
                          //   |__________|    |_|
  unsigned int m_data{};  // 0b0000'0000'0000'0000'0000'0000'0000'0000
                          //               |__|    |__| |_____||_____|
                          //               TO      flags  to    from
                          //               PIECE
                          //               CODE
};
