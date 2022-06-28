#include "mainwindow.h"
#include "ui_mainwindow.h"

Canvas canvas;
Pen pen(0,0,0);

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QColor backcolor=qRgb(255,255,255);
    map=new QPixmap(200,200);
    map->fill(backcolor);
    setMouseTracking(true);

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void textoperation(char *filename, char *savefile)
{
    string file=filename;
    ifstream in;
    in.open(file.c_str(),ios::in);
    while(!in.eof()){
        char *str=new char[256];
        in.getline(str,256);
        //string str1=str;
        //cout<<str1<<endl;
        string s=str;

        vector<string> order=split(s,' ');


        if(order[0]=="resetCanvas"){
            int w,h;
            w=atoi(order[1].c_str());
            h=atoi(order[2].c_str());
            canvas.resetCanvas(w,h);
        }
        else if(order[0]=="saveCanvas"){
            canvas.saveCanvas(order[1],savefile);
        }
        else if(order[0]=="setColor"){
            int r,g,b;
            r=atoi(order[1].c_str());
            g=atoi(order[2].c_str());
            b=atoi(order[3].c_str());
            pen.setColor(r,g,b);
        }
        else if(order[0]=="drawLine"){
            int id,x1,x2,y1,y2;
            id=atoi(order[1].c_str());
            x1=atoi(order[2].c_str());
            y1=atoi(order[3].c_str());
            x2=atoi(order[4].c_str());
            y2=atoi(order[5].c_str());
            canvas.drawLine(id,x1,y1,x2,y2,order[6],pen);
        }
        else if(order[0]=="drawPolygon"){
            int id,n;
            id=atoi(order[1].c_str());
            n=atoi(order[2].c_str());
            char *xy=new char[256];
            in.getline(xy,256);
            string XY=xy;
            vector<string> points=split(XY,' ');
            int x[100],y[100];
            for(int i=0;i<100;i++)
            {
                x[i]=-1;
                y[i]=-1;
            }
            int k=0;
            for(int i=0;i<points.size();i++)
            {
                if(i%2==0){
                    x[k]=atoi(points[i].c_str());
                }
                else{
                    y[k]=atoi(points[i].c_str());
                    k++;
                }
            }
            canvas.drawPolygon(id,n,order[3],x,y,pen);
        }
        else if(order[0]=="drawEllipse"){
            int id,x,y,rx,ry;
            id=atoi(order[1].c_str());
            x=atoi(order[2].c_str());
            y=atoi(order[3].c_str());
            rx=atoi(order[4].c_str());
            ry=atoi(order[5].c_str());
            canvas.drawEllipse(id,x,y,rx,ry,pen);
        }
        else if(order[0]=="drawCurve"){
            int id,n;
            id=atoi(order[1].c_str());
            n=atoi(order[2].c_str());
            char *xy=new char[256];
            in.getline(xy,256);
            string XY=xy;
            vector<string> points=split(XY,' ');
            int x[100],y[100];
            for(int i=0;i<100;i++)
            {
                x[i]=-1;
                y[i]=-1;
            }
            int k=0;
            for(int i=0;i<points.size();i++)
            {
                if(i%2==0){
                    x[k]=atoi(points[i].c_str());
                }
                else{
                    y[k]=atoi(points[i].c_str());
                    k++;
                }
            }
            canvas.drawCurve(id,n,order[3],x,y,pen);
        }
        else if(order[0]=="translate"){
            int id,dx,dy;
            id=atoi(order[1].c_str());
            dx=atoi(order[2].c_str());
            dy=atoi(order[3].c_str());
            canvas.translate(id,dx,dy);
        }
        else if(order[0]=="rotate"){
            int id,x,y,r;
            id=atoi(order[1].c_str());
            x=atoi(order[2].c_str());
            y=atoi(order[3].c_str());
            r=atoi(order[4].c_str());
            canvas.rotate(id,x,y,r);
        }
        else if(order[0]=="scale"){
            int id,x,y;
            float s;
            id=atoi(order[1].c_str());
            x=atoi(order[2].c_str());
            y=atoi(order[3].c_str());
            s=atof(order[4].c_str());
            canvas.scale(id,x,y,s);
        }
        else if(order[0]=="clip"){
            int id,x1,y1,x2,y2;
            id=atoi(order[1].c_str());
            x1=atoi(order[2].c_str());
            y1=atoi(order[3].c_str());
            x2=atoi(order[4].c_str());
            y2=atoi(order[5].c_str());
            canvas.clip(id,x1,y1,x2,y2,order[6]);
        }
        else
        {
            continue;
        }




    }
    in.close();
}



void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,200,200,*map);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    firstpoint=e->pos();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    lastpoint=e->pos();
    QPen pen;
    pen.setColor(Qt::red);
    QPainter painter(map);

}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int x=e->x();
    int y=e->y();
    int w=map->width();
    int h=map->height();
    if(x>=w-5&&x<=w+5&&y>=h-5&&y<=h+5)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}
