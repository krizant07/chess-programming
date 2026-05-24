#include <array>
#include <cstdint>

class PieceList {
public:
    PieceList();
    void append(std::byte, uint8_t);
    void debugPrint();
private:
    std::array<std::byte, 32> m_pieceList {};
    std::array<uint8_t, 64> m_indices {};
    uint8_t m_size {};
};  
