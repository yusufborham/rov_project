import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 6000))  # Listen on all interfaces

print("Listening on port 6000...")
while True:
    data, addr = sock.recvfrom(1024)
    print(f"Received from {addr}: {data}")
