#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loader.h"
#include "image.h"
#include "converter.h"
#include "filter.h"
#include "thread"
#include "allocator.h"

#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <cstdlib>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/voshkanov/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap img(this->fileName);

    QLabel *label = new QLabel(this);


    Image image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gabour(image, 2, 0, 0, 1, 1); // lambda theta psi sigma gamma
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/home/voshkanov/Desktop/results/gabour0.jpg"));

    image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gabour(image, 2, 45, 0, 1, 1); // lambda theta psi sigma gamma
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/home/voshkanov/Desktop/results/gabour45.jpg"));

    image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gabour(image, 2, 90, 0, 1, 1); // lambda theta psi sigma gamma
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/home/voshkanov/Desktop/results/gabour90.jpg"));

    image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gabour(image, 2, 135, 0, 1, 1); // lambda theta psi sigma gamma
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/home/voshkanov/Desktop/results/gabour135.jpg"));

    image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::sobel(image);
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/home/voshkanov/Desktop/results/sobel.jpg"));
//    label->se(img.width());
//    label->height(img.height());
//    QVBoxLayout * mainLayout = new QVBoxLayout;
//     QMainWindow->setCentralWidget(label);
//    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    this->setCentralWidget(label);

    label->setPixmap(img.scaled(800,800,Qt::KeepAspectRatio));
    QWidget * second = new QWidget;
    QPushButton *butt = new QPushButton("Gauss");
    QVBoxLayout * layout = new QVBoxLayout;
    this->ed = new QLineEdit();
    layout->addWidget(ed);
    layout->addWidget(butt);

    connect(butt, &QPushButton::released, this, &MainWindow::handleButton);
//    this->handleButton(this->fileName, this);

    second->setLayout(layout);
    second->show();



//    wnd->show();
}
void MainWindow :: handleButton () {
    float const sigma = this->ed->text().toFloat();
    Image image = loader::load(this->fileName);
    image = Converter::RGBtoLAB(image);
    image = Filter::gauss(image, sigma); // lambda theta psi sigma gamma
    Converter::normalizeLAB(image);
    image = Converter::LABtoRGB(image);
    Converter::normalizeRGB(image);
    loader::saveTmpImage(image, QString("/tmp/gauss.jpg"));
    QPixmap img(QString("/tmp/gauss.jpg"));
    QLabel *label = new QLabel();
    this->setCentralWidget(label);

    label->setPixmap(img.scaled(800,800,Qt::KeepAspectRatio));

}
MainWindow::~MainWindow()
{
    delete ui;
}
