import socket
import time

ROV_IP = "192.168.1.200"  # Arduino ENC28J60 IP
ROV_PORT = 1337            # Arduino listens on this port

LOCAL_IP = "192.168.1.3"   # Your laptop Ethernet static IP
LOCAL_PORT = 6000          # Local port to receive data

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_IP, LOCAL_PORT))
sock.settimeout(5)  # 5 second timeout

# sock.sendto(b'F', (ROV_IP, ROV_PORT))
# print(f"Sent 'F' to {ROV_IP}:{ROV_PORT}")

try:
    while True:
        data, addr = sock.recvfrom(1024)
        print(f"Received from {addr}: {data.decode().strip()}")
except socket.timeout:
    print("No response received within timeout")

sock.close()
