#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void handleButton ();
    QString fileName;
    QLineEdit * ed;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
