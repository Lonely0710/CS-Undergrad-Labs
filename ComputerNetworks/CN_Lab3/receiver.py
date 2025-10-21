import socket
import threading
import time
import random
import queue
from config import (
    UDP_IP, SENDER_PORT, RECEIVER_PORT,
    HEADER_SIZE, MAX_PAYLOAD_SIZE,
    LOSS_RATE, DELAY_MS, FLAG_DATA, FLAG_ACK
)
from packet import create_packet, parse_packet # Import packet functions

class GBNReceiver:
    def __init__(self, receiver_ip, receiver_port, sender_ip, sender_port):
        self.receiver_address = (receiver_ip, receiver_port)
        self.sender_address = (sender_ip, sender_port) # Need sender address to send ACKs back
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(self.receiver_address)
        self.sock.settimeout(0.1) # Small timeout for recvfrom

        self.expected_seq_num = 0 # Next expected packet sequence number
        self.lock = threading.Lock() # Thread lock for shared state
        self._stop_event = threading.Event() 
        self.received_data_queue = queue.Queue() # Queue to pass received data to the application layer

        print(f"Receiver started on {self.receiver_address}")

    def _send_ack(self, ack_num):
        """发送一个 ACK 报文"""
        ack_packet = create_packet(0, ack_num, FLAG_ACK) # ACK packet's seq_num can be ignored
        # Simulate network conditions for ACKs
        if random.random() > LOSS_RATE:
            if DELAY_MS > 0:
                 time.sleep(DELAY_MS / 1000.0)
            try:
                self.sock.sendto(ack_packet, self.sender_address)
                # print(f"DEBUG: Sent ACK for {ack_num}")
            except socket.error as e:
                print(f"Socket error during ACK send: {e}")
                # Decide how to handle - maybe set stop_event


    def _receiver_loop(self):
        """在单独线程中接收数据"""
        while not self._stop_event.is_set():
            try:
                # Receive data with potential max size
                data, addr = self.sock.recvfrom(HEADER_SIZE + MAX_PAYLOAD_SIZE)

                # Simulate network delay on received data
                if DELAY_MS > 0:
                    time.sleep(DELAY_MS / 1000.0)

                # Ensure packet is from the expected sender (important in real networks)
                if addr != self.sender_address:
                    print(f"DEBUG: Received packet from unexpected address {addr}. Ignoring.")
                    continue

                seq_num, ack_num, flags, payload, checksum_ok = parse_packet(data)

                if checksum_ok and flags == FLAG_DATA:
                    with self.lock:
                        # print(f"DEBUG: Received DATA packet {seq_num}, Expected {self.expected_seq_num}")
                        if seq_num == self.expected_seq_num:
                            # Received in-order packet
                            print(f"Received in-order packet {seq_num}. Delivering to app.")
                            self.received_data_queue.put(payload) # Pass data to application layer
                            self.expected_seq_num += 1
                            # Send cumulative ACK for the *new* expected sequence number
                            self._send_ack(self.expected_seq_num)
                        else:
                            # Received out-of-order or duplicate packet
                            # In GBN, we discard out-of-order and resend ACK for the last correctly received
                            print(f"Received out-of-order packet {seq_num}. Expected {self.expected_seq_num}. Discarding and resending ACK.")
                            # Discard the packet, and re-send ACK for the last in-order packet received
                            self._send_ack(self.expected_seq_num)
                elif checksum_ok and flags == FLAG_ACK:
                     # Receiver might receive its own ACKs if sent to broadcast/multicast or loopback in certain setups.
                     # In this P2P simulation, it's unlikely, but good practice to ignore ACKs here.
                     pass
                elif not checksum_ok:
                     print(f"Received packet with checksum error. Discarding.")

            except socket.timeout:
                # print("DEBUG: Data receiver socket timeout")
                pass # No data received, continue loop
            except Exception as e:
                print(f"ERROR in data receiver: {e}")
                self._stop_event.set() # Indicate a critical error


    def start(self):
        """启动接收线程"""
        self._receiver_thread = threading.Thread(target=self._receiver_loop)
        self._receiver_thread.daemon = True # Allow main thread to exit
        self._receiver_thread.start()

    def stop(self):
        """停止接收线程"""
        self._stop_event.set()
        # if self._receiver_thread and self._receiver_thread.is_alive():
        #      self._receiver_thread.join() # Only join if not daemon
        self.sock.close()
        print("Receiver stopped.")

    def get_received_data(self):
        """从应用层队列获取接收到的所有数据"""
        received_chunks = []
        # Retrieve all items currently in the queue
        while True:
            try:
                chunk = self.received_data_queue.get_nowait()
                received_chunks.append(chunk)
            except queue.Empty:
                break
        return b''.join(received_chunks)