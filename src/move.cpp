#include "move.h"

Move::Move(unsigned int from, unsigned int to, unsigned int flags, unsigned int fromPiece, unsigned int toPiece)
    : m_data{((toPiece & 0x07) << 19) | ((fromPiece & 0x07) << 16) | ((flags & 0x0f) << 12) | ((to & 0x3f) << 6) |
             (from & 0x3f)} {}
