# main.py
from PyQt6.QtWidgets import QApplication
from gui import ROV_GUI
from comms import ROVComms
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)

    # Uncomment and configure communication if needed
    # comm = ROVComms(ip="192.168.1.177", port=80)
    # gui = ROV_GUI(comms=comm)
    
    gui = ROV_GUI()  # Without communication
    gui.show()
    gui.update_data(25, 1.0)  # Example data update

    sys.exit(app.exec())
