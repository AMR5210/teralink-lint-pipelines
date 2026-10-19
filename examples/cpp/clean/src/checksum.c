/**
 * @file checksum.c
 * @brief Implementation of the XOR checksum.
 */
#include "teralink/checksum.h"

uint8_t teralink_xor_checksum(const uint8_t* data, size_t length) {
    uint8_t result = 0U;
    for (size_t i = 0U; i < length; i++) {
        result ^= data[i];
    }
    return result;
}
