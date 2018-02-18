
import sys
from PyQt5.QtWidgets import (QLabel,QMainWindow, QTextEdit,
    QAction, QFileDialog, QApplication,QPushButton, QSlider,QVBoxLayout, QHBoxLayout)
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5 import QtGui, QtCore, QtWidgets
from PIL.ImageQt import ImageQt
from PIL import Image
import skimage
import math
from skimage import io
import numpy as np
import matplotlib.pyplot as plt

class Second(QMainWindow):
    def __init__(self, parent=None):
        super(Second, self).__init__(parent)
class Example(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.SKIP = 10
        self.type = "square"
        self.rubberband = QtWidgets.QRubberBand(QtWidgets.QRubberBand.Rectangle, self)
        self.setMouseTracking(True)
    def mousePressEvent(self, QMouseEvent):
        pos = QMouseEvent.pos()
        norm_array = np.divide(self.img_array, 255.0)
        # print (norm_array.shape)
        RGB = norm_array[pos.y() - 28][pos.x()]
        # self.rgbArray = norm_array
        if self.type == "dot":
            max = np.amax(RGB)
            min = np.amin(RGB)
            R = RGB[0]
            G = RGB[1]
            B = RGB[2]
            print()
            print('R: {0:.1f}.'.format(R))
            print('G: {0:.1f}.'.format(G))
            print('B: {0:.1f}.'.format(B))
            self.pointHSV(R,G,B,max, min)
            XYZ = self.RGBtoXYZ(RGB)
            self.XYZtoLAB(XYZ)

        elif self.type == "square":
            self.origin = QMouseEvent.pos()
            self.rubberband.setGeometry(QtCore.QRect(self.origin, QtCore.QSize()))
            self.rubberband.show()

    def transform(self):
        norm_array = np.divide(self.img_array, 255.0)
        H = self.slH.value()*10.
        S = self.slS.value()/10.
        V = self.slV.value()/10.

        self.prepareMatrix(H, S, V)
        for i in range (0, norm_array.shape[0]):
            for j in range (0, norm_array.shape[1]):
                norm_array[i][j] = self.transformHSV(norm_array[i][j])
                if (norm_array[i][j][0] > 1):
                    norm_array[i][j][0] = 1
                if (norm_array[i][j][1] > 1):
                    norm_array[i][j][1] = 1
                if (norm_array[i][j][2] > 1):
                    norm_array[i][j][2] = 1
        fname = '/home/voshkanov/Pictures/temp.jpg'
        io.imsave(fname, norm_array);
        pixmap = QPixmap(fname)
        self.img_array = io.imread(fname)

        self.label.setPixmap(pixmap)

    def mouseMoveEvent(self, event):
        if self.type == "square":
            if self.rubberband.isVisible():
                self.rubberband.setGeometry(QtCore.QRect(self.origin, event.pos()))
                # self.mouseMoveEvent(self,event)

    def printValuesOfRect (self, x_top_left, y_top_left, x_bottom_right, y_bottom_right):
        print ("RGB:")
        for i in range (x_top_left, x_bottom_right):
            for j in range (y_top_left, y_bottom_right):
                print(self.img_array[i][j],end='')
            print()
        print ("HSV:")
        for i in range (x_top_left, x_bottom_right):
            for j in range (y_top_left, y_bottom_right):
                RGB = np.divide(self.img_array[i][j],255.)
                max = np.amax(RGB)
                min = np.amin(RGB)
                np.set_printoptions(precision=1)
                print(self.pointHSV(RGB[0],RGB[1],RGB[2],max, min), end='')
            print()
        print ("LAB:")
        for i in range (x_top_left, x_bottom_right):
            for j in range (y_top_left, y_bottom_right):
                RGB = np.divide(self.img_array[i][j],255.)
                np.set_printoptions(precision=1)
                print(self.RGBtoLAB(RGB), end='')
            print()
        # print(self.rgbArray[x_top_left:x_bottom_right][y_top_left:y_bottom_right])

    def hist(self):
        self.SKIP = round(self.img_array.shape[1] / 200)
        print ("skip", self.SKIP)
        # self.SKIP = 5
        norm_array = np.divide(self.img_array, 255.0)
        self.lab = np.zeros((norm_array.shape[0] + 1, norm_array.shape[1] + 1), dtype=int)
        self.a = np.zeros((norm_array.shape[0] + 1, norm_array.shape[1] + 1), dtype=int)
        self.b = np.zeros((norm_array.shape[0] + 1, norm_array.shape[1] + 1), dtype=int)
        for i in range (0, norm_array.shape[0]):
            for j in range (0, norm_array.shape[1]):
                if (i % self.SKIP == 0 and j % self.SKIP == 0):
                    RGB = np.divide(self.img_array[i][j],255.)
                    LAB = self.RGBtoLAB(RGB)
                    self.lab[i][j] = LAB[0]
                    self.a[i][j] = LAB[1]
                    self.b[i][j] = LAB[2]
        arr = []
        for i in range (0, self.lab.shape[0]):
            for j in range (0, self.lab.shape[1]):
                if (i % self.SKIP == 0 and j % self.SKIP == 0):
                    arr.append(self.lab[i][j])

        hist_data1 = np.zeros((101), dtype=int)
        hist_x1 = np.zeros((101), dtype=int)
        # print (sorted(arr))
        arr = sorted(arr)
        for i in range (0, len(arr)):
            hist_data1[round(arr[i])] += 1
        for i in range (0, 101):
            hist_x1[i] = i
        hist_data1 = hist_data1 * self.SKIP ** 2;



        plt.subplot(221)
        n, bins, patches = plt.hist(hist_x1, bins=101, weights=hist_data1,facecolor='g')
        plt.xlabel('L component')
        plt.ylabel('Count')
        plt.title('Histogram L')
        plt.axis([0, 100, 0, np.amax(hist_data1)])
        plt.grid(True)
        plt.show()

        arr = []
        for i in range (0, self.lab.shape[0]):
            for j in range (0, self.lab.shape[1]):
                if (i % self.SKIP == 0 and j % self.SKIP == 0):
                    if (self.b[i][j] > 127):
                        arr.append(127)
                    else:
                        arr.append(self.a[i][j])
        hist_data2 = np.zeros((258), dtype=int)
        hist_x1 = np.zeros((258), dtype=int)
        # print (sorted(arr))
        arr = sorted(arr)
        for i in range (0, len(arr)):
            hist_data2[round(arr[i] + 128)] += 1
        for i in range (0, 256):
            hist_x1[i] = i - 128
        hist_data2 = hist_data2 * self.SKIP**2;
        plt.subplot(222)
        n, bins, patches = plt.hist(hist_x1, bins=256, weights=hist_data2,facecolor='g')
        plt.xlabel('A component')
        plt.ylabel('Count')
        plt.axis([-128, 128, 0, np.amax(hist_data2)])
        plt.grid(True)
        plt.show()

        transp = np.transpose(self.lab)
        arr = []
        for i in range (0, self.lab.shape[0]):
            for j in range (0, self.lab.shape[1]):
                if (i % self.SKIP == 0 and j % self.SKIP == 0):
                    if (self.b[i][j] > 127):
                        arr.append(127)
                    else:
                        arr.append(self.b[i][j])
        hist_data3 = np.zeros((256), dtype=int)
        hist_x1 = np.zeros((256), dtype=int)
        # print (sorted(arr))
        arr = sorted(arr)
        for i in range (0, len(arr)):
            hist_data3[round(arr[i] + 128)] += 1
        for i in range (0, 256):
            hist_x1[i] = i - 128
        hist_data3 = hist_data3 * self.SKIP;

        plt.subplot(223)
        n, bins, patches = plt.hist(hist_x1, bins=256, weights=hist_data3,facecolor='g')
        plt.xlabel('B component')
        plt.ylabel('Count')
        plt.axis([-128, 128, 0, np.amax(hist_data3)])
        plt.grid(True)
        plt.show()

    def mouseReleaseEvent(self, event):
        if self.type == "square":
            rect = self.rubberband.geometry()
            self.printValuesOfRect(rect.x(), rect.y(), rect.x() + rect.width(),rect.y() + rect.height())

    def RGBtoLAB(self, RGB):
        self.M = np.matrix([[0.488, 0.310, 0.200],
                       [0.176, 0.812, 0.010],
                       [0.000, 0.010, 0.989]])
        XYZ = self.RGBtoXYZ(RGB)
        return self.XYZtoLAB(XYZ)

    def XYZtoLAB(self, XYZ):
        Xn = 1.0
        Yn = 1.0
        Zn = 1.0

        X = XYZ[0]
        Y = XYZ[1]
        Z = XYZ[2]

        xr = X/Xn
        yr = Y/Yn
        zr = Z/Zn

        fx = (xr)**(1/3)
        fy = (yr)**(1/3)
        fz = (zr)**(1/3)

        L = 116 * fy - 16
        a = 500 * (fx - fy)
        b = 200 * (fy - fz)
        if (self.type == "dot"):
            print('L: {0:.1f}.'.format(L))
            print('a: {0:.1f}.'.format(a))
            print('b: {0:.1f}.'.format(b))
        return np.array([L,a,b])


    # def f (self, t):
    #     if t > (6 / 29.) ** 3:
    #         return t ** (1/3.)
    #     else:
    #         return (1/3.) * (29/6.)**2 * t + 4/29.

    def RGBtoXYZ(self, RGB):
        # self.M = np.matrix([[0.488, 0.310, 0.200],
        #                [0.176, 0.812, 0.010],
        #                [0.000, 0.010, 0.989]])
        # product = np.matmul(self.M, RGB)
        product = np.array([0.488*RGB[0]+0.310*RGB[1]+0.200*RGB[2], 0.176*RGB[0]+0.812*RGB[1]+0.010*RGB[2], 0.01*RGB[1] + 0.989*RGB[2]])

        return product

    def dot(self):
        self.type = "dot"
    def square(self):
        self.type = "square"

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
        if (self.type == "dot") :
            print('H: {0:.1f}.'.format(H))
            print('S: {0:.1f}.'.format(S))
            print('V: {0:.1f}.'.format(V))
        return np.array([H,S,V])

    def prepareMatrix(self, H, S, V):
        U = math.cos(H * 3.1415 / 180.)
        W = math.sin(H * 3.1415 / 180.)
        print("uw")
        print (U)
        print (W)
        M1 = np.matrix([[0.299, 0.587, 0.114],
                       [0.596, -0.274, -0.321],
                       [0.211, -0.523, 0.311]])
        M2 = np.matrix([[V, 0, 0],
                       [0, V*S*U, -V*S*W],
                       [0, V*S*W, V*S*U]])
        M3 = np.matrix([[1., 0.956, 0.621],
                       [1., -0.272, -0.647],
                       [1., -1.107, 1.705]])
        prod1 = np.matmul(M1, M2)
        M = np.matmul (prod1, M3)
        self.transformMatrix = M

    def transformHSV(self, RGB):
        NEW_HSV = np.matmul(self.transformMatrix, RGB)
        product = np.array(NEW_HSV)
        return product[0]

    def initUI(self):


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
        self.setWindowTitle('Image')
        self.show()

        window2 = Second(self)
        window2.setWindowTitle('HSV')

        self.slH = QSlider(QtCore.Qt.Horizontal, window2)

        self.slH.setMinimum(-18)
        self.slH.setMaximum(18)
        self.slH.setValue(0)
        self.slH.setTickPosition(QSlider.TicksBelow)
        self.slH.setTickInterval(1)

        self.slS = QSlider(QtCore.Qt.Horizontal, window2)
        self.slS.move(0, 40)
        self.slS.setMinimum(0)
        self.slS.setMaximum(20)
        self.slS.setValue(10)
        self.slS.setTickPosition(QSlider.TicksBelow)
        self.slS.setTickInterval(1)


        self.slV = QSlider(QtCore.Qt.Horizontal, window2)
        self.slV.move(0, 80)

        self.slV.setMinimum(0)
        self.slV.setMaximum(20)
        self.slV.setValue(10)
        self.slV.setTickPosition(QSlider.TicksBelow)
        self.slV.setTickInterval(1)

        button = QPushButton('Ok', window2)
        button.move(0,120)
        button.clicked.connect(self.transform)
        button = QPushButton('Hist', window2)
        button.move(0,160)
        button.clicked.connect(self.hist)
        button = QPushButton('Dot', window2)
        button.move(0,200)
        button.clicked.connect(self.dot)
        button = QPushButton('Rect', window2)
        button.move(0,240)
        button.clicked.connect(self.square)


        # window2.addWidget(self.sl)
        window2.resize(100,300)
        window2.show()

    def showDialog(self):

        fname = QFileDialog.getOpenFileName(self, 'Open file', '/home/voshkanov/Pictures')[0]
        self.img_array = io.imread(fname)


        pixmap = QPixmap(fname)
        self.label.setPixmap(pixmap)
        self.resize(pixmap.width(), pixmap.height())
        # norm_array = np.divide(self.img_array, 255.0)
        # for i in range (0, norm_array.shape[0]):
        #     for j in range (0, norm_array.shape[1]):
        #         RGB = np.divide(self.img_array[i][j],255.)
        #         self.lab[i][j] = self.RGBtoLAB(RGB)[0]


if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
