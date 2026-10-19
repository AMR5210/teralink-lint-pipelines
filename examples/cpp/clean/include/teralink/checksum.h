/**
 * @file checksum.h
 * @brief XOR checksum, written in C to show that the pipeline also covers C sources.
 */
#ifndef TERALINK_CHECKSUM_H
#define TERALINK_CHECKSUM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Compute the XOR of all bytes in a buffer.
 *
 * @param data Buffer to checksum. Must not be NULL when @p length is non-zero.
 * @param length Number of bytes in @p data.
 * @return XOR of all bytes, or 0 for an empty buffer.
 */
uint8_t teralink_xor_checksum(const uint8_t* data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* TERALINK_CHECKSUM_H */
