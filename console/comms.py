import socket

class ROVComms:
    def __init__(self, ip='192.168.1.177', port=80):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((ip, port))

    def send_command(self, command: str):
        self.sock.sendall(command.encode())

    def receive_data(self):
        line = self.sock.recv(1024).decode().strip()
        temp_str, pressure_str, command = line.split(',')
        temp = float(temp_str)
        pressure = float(pressure_str)
        print(f"Temp: {temp}, Pressure: {pressure}, Command: {command}")
        list_data = [temp, pressure, command]
        return list_data

    def close(self):
        self.sock.close()
