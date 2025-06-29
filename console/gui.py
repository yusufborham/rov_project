# gui.py
from PyQt6.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QFrame
from PyQt6.QtCore import Qt, QTimer
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

        # Status frame
        self.status_frame = QFrame()
        self.status_frame.setStyleSheet("background-color: #333333; border: 2px solid #FF9900;")
        self.status_label = QLabel("Status: IDLE", self.status_frame)
        self.status_label.setFont(data_font)
        self.status_label.setStyleSheet("color: white;")
        status_layout = QVBoxLayout()
        status_layout.addWidget(self.status_label)
        self.status_frame.setLayout(status_layout)

        # Layouts
        top_layout = QHBoxLayout()
        top_layout.addWidget(self.temp_frame)
        top_layout.addWidget(self.pressure_frame)

        main_layout = QVBoxLayout()
        main_layout.addLayout(top_layout)
        main_layout.addWidget(self.status_frame)
        self.setLayout(main_layout)

        if self.comms:
            self.timer = QTimer(self)
            self.timer.timeout.connect(self.poll_data)
            self.timer.start(200)  # 5 times/sec polling

    def poll_data(self):
        try:
            result = self.comms.receive_data()
            if result:
                temp, pressure, _ = result
                self.update_data(temp, pressure)
        except Exception as e:
            print(f"Polling error: {e}")


    def update_data(self, temp, pressure):
        self.temp_label.setText(f"Temperature: {temp:.2f} °C")
        self.pressure_label.setText(f"Pressure: {pressure:.3f} bar")

    def update_status(self, status_text):
        self.status_label.setText(f"Status: {status_text}")

    def keyPressEvent(self, event):
        key = event.key()
        key_map = {
            Qt.Key.Key_F: "F",
            Qt.Key.Key_B: "B",
            Qt.Key.Key_U: "U",
            Qt.Key.Key_D: "D",
            Qt.Key.Key_L: "L",
            Qt.Key.Key_R: "R",
            Qt.Key.Key_S: "S"
        }

        if key in key_map:
            command = key_map[key]
            self.update_status(command)
            if self.comms:
                self.comms.send_command(command)
        else:
            self.update_status("IDLE")
