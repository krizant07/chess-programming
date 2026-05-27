#pragma once

#include <vector>
#include <cstdint>

#include "move.h"

class MoveList {
public:
    MoveList();

    std::size_t size() const;

    void appendMove(std::byte, uint8_t, int8_t, uint8_t = 0b0000);
    void appendCapture(std::byte, std::byte, uint8_t, int8_t, uint8_t = 0b0100);
private:

    std::vector<Move> m_list {};
};
