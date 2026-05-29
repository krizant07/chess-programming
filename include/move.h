#pragma once

class Move {
public:
  Move(unsigned int from, unsigned int to, unsigned int flags, unsigned int fromPiece, unsigned int toPiece);

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
