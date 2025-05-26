# gui.py
from PyQt6.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QFrame
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QFont

class ROV_GUI(QWidget):
    def __init__(self, comms=None):
        super().__init__()
        self.comms = comms

        self.setWindowTitle("ROV GUI")
        self.setGeometry(100, 100, 800, 600)
        self.setStyleSheet("background-color: black;")

        data_font = QFont("Arial", 25)

        # Temperature frame
        self.temp_frame = QFrame()
        self.temp_frame.setStyleSheet("background-color: #222222; border: 2px solid #00FF00;")
        self.temp_label = QLabel("Temperature: -- °C", self.temp_frame)
        self.temp_label.setFont(data_font)
        self.temp_label.setStyleSheet("color: white;")
        temp_layout = QVBoxLayout()
        temp_layout.addWidget(self.temp_label)
        self.temp_frame.setLayout(temp_layout)

        # Pressure frame
        self.pressure_frame = QFrame()
        self.pressure_frame.setStyleSheet("background-color: #222222; border: 2px solid #00FFFF;")
        self.pressure_label = QLabel("Pressure: -- bar", self.pressure_frame)
        self.pressure_label.setFont(data_font)
        self.pressure_label.setStyleSheet("color: white;")
        pressure_layout = QVBoxLayout()
        pressure_layout.addWidget(self.pressure_label)
        self.pressure_frame.setLayout(pressure_layout)

        # Status frame (for FORWARD/BACKWARD/etc.)
        self.status_frame = QFrame()
        self.status_frame.setStyleSheet("background-color: #333333; border: 2px solid #FF9900;")
        self.status_label = QLabel("Status: IDLE", self.status_frame)
        self.status_label.setFont(data_font)
        self.status_label.setStyleSheet("color: white;")
        status_layout = QVBoxLayout()
        status_layout.addWidget(self.status_label)
        self.status_frame.setLayout(status_layout)

        # Horizontal layout for temp + pressure
        top_layout = QHBoxLayout()
        top_layout.addWidget(self.temp_frame)
        top_layout.addWidget(self.pressure_frame)

        # Main layout
        main_layout = QVBoxLayout()
        main_layout.addLayout(top_layout)
        main_layout.addWidget(self.status_frame)
        self.setLayout(main_layout)

    def update_data(self, temp, pressure):
        self.temp_label.setText(f"Temperature: {temp} °C")
        self.pressure_label.setText(f"Pressure: {pressure} bar")

    def update_status(self, status_text):
        self.status_label.setText(f"Status: {status_text}")

    def keyPressEvent(self, event):
        key = event.key()
        print("Key pressed:", key)

        if key == Qt.Key.Key_W:
            self.update_status("FORWARD")
            if self.comms: self.comms.send_command("FORWARD")
        elif key == Qt.Key.Key_S:
            self.update_status("BACKWARD")
            if self.comms: self.comms.send_command("BACKWARD")
        elif key == Qt.Key.Key_A:
            self.update_status("LEFT")
            if self.comms: self.comms.send_command("LEFT")
        elif key == Qt.Key.Key_D:
            self.update_status("RIGHT")
            if self.comms: self.comms.send_command("RIGHT")
        else:
            self.update_status("IDLE")
