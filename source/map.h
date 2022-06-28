#ifndef MAP_H
#define MAP_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPainter>
#include <QWidget>
#include <QColorDialog>
#include <QSlider>
#include <QStyle>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "function.h"

#define interval 32
#define rimagex 6

using namespace std;


class Widget : public QWidget

{

    Q_OBJECT

public:

    explicit Widget(QWidget *parent = nullptr);

private:

    QPoint firstpoint;
    QPoint lastpoint;
    QPixmap *map;
    QPixmap *tmpmap;
    QColor nowcolor;
    QPen *mypen;
    int penw;
    bool resizemap;
    bool resizex,resizey;
    bool linedraw,LineDDA,LineBresenham,linedrawing;
    bool polygondraw,polygonDDA,polygonBresenham,polygondrawing;
    bool ellipsedraw,ellipsedrawing;
    bool curvedraw,curveBezier,curveBspline,curvedrawing;
    bool Select,move,Rotate,Scalelt,Scalert,Scalelb,Scalerb;
    int id;
    vector<Unit> units;
    vector<Line> lines;
    vector<Polygon> polygons;
    vector<Ellipse> ellipses;
    vector<Curve> curves;
    vector<pair<int,int>> resline;
    vector<pair<int,int>> resellipse;
    vector<pair<int,int>> curvepoint;
    vector<QPoint> points;

    Unit nowunit;
    int rectxl,rectyl,rectw,recth;
    int scalew,scaleh;
    int xr,yr;


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void drawLineOnMap(Line aline);
    void drawEllipseOnMap(Ellipse &aellipse);
    void drawPolygonOnMap(Polygon apolygon);
    void drawCurveOnMap(Curve acurve);
    bool selectLine(Line aline,int x,int y);
    bool selectPolygon(Polygon apolygon,int x,int y);
    bool selectEllipse(Ellipse aellipse,int x,int y);
    bool selectCurve(Curve acurve,int x,int y);


private slots:
    void savefile()
    {
        QString file_name = QFileDialog::getSaveFileName(this,
                                                         tr("save a picture."),
                                                         "D:/",
                                                         tr("*.png;;*.jpg;;*.bmp"));
        map->save(file_name);


    }

    void openfile()
    {
        QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open a picture."),
                "D:/",
                tr("images(*.png *jpeg *bmp)"));
        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the order file!");
        }
        else {
            string file=fileName.toStdString();
            QImage img(file.c_str());
            map=new QPixmap(QSize(img.width(),img.height()));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            QPainter painter(map);
            painter.drawImage(0,0,img);
            repaint();

        }

    }

    void select()
    {
        Select=true;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }

    void line(){
        //cout<<"drawline"<<endl;
        linedraw=true;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;
        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }

    void setlineDDA()
    {
        //cout<<"lineDDA"<<endl;
        LineDDA=true;
        LineBresenham=false;

        linedraw=true;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void setlineBresenham()
    {
        //cout<<"lineBresenham"<<endl;
        LineDDA=false;
        LineBresenham=true;

        linedraw=true;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void polygon()
    {
//        cout<<"drawpolygon"<<endl;
        linedraw=false;
        polygondraw=true;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void setpolygonDDA()
    {
        //cout<<"polygonDDA"<<endl;
        polygonDDA=true;
        polygonBresenham=false;

        linedraw=false;
        polygondraw=true;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void setpolygonBresenham()
    {
        //cout<<"polygonBresenham"<<endl;
        polygonDDA=false;
        polygonBresenham=true;

        linedraw=false;
        polygondraw=true;
        ellipsedraw=false;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void ellipse()
    {
        //cout<<"drawellipse"<<endl;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=true;
        curvedraw=false;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void curve()
    {
        //cout<<"drawcurve"<<endl;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=true;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void setcurveBezier()
    {
        //cout<<"curveBezier"<<endl;
        curveBezier=true;
        curveBspline=false;

        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=true;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }
    void setcurveBspline()
    {
        //cout<<"curveBspline"<<endl;
        curveBezier=false;
        curveBspline=true;

        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=true;
        Select=false;

        int w=map->width();
        int h=map->height();
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,h));
        map->fill(backcolor);

        for(int i=0;i<lines.size();i++)
        {
            drawLineOnMap(lines[i]);
        }
        for(int i=0;i<ellipses.size();i++)
        {
            drawEllipseOnMap(ellipses[i]);
        }
        for(int i=0;i<polygons.size();i++)
        {
            drawPolygonOnMap(polygons[i]);
        }
        for(int i=0;i<curves.size();i++)
        {
            drawCurveOnMap(curves[i]);
        }
        this->repaint();
    }

    void color()
    {
        nowcolor=QColorDialog::getColor(Qt::black,this);
        mypen->setColor(nowcolor);

    }
    void width(){
        QSlider *s=new QSlider();
        s->setOrientation(Qt::Horizontal);
        s->setMinimum(1);
        s->setMaximum(10);
        s->setValue(penw);
        s->setSingleStep(1);
        s->setTickPosition(QSlider::TicksAbove);
//        s->setStyleSheet(QString("float:right;"));
        s->setWindowTitle("Width");
        s->setWindowIcon(QIcon("://image/width.png"));
        s->resize(QSize(300,30));
        s->show();
        connect(s,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));

    }
    void changePenWidth(int w)
    {
        penw=w;
        mypen->setWidth(penw);
    }

    void clear()
    {
        QColor backcolor=qRgb(255,255,255);
        map=new QPixmap(400,400);
        map->fill(backcolor);
        units.clear();
        lines.clear();
        polygons.clear();
        ellipses.clear();
        curves.clear();
        mypen->setColor(QColor(0,0,0));
        mypen->setWidth(1);
        penw=1;

        linedraw=false;linedrawing=false;LineDDA=true;LineBresenham=false;
        polygondraw=false;polygondrawing=false;polygonDDA=true;polygonBresenham=false;
        ellipsedraw=false;ellipsedrawing=false;
        curvedraw=false;curvedrawing=false;curveBezier=true;curveBspline=false;
        repaint();
    }
};

#endif // MAP_H
