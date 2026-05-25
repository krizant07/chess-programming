#pragma once

#include <array>
#include <cstdint>

class PieceList {
public:
    PieceList();
    void append(std::byte, uint8_t);
    uint8_t size();

    uint8_t operator[] (int);
private:
    std::array<uint8_t, 32> m_pieceList {}; // Stores indices into board64 for Board Array
    std::array<uint8_t, 64> m_indices {}; // Stores indices into pieceLists indexed with board index
    uint8_t m_size {};
};  
