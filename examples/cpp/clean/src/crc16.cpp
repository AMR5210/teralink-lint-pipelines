/**
 * @file crc16.cpp
 * @brief Implementation of the CRC-16/CCITT-FALSE helpers.
 */
#include "teralink/crc16.hpp"

namespace teralink {

namespace {

constexpr std::uint32_t CRC16_POLY = 0x1021U;
constexpr std::uint32_t CRC16_TOP_BIT = 0x8000U;
constexpr std::uint32_t CRC16_MASK = 0xFFFFU;
constexpr unsigned int BITS_PER_BYTE = 8U;

}  // namespace

std::uint16_t crc16Update(std::uint16_t crc, std::uint8_t byte) noexcept {
    std::uint32_t value = static_cast<std::uint32_t>(crc) ^ (static_cast<std::uint32_t>(byte) << BITS_PER_BYTE);
    for (unsigned int bit = 0U; bit < BITS_PER_BYTE; ++bit) {
        if ((value & CRC16_TOP_BIT) != 0U) {
            value = (value << 1U) ^ CRC16_POLY;
        } else {
            value <<= 1U;
        }
    }
    return static_cast<std::uint16_t>(value & CRC16_MASK);
}

}  // namespace teralink
