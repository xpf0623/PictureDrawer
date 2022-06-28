#include "mainwindow.h"
#include "map.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    if(argc==1){




            Widget w;
            w.setWindowTitle("画图");
            w.setWindowIcon(QIcon("://image/draw.png"));
            w.show();

            return a.exec();
    }
    else
    {
             textoperation(argv[1],argv[2]);
             return 0;
    }
}
