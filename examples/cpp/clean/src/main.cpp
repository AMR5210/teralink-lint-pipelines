/**
 * @file main.cpp
 * @brief Demo program: checks the CRC and XOR helpers against known-good values.
 */
#include <array>
#include <cstdint>
#include <iostream>

#include "teralink/checksum.h"
#include "teralink/crc16.hpp"

/**
 * @brief Program entry point.
 *
 * @return 0 when both checksums match their known-good values, 1 otherwise.
 */
int main() {
    // ASCII "123456789": the standard check input for CRC algorithms.
    constexpr std::array<std::uint8_t, 9> CHECK_INPUT = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    constexpr std::uint16_t CRC_EXPECTED = 0x29B1U;
    constexpr std::uint8_t XOR_EXPECTED = 0x31U;

    const std::uint16_t crc = teralink::crc16(CHECK_INPUT);
    const std::uint8_t xorSum = teralink_xor_checksum(CHECK_INPUT.data(), CHECK_INPUT.size());

    const bool crcOk = (crc == CRC_EXPECTED);
    const bool xorOk = (xorSum == XOR_EXPECTED);
    std::cout << "crc16: " << (crcOk ? "ok" : "MISMATCH") << ", xor: " << (xorOk ? "ok" : "MISMATCH") << '\n';

    return (crcOk && xorOk) ? 0 : 1;
}
