
import sys
from PyQt5.QtWidgets import (QLabel,QMainWindow, QTextEdit,
    QAction, QFileDialog, QApplication, QHBoxLayout)
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5 import QtGui, QtCore
from PIL.ImageQt import ImageQt
from PIL import Image
import skimage
from skimage import io
import numpy as np

class Example(QMainWindow):

    def __init__(self):
        super().__init__()

        self.initUI()
    def mousePressEvent(self, QMouseEvent):
        pos = QMouseEvent.pos()
        norm_array = np.divide(self.img_array, 255.0)
        # print (self.img_array[pos.x()][pos.y() - 28])
        RGB = norm_array[pos.x()][pos.y() - 28]
        max = np.amax(RGB)
        min = np.amin(RGB)
        R = RGB[0]
        G = RGB[1]
        B = RGB[2]
        print('R: {0:.1f}.'.format(R))
        print('G: {0:.1f}.'.format(G))
        print('B: {0:.1f}.'.format(B))
        self.pointHSV(R,G,B,max, min)
        XYZ = self.RGBtoXYZ(RGB)
        # print (XYZ)
        self.XYZtoLAB(XYZ)

    def XYZtoLAB(self, XYZ):
        Xn = 0.31382
        Yn = 0.33100
        Zn = 0.35518

        X = XYZ[0]
        Y = XYZ[1]
        Z = XYZ[2]

        L = 116 * self.f(Y / Yn) - 16
        a = 500 * (self.f(X / Xn) - self.f(Y / Yn))
        b = 200 * (self.f(Y / Yn) - self.f(Z / Zn))
        print()
        print('L: {0:.1f}.'.format(L))
        print('a: {0:.1f}.'.format(a))
        print('b: {0:.1f}.'.format(b))

    def f (self, t):
        if t > (6 / 29.) ** 3:
            return t ** (1/3.)
        else:
            return (1/3.) * (29/6.)**2 * t + 4/29.

    def RGBtoXYZ(self, RGB):
        M = np.matrix([[0.5767309, 0.185540, 0.1881852],
                       [0.2973769, 0.6273491, 0.0752741],
                       [0.0270343, 0.0706872, 0.9911085]])
        product = np.matmul(M, RGB)
        product = np.array(product)
        return product[0]



    def pointHSV (self, R, G, B, MAX, MIN):
        if MAX == MIN :
            H = 0
        elif MAX == R and G >= B:
            H = 60*(G-B)/(MAX-MIN)
        elif MAX == R and G < B:
            H = 60*(G-B)/(MAX-MIN) + 360
        elif MAX == G:
            H = 60*(B-R)/(MAX-MIN) + 120
        elif MAX == B:
            H = 60*(R-G)/(MAX-MIN) + 240
        else:
            H = 0

        if MAX == 0:
            S = 0
        else:
            S = 1 - MIN/MAX

        S *= 100

        V = MAX

        V *= 100
        print()
        print('H: {0:.1f}.'.format(H))
        print('S: {0:.1f}.'.format(S))
        print('V: {0:.1f}.'.format(V))


    def initUI(self):

        # self.textEdit = QTextEdit()
        # self.setCentralWidget(self.textEdit)
        # self.statusBar()

        self.label = QLabel();
        self.setCentralWidget(self.label)


        openFile = QAction(QIcon('open.png'), 'Open', self)
        openFile.setShortcut('Ctrl+O')
        openFile.setStatusTip('Open new File')
        openFile.triggered.connect(self.showDialog)

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(openFile)

        self.setGeometry(300, 300, 350, 300)
        self.setWindowTitle('File dialog')
        self.show()


    def showDialog(self):

        fname = QFileDialog.getOpenFileName(self, 'Open file', '/home/voshkanov/Pictures')[0]
        self.img_array = io.imread(fname)
        pixmap = QPixmap(fname)
        self.label.setPixmap(pixmap)
        self.resize(pixmap.width(), pixmap.height())


if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
