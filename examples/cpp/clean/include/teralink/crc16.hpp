/**
 * @file crc16.hpp
 * @brief CRC-16/CCITT-FALSE helpers.
 */
#ifndef TERALINK_CRC16_HPP
#define TERALINK_CRC16_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace teralink {

/** @brief Initial value of a CRC-16/CCITT-FALSE computation. */
constexpr std::uint16_t CRC16_INIT = 0xFFFFU;

/**
 * @brief Fold one byte into a running CRC-16/CCITT-FALSE value.
 *
 * @param crc Running CRC value, starting from teralink::CRC16_INIT.
 * @param byte Next message byte.
 * @return Updated CRC value.
 */
[[nodiscard]] std::uint16_t crc16Update(std::uint16_t crc, std::uint8_t byte) noexcept;

/**
 * @brief Compute the CRC-16/CCITT-FALSE of a fixed-size buffer.
 *
 * @tparam N Number of bytes in the buffer.
 * @param data Message bytes.
 * @return CRC of @p data.
 */
template <std::size_t N>
[[nodiscard]] std::uint16_t crc16(const std::array<std::uint8_t, N>& data) noexcept {
    std::uint16_t crc = CRC16_INIT;
    for (const std::uint8_t byte : data) {
        crc = crc16Update(crc, byte);
    }
    return crc;
}

}  // namespace teralink

#endif  // TERALINK_CRC16_HPP
