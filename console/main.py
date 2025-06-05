from PyQt6.QtWidgets import QApplication
from gui import ROV_GUI
from comms import ROVComms
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)

    comm = ROVComms(ip="192.168.1.200", port=1337, listen_port=6000)
    gui = ROV_GUI(comms=comm)

    gui.show()
    gui.update_data(10,100)
    sys.exit(app.exec())
