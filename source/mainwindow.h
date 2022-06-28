#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QPainter>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "function.h"
using namespace std;
std::vector<std::string> split(const std::string& s, char delimiter);

namespace Ui {
class MainWindow;
}

void textoperation(char *filename,char *savefile);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:

    Ui::MainWindow *ui;
    QPoint firstpoint;
    QPoint lastpoint;
    QPixmap *map;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};



#endif // MAINWINDOW_H
