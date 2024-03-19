from PySide6.QtWidgets import QDialog, QLabel, QComboBox, QHBoxLayout, QPushButton
from PySide6.QtGui import QPixmap
from PySide6.QtCore import  Qt, Signal, Slot

from threading import Thread

import serial.tools.list_ports as ports

SPLASH_WIDTH = 600
SPLASH_HEIGHT = 400

class Splash(QDialog):

    ready = Signal(str)

    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("8x8 Led matrix controller")

        x = (self.screen().availableSize().width() - SPLASH_WIDTH)/2
        y = (self.screen().availableSize().height() - SPLASH_HEIGHT)/2

        self.setGeometry(x, y, SPLASH_WIDTH, SPLASH_HEIGHT)
        self._setBg()
        self._setStyle()

        horizontal_layout = QHBoxLayout()
        horizontal_layout.setAlignment(Qt.AlignmentFlag.AlignBottom|Qt.AlignmentFlag.AlignRight)
        
        self.portCombo = QComboBox()
        info = QLabel()
        info.setText("Choose a PORT")
        info.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignHCenter)

        #define button
        startBtn = QPushButton()
        startBtn.setText("Start controller")
        startBtn.clicked.connect(self.startMainApp)

        horizontal_layout.addWidget(info)
        horizontal_layout.addWidget(self.portCombo)
        horizontal_layout.addWidget(startBtn)

        for port in ports.comports():
            self.portCombo.addItem(port.device)
    
        self.thread = Thread(target=self._updatePorts)
        self.running = True

        self.setLayout(horizontal_layout)
    
    @Slot()
    def startMainApp(self):
        self.ready.emit(self.portCombo.currentText())
        self.close()

    def _updatePorts(self):
        while True:
            self.portCombo.clear()
            for port in ports.comports():
                self.portCombo.addItem(port.device)
            try:
                self.portCombo.config(values=self.port_names)
            except Exception as e:
                pass


    def _setStyle(self):
        self.setStyleSheet("QLabel{font: consolas, 24px; color: white; font-weight:bold}")


    def _setBg(self):
        label = QLabel(self)
        label.setGeometry(0, 0, SPLASH_WIDTH, SPLASH_HEIGHT)
        pix = QPixmap("bg.jpg").scaled(SPLASH_WIDTH, SPLASH_HEIGHT)

        label.setPixmap(pix)


