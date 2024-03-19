from PySide6.QtWidgets import QWidget, QPushButton, QHBoxLayout, QVBoxLayout, QLineEdit, QLabel

from PySide6.QtCore import Qt, Slot, QIODeviceBase, QByteArray

from PySide6.QtSerialPort import QSerialPort

import struct
from ledMatrix import LedMatrix



class CentralWidget(QWidget):
    ser : QSerialPort

    def __init__(self, parent=None):
            super().__init__(parent)
            # self.showMaximized()
            self.setWindowTitle("8x8 Led Matrix controller")

            self.ser = QSerialPort(self)
            self.ser.setBaudRate(QSerialPort.BaudRate.Baud9600)
            
            self.ledMatrix = LedMatrix()

            matrixLayout = QHBoxLayout()

            commandLayout = QVBoxLayout()
            revereseBtn = QPushButton("Reverse selection")
            revereseBtn.setMaximumWidth(100)
            revereseBtn.clicked.connect(self.reverseSprite)

            resetBtn = QPushButton("Clear")
            resetBtn.setMaximumWidth(100)
            resetBtn.clicked.connect(self.clearSprite)
            printSpriteBtn = QPushButton("Print Sprite")
            printSpriteBtn.clicked.connect(self.sendSpriteData)
            printSpriteBtn.setMaximumWidth(100)

            commandLayout.addWidget(revereseBtn)
            commandLayout.addWidget(resetBtn)
            commandLayout.addWidget(printSpriteBtn)

            matrixLayout.addWidget(self.ledMatrix)
            matrixLayout.addLayout(commandLayout)
            matrixLayout.setAlignment(Qt.AlignmentFlag.AlignJustify|Qt.AlignmentFlag.AlignTop)


            hlayout = QHBoxLayout()
            self.textEdit = QLineEdit()
            sendTextBtn = QPushButton()
            sendTextBtn.clicked.connect(self.sendTextData)
            sendTextBtn.setText("Print text")
            
            hlayout.addWidget(self.textEdit)
            hlayout.addWidget(sendTextBtn)

            vlayout = QVBoxLayout()
            vlayout.addLayout(matrixLayout)
            vlayout.addSpacing(10)
            

            label = QLabel("Send a text instead...(127 char max)")
            vlayout.addWidget(label)
            label.setObjectName("infoLabel")
            vlayout.addLayout(hlayout)

            self.setLayout(vlayout)
            

    def start(self, portName):
        self.ser.setPortName(portName)
        self.ser.open(QIODeviceBase.OpenModeFlag.WriteOnly)
    
    @Slot()
    def sendSpriteData(self):
        hex_table = self.ledMatrix.generate()
        self.sendData(hex_table, isText=False)

    @Slot()
    def clearSprite(self):
        self.ledMatrix.reset()
        hex_table = self.ledMatrix.generate()
        self.sendData(hex_table, isText=False)
         

    @Slot()
    def reverseSprite(self):
         pass
    
    @Slot()
    def sendTextData(self):
        text = self.textEdit.text()
        self.sendData(QByteArray(text), isText=True)

    def sendData(self, data : QByteArray, isText : bool):
        """
            Le type de données à envoyer (N octet):
                - 1er octet : 
                    .bit 7=0 => afficher un char; bit7=1 => afficher un texte
                    .bit 6 - bit 0: => Le nombre de caractère dans le texte (7bits => 127 carac possibles)
                - 2 => Nème octet : data  
        """
        #Initialisation d'une entête vide
        header = 0b00000000

        #mise à 1 du MSB et insertion de la taille du texte si on veut envoyer du texte
        if isText :
            header = header | 0b10000000
            text_size = data.length() 
            if text_size > 127:
                 text_size = 127
            header = header | text_size

        dataToSend = QByteArray()

        dataToSend.append(struct.pack('B',int(header)))
        dataToSend.append(data)

        # print(dataToSend)
        self.ser.write(dataToSend)


    
        
    