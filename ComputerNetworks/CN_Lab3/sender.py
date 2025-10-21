import socket
import threading
import time
import random
from config import (
    UDP_IP, SENDER_PORT, RECEIVER_PORT,
    WINDOW_SIZE, TIMEOUT_INTERVAL, MAX_PAYLOAD_SIZE,
    LOSS_RATE, DELAY_MS, FLAG_DATA, FLAG_ACK
)
from packet import create_packet, parse_packet # Import packet functions

class GBNSender:
    def __init__(self, sender_ip, sender_port, receiver_ip, receiver_port):
        self.sender_address = (sender_ip, sender_port)
        self.receiver_address = (receiver_ip, receiver_port)
        self.window_size = WINDOW_SIZE
        self.timeout_interval = TIMEOUT_INTERVAL
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(self.sender_address)
        self.sock.settimeout(0.1) # Small timeout for recvfrom

        self.send_base = 0          # 发送窗口基序号 (已确认的最小序号)
        self.next_seq_num = 0       # 下一个要发送的报文序号
        self.buffer = {}            # 存储已发送但未确认的报文 {seq_num: packet_bytes}
        self.lock = threading.Lock() # 线程锁，保护共享状态
        self.timer = None           # 计时器对象
        self._stop_event = threading.Event() # 用于停止线程

        print(f"Sender started on {self.sender_address}, sending to {self.receiver_address}")

    def _start_timer(self):
        """启动或重置计时器"""
        if self.timer:
            self.timer.cancel()
        # GBN计时器针对窗口中的第一个报文 (send_base)
        if self.send_base < self.next_seq_num: # 窗口内有未确认报文
            self.timer = threading.Timer(self.timeout_interval, self._handle_timeout)
            self.timer.start()
            # print(f"DEBUG: Timer started for seq {self.send_base}")

    def _stop_timer(self):
        """停止计时器"""
        if self.timer:
            self.timer.cancel()
        self.timer = None
        # print("DEBUG: Timer stopped")

    def _handle_timeout(self):
        """处理超时事件：重传窗口内的所有报文"""
        with self.lock:
            if self._stop_event.is_set(): # Check if stopping
                return

            if self.send_base < self.next_seq_num: # 再次检查，防止在锁等待期间收到ACK
                print(f"TIMEOUT! Retransmitting packets from {self.send_base} to {self.next_seq_num-1}")
                # 重传从 send_base 到 next_seq_num - 1 的所有报文
                for seq in range(self.send_base, self.next_seq_num):
                    packet_bytes = self.buffer.get(seq)
                    if packet_bytes:
                        # Simulate network conditions for retransmissions too
                        if random.random() > LOSS_RATE:
                            if DELAY_MS > 0:
                                time.sleep(DELAY_MS / 1000.0)
                            try:
                                self.sock.sendto(packet_bytes, self.receiver_address)
                                # print(f"DEBUG: Retransmitted packet {seq}")
                            except socket.error as e:
                                print(f"Socket error during retransmit send: {e}")
                                self._stop_event.set() # Indicate a critical error
                                break
                        # else:
                           # print(f"DEBUG: Simulated loss of retransmitted packet {seq}")

                # 重启计时器
                if not self._stop_event.is_set():
                     self._start_timer()
            # else:
                # print("DEBUG: Timeout handled, but window is empty (maybe ACK arrived just before lock)")


    def _send_packet(self, seq_num, payload):
        """创建并发送一个数据报文"""
        packet_bytes = create_packet(seq_num, 0, FLAG_DATA, payload)
        self.buffer[seq_num] = packet_bytes # 缓存报文

        # Simulate network conditions
        if random.random() > LOSS_RATE:
            if DELAY_MS > 0:
                 time.sleep(DELAY_MS / 1000.0)
            try:
                self.sock.sendto(packet_bytes, self.receiver_address)
                # print(f"DEBUG: Sent packet {seq_num}")
            except socket.error as e:
                 print(f"Socket error during send: {e}")
                 self._stop_event.set() # Indicate a critical error
        # else:
            # print(f"DEBUG: Simulated loss of packet {seq_num}")


    def _ack_receiver_loop(self):
        """在单独线程中接收 ACK"""
        while not self._stop_event.is_set():
            try:
                data, addr = self.sock.recvfrom(1024) # Max packet size for ACK
                # Simulate network delay on received data
                if DELAY_MS > 0:
                    time.sleep(DELAY_MS / 1000.0)

                seq_num, ack_num, flags, payload, checksum_ok = parse_packet(data)

                if checksum_ok and flags == FLAG_ACK:
                    # print(f"DEBUG: Received ACK for {ack_num}")
                    with self.lock:
                        # GBN 接收累积 ACK
                        # ack_num 意味着报文 ack_num-1 及其之前的所有报文都已确认
                        if ack_num > self.send_base:
                            # print(f"DEBUG: Window advanced from {self.send_base} to {ack_num}")
                            # 从 buffer 中移除已确认的报文
                            for seq in range(self.send_base, ack_num):
                                if seq in self.buffer:
                                    del self.buffer[seq]
                            self.send_base = ack_num

                            # 如果窗口为空，停止计时器
                            if self.send_base == self.next_seq_num:
                                self._stop_timer()
                            else:
                                # 否则，重启计时器 (为新的窗口基序号)
                                self._start_timer()
            except socket.timeout:
                # print("DEBUG: ACK receiver socket timeout")
                pass # 没有收到数据，继续循环
            except Exception as e:
                print(f"ERROR in ACK receiver: {e}")
                self._stop_event.set() # Indicate a critical error


    def send_data(self, data):
        """将应用层数据发送出去"""
        # 启动 ACK 接收线程
        ack_thread = threading.Thread(target=self._ack_receiver_loop)
        ack_thread.daemon = True # Allow main thread to exit even if this thread is running
        ack_thread.start()

        i = 0
        while i < len(data) and not self._stop_event.is_set():
            with self.lock:
                # 检查窗口是否已满
                if self.next_seq_num < self.send_base + self.window_size:
                    # 窗口未满，发送下一个报文
                    chunk = data[i : i + MAX_PAYLOAD_SIZE]
                    self._send_packet(self.next_seq_num, chunk)

                    # 如果这是窗口中的第一个报文，启动计时器
                    if self.send_base == self.next_seq_num:
                        self._start_timer()

                    self.next_seq_num += 1
                    i += MAX_PAYLOAD_SIZE
                else:
                    # 窗口已满，等待 ACK (通过小的 socket timeout 并在循环中重试)
                    # print("DEBUG: Window is full. Waiting for ACK.")
                    pass # 释放锁，让其他线程有机会运行

            time.sleep(0.005) # Small sleep to yield CPU and allow other threads/processes

        # 数据发送完毕，等待所有报文被确认
        print("All data segmented and sent (within window limits). Waiting for final ACKs...")
        while True:
             with self.lock:
                 if self._stop_event.is_set():
                     print("Sender stopping due to error.")
                     break
                 if self.send_base == self.next_seq_num:
                     print("All packets acknowledged.")
                     break
             time.sleep(0.1) # 等待最终 ACK

        # Signal ACK receiver thread to stop and join
        self._stop_event.set()
        # ack_thread is daemon, so it will exit when main thread exits.
        # If not daemon, uncomment: ack_thread.join()
        self.sock.close()
        print("Sender finished.")

    def stop(self):
         """Explicitly stop the sender"""
         self._stop_event.set()
         if self.timer:
             self.timer.cancel()
         self.sock.close()