from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QPushButton
from PySide6.QtCore import QByteArray

from led import Led

import struct

class LedMatrix(QWidget):

    leds = []
    
    def __init__(self, rows = 8, columns = 8, parent : QWidget = None):
        super().__init__(parent=parent)

        self.matrix = [[0 for _ in range(rows)] for _ in range(columns)]

        self.rows = rows
        self.columns = columns
        layout = QHBoxLayout()
        layout.setSpacing(5)
        self.setLayout(layout)
        self.createMatrix()

        self.setMaximumHeight(40*8+5*7)


    """
        Géneration du byteBuffer pour enviyer une caractère
        self.matrix est une matrice (8x8), elle contient l'état des leds d'une colonne
    """       
    def generate(self) -> []:
        data = QByteArray()

        for i in range(0, self.columns):
            col = self.matrix[i]
            
            decimal_value = 0

            #Iverser la liste pour envoyer les MSB en premier.
            #Convertir l'état (binaire) des leds de la colonne en décimal
            col.reverse()
            for row in range(0, len(col)):
                decimal_value += col[row] * (2**(len(col)-row-1))
            
            #restorer la colonne 
            col.reverse()

            #Conversion et Ajout de la valeur décimale dans sle buffer
            data.append(struct.pack('B',decimal_value))

        return data

    def createMatrix(self):
        for col in range(self.columns):
            
            col_layout = QVBoxLayout()
            col_layout.setSpacing(5)
            col_layout.setAlignment(Qt.AlignmentFlag.AlignVCenter|Qt.AlignmentFlag.AlignLeft)
            for row in range(self.rows):
                led = Led(row=row, col=col, matrix=self.matrix)
                self.leds.append(led)
                col_layout.addWidget(led)
            self.layout().addLayout(col_layout)


    def reset(self):
        for led in self.leds:
            led.reset()
       