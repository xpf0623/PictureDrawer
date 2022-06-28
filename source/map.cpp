#include "map.h"
#include "function.h"



Widget::Widget(QWidget *parent): QWidget(parent),resizemap(false),resizex(false),resizey(false),penw(1),
    linedraw(false),LineDDA(true),LineBresenham(false),linedrawing(false),
    polygondraw(false),polygonDDA(true),polygonBresenham(false),polygondrawing(false),
    ellipsedraw(false),ellipsedrawing(false),
    curvedraw(false),curveBezier(true),curveBspline(false),curvedrawing(false),
    id(0),Select(true),move(false),Rotate(false),Scalelt(false),Scalelb(false),Scalerb(false),Scalert(false)
{
    rectxl=rectyl=rectw=recth=0;
    xr=yr=0;


    mypen = new QPen(Qt::black);

    QColor backcolor=qRgb(255,255,255);
    map=new QPixmap(400,400);
    map->fill(backcolor);



    QToolBar *toolbar=new QToolBar(this);

    QToolButton *toolButton_file = new QToolButton(this);
    toolButton_file->setText("file");
    toolButton_file->setIcon(QIcon("://image/file.png"));
    toolButton_file->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu * m0 = new QMenu(this);
    QAction *A1 = new QAction("save");
    QAction *A2 = new QAction("open");
    m0->addAction(A1);
    m0->addAction(A2);
    toolButton_file->setMenu(m0);
    toolbar->addWidget(toolButton_file);

    QToolButton *toolButton_select = new QToolButton(this);
    toolButton_select->setText("select");
    toolButton_select->setIcon(QIcon("://image/arrow.png"));
    toolbar->addWidget(toolButton_select);

    QToolButton *toolButton_line = new QToolButton(this);
    toolButton_line->setText("Line");
    toolButton_line->setIcon(QIcon("://image/line.png"));
    toolButton_line->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu * m1 = new QMenu(this);
    QAction *a1 = new QAction("DDA");
    QAction *a2 = new QAction("Bresenham");
    m1->addAction(a1);
    m1->addAction(a2);
    toolButton_line->setMenu(m1);
    toolbar->addWidget(toolButton_line);

    QToolButton *toolButton_polygon = new QToolButton(this);
    toolButton_polygon->setText("polygon");
    toolButton_polygon->setIcon(QIcon("://image/polygon.png"));
    toolButton_polygon->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu * m2 = new QMenu(this);
    QAction *a3 = new QAction("DDA");
    QAction *a4 = new QAction("Bresenham");
    m2->addAction(a3);
    m2->addAction(a4);
    toolButton_polygon->setMenu(m2);
    toolbar->addWidget(toolButton_polygon);

    QToolButton *toolButton_ellipse = new QToolButton(this);
    toolButton_ellipse->setText("ellipse");
    toolButton_ellipse->setIcon(QIcon("://image/ellipse.png"));
    toolbar->addWidget(toolButton_ellipse);

    QToolButton *toolButton_curve = new QToolButton(this);
    toolButton_curve->setText("curve");
    toolButton_curve->setIcon(QIcon("://image/curve.png"));
    toolButton_curve->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu * m3 = new QMenu(this);
    QAction *a5 = new QAction("Bezier");
    QAction *a6 = new QAction("Bspline");
    m3->addAction(a5);
    m3->addAction(a6);
    toolButton_curve->setMenu(m3);
    toolbar->addWidget(toolButton_curve);

    QToolButton *toolButton_options = new QToolButton(this);
    toolButton_options->setText("options");
    toolButton_options->setIcon(QIcon("://image/options.png"));
    toolButton_options->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu * m4 = new QMenu(this);
    QAction *a7 = new QAction("color");
    QAction *a8 = new QAction("width");
    QAction *a9 = new QAction("clear");
    m4->addAction(a7);
    m4->addAction(a8);
    m4->addAction(a9);
    toolButton_options->setMenu(m4);
    toolbar->addWidget(toolButton_options);

    this->resize(QSize(500,500));

    connect(A1,SIGNAL(triggered(bool)),this,SLOT(savefile()));
    connect(A2,SIGNAL(triggered(bool)),this,SLOT(openfile()));

    connect(toolButton_select,SIGNAL(clicked(bool)),this,SLOT(select()));

    connect(toolButton_line,SIGNAL(clicked(bool)),this,SLOT(line()));
    connect(a1,SIGNAL(triggered(bool)),this,SLOT(setlineDDA()));
    connect(a2,SIGNAL(triggered(bool)),this,SLOT(setlineBresenham()));

    connect(toolButton_polygon,SIGNAL(clicked(bool)),this,SLOT(polygon()));
    connect(a3,SIGNAL(triggered(bool)),this,SLOT(setpolygonDDA()));
    connect(a4,SIGNAL(triggered(bool)),this,SLOT(setpolygonBresenham()));

    connect(toolButton_ellipse,SIGNAL(clicked(bool)),this,SLOT(ellipse()));

    connect(toolButton_curve,SIGNAL(clicked(bool)),this,SLOT(curve()));
    connect(a5,SIGNAL(triggered(bool)),this,SLOT(setcurveBezier()));
    connect(a6,SIGNAL(triggered(bool)),this,SLOT(setcurveBspline()));



    connect(a7,SIGNAL(triggered(bool)),this,SLOT(color()));
    connect(a8,SIGNAL(triggered(bool)),this,SLOT(width()));
    connect(a9,SIGNAL(triggered(bool)),this,SLOT(clear()));

    setMouseTracking(true);

}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(linedrawing||ellipsedrawing||polygondrawing||curvedrawing){
        painter.drawPixmap(0,interval,tmpmap->width(),tmpmap->height(),*tmpmap);
    }
    else
        painter.drawPixmap(0,interval,map->width(),map->height(),*map);

}

bool Widget::selectLine(Line aline, int x, int y)
{
    int x1=aline.x1;
    int y1=aline.y1;
    int x2=aline.x2;
    int y2=aline.y2;
    if(x1==x2)
    {
        if(y1<y2)
        {
            if(x>=x1-5&&x<=x1+5&&y>=y1-5&&y<=y2+5)
                return true;
        }
        else
        {
            if(x>=x1-5&&x<=x1+5&&y>=y2-5&&y<=y1+5)
                return true;
        }
        return false;
    }
    else if(y1==y2)
    {
        if(x1<x2)
        {
            if(x>=x1-5&&x<=x2+5&&y>=y1-5&&y<=y1+5)
                return true;
        }
        else
        {
            if(x>=x2-5&&x<=x1+5&&y>=y1-5&&y<=y1+5)
                return true;
        }
        return false;
    }
    else
    {
        double m=double(y2-y1)/(x2-x1);
        double b=y1-m*x1;
        if(x1<x2)
        {
            if(x<x1-5||x>x2+5){

                return false;
            }
        }
        else
        {
            if(x<x2-5||x>x1+5){

                return false;
            }
        }
        if(y1<y2)
        {
            if(y<y1-5||y>y2+5){

                return false;
            }
        }
        else
        {
            if(y<y2-5||y>y1+5){

                return false;
            }
        }
        if(m>0){
            if(m*x+b>=y-5&&m*x+b<=y+5)
                return true;
            else if(double(y-b)/m>=x-5&&double(y-b)/m<=x+5)
                return true;
            else{

                return false;
            }
        }
        else
        {
            if(m*x+b+5>=y&&m*x+b-5<=y)
                return true;
            else if(double(y-b)/m-5<=x&&double(y-b)/m+5>=x)
                return true;
            else{

                return false;
            }
        }
    }
}

bool Widget::selectPolygon(Polygon apolygon, int x, int y)
{
    int xmin,ymin,xmax,ymax;
    xmin=apolygon.x[0];
    xmax=apolygon.x[0];
    ymin=apolygon.y[0];
    ymax=apolygon.y[0];
    for(int i=1;i<apolygon.n;i++)
    {
        if(apolygon.x[i]>xmax)
            xmax=apolygon.x[i];
        if(apolygon.x[i]<xmin)
            xmin=apolygon.x[i];
        if(apolygon.y[i]>ymax)
            ymax=apolygon.y[i];
        if(apolygon.y[i]<ymin)
            ymin=apolygon.y[i];

    }
    if(x<xmin-5||x>xmax+5)
        return false;
    if(y<ymin-5||y>ymax+5)
        return false;
    for(int i=0;i<apolygon.n;i++)
    {
        if(i!=apolygon.n-1)
        {
            int x1=apolygon.x[i];
            int y1=apolygon.y[i];
            int x2=apolygon.x[i+1];
            int y2=apolygon.y[i+1];
            if(x1==x2)
            {
                if(y1<y2)
                {
                    if(x>=x1-5&&x<=x1+5&&y>=y1-5&&y<=y2+5)
                        return true;
                }
                else
                {
                    if(x>=x1-5&&x<=x1+5&&y>=y2-5&&y<=y1+5)
                        return true;
                }
                return false;
            }
            else if(y1==y2)
            {
                if(x1<x2)
                {
                    if(x>=x1-5&&x<=x2+5&&y>=y1-5&&y<=y1+5)
                        return true;
                }
                else
                {
                    if(x>=x2-5&&x<=x1+5&&y>=y1-5&&y<=y1+5)
                        return true;
                }
                return false;
            }
            else{
                double m=double(y2-y1)/(x2-x1);
                double b=y1-m*x1;
                if(m*x+b>=y-5&&m*x+b<=y+5)
                    return true;
                else if(double(y-b)/m>=x-5&&double(y-b)/m<=x+5)
                    return true;

            }
        }
        else
        {
            int x1=apolygon.x[i];
            int y1=apolygon.y[i];
            int x2=apolygon.x[0];
            int y2=apolygon.y[0];

            if(x1==x2)
            {
                if(y1<y2)
                {
                    if(x>=x1-5&&x<=x1+5&&y>=y1-5&&y<=y2+5)
                        return true;
                }
                else
                {
                    if(x>=x1-5&&x<=x1+5&&y>=y2-5&&y<=y1+5)
                        return true;
                }
                return false;
            }
            else if(y1==y2)
            {
                if(x1<x2)
                {
                    if(x>=x1-5&&x<=x2+5&&y>=y1-5&&y<=y1+5)
                        return true;
                }
                else
                {
                    if(x>=x2-5&&x<=x1+5&&y>=y1-5&&y<=y1+5)
                        return true;
                }
                return false;
            }
            else{

                double m=double(y2-y1)/(x2-x1);
                double b=y1-m*x1;
                if(m*x+b>=y-5&&m*x+b<=y+5)
                    return true;
                else if(double(y-b)/m>=x-5&&double(y-b)/m<=x+5)
                    return true;

            }
        }
    }
    return false;
}

bool Widget::selectEllipse(Ellipse aellipse, int x, int y)
{
    if(aellipse.r%360!=0)
    {
        double r1=-double(aellipse.r)*3.1415926/180;
        double tmpx,tmpy;
        tmpx=double(x)*cos(r1)-double(y)*sin(r1)+(1.0-cos(r1))*double(aellipse.x)+double(aellipse.y)*sin(r1);
        tmpy=double(x)*sin(r1)+double(y)*cos(r1)+(1.0-cos(r1))*double(aellipse.y)-double(aellipse.x)*sin(r1);
        x=tmpx;
        y=tmpy;
    }
    int ex=aellipse.x;
    int ey=aellipse.y;
    int rx=aellipse.rx;
    int ry=aellipse.ry;

    double d=double(x-ex)*(x-ex)/(rx*rx)+double(y-ey)*(y-ey)/(ry*ry);
    if(d>=0.8&&d<=1.2)
        return true;
    return false;

}

bool Widget::selectCurve(Curve acurve, int x, int y)
{
    vector<pair<int,int>> res;
    if(acurve.algo=="Bezier")
    {
        res=CurveBezier(acurve.x,acurve.y,acurve.n);
    }
    else
    {
        res=CurveBspline(acurve.x,acurve.y,acurve.n);
    }
    int n=res.size();
    for(int i=0;i<30;i++)
    {
        int xc=res[i*n/30].first;
        int yc=res[i*n/30].second;
        if(x>=xc-5&&x<=xc+5&&y>=yc-5&&y<=yc+5)
            return true;
    }
    return false;
}

void Widget::drawLineOnMap(Line aline)
{
    QPen *pen=new QPen(QColor(aline.clr.rvalue(),aline.clr.gvalue(),aline.clr.bvalue()));
    pen->setWidth(aline.penw);
//    pen.setColor(Qt::black);
//    pen.setWidth(1);

    QPainter painter(map);
    painter.setPen(*pen);

    if(aline.algo=="DDA")
    {
        resline=lineDDA(aline.x1,aline.y1,aline.x2,aline.y2);
    }
    else
    {
        resline=lineBresenham(aline.x1,aline.y1,aline.x2,aline.y2);
    }
    for(int j=0;j<resline.size();j++)
    {
        painter.drawPoint(resline[j].first,resline[j].second-interval);

    }
}

void Widget::drawEllipseOnMap(Ellipse &aellipse)
{

    QPen *pen=new QPen(QColor(aellipse.clr.rvalue(),aellipse.clr.gvalue(),aellipse.clr.bvalue()));
    pen->setWidth(aellipse.penw);
    QPainter painter(map);
    painter.setPen(*pen);
    vector<pair<int,int>> res=ELLIPSE(aellipse.rx,aellipse.ry);
    aellipse.changeres1(res);
    aellipse.fill();
    if(aellipse.r%360!=0)
    {
        Ellipserotate(aellipse,aellipse.r);
    }
    for(int i=0;i<aellipse.res1.size();i++)
    {
        int x1=aellipse.res1[i].first;
        int y1=aellipse.res1[i].second;
        int x2=aellipse.res2[i].first;
        int y2=aellipse.res2[i].second;
        int x3=aellipse.res3[i].first;
        int y3=aellipse.res3[i].second;
        int x4=aellipse.res4[i].first;
        int y4=aellipse.res4[i].second;


        x1+=aellipse.x;
        x2+=aellipse.x;
        x3+=aellipse.x;
        x4+=aellipse.x;
        y1+=aellipse.y;
        y2+=aellipse.y;
        y3+=aellipse.y;
        y4+=aellipse.y;

        painter.drawPoint(x1,y1-interval);
        painter.drawPoint(x2,y2-interval);
        painter.drawPoint(x3,y3-interval);
        painter.drawPoint(x4,y4-interval);
    }
}

void Widget::drawPolygonOnMap(Polygon apolygon)
{
    QPen *pen=new QPen(QColor(apolygon.clr.rvalue(),apolygon.clr.gvalue(),apolygon.clr.bvalue()));
    pen->setWidth(apolygon.penw);
    QPainter painter(map);
    painter.setPen(*pen);

    if(apolygon.algo=="DDA")
    {
        for(int i=0;i<apolygon.n;i++)
        {
            if(i==apolygon.n-1)
            {
                resline=lineDDA(apolygon.x[i],apolygon.y[i],apolygon.x[0],apolygon.y[0]);
            }
            else
            {
                resline=lineDDA(apolygon.x[i],apolygon.y[i],apolygon.x[i+1],apolygon.y[i+1]);
            }
            for(int j=0;j<resline.size();j++)
            {
                painter.drawPoint(resline[j].first,resline[j].second-interval);

            }
        }
    }
    else
    {
        for(int i=0;i<apolygon.n;i++)
        {
            if(i==apolygon.n-1)
            {
                resline=lineBresenham(apolygon.x[i],apolygon.y[i],apolygon.x[0],apolygon.y[0]);
            }
            else
            {
                resline=lineBresenham(apolygon.x[i],apolygon.y[i],apolygon.x[i+1],apolygon.y[i+1]);
            }
            for(int j=0;j<resline.size();j++)
            {
                painter.drawPoint(resline[j].first,resline[j].second-interval);

            }
        }
    }
}

void Widget::drawCurveOnMap(Curve acurve)
{
    QPen *pen=new QPen(QColor(acurve.clr.rvalue(),acurve.clr.gvalue(),acurve.clr.bvalue()));
    pen->setWidth(acurve.penw);
    QPainter painter(map);
    painter.setPen(*pen);

    if(acurve.algo=="Bezier")
    {
        curvepoint=CurveBezier(acurve.x,acurve.y,acurve.n);
        for(int i=1;i<curvepoint.size();i++)
        {
            resline=lineDDA(curvepoint[i-1].first,curvepoint[i-1].second,curvepoint[i].first,curvepoint[i].second);
            for(int j=0;j<resline.size();j++)
            {
                int x=resline[j].first;
                int y=resline[j].second;
                painter.drawPoint(x,y-interval);
            }
        }
    }
    else
    {
        curvepoint=CurveBspline(acurve.x,acurve.y,acurve.n);
        for(int i=1;i<curvepoint.size();i++)
        {
            resline=lineDDA(curvepoint[i-1].first,curvepoint[i-1].second,curvepoint[i].first,curvepoint[i].second);
            for(int j=0;j<resline.size();j++)
            {
                int x=resline[j].first;
                int y=resline[j].second;
                painter.drawPoint(x,y-interval);
            }
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    firstpoint=e->pos();
    int x=e->x();
    int y=e->y();
    int w=map->width();
    int h=map->height()+interval;
    if(x>=w-5&&x<=w+5&&y>=h-5&&y<=h+5)
    {
        resizemap=true;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
    }
    else if(x>=w-5&&x<=w+5)
    {
        resizex=true;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
    }
    else if(y>=h-5&&y<=h+5)
    {
        resizey=true;
        linedraw=false;
        polygondraw=false;
        ellipsedraw=false;
        curvedraw=false;
    }





    if(linedraw==true&&(e->button()&Qt::LeftButton))
    {
        firstpoint=e->pos();
        linedrawing=true;
    }
    else if(polygondraw&&(e->button()&Qt::LeftButton)&&!polygondrawing)
    {
        firstpoint=e->pos();
        points.push_back(firstpoint);
        polygondrawing=true;

    }
    else if(polygondraw&&(e->button()&Qt::LeftButton)&&polygondrawing)
    {
        points.push_back(e->pos());
        int n=points.size();
//        QPen pen;
//        pen.setColor(Qt::black);
//        pen.setWidth(1);
        QPainter painter(map);
        painter.setPen(*mypen);

        if(polygonDDA)
        {
            resline=lineDDA(points[n-1].x(),points[n-1].y(),points[n-2].x(),points[n-2].y());
        }
        else
        {
            resline=lineBresenham(points[n-1].x(),points[n-1].y(),points[n-2].x(),points[n-2].y());
        }
        for(int i=0;i<resline.size();i++)
        {
            painter.drawPoint(resline[i].first,resline[i].second-interval);

        }
        tmpmap = new QPixmap(*map);
        repaint();
    }
    else if(ellipsedraw&&(e->button()&Qt::LeftButton))
    {
        firstpoint=e->pos();
        ellipsedrawing=true;
    }
    else if(curvedraw&&(e->button()&Qt::LeftButton)&&!curvedrawing)
    {
        curvedrawing=true;
        tmpmap=new QPixmap(*map);
//        QPen pen;
//        pen.setColor(Qt::black);
//        pen.setWidth(1);
        QPainter painter(tmpmap);
        painter.setPen(*mypen);

        firstpoint=e->pos();
        for(int i=firstpoint.x()-2;i<=firstpoint.x()+2;i++)
        {
            for(int j=firstpoint.y()-2;j<=firstpoint.y()+2;j++)
            {
                painter.drawPoint(i,j-interval);
            }
        }
        points.push_back(firstpoint);
        repaint();
    }
    else if(curvedraw&&(e->button()&Qt::LeftButton)&&curvedrawing)
    {
//        QPen pen;
//        pen.setColor(Qt::black);
//        pen.setWidth(1);
        QPainter painter(tmpmap);
        painter.setPen(*mypen);

        firstpoint=e->pos();
        for(int i=firstpoint.x()-2;i<=firstpoint.x()+2;i++)
        {
            for(int j=firstpoint.y()-2;j<=firstpoint.y()+2;j++)
            {
                painter.drawPoint(i,j-interval);
            }
        }
        points.push_back(firstpoint);
        repaint();
    }
    else if(Select&&(e->button()&Qt::LeftButton))
    {
        firstpoint=e->pos();
        int x=firstpoint.x();
        int y=firstpoint.y();

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
        move=false;

        if(x>=rectxl+rectw/2-rimagex&&x<=rectxl+rectw/2-rimagex+16&&y>=rectyl-26&&y<=rectyl-16)
        {
            setCursor(Qt::ClosedHandCursor);
            xr=rectxl+rectw/2;
            yr=rectyl+recth/2;
            Rotate=true;
            move=false;
            linedraw=false;
            polygondraw=false;
            ellipsedraw=false;
            curvedraw=false;
            return;
        }
        else if(x>=rectxl-5&&x<=rectxl+5&&y>=rectyl-5&&y<=rectyl+5)
        {
            setCursor(Qt::SizeFDiagCursor);
            scalew=rectw;
            scaleh=recth;
            Scalelt=true;
            move=false;
            linedraw=false;
            polygondraw=false;
            ellipsedraw=false;
            curvedraw=false;
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            return;
        }
        else if(x>=rectxl+rectw-5&&x<=rectxl+rectw+5&&y>=rectyl-5&&y<=rectyl+5)
        {
            setCursor(Qt::SizeBDiagCursor);
            Scalert=true;
            scalew=rectw;
            scaleh=recth;
            move=false;
            linedraw=false;
            polygondraw=false;
            ellipsedraw=false;
            curvedraw=false;
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            return;
        }
        else if(x>=rectxl-5&&x<=rectxl+5&&y>=rectyl+recth-5&&y<=rectyl+recth+5)
        {
            setCursor(Qt::SizeBDiagCursor);
            Scalelb=true;
            scalew=rectw;
            scaleh=recth;
            move=false;
            linedraw=false;
            polygondraw=false;
            ellipsedraw=false;
            curvedraw=false;
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            return;
        }
        else if(x>=rectxl+rectw-5&&x<=rectxl+rectw+5&&y>=rectyl+recth-5&&y<=rectyl+recth+5)
        {
            setCursor(Qt::SizeFDiagCursor);
            Scalerb=true;
            scalew=rectw;
            scaleh=recth;
            move=false;
            linedraw=false;
            polygondraw=false;
            ellipsedraw=false;
            curvedraw=false;
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            return;
        }

        int n=units.size();
        for(int i=n-1;i>=0;i--)
        {

            if(units[i].type==1)
            {
                Line aline;
                for(int j=0;j<lines.size();j++)
                {
                    if(lines[j].ID==units[i].ID)
                        aline=lines[j];

                }
                if(selectLine(aline,x,y))
                {
                    nowunit=units[i];
                    QPainter painter(map);
                    QPen pen;
                    pen.setColor(QColor(0,170,255));
                    pen.setWidth(1);
                    painter.setPen(pen);
                    int x,y;
                    if(aline.x1<aline.x2)
                        x=aline.x1;
                    else
                        x=aline.x2;
                    if(aline.y1<aline.y2)
                        y=aline.y1;
                    else
                        y=aline.y2;
                    rectxl=x;
                    rectyl=y;
                    rectw=abs(aline.x2-aline.x1);
                    recth=abs(aline.y2-aline.y1);
                    painter.drawRect(rectxl,rectyl-interval,rectw,recth);
                    painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
                    painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
                    painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
                    repaint();
                    move=true;
                    break;
                }

            }
            else if(units[i].type==2)
            {
                Polygon apolygon;
                for(int j=0;j<polygons.size();j++)
                {
                    if(polygons[j].ID==units[i].ID)
                        apolygon=polygons[j];
                }
                if(selectPolygon(apolygon,x,y))
                {
                    nowunit=units[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=apolygon.x[0];
                    xmax=apolygon.x[0];
                    ymin=apolygon.y[0];
                    ymax=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]<xmin)
                            xmin=apolygon.x[k];
                        if(apolygon.x[k]>xmax)
                            xmax=apolygon.x[k];
                        if(apolygon.y[k]<ymin)
                            ymin=apolygon.y[k];
                        if(apolygon.y[k]>ymax)
                            ymax=apolygon.y[k];
                    }
                    QPainter painter(map);
                    QPen pen;
                    pen.setColor(QColor(0,170,255));
                    pen.setWidth(1);
                    painter.setPen(pen);
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=abs(xmax-xmin);
                    recth=abs(ymax-ymin);
                    painter.drawRect(rectxl,rectyl-interval,rectw,recth);
                    painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
                    painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
                    painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
                    repaint();
                    move=true;
                    break;
                }
            }
            else if(units[i].type==3)
            {
                Ellipse aellipse;
                for(int j=0;j<ellipses.size();j++)
                {
                    if(ellipses[j].ID==units[i].ID)
                        aellipse=ellipses[j];
                }
                if(selectEllipse(aellipse,x,y))
                {
                    nowunit=units[i];
                    QPainter painter(map);
                    QPen pen;
                    pen.setColor(QColor(0,170,255));
                    pen.setWidth(1);
                    painter.setPen(pen);

                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }

                    rectxl=xmin+aellipse.x-1;
                    rectyl=ymin+aellipse.y-1;
                    rectw=abs(xmax-xmin)+2;
                    recth=abs(ymax-ymin)+2;

                    painter.drawRect(rectxl,rectyl-interval,rectw,recth);
                    painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
                    painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
                    painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
                    repaint();
                    move=true;
                    break;
                }
            }
            else if(units[i].type==4)
            {
                Curve acurve;
                for(int j=0;j<curves.size();j++)
                {
                    if(curves[j].ID==units[i].ID)
                        acurve=curves[j];
                }
                if(selectCurve(acurve,x,y))
                {
                    nowunit=units[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=acurve.x[0];
                    xmax=acurve.x[0];
                    ymin=acurve.y[0];
                    ymax=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]<xmin)
                            xmin=acurve.x[k];
                        if(acurve.x[k]>xmax)
                            xmax=acurve.x[k];
                        if(acurve.y[k]<ymin)
                            ymin=acurve.y[k];
                        if(acurve.y[k]>ymax)
                            ymax=acurve.y[k];
                    }
                    QPainter painter(map);
                    QPen pen;
                    pen.setColor(QColor(0,170,255));
                    pen.setWidth(1);
                    painter.setPen(pen);
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=abs(xmax-xmin);
                    recth=abs(ymax-ymin);
                    painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
                    painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
                    painter.drawRect(rectxl,rectyl-interval,rectw,recth);
                    painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
                    painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
                    painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
                    repaint();
                    move=true;
                    break;
                }
            }
        }




    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    lastpoint=e->pos();


    if(resizemap&&(e->button()&Qt::LeftButton)){
        resizemap=false;
    }
    else if(resizex&&(e->button()&Qt::LeftButton))
    {
        resizex=false;
    }
    else if(resizey&&(e->button()&Qt::LeftButton))
    {
        resizey=false;
    }

    if(Scalelt&&(e->button()&Qt::LeftButton))
    {
        Scalelt=false;
        int x=lastpoint.x();

        int xs=rectxl+rectw;
        int ys=rectyl+recth;


        int xn=x;
        if(x>=xs-20)
            xn=xs-20;

        double s=double(xs-xn)/scalew;
        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].scale(xs,ys,s);
                    if(lines[i].x1<lines[i].x2)
                        rectxl=lines[i].x1;
                    else
                        rectxl=lines[i].x2;
                    if(lines[i].y1<lines[i].y2)
                        rectyl=lines[i].y1;
                    else
                        rectyl=lines[i].y2;
                    rectw=xs-rectxl;
                    recth=ys-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {
                    polygons[i].scale(xs,ys,s);
                    Polygon apolygon=polygons[i];
                    int xmin,ymin;
                    xmin=apolygon.x[0];
                    ymin=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]<xmin)
                            xmin=apolygon.x[k];
                        if(apolygon.y[k]<ymin)
                            ymin=apolygon.y[k];
                    }
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=xs-rectxl;
                    recth=ys-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].scale(xs,ys,s);

                    Ellipse aellipse=ellipses[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }

                    rectxl=xmin+aellipse.x-1;
                    rectyl=ymin+aellipse.y-1;
                    rectw=xs-rectxl;
                    recth=ys-rectyl;
                    break;
                }

            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {
                    curves[i].scale(xs,ys,s);
                    Curve acurve=curves[i];
                    int xmin,ymin;
                    xmin=acurve.x[0];
                    ymin=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]<xmin)
                            xmin=acurve.x[k];
                        if(acurve.y[k]<ymin)
                            ymin=acurve.y[k];
                    }
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=xs-rectxl;
                    recth=ys-rectyl;
                    break;
                }
            }
        }

        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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
        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);

        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
        this->repaint();
    }
    else if(Scalert&&(e->button()&Qt::LeftButton))
    {
        Scalert=false;
        int x=lastpoint.x();

        int xs=rectxl;
        int ys=rectyl+recth;
        double m=-double(recth)/rectw;

        int xn=x;
        if(x<=xs+20)
            xn=xs+20;

        double s=double(xn-xs)/scalew;

        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].scale(xs,ys,s);
                    if(lines[i].x1<lines[i].x2)
                        rectw=lines[i].x2-lines[i].x1;
                    else
                        rectw=lines[i].x1-lines[i].x2;
                    if(lines[i].y1<lines[i].y2)
                        rectyl=lines[i].y1;
                    else
                        rectyl=lines[i].y2;
                    recth=ys-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {
                    polygons[i].scale(xs,ys,s);
                    Polygon apolygon=polygons[i];
                    int xmax,ymin;
                    xmax=apolygon.x[0];
                    ymin=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]>xmax)
                            xmax=apolygon.x[k];
                        if(apolygon.y[k]<ymin)
                            ymin=apolygon.y[k];
                    }
                    rectw=xmax-rectxl;
                    rectyl=ymin;
                    recth=ys-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].scale(xs,ys,s);

                    Ellipse aellipse=ellipses[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }


                    rectyl=ymin+aellipse.y-1;
                    rectw=abs(xmax+aellipse.x-rectxl)+2;
                    recth=ys-rectyl+1;
                    break;
                }

            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {
                    curves[i].scale(xs,ys,s);
                    Curve acurve=curves[i];
                    int xmax,ymin;
                    xmax=acurve.x[0];
                    ymin=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]>xmax)
                            xmax=acurve.x[k];
                        if(acurve.y[k]<ymin)
                            ymin=acurve.y[k];
                    }
                    rectw=xmax-rectxl;
                    rectyl=ymin;
                    recth=ys-rectyl;
                    break;
                }
            }
        }

        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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
        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);


        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
        this->repaint();
    }
    else if(Scalelb&&(e->button()&Qt::LeftButton))
    {
        Scalelb=false;
        int x=lastpoint.x();

        int xs=rectxl+rectw;
        int ys=rectyl;
        double m=-double(recth)/rectw;

        int xn=x;
        if(x>=xs-20)
            xn=xs-20;

        double s=double(xs-xn)/scalew;

        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {

                    lines[i].scale(xs,ys,s);
                    if(lines[i].x1<lines[i].x2)
                        rectxl=lines[i].x1;
                    else
                        rectxl=lines[i].x2;
                    if(lines[i].y1<lines[i].y2)
                        recth=lines[i].y2-lines[i].y1;
                    else
                        recth=lines[i].y1-lines[i].y2;
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {

                    polygons[i].scale(xs,ys,s);
                    Polygon apolygon=polygons[i];
                    int xmin,ymax;
                    xmin=apolygon.x[0];
                    ymax=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]<xmin)
                            xmin=apolygon.x[k];
                        if(apolygon.y[k]>ymax)
                            ymax=apolygon.y[k];
                    }
                    rectxl=xmin;
                    recth=ymax-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {

                    ellipses[i].scale(xs,ys,s);

                    Ellipse aellipse=ellipses[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }

                    rectxl=xmin+aellipse.x-1;
                    recth=abs(ymax+aellipse.y-rectyl)+2;
                    break;
                }

            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {

                    curves[i].scale(xs,ys,s);
                    Curve acurve=curves[i];
                    int xmin,ymax;
                    xmin=acurve.x[0];
                    ymax=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]<xmin)
                            xmin=acurve.x[k];
                        if(acurve.y[k]>ymax)
                            ymax=acurve.y[k];
                    }
                    rectxl=xmin;
                    recth=ymax-rectyl;
                    break;
                }
            }
        }

        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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
        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);
        rectw=xs-rectxl;

        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
        this->repaint();
    }
    else if(Scalerb&&(e->button()&Qt::LeftButton))
    {
        Scalerb=false;
        int x=lastpoint.x();


        int xs=rectxl;
        int ys=rectyl;
        double m=double(recth)/rectw;

        int xn=x;
        if(x<=xs+20)
            xn=xs+20;

        double s=double(xn-xs)/scalew;

        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {

                    lines[i].scale(xs,ys,s);
                    if(lines[i].x1<lines[i].x2)
                        rectw=lines[i].x2-lines[i].x1;
                    else
                        rectw=lines[i].x1-lines[i].x2;
                    if(lines[i].y1<lines[i].y2)
                        recth=lines[i].y2-lines[i].y1;
                    else
                        recth=lines[i].y1-lines[i].y2;
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {

                    polygons[i].scale(xs,ys,s);
                    Polygon apolygon=polygons[i];
                    int xmax,ymax;
                    xmax=apolygon.x[0];
                    ymax=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]>xmax)
                            xmax=apolygon.x[k];
                        if(apolygon.y[k]>ymax)
                            ymax=apolygon.y[k];
                    }
                    rectw=xmax-rectxl;
                    recth=ymax-rectyl;
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {

                    ellipses[i].scale(xs,ys,s);

                    Ellipse aellipse=ellipses[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }


                    rectw=abs(xmax+aellipse.x-rectxl)+2;
                    recth=abs(ymax+aellipse.y-rectyl)+2;
                    break;
                }

            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {

                    curves[i].scale(xs,ys,s);
                    Curve acurve=curves[i];
                    int xmax,ymax;
                    xmax=acurve.x[0];
                    ymax=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]>xmax)
                            xmax=acurve.x[k];
                        if(acurve.y[k]>ymax)
                            ymax=acurve.y[k];
                    }
                    rectw=xmax-rectxl;
                    recth=ymax-rectyl;
                    break;
                }
            }
        }

        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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
        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);


        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
        this->repaint();
    }
    else if(move&&(e->button()&Qt::LeftButton))
    {
        move=false;
        int x=lastpoint.x();
        int y=lastpoint.y();
        int dx=x-firstpoint.x();
        int dy=y-firstpoint.y();
        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1+=dx;
                    lines[i].y1+=dy;
                    lines[i].x2+=dx;
                    lines[i].y2+=dy;
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]+=dx;
                        polygons[i].y[j]+=dy;
                    }
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x+=dx;
                    ellipses[i].y+=dy;
                    break;
                }
            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]+=dx;
                        curves[i].y[j]+=dy;
                    }
                    break;
                }
            }
        }

        rectxl+=dx;
        rectyl+=dy;
        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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
        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
        this->repaint();
    }
    else if(Rotate&&(e->button()&Qt::LeftButton))
    {

        Rotate=false;
        int x=lastpoint.x();
        int y=lastpoint.y();
        int dx=x-xr;
        int dy=y-yr;
        int r;
        r=int(round(atan2(dx,-dy)*180/3.1415926));

        if(nowunit.type==1)
        {
            for(int i=0;i<lines.size();i++)
            {
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].rotate(xr,yr,r);
                    Line aline=lines[i];
                    if(aline.x1<aline.x2)
                        rectxl=aline.x1;
                    else
                        rectxl=aline.x2;
                    if(aline.y1<aline.y2)
                        rectyl=aline.y1;
                    else
                        rectyl=aline.y2;
                    rectw=abs(aline.x2-aline.x1);
                    recth=abs(aline.y2-aline.y1);
                    break;
                }
            }
        }
        else if(nowunit.type==2)
        {
            for(int i=0;i<polygons.size();i++)
            {
                if(polygons[i].ID==nowunit.ID)
                {
                    polygons[i].rotate(xr,yr,r);
                    Polygon apolygon=polygons[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=apolygon.x[0];
                    xmax=apolygon.x[0];
                    ymin=apolygon.y[0];
                    ymax=apolygon.y[0];
                    for(int k=1;k<apolygon.n;k++)
                    {
                        if(apolygon.x[k]<xmin)
                            xmin=apolygon.x[k];
                        if(apolygon.x[k]>xmax)
                            xmax=apolygon.x[k];
                        if(apolygon.y[k]<ymin)
                            ymin=apolygon.y[k];
                        if(apolygon.y[k]>ymax)
                            ymax=apolygon.y[k];
                    }
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=abs(xmax-xmin);
                    recth=abs(ymax-ymin);
                    break;
                }
            }
        }
        else if(nowunit.type==3)
        {
            for(int i=0;i<ellipses.size();i++)
            {
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].rotate(xr,yr,r);

                    Ellipse aellipse=ellipses[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=aellipse.res1[0].first;
                    xmax=aellipse.res1[0].first;
                    ymin=aellipse.res1[0].second;
                    ymax=aellipse.res1[0].second;
                    for(int k=1;k<aellipse.res1.size();k++)
                    {
                        if(aellipse.res1[k].first<xmin)
                            xmin=aellipse.res1[k].first;
                        if(aellipse.res1[k].first>xmax)
                            xmax=aellipse.res1[k].first;
                        if(aellipse.res1[k].second<ymin)
                            ymin=aellipse.res1[k].second;
                        if(aellipse.res1[k].second>ymax)
                            ymax=aellipse.res1[k].second;
                    }
                    for(int k=1;k<aellipse.res2.size();k++)
                    {
                        if(aellipse.res2[k].first<xmin)
                            xmin=aellipse.res2[k].first;
                        if(aellipse.res2[k].first>xmax)
                            xmax=aellipse.res2[k].first;
                        if(aellipse.res2[k].second<ymin)
                            ymin=aellipse.res2[k].second;
                        if(aellipse.res2[k].second>ymax)
                            ymax=aellipse.res2[k].second;
                    }
                    for(int k=1;k<aellipse.res3.size();k++)
                    {
                        if(aellipse.res3[k].first<xmin)
                            xmin=aellipse.res3[k].first;
                        if(aellipse.res3[k].first>xmax)
                            xmax=aellipse.res3[k].first;
                        if(aellipse.res3[k].second<ymin)
                            ymin=aellipse.res3[k].second;
                        if(aellipse.res3[k].second>ymax)
                            ymax=aellipse.res3[k].second;
                    }
                    for(int k=1;k<aellipse.res4.size();k++)
                    {
                        if(aellipse.res4[k].first<xmin)
                            xmin=aellipse.res4[k].first;
                        if(aellipse.res4[k].first>xmax)
                            xmax=aellipse.res4[k].first;
                        if(aellipse.res4[k].second<ymin)
                            ymin=aellipse.res4[k].second;
                        if(aellipse.res4[k].second>ymax)
                            ymax=aellipse.res4[k].second;
                    }

                    rectxl=xmin+aellipse.x-1;
                    rectyl=ymin+aellipse.y-1;
                    rectw=abs(xmax-xmin)+2;
                    recth=abs(ymax-ymin)+2;

                    break;
                }

            }
        }
        else if(nowunit.type==4)
        {
            for(int i=0;i<curves.size();i++)
            {
                if(curves[i].ID==nowunit.ID)
                {
                    curves[i].rotate(xr,yr,r);
                    Curve acurve=curves[i];
                    int xmin,xmax,ymin,ymax;
                    xmin=acurve.x[0];
                    xmax=acurve.x[0];
                    ymin=acurve.y[0];
                    ymax=acurve.y[0];
                    for(int k=1;k<acurve.n;k++)
                    {
                        if(acurve.x[k]<xmin)
                            xmin=acurve.x[k];
                        if(acurve.x[k]>xmax)
                            xmax=acurve.x[k];
                        if(acurve.y[k]<ymin)
                            ymin=acurve.y[k];
                        if(acurve.y[k]>ymax)
                            ymax=acurve.y[k];
                    }
                    rectxl=xmin;
                    rectyl=ymin;
                    rectw=abs(xmax-xmin);
                    recth=abs(ymax-ymin);
                    break;
                }
            }
        }

        int width=map->width();
        int height=map->height();
        map = new QPixmap(QSize(width,height));
        QColor backcolor=qRgb(255,255,255);
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

        QPainter painter(map);
        QPen pen;
        pen.setColor(QColor(0,170,255));
        pen.setWidth(1);
        painter.setPen(pen);

        painter.drawRect(rectxl,rectyl-interval,rectw,recth);
        painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
        painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
        painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
        painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
        painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));

        this->repaint();

    }
    else if(linedrawing&&(e->button()&Qt::LeftButton))
    {
        linedrawing=false;
        QColor clr=mypen->color();
        Pen pen(clr.red(),clr.green(),clr.blue());
        if(LineDDA)
        {
            Line aline(id,1,pen,firstpoint.x(),firstpoint.y(),lastpoint.x(),lastpoint.y(),"DDA");
            aline.penw=mypen->width();
            units.push_back(aline);
            lines.push_back(aline);
        }
        else
        {
            Line aline(id,1,pen,firstpoint.x(),firstpoint.y(),lastpoint.x(),lastpoint.y(),"Bresenham");
            aline.penw=mypen->width();
            units.push_back(aline);
            lines.push_back(aline);
        }
        id++;
        map->operator =(*tmpmap);
        tmpmap->~QPixmap();
        repaint();
    }

    else if(ellipsedrawing&&(e->button()&Qt::LeftButton))
    {
        ellipsedrawing=false;
        QColor clr=mypen->color();
        Pen pen(clr.red(),clr.green(),clr.blue());
        int x=(firstpoint.x()+lastpoint.x())/2;
        int y=(firstpoint.y()+lastpoint.y())/2;
        int rx=(abs(firstpoint.x()-lastpoint.x()))/2;
        int ry=(abs(firstpoint.y()-lastpoint.y()))/2;
        resellipse=ELLIPSE(rx,ry);
        Ellipse aellipse(id,3,pen,x,y,rx,ry,resellipse);
        aellipse.penw=mypen->width();
        units.push_back(aellipse);
        ellipses.push_back(aellipse);
        id++;
        map->operator =(*tmpmap);
        tmpmap->~QPixmap();
        repaint();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    int x=e->x();
    int y=e->y();
    int w=map->width();
    int h=map->height()+interval;


    if(x<w-5||x>w+5||y<h-5||y>h+5)
    {
        setCursor(Qt::ArrowCursor);
    }
    if(resizemap&&(e->buttons()&Qt::LeftButton))
    {
        linedraw=false;
        ellipsedraw=false;
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(x,y-interval));
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
    else if(resizex&&(e->buttons()&Qt::LeftButton))
    {
        linedraw=false;
        ellipsedraw=false;
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(x,h-interval));
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
    else if(resizey&&(e->buttons()&Qt::LeftButton))
    {
        linedraw=false;
        ellipsedraw=false;
        QColor backcolor=qRgb(255,255,255);
        map = new QPixmap(QSize(w,y-interval));
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

    if(Select)
    {
        if(x>=rectxl+rectw/2-rimagex&&x<=rectxl+rectw/2-rimagex+16&&y>=rectyl-26&&y<=rectyl-16)
        {
            setCursor(Qt::OpenHandCursor);
        }
        else if(x>=rectxl-5&&x<=rectxl+5&&y>=rectyl-5&&y<=rectyl+5)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if(x>=rectxl+rectw-5&&x<=rectxl+rectw+5&&y>=rectyl-5&&y<=rectyl+5)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if(x>=rectxl-5&&x<=rectxl+5&&y>=rectyl+recth-5&&y<=rectyl+recth+5)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if(x>=rectxl+rectw-5&&x<=rectxl+rectw+5&&y>=rectyl+recth-5&&y<=rectyl+recth+5)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
        if(Scalelt&&(e->buttons()&Qt::LeftButton))
        {

            vector<int> tmpx,tmpy;
            int tmprx,tmpry;
            int xs=rectxl+rectw;
            int ys=rectyl+recth;
            double m=double(recth)/rectw;

            int xn=x;
            if(x>=xs-20)
                xn=xs-20;

            double s=double(xs-xn)/scalew;

            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(lines[i].x1);
                        tmpx.push_back(lines[i].x2);
                        tmpy.push_back(lines[i].y1);
                        tmpy.push_back(lines[i].y2);
                        lines[i].scale(xs,ys,s);
                        if(lines[i].x1<lines[i].x2)
                            rectxl=lines[i].x1;
                        else
                            rectxl=lines[i].x2;
                        if(lines[i].y1<lines[i].y2)
                            rectyl=lines[i].y1;
                        else
                            rectyl=lines[i].y2;
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            tmpx.push_back(polygons[i].x[j]);
                            tmpy.push_back(polygons[i].y[j]);
                        }
                        polygons[i].scale(xs,ys,s);
                        Polygon apolygon=polygons[i];
                        int xmin,ymin;
                        xmin=apolygon.x[0];
                        ymin=apolygon.y[0];
                        for(int k=1;k<apolygon.n;k++)
                        {
                            if(apolygon.x[k]<xmin)
                                xmin=apolygon.x[k];
                            if(apolygon.y[k]<ymin)
                                ymin=apolygon.y[k];
                        }
                        rectxl=xmin;
                        rectyl=ymin;
                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(ellipses[i].x);
                        tmpy.push_back(ellipses[i].y);
                        tmprx=ellipses[i].rx;
                        tmpry=ellipses[i].ry;
                        ellipses[i].scale(xs,ys,s);

                        Ellipse aellipse=ellipses[i];
                        int xmin,xmax,ymin,ymax;
                        xmin=aellipse.res1[0].first;
                        xmax=aellipse.res1[0].first;
                        ymin=aellipse.res1[0].second;
                        ymax=aellipse.res1[0].second;
                        for(int k=1;k<aellipse.res1.size();k++)
                        {
                            if(aellipse.res1[k].first<xmin)
                                xmin=aellipse.res1[k].first;
                            if(aellipse.res1[k].first>xmax)
                                xmax=aellipse.res1[k].first;
                            if(aellipse.res1[k].second<ymin)
                                ymin=aellipse.res1[k].second;
                            if(aellipse.res1[k].second>ymax)
                                ymax=aellipse.res1[k].second;
                        }
                        for(int k=1;k<aellipse.res2.size();k++)
                        {
                            if(aellipse.res2[k].first<xmin)
                                xmin=aellipse.res2[k].first;
                            if(aellipse.res2[k].first>xmax)
                                xmax=aellipse.res2[k].first;
                            if(aellipse.res2[k].second<ymin)
                                ymin=aellipse.res2[k].second;
                            if(aellipse.res2[k].second>ymax)
                                ymax=aellipse.res2[k].second;
                        }
                        for(int k=1;k<aellipse.res3.size();k++)
                        {
                            if(aellipse.res3[k].first<xmin)
                                xmin=aellipse.res3[k].first;
                            if(aellipse.res3[k].first>xmax)
                                xmax=aellipse.res3[k].first;
                            if(aellipse.res3[k].second<ymin)
                                ymin=aellipse.res3[k].second;
                            if(aellipse.res3[k].second>ymax)
                                ymax=aellipse.res3[k].second;
                        }
                        for(int k=1;k<aellipse.res4.size();k++)
                        {
                            if(aellipse.res4[k].first<xmin)
                                xmin=aellipse.res4[k].first;
                            if(aellipse.res4[k].first>xmax)
                                xmax=aellipse.res4[k].first;
                            if(aellipse.res4[k].second<ymin)
                                ymin=aellipse.res4[k].second;
                            if(aellipse.res4[k].second>ymax)
                                ymax=aellipse.res4[k].second;
                        }

                        rectxl=xmin+aellipse.x-1;
                        rectyl=ymin+aellipse.y-1;
                        break;
                    }

                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            tmpx.push_back(curves[i].x[j]);
                            tmpy.push_back(curves[i].y[j]);
                        }
                        curves[i].scale(xs,ys,s);
                        Curve acurve=curves[i];
                        int xmin,ymin;
                        xmin=acurve.x[0];
                        ymin=acurve.y[0];
                        for(int k=1;k<acurve.n;k++)
                        {
                            if(acurve.x[k]<xmin)
                                xmin=acurve.x[k];
                            if(acurve.y[k]<ymin)
                                ymin=acurve.y[k];
                        }
                        rectxl=xmin;
                        rectyl=ymin;
                        break;
                    }
                }
            }

            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1=tmpx[0];
                    lines[i].y1=tmpy[0];
                    lines[i].x2=tmpx[1];
                    lines[i].y2=tmpy[1];
                    tmpx.clear();
                    tmpy.clear();
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x=tmpx[0];
                    ellipses[i].y=tmpy[0];
                    ellipses[i].rx=tmprx;
                    ellipses[i].ry=tmpry;
                    tmprx=0;
                    tmpry=0;
                    tmpx.clear();
                    tmpy.clear();

                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]=tmpx[j];
                        polygons[i].y[j]=tmpy[j];
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]=tmpx[j];
                        curves[i].y[j]=tmpy[j];
                    }
                }
            }
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            rectw=xs-rectxl;
            recth=ys-rectyl;

            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            this->repaint();

        }
        else if(Scalert&&(e->buttons()&Qt::LeftButton))
        {
            vector<int> tmpx,tmpy;
            int tmprx,tmpry;
            int xs=rectxl;
            int ys=rectyl+recth;
            double m=-double(recth)/rectw;

            int xn=x;
            if(x<=xs+20)
                xn=xs+20;

            double s=double(xn-xs)/scalew;

            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(lines[i].x1);
                        tmpx.push_back(lines[i].x2);
                        tmpy.push_back(lines[i].y1);
                        tmpy.push_back(lines[i].y2);
                        lines[i].scale(xs,ys,s);
                        if(lines[i].x1<lines[i].x2)
                            rectw=lines[i].x2-lines[i].x1;
                        else
                            rectw=lines[i].x1-lines[i].x2;
                        if(lines[i].y1<lines[i].y2)
                            rectyl=lines[i].y1;
                        else
                            rectyl=lines[i].y2;
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            tmpx.push_back(polygons[i].x[j]);
                            tmpy.push_back(polygons[i].y[j]);
                        }
                        polygons[i].scale(xs,ys,s);
                        Polygon apolygon=polygons[i];
                        int xmax,ymin;
                        xmax=apolygon.x[0];
                        ymin=apolygon.y[0];
                        for(int k=1;k<apolygon.n;k++)
                        {
                            if(apolygon.x[k]>xmax)
                                xmax=apolygon.x[k];
                            if(apolygon.y[k]<ymin)
                                ymin=apolygon.y[k];
                        }
                        rectw=xmax-rectxl;
                        rectyl=ymin;
                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(ellipses[i].x);
                        tmpy.push_back(ellipses[i].y);
                        tmprx=ellipses[i].rx;
                        tmpry=ellipses[i].ry;
                        ellipses[i].scale(xs,ys,s);

                        Ellipse aellipse=ellipses[i];
                        int xmin,xmax,ymin,ymax;
                        xmin=aellipse.res1[0].first;
                        xmax=aellipse.res1[0].first;
                        ymin=aellipse.res1[0].second;
                        ymax=aellipse.res1[0].second;
                        for(int k=1;k<aellipse.res1.size();k++)
                        {
                            if(aellipse.res1[k].first<xmin)
                                xmin=aellipse.res1[k].first;
                            if(aellipse.res1[k].first>xmax)
                                xmax=aellipse.res1[k].first;
                            if(aellipse.res1[k].second<ymin)
                                ymin=aellipse.res1[k].second;
                            if(aellipse.res1[k].second>ymax)
                                ymax=aellipse.res1[k].second;
                        }
                        for(int k=1;k<aellipse.res2.size();k++)
                        {
                            if(aellipse.res2[k].first<xmin)
                                xmin=aellipse.res2[k].first;
                            if(aellipse.res2[k].first>xmax)
                                xmax=aellipse.res2[k].first;
                            if(aellipse.res2[k].second<ymin)
                                ymin=aellipse.res2[k].second;
                            if(aellipse.res2[k].second>ymax)
                                ymax=aellipse.res2[k].second;
                        }
                        for(int k=1;k<aellipse.res3.size();k++)
                        {
                            if(aellipse.res3[k].first<xmin)
                                xmin=aellipse.res3[k].first;
                            if(aellipse.res3[k].first>xmax)
                                xmax=aellipse.res3[k].first;
                            if(aellipse.res3[k].second<ymin)
                                ymin=aellipse.res3[k].second;
                            if(aellipse.res3[k].second>ymax)
                                ymax=aellipse.res3[k].second;
                        }
                        for(int k=1;k<aellipse.res4.size();k++)
                        {
                            if(aellipse.res4[k].first<xmin)
                                xmin=aellipse.res4[k].first;
                            if(aellipse.res4[k].first>xmax)
                                xmax=aellipse.res4[k].first;
                            if(aellipse.res4[k].second<ymin)
                                ymin=aellipse.res4[k].second;
                            if(aellipse.res4[k].second>ymax)
                                ymax=aellipse.res4[k].second;
                        }


                        rectyl=ymin+aellipse.y-1;
                        rectw=abs(xmax+aellipse.x-rectxl)+2;
                        break;
                    }

                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            tmpx.push_back(curves[i].x[j]);
                            tmpy.push_back(curves[i].y[j]);
                        }
                        curves[i].scale(xs,ys,s);
                        Curve acurve=curves[i];
                        int xmax,ymin;
                        xmax=acurve.x[0];
                        ymin=acurve.y[0];
                        for(int k=1;k<acurve.n;k++)
                        {
                            if(acurve.x[k]>xmax)
                                xmax=acurve.x[k];
                            if(acurve.y[k]<ymin)
                                ymin=acurve.y[k];
                        }
                        rectw=xmax-rectxl;
                        rectyl=ymin;
                        break;
                    }
                }
            }

            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1=tmpx[0];
                    lines[i].y1=tmpy[0];
                    lines[i].x2=tmpx[1];
                    lines[i].y2=tmpy[1];
                    tmpx.clear();
                    tmpy.clear();
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x=tmpx[0];
                    ellipses[i].y=tmpy[0];
                    ellipses[i].rx=tmprx;
                    ellipses[i].ry=tmpry;
                    tmprx=0;
                    tmpry=0;
                    tmpx.clear();
                    tmpy.clear();

                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]=tmpx[j];
                        polygons[i].y[j]=tmpy[j];
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]=tmpx[j];
                        curves[i].y[j]=tmpy[j];
                    }
                }
            }
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            recth=ys-rectyl;

            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            this->repaint();
        }
        else if(Scalelb&&(e->buttons()&Qt::LeftButton))
        {
            vector<int> tmpx,tmpy;
            int tmprx,tmpry;
            int xs=rectxl+rectw;
            int ys=rectyl;

            int xn=x;
            if(x>=xs-20)
                xn=xs-20;

            double s=double(xs-xn)/scalew;

            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(lines[i].x1);
                        tmpx.push_back(lines[i].x2);
                        tmpy.push_back(lines[i].y1);
                        tmpy.push_back(lines[i].y2);
                        lines[i].scale(xs,ys,s);
                        if(lines[i].x1<lines[i].x2)
                            rectxl=lines[i].x1;
                        else
                            rectxl=lines[i].x2;
                        if(lines[i].y1<lines[i].y2)
                            recth=lines[i].y2-lines[i].y1;
                        else
                            recth=lines[i].y1-lines[i].y2;
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            tmpx.push_back(polygons[i].x[j]);
                            tmpy.push_back(polygons[i].y[j]);
                        }
                        polygons[i].scale(xs,ys,s);
                        Polygon apolygon=polygons[i];
                        int xmin,ymax;
                        xmin=apolygon.x[0];
                        ymax=apolygon.y[0];
                        for(int k=1;k<apolygon.n;k++)
                        {
                            if(apolygon.x[k]<xmin)
                                xmin=apolygon.x[k];
                            if(apolygon.y[k]>ymax)
                                ymax=apolygon.y[k];
                        }
                        rectxl=xmin;
                        recth=ymax-rectyl;
                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(ellipses[i].x);
                        tmpy.push_back(ellipses[i].y);
                        tmprx=ellipses[i].rx;
                        tmpry=ellipses[i].ry;
                        ellipses[i].scale(xs,ys,s);

                        Ellipse aellipse=ellipses[i];
                        int xmin,xmax,ymin,ymax;
                        xmin=aellipse.res1[0].first;
                        xmax=aellipse.res1[0].first;
                        ymin=aellipse.res1[0].second;
                        ymax=aellipse.res1[0].second;
                        for(int k=1;k<aellipse.res1.size();k++)
                        {
                            if(aellipse.res1[k].first<xmin)
                                xmin=aellipse.res1[k].first;
                            if(aellipse.res1[k].first>xmax)
                                xmax=aellipse.res1[k].first;
                            if(aellipse.res1[k].second<ymin)
                                ymin=aellipse.res1[k].second;
                            if(aellipse.res1[k].second>ymax)
                                ymax=aellipse.res1[k].second;
                        }
                        for(int k=1;k<aellipse.res2.size();k++)
                        {
                            if(aellipse.res2[k].first<xmin)
                                xmin=aellipse.res2[k].first;
                            if(aellipse.res2[k].first>xmax)
                                xmax=aellipse.res2[k].first;
                            if(aellipse.res2[k].second<ymin)
                                ymin=aellipse.res2[k].second;
                            if(aellipse.res2[k].second>ymax)
                                ymax=aellipse.res2[k].second;
                        }
                        for(int k=1;k<aellipse.res3.size();k++)
                        {
                            if(aellipse.res3[k].first<xmin)
                                xmin=aellipse.res3[k].first;
                            if(aellipse.res3[k].first>xmax)
                                xmax=aellipse.res3[k].first;
                            if(aellipse.res3[k].second<ymin)
                                ymin=aellipse.res3[k].second;
                            if(aellipse.res3[k].second>ymax)
                                ymax=aellipse.res3[k].second;
                        }
                        for(int k=1;k<aellipse.res4.size();k++)
                        {
                            if(aellipse.res4[k].first<xmin)
                                xmin=aellipse.res4[k].first;
                            if(aellipse.res4[k].first>xmax)
                                xmax=aellipse.res4[k].first;
                            if(aellipse.res4[k].second<ymin)
                                ymin=aellipse.res4[k].second;
                            if(aellipse.res4[k].second>ymax)
                                ymax=aellipse.res4[k].second;
                        }

                        rectxl=xmin+aellipse.x-1;
                        recth=abs(ymax+aellipse.y-rectyl)+2;
                        break;
                    }

                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            tmpx.push_back(curves[i].x[j]);
                            tmpy.push_back(curves[i].y[j]);
                        }
                        curves[i].scale(xs,ys,s);
                        Curve acurve=curves[i];
                        int xmin,ymax;
                        xmin=acurve.x[0];
                        ymax=acurve.y[0];
                        for(int k=1;k<acurve.n;k++)
                        {
                            if(acurve.x[k]<xmin)
                                xmin=acurve.x[k];
                            if(acurve.y[k]>ymax)
                                ymax=acurve.y[k];
                        }
                        rectxl=xmin;
                        recth=ymax-rectyl;
                        break;
                    }
                }
            }

            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1=tmpx[0];
                    lines[i].y1=tmpy[0];
                    lines[i].x2=tmpx[1];
                    lines[i].y2=tmpy[1];
                    tmpx.clear();
                    tmpy.clear();
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x=tmpx[0];
                    ellipses[i].y=tmpy[0];
                    ellipses[i].rx=tmprx;
                    ellipses[i].ry=tmpry;
                    tmprx=0;
                    tmpry=0;
                    tmpx.clear();
                    tmpy.clear();

                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]=tmpx[j];
                        polygons[i].y[j]=tmpy[j];
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]=tmpx[j];
                        curves[i].y[j]=tmpy[j];
                    }
                }
            }
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            rectw=xs-rectxl;

            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            this->repaint();
        }
        else if(Scalerb&&(e->buttons()&Qt::LeftButton))
        {
            vector<int> tmpx,tmpy;
            int tmprx,tmpry;
            int xs=rectxl;
            int ys=rectyl;
            double m=double(recth)/rectw;

            int xn=x;
            if(x<=xs+20)
                xn=xs+20;

            double s=double(xn-xs)/scalew;

            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(lines[i].x1);
                        tmpx.push_back(lines[i].x2);
                        tmpy.push_back(lines[i].y1);
                        tmpy.push_back(lines[i].y2);
                        lines[i].scale(xs,ys,s);
                        if(lines[i].x1<lines[i].x2)
                            rectw=lines[i].x2-lines[i].x1;
                        else
                            rectw=lines[i].x1-lines[i].x2;
                        if(lines[i].y1<lines[i].y2)
                            recth=lines[i].y2-lines[i].y1;
                        else
                            recth=lines[i].y1-lines[i].y2;
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            tmpx.push_back(polygons[i].x[j]);
                            tmpy.push_back(polygons[i].y[j]);
                        }
                        polygons[i].scale(xs,ys,s);
                        Polygon apolygon=polygons[i];
                        int xmax,ymax;
                        xmax=apolygon.x[0];
                        ymax=apolygon.y[0];
                        for(int k=1;k<apolygon.n;k++)
                        {
                            if(apolygon.x[k]>xmax)
                                xmax=apolygon.x[k];
                            if(apolygon.y[k]>ymax)
                                ymax=apolygon.y[k];
                        }
                        rectw=xmax-rectxl;
                        recth=ymax-rectyl;
                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(ellipses[i].x);
                        tmpy.push_back(ellipses[i].y);
                        tmprx=ellipses[i].rx;
                        tmpry=ellipses[i].ry;
                        ellipses[i].scale(xs,ys,s);

                        Ellipse aellipse=ellipses[i];
                        int xmin,xmax,ymin,ymax;
                        xmin=aellipse.res1[0].first;
                        xmax=aellipse.res1[0].first;
                        ymin=aellipse.res1[0].second;
                        ymax=aellipse.res1[0].second;
                        for(int k=1;k<aellipse.res1.size();k++)
                        {
                            if(aellipse.res1[k].first<xmin)
                                xmin=aellipse.res1[k].first;
                            if(aellipse.res1[k].first>xmax)
                                xmax=aellipse.res1[k].first;
                            if(aellipse.res1[k].second<ymin)
                                ymin=aellipse.res1[k].second;
                            if(aellipse.res1[k].second>ymax)
                                ymax=aellipse.res1[k].second;
                        }
                        for(int k=1;k<aellipse.res2.size();k++)
                        {
                            if(aellipse.res2[k].first<xmin)
                                xmin=aellipse.res2[k].first;
                            if(aellipse.res2[k].first>xmax)
                                xmax=aellipse.res2[k].first;
                            if(aellipse.res2[k].second<ymin)
                                ymin=aellipse.res2[k].second;
                            if(aellipse.res2[k].second>ymax)
                                ymax=aellipse.res2[k].second;
                        }
                        for(int k=1;k<aellipse.res3.size();k++)
                        {
                            if(aellipse.res3[k].first<xmin)
                                xmin=aellipse.res3[k].first;
                            if(aellipse.res3[k].first>xmax)
                                xmax=aellipse.res3[k].first;
                            if(aellipse.res3[k].second<ymin)
                                ymin=aellipse.res3[k].second;
                            if(aellipse.res3[k].second>ymax)
                                ymax=aellipse.res3[k].second;
                        }
                        for(int k=1;k<aellipse.res4.size();k++)
                        {
                            if(aellipse.res4[k].first<xmin)
                                xmin=aellipse.res4[k].first;
                            if(aellipse.res4[k].first>xmax)
                                xmax=aellipse.res4[k].first;
                            if(aellipse.res4[k].second<ymin)
                                ymin=aellipse.res4[k].second;
                            if(aellipse.res4[k].second>ymax)
                                ymax=aellipse.res4[k].second;
                        }

                        rectw=abs(xmax+aellipse.x-rectxl)+2;
                        recth=abs(ymax+aellipse.y-rectyl)+2;
                        break;
                    }

                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            tmpx.push_back(curves[i].x[j]);
                            tmpy.push_back(curves[i].y[j]);
                        }
                        curves[i].scale(xs,ys,s);
                        Curve acurve=curves[i];
                        int xmax,ymax;
                        xmax=acurve.x[0];
                        ymax=acurve.y[0];
                        for(int k=1;k<acurve.n;k++)
                        {
                            if(acurve.x[k]>xmax)
                                xmax=acurve.x[k];
                            if(acurve.y[k]>ymax)
                                ymax=acurve.y[k];
                        }
                        rectw=xmax-rectxl;
                        recth=ymax-rectyl;
                        break;
                    }
                }
            }

            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1=tmpx[0];
                    lines[i].y1=tmpy[0];
                    lines[i].x2=tmpx[1];
                    lines[i].y2=tmpy[1];
                    tmpx.clear();
                    tmpy.clear();
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x=tmpx[0];
                    ellipses[i].y=tmpy[0];
                    ellipses[i].rx=tmprx;
                    ellipses[i].ry=tmpry;
                    tmprx=0;
                    tmpry=0;
                    tmpx.clear();
                    tmpy.clear();

                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]=tmpx[j];
                        polygons[i].y[j]=tmpy[j];
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]=tmpx[j];
                        curves[i].y[j]=tmpy[j];
                    }
                }
            }
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);


            painter.drawRect(rectxl,rectyl-interval,rectw,recth);
            painter.drawRect(rectxl-5,rectyl-5-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl-5-interval,5,5);
            painter.drawRect(rectxl-5,rectyl+recth-interval,5,5);
            painter.drawRect(rectxl+rectw,rectyl+recth-interval,5,5);
            painter.drawImage(rectxl+rectw/2-rimagex,rectyl-26-interval,QImage("://image/rotate.png"));
            painter.drawLine(QPoint(rectxl+rectw/2,rectyl-interval),QPoint(rectxl+rectw/2,rectyl-10-interval));
            this->repaint();
        }
        else if(move&&(e->buttons()&Qt::LeftButton))
        {


            int dx=x-firstpoint.x();
            int dy=y-firstpoint.y();
            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        lines[i].x1+=dx;
                        lines[i].y1+=dy;
                        lines[i].x2+=dx;
                        lines[i].y2+=dy;
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            polygons[i].x[j]+=dx;
                            polygons[i].y[j]+=dy;
                        }
                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        ellipses[i].x+=dx;
                        ellipses[i].y+=dy;
                        break;
                    }
                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            curves[i].x[j]+=dx;
                            curves[i].y[j]+=dy;
                        }
                        break;
                    }
                }
            }


            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1-=dx;
                    lines[i].y1-=dy;
                    lines[i].x2-=dx;
                    lines[i].y2-=dy;
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x-=dx;
                    ellipses[i].y-=dy;
                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]-=dx;
                        polygons[i].y[j]-=dy;
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]-=dx;
                        curves[i].y[j]-=dy;
                    }
                }
            }
            QPainter painter(map);
            QPen pen;
            pen.setColor(QColor(0,170,255));
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(rectxl+dx,rectyl+dy-interval,rectw,recth);
            painter.drawRect(rectxl+dx-5,rectyl+dy-5-interval,5,5);
            painter.drawRect(rectxl+dx+rectw,rectyl+dy-5-interval,5,5);
            painter.drawRect(rectxl+dx-5,rectyl+recth+dy-interval,5,5);
            painter.drawRect(rectxl+dx+rectw,rectyl+recth+dy-interval,5,5);
            painter.drawImage(rectxl+dx+rectw/2-rimagex,rectyl+dy-26-interval,QImage("://image/rotate.png"));
            painter.drawLine(QPoint(rectxl+dx+rectw/2,rectyl+dy-interval),QPoint(rectxl+dx+rectw/2,rectyl+dy-10-interval));
            this->repaint();


        }
        else if(Rotate&&(e->buttons()&Qt::LeftButton))
        {
            setCursor(Qt::ClosedHandCursor);
            vector<int> tmpx,tmpy;
            int dx=x-xr;
            int dy=y-yr;
            int r;


            r=int(round(atan2(dx,-dy)*180/3.1415926));

            if(nowunit.type==1)
            {
                for(int i=0;i<lines.size();i++)
                {
                    if(lines[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(lines[i].x1);
                        tmpx.push_back(lines[i].x2);
                        tmpy.push_back(lines[i].y1);
                        tmpy.push_back(lines[i].y2);
                        lines[i].rotate(xr,yr,r);
                        break;
                    }
                }
            }
            else if(nowunit.type==2)
            {
                for(int i=0;i<polygons.size();i++)
                {
                    if(polygons[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<polygons[i].n;j++)
                        {
                            tmpx.push_back(polygons[i].x[j]);
                            tmpy.push_back(polygons[i].y[j]);
                        }
                        polygons[i].rotate(xr,yr,r);

                        break;
                    }
                }
            }
            else if(nowunit.type==3)
            {
                for(int i=0;i<ellipses.size();i++)
                {
                    if(ellipses[i].ID==nowunit.ID)
                    {
                        tmpx.push_back(ellipses[i].x);
                        tmpy.push_back(ellipses[i].y);
                        ellipses[i].rotate(xr,yr,r);
                        break;
                    }

                }
            }
            else if(nowunit.type==4)
            {
                for(int i=0;i<curves.size();i++)
                {
                    if(curves[i].ID==nowunit.ID)
                    {
                        for(int j=0;j<curves[i].n;j++)
                        {
                            tmpx.push_back(curves[i].x[j]);
                            tmpy.push_back(curves[i].y[j]);
                        }
                        curves[i].rotate(xr,yr,r);
                        break;
                    }
                }
            }

            int width=map->width();
            int height=map->height();
            map = new QPixmap(QSize(width,height));
            QColor backcolor=qRgb(255,255,255);
            map->fill(backcolor);
            for(int i=0;i<lines.size();i++)
            {
                drawLineOnMap(lines[i]);
                if(lines[i].ID==nowunit.ID)
                {
                    lines[i].x1=tmpx[0];
                    lines[i].y1=tmpy[0];
                    lines[i].x2=tmpx[1];
                    lines[i].y2=tmpy[1];
                    tmpx.clear();
                    tmpy.clear();
                }
            }
            for(int i=0;i<ellipses.size();i++)
            {
                drawEllipseOnMap(ellipses[i]);
                if(ellipses[i].ID==nowunit.ID)
                {
                    ellipses[i].x=tmpx[0];
                    ellipses[i].y=tmpy[0];
                    ellipses[i].r-=r;
                    tmpx.clear();
                    tmpy.clear();

                }
            }
            for(int i=0;i<polygons.size();i++)
            {
                drawPolygonOnMap(polygons[i]);
                if(polygons[i].ID==nowunit.ID)
                {
                    for(int j=0;j<polygons[i].n;j++)
                    {
                        polygons[i].x[j]=tmpx[j];
                        polygons[i].y[j]=tmpy[j];
                    }

                }
            }
            for(int i=0;i<curves.size();i++)
            {
                drawCurveOnMap(curves[i]);
                if(curves[i].ID==nowunit.ID)
                {
                    for(int j=0;j<curves[i].n;j++)
                    {
                        curves[i].x[j]=tmpx[j];
                        curves[i].y[j]=tmpy[j];
                    }
                }
            }


            this->repaint();

        }

    }

    else if(linedraw&&(e->buttons()&Qt::LeftButton)&&linedrawing)
    {
        if(x<w-5||x>w+5||y<h-5||y>h+5)
        {
            setCursor(Qt::ArrowCursor);
        }
        int x1=firstpoint.x();
        int y1=firstpoint.y();
        tmpmap = new QPixmap(*map);
        QPainter painter(tmpmap);
        painter.setPen(*mypen);
        if(LineDDA)
            resline=lineDDA(x1,y1,x,y);
        else
            resline=lineBresenham(x1,y1,x,y);



        for(int i=0;i<resline.size();i++)
        {
            painter.drawPoint(resline[i].first,resline[i].second-interval);

        }
        repaint();
    }
    else if(polygondraw&&polygondrawing){
        if(x<w-5||x>w+5||y<h-5||y>h+5)
        {
            setCursor(Qt::ArrowCursor);
        }
        int n=points.size();
        int x0=points[0].x();
        int y0=points[0].y();
        int xn=points[n-1].x();
        int yn=points[n-1].y();
        tmpmap = new QPixmap(*map);
        QPainter painter(tmpmap);
        painter.setPen(*mypen);
        if(polygonDDA)
        {
            resline=lineDDA(x0,y0,x,y);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
            resline=lineDDA(x,y,xn,yn);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
        }
        else
        {
            resline=lineBresenham(x0,y0,x,y);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
            resline=lineBresenham(x,y,xn,yn);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
        }
        repaint();
    }
    else if(ellipsedraw&&(e->buttons()&Qt::LeftButton)&&ellipsedrawing)
    {
        if(x<w-5||x>w+5||y<h-5||y>h+5)
        {
            setCursor(Qt::ArrowCursor);
        }
        int x1=firstpoint.x();
        int y1=firstpoint.y();
        tmpmap = new QPixmap(*map);
        QPainter painter(tmpmap);
        painter.setPen(*mypen);

        int xe=(x+x1)/2;
        int ye=(y+y1)/2;
        int rx=(abs(x-x1))/2;
        int ry=(abs(y-y1))/2;

        if(rx!=0&&ry!=0){
            resellipse=ELLIPSE(rx,ry);
            for(int i=0;i<resellipse.size();i++)
            {
                int x0=resellipse[i].first;
                int y0=resellipse[i].second;
                int x2,y2,x3,y3,x4,y4;
                x2=x0;
                y2=-y0;
                x3=-x0;
                y3=-y0;
                x4=-x0;
                y4=y0;

                x0+=xe;
                y0+=ye;
                x2+=xe;
                y2+=ye;
                x3+=xe;
                y3+=ye;
                x4+=xe;
                y4+=ye;
                painter.drawPoint(x0,y0-interval);
                painter.drawPoint(x2,y2-interval);
                painter.drawPoint(x3,y3-interval);
                painter.drawPoint(x4,y4-interval);
            }

        }
        repaint();
    }
    if(x>=w-5&&x<=w+5&&y>=h-5&&y<=h+5)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(x>=w-5&&x<=w+5)
    {
        setCursor(Qt::SizeHorCursor);
    }
    else if(y>=h-5&&y<=h+5)
    {
        setCursor(Qt::SizeVerCursor);
    }

}

void Widget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(polygondrawing){
        polygondrawing=false;
        int x=e->pos().x();
        int y=e->pos().y();
        int n=points.size();
        int x0=points[0].x();
        int y0=points[0].y();
        int xn=points[n-2].x();
        int yn=points[n-2].y();

        QPainter painter(map);
        painter.setPen(*mypen);
        if(polygonDDA)
        {
            resline=lineDDA(x0,y0,x,y);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
            resline=lineDDA(x,y,xn,yn);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
        }
        else
        {
            resline=lineBresenham(x0,y0,x,y);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
            resline=lineBresenham(x,y,xn,yn);
            for(int i=0;i<resline.size();i++)
            {
                painter.drawPoint(resline[i].first,resline[i].second-interval);

            }
        }
        int *xs;
        int *ys;
        xs=new int[n];
        ys=new int[n];
        for(int i=0;i<points.size();i++)
        {
            xs[i]=points[i].x();
            ys[i]=points[i].y();
        }
        QColor clr=mypen->color();
        Pen pen1(clr.red(),clr.green(),clr.blue());
        if(polygonDDA){
            Polygon apolygon(id,2,pen1,n,xs,ys,"DDA");
            apolygon.penw=mypen->width();
            units.push_back(apolygon);
            polygons.push_back(apolygon);
        }
        else
        {
            Polygon apolygon(id,2,pen1,n,xs,ys,"Bresenham");
            apolygon.penw=mypen->width();
            units.push_back(apolygon);
            polygons.push_back(apolygon);
        }
        id++;
        points.clear();
        repaint();
    }
    else if(curvedraw&&curvedrawing)
    {
        curvedrawing=false;
        tmpmap->~QPixmap();
//        QPen pen;
//        pen.setColor(Qt::black);
//        pen.setWidth(1);
        QPainter painter(map);
        painter.setPen(*mypen);
        int n=points.size();
        int *xs;
        int *ys;
        xs=new int[n];
        ys=new int[n];
        for(int i=0;i<points.size();i++)
        {
            xs[i]=points[i].x();
            ys[i]=points[i].y();
        }
        if(curveBezier)
        {
            curvepoint=CurveBezier(xs,ys,n);
            for(int i=1;i<curvepoint.size();i++)
            {
                resline=lineDDA(curvepoint[i-1].first,curvepoint[i-1].second,curvepoint[i].first,curvepoint[i].second);
                for(int j=0;j<resline.size();j++)
                {
                    int x=resline[j].first;
                    int y=resline[j].second;
                    painter.drawPoint(x,y-interval);
                }
            }
        }
        else
        {
            curvepoint=CurveBspline(xs,ys,n);
            for(int i=1;i<curvepoint.size();i++)
            {
                resline=lineDDA(curvepoint[i-1].first,curvepoint[i-1].second,curvepoint[i].first,curvepoint[i].second);
                for(int j=0;j<resline.size();j++)
                {
                    int x=resline[j].first;
                    int y=resline[j].second;
                    painter.drawPoint(x,y-interval);
                }
            }
        }
        QColor clr=mypen->color();
        Pen pen1(clr.red(),clr.green(),clr.blue());
        if(curveBezier){
            Curve acurve(id,4,pen1,n,xs,ys,"Bezier");
            acurve.penw=mypen->width();
            units.push_back(acurve);
            curves.push_back(acurve);
        }
        else
        {
            Curve acurve(id,4,pen1,n,xs,ys,"Bspline");
            acurve.penw=mypen->width();
            units.push_back(acurve);
            curves.push_back(acurve);
        }
        id++;
        points.clear();
        repaint();
    }
    else{
        return;
    }
}
