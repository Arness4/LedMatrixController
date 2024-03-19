# This Python file uses the following encoding: utf-8
import sys
from PySide6.QtWidgets import QApplication,  QMainWindow, QWidget, QMenu
from splash import Splash

from PySide6.QtCore import Qt

from container import CentralWidget

class MainWindow(QMainWindow):
    def __init__(self, parent: QWidget = None) -> None:
        super().__init__(parent)
        self.centralWidget = CentralWidget()
        self.setCentralWidget(self.centralWidget)

        self.setWindowTitle("Led Controller")
        self.createMenus()

        self.setStyleSheet("""QWidget{background:green;} 
                            QLineEdit{color:white; font:bold 16px 'Calibri';} QPushButton{background:white;color:black}
                            #infoLabel{font:bold 16px 'Consolas'}
                           QMenubar{background:white;}""")

        
        
    def createMenus(self):
        fileMenu = QMenu("File")
        fileMenu.addAction("Serial", self.serialSetting)
        fileMenu.addSeparator()
        
        fileMenu.addAction("Leave", self.close)
        self.menuBar().addMenu(fileMenu)
    
    def show_and_startSerial(self, portName : str):
        self.centralWidget.start(portName=portName)
        self.show()
        
    
    def serialSetting(self):
        pass
	

if __name__ == "__main__":
    app = QApplication([])

    splash = Splash()
    splash.show()

    window = MainWindow()
    #window.show()
    splash.ready.connect(window.show_and_startSerial)
    splash.finished.connect(splash.deleteLater)
    
    sys.exit(app.exec())
