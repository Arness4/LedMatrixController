from PySide6.QtWidgets import QWidget
from PySide6.QtGui import QColor, QPainter
from PySide6.QtCore import Qt, QRectF, Slot

class Led(QWidget): 
    def __init__(self,row : int, col: int, matrix : [[]] ,radius : int = 20, parent=None):
        super().__init__(parent)
        self.color = QColor(Qt.white)
        self.hovered = False
        self.radius = radius

        self.setFixedWidth(radius*2)
        self.setFixedHeight(radius*2)

        self.setStyleSheet("background-color:black")

        #boolean 1 : selected, 0: not selected => more practice to generate binary code
        self.isSelected = 0

        self.row = row
        self.col = col
        self.matrix = matrix
        
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.setPen(Qt.NoPen)
        painter.setBrush(self.color)
        rect = QRectF(0.0, 0.0, self.radius * 2, self.radius*2)
        painter.drawEllipse(rect)

    def enterEvent(self, event):
        self.hovered = True
        self.update()

    def leaveEvent(self, event):
        self.hovered = False
        self.update()

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.color = QColor(Qt.red) if self.color != QColor(Qt.red) else QColor(Qt.white)

            self.isSelected = 0 if self.isSelected == 1 else 1
            self.matrix[self.col][self.row] = self.isSelected
            self.update()

    @Slot()
    def reset(self):
        self.color = QColor(Qt.white)
        self.isSelected = 0
        self.matrix[self.col][self.row] = self.isSelected
        self.update()