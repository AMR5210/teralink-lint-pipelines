"""Telemetry packet helpers used to demonstrate the TeraLink Python lint pipeline."""

from __future__ import annotations

from dataclasses import dataclass

CRC16_POLY = 0x1021
CRC16_INIT = 0xFFFF
CRC16_MASK = 0xFFFF
CRC16_TOP_BIT = 0x8000
BITS_PER_BYTE = 8
HEADER_LENGTH = 4
CRC_LENGTH = 2


class PacketError(ValueError):
    """Raised when a telemetry packet fails validation."""


@dataclass(frozen=True)
class Packet:
    """A decoded telemetry packet.

    Attributes:
        apid: Application process identifier of the sender.
        payload: Packet payload bytes.
    """

    apid: int
    payload: bytes


def crc16_ccitt(data: bytes, crc: int = CRC16_INIT) -> int:
    """Compute CRC-16/CCITT-FALSE over ``data``.

    Args:
        data: Message bytes to checksum.
        crc: Running CRC value; pass a previous result to continue a stream.

    Returns:
        The 16-bit CRC of ``data``.
    """
    for byte in data:
        crc ^= byte << BITS_PER_BYTE
        for _ in range(BITS_PER_BYTE):
            if crc & CRC16_TOP_BIT:
                crc = (crc << 1) ^ CRC16_POLY
            else:
                crc <<= 1
            crc &= CRC16_MASK
    return crc


def parse_packet(raw: bytes) -> Packet:
    """Decode a raw packet and verify its CRC.

    Args:
        raw: Bytes laid out as ``apid(2) | length(2) | payload | crc(2)``.

    Returns:
        The decoded packet.

    Raises:
        PacketError: If the packet is truncated, the length field disagrees with
            the packet size, or the CRC does not match.
    """
    if len(raw) < HEADER_LENGTH + CRC_LENGTH:
        raise PacketError("packet too short")
    apid = int.from_bytes(raw[0:2], "big")
    length = int.from_bytes(raw[2:4], "big")
    if len(raw) != HEADER_LENGTH + length + CRC_LENGTH:
        raise PacketError("length field does not match packet size")
    expected = int.from_bytes(raw[-CRC_LENGTH:], "big")
    if crc16_ccitt(raw[:-CRC_LENGTH]) != expected:
        raise PacketError("CRC mismatch")
    return Packet(apid=apid, payload=raw[HEADER_LENGTH : HEADER_LENGTH + length])
