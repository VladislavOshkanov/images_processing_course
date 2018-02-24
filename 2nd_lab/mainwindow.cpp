#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loader.h"
#include "image.h"
#include "converter.h"
#include "filter.h"

#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/voshkanov/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap img(fileName);
    QLabel *label = new QLabel(this);
    Image image = loader::load(fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gauss(image, 20);

    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image);

    this->setCentralWidget(label);
    label->setPixmap(img);

}

MainWindow::~MainWindow()
{
    delete ui;
}
