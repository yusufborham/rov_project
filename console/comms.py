import socket


class ROVComms:
    def __init__(self, ip='192.168.1.200', port=1337, listen_port=6000):
        self.remote_ip = ip
        self.remote_port = port
        self.listen_port = listen_port

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(('', listen_port))  # Listen for replies on listen_port
        self.sock.setblocking(False)

    def send_command(self, command: str):
        self.sock.sendto(command.encode(), (self.remote_ip, self.remote_port))
    def receive_data(self):
        try:
            data, addr = self.sock.recvfrom(1024)
            message = data.decode().strip()
            print(f"[GUI] Received reply from {addr}: {message}")

            # Check and extract temperature and pressure
            if "Temp is" in message and "pressure" in message:
                try:
                    parts = message.replace("Temp is", "").replace("pressure", "").split(",")
                    temp = float(parts[0].strip())
                    pressure = float(parts[1].strip()) / 100000  # Convert to bar
                    return [temp, pressure, "Sensor"]
                except (ValueError, IndexError) as parse_error:
                    print(f"[GUI] Parsing error: {parse_error}")
        except socket.timeout:
            print("[GUI] No data received (timeout)")
        except BlockingIOError:
            pass
        return None


    def close(self):
        self.sock.close()
