import struct
import zlib
from config import HEADER_FORMAT, HEADER_SIZE, FLAG_DATA, FLAG_ACK

def create_packet(seq_num, ack_num, flags, payload=b''):
    """
    创建报文 bytes。
    计算校验和，校验范围是头部（不含校验和字段）和载荷。
    """
    # Checksum is calculated over the entire packet excluding the checksum field itself
    header_without_checksum = struct.pack("!IIB", seq_num, ack_num, flags)
    data_to_checksum = header_without_checksum + payload
    checksum = zlib.crc32(data_to_checksum) & 0xffff # Use lower 16 bits

    header = struct.pack(HEADER_FORMAT, seq_num, ack_num, flags, checksum)
    return header + payload

def parse_packet(data):
    """
    解析报文 bytes，返回 (seq_num, ack_num, flags, payload) 或 None (校验失败)。
    """
    if len(data) < HEADER_SIZE:
        # print("DEBUG: Received data too short for header")
        return None, None, None, None, False # Added False for checksum status

    header = data[:HEADER_SIZE]
    try:
        seq_num, ack_num, flags, received_checksum = struct.unpack(HEADER_FORMAT, header)
    except struct.error:
         # print("DEBUG: Failed to unpack header")
         return None, None, None, None, False

    payload = data[HEADER_SIZE:]

    # Verify checksum
    header_without_checksum = struct.pack("!IIB", seq_num, ack_num, flags)
    data_to_checksum = header_without_checksum + payload
    calculated_checksum = zlib.crc32(data_to_checksum) & 0xffff

    if received_checksum != calculated_checksum:
        # print(f"DEBUG: Checksum mismatch for seq {seq_num}. Expected {calculated_checksum}, got {received_checksum}")
        return None, None, None, None, False # Checksum failed

    return seq_num, ack_num, flags, payload, True # Checksum passed