import struct

# --- Network Configuration ---
UDP_IP = "127.0.0.1"
SENDER_PORT = 12000
RECEIVER_PORT = 12001

# --- Protocol Configuration ---
WINDOW_SIZE = 4       # 发送窗口大小
TIMEOUT_INTERVAL = 0.5 # 超时时间（秒）
MAX_PAYLOAD_SIZE = 500 # 每个报文的最大数据载荷大小

# --- Packet Header Configuration ---
# Format: ! I I B H (Network byte order, unsigned int, unsigned int, unsigned byte, unsigned short)
HEADER_FORMAT = "!IIBH"
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)

FLAG_DATA = 1 # 数据报文标志
FLAG_ACK = 2  # 确认报文标志

# --- Network Simulation Parameters ---
LOSS_RATE = 0.2      # 丢包率 (0.0 - 1.0)
DELAY_MS = 80        # 模拟单向传播延迟（毫秒）