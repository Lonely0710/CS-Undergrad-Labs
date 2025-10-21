import time
import sys # To handle command line arguments if needed later

# Import modules
from config import UDP_IP, SENDER_PORT, RECEIVER_PORT
from sender import GBNSender
from receiver import GBNReceiver

if __name__ == "__main__":
    # --- Configuration ---
    sender_ip = UDP_IP
    sender_port = SENDER_PORT
    receiver_ip = UDP_IP # For local testing, receiver is on the same IP
    receiver_port = RECEIVER_PORT

    # --- Data to Send ---
    # Create some dummy data
    large_data = b"This is a test message for reliable data transfer using Go-Back-N protocol in Python. " * 100 
    print(f"Attempting to send {len(large_data)} bytes of data.")

    # --- Setup ---
    # Receiver needs to know sender's address to send ACKs back
    receiver = GBNReceiver(receiver_ip, receiver_port, sender_ip, sender_port)
    receiver.start() # Start the receiver's listening thread

    time.sleep(1) # Give the receiver a moment to bind and start

    # Sender needs to know receiver's address
    sender = GBNSender(sender_ip, sender_port, receiver_ip, receiver_port)

    # --- Run Simulation ---
    # The send_data method handles the sending loop and ACK reception internally
    sender.send_data(large_data)

    # --- Cleanup and Verification ---
    print("Main thread waiting briefly for final packets/ACKs...")
    time.sleep(3) # Adjust this based on expected worst-case delay/retransmissions

    # Stop the receiver
    receiver.stop()

    # Retrieve and verify the received data
    received_data = receiver.get_received_data()

    print("\n--- Verification ---")
    print(f"Original data length: {len(large_data)}")
    print(f"Received data length: {len(received_data)}")

    if large_data == received_data:
        print("Data integrity check PASSED: Received data matches original data.")
    else:
        print("Data integrity check FAILED: Received data does NOT match original data.")
        # Optional: Print initial parts or compare byte by byte for debugging
        # print("Original (first 100 bytes):", large_data[:100])
        # print("Received (first 100 bytes):", received_data[:100])
        # for i in range(min(len(large_data), len(received_data))):
        #     if large_data[i] != received_data[i]:
        #         print(f"Mismatch at byte {i}: Original {large_data[i]}, Received {received_data[i]}")
        #         break

    print("--- Simulation Finished ---")