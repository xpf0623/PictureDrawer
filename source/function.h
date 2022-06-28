#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<cmath>
#include<vector>
#include"stdlib.h"
#include "mainwindow.h"
#include<math.h>
#define _USE_MATH_DEFINES
using namespace std;

vector<pair<int,int>> lineDDA(int x1,int y1,int x2,int y2);
vector<pair<int,int>> lineBresenham(int x1,int y1,int x2,int y2);
vector<pair<int,int>> ELLIPSE(int rx,int ry);
float Bazierpoint(int p[],double t,int n);
vector<pair<int,int>> CurveBezier(int x[],int y[],int n);
vector<pair<int,int>> CurveBspline(int x[],int y[],int n);


struct RGB
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

typedef struct                       /**** BMP file header structure ****/
{
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} MyBITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} MyBITMAPINFOHEADER;

/*typedef struct tagRGBQUAD
{
    BYTE rgbBlue;//蓝色的亮度（值范围为0-255)
    BYTE rgbGreen;//绿色的亮度（值范围为0-255)
    BYTE rgbRed;//红色的亮度（值范围为0-255)
    BYTE rgbReserved;//保留，必须为0
}RGBQUAD;*/


class Pen{
private:
    int R,G,B;
public:
    Pen(){R=255;G=255;B=255;}
    Pen(int r,int g,int b){R=r;G=g;B=b;}
    void setColor(int r,int g,int b);
    int rvalue();
    int gvalue();
    int bvalue();
};

class Unit{
public:
    int ID;
    int type;
    Pen clr;
    int penw;
    Unit(){ID=-1;type=0;penw=1;}
    Unit(int id,int Type,Pen pen,int penW):clr(pen)
    {ID=id;type=Type;penw=penW;}

    void rotate(int x,int y,int r);
};

class Line:public Unit{
public:
    int x1,y1,x2,y2;
    string algo;
    Line():Unit(){x1=x2=y1=y2=0;}
    Line(int id,int Type,Pen pen,int X1,int Y1,int X2,int Y2,string algorithm):Unit(id,Type,pen,1)
    {x1=X1;x2=X2;y1=Y1;y2=Y2;algo=algorithm;}
    int clipCohen(int X1,int Y1,int X2,int Y2);
    int clipLiang(int p,int q,double *u1,double *u2);
    void translate(int dx,int dy);
    void scale(int x,int y,float s);
    void rotate(int x,int y,int r);

};

class Polygon:public Unit{
public:
    int n;
    int *x,*y;
    string algo;
    Polygon():Unit(){n=0;}
    Polygon(int id,int Type,Pen pen,int N,int X[],int Y[],string algorithm):Unit(id,Type,pen,1)
    {n=N;x=new int[N];y=new int[N];for(int i=0;i<n;i++){x[i]=X[i];y[i]=Y[i];}algo=algorithm;}
    void translate(int dx,int dy);
    void rotate(int x,int y,int r);
    void scale(int x,int y,float s);
};

class Ellipse:public Unit{
public:
    int x,y,rx,ry;
    vector<pair<int,int>> res1;
    vector<pair<int,int>> res2;
    vector<pair<int,int>> res3;
    vector<pair<int,int>> res4;
    int r;
    vector<int> xr;
    vector<int> yr;
    Ellipse():Unit(){x=y=rx=ry=0;r=0;}
    Ellipse(int id,int Type,Pen pen,int X,int Y,int RX,int RY,vector<pair<int,int>> RES):Unit(id,Type,pen,1)
    {x=X;y=Y;rx=RX;ry=RY;res1=RES;r=0;}
    void changeres1(vector<pair<int,int>> res)
    {
        res1.clear();
        res1=res;
    }
    void fill()
    {
        res2.clear();
        res3.clear();
        res4.clear();
        for(int i=0;i<res1.size();i++)
        {
            int x2,y2,x3,y3,x4,y4;
            x2=res1[i].first;
            y2=-res1[i].second;
            x3=-res1[i].first;
            y3=-res1[i].second;
            x4=-res1[i].first;
            y4=res1[i].second;
            res2.push_back(make_pair(x2,y2));
            res3.push_back(make_pair(x3,y3));
            res4.push_back(make_pair(x4,y4));
        }
    }
    void translate(int dx,int dy);
    void rotate(int x,int y,int r);
    void scale(int x,int y,float s);
};

class Curve:public Unit{
public:
    int n;
    int *x,*y;
    string algo;
    Curve():Unit(){n=0;}
    Curve(int id,int Type,Pen pen,int N,int X[],int Y[],string algorithm):Unit(id,Type,pen,1)
    {n=N;x=new int[N];y=new int[N];for(int i=0;i<n;i++){x[i]=X[i];y[i]=Y[i];}algo=algorithm;}
    void translate(int dx,int dy);
    void rotate(int x,int y,int r);
    void scale(int x,int y,float s);
};

void Ellipserotate(Ellipse &ellipse,int r);

class Canvas{
private:
    vector<Unit> units;
    vector<Line> lines;
    vector<Polygon> polygons;
    vector<Ellipse> ellipses;
    vector<Curve> curves;
    RGB *rgb;
    int w,h;
public:
    void resetCanvas(int width,int height);
    void saveCanvas(string name,char *savefile);
    void drawLine(int id,int x1,int y1,int x2,int y2,string algorithm,Pen pen);

    void drawPolygon(int id,int n,string algorithm,int x[],int y[],Pen pen);
    void drawEllipse(int id,int x,int y,int rx,int ry,Pen pen);
    void drawellipse(Ellipse ellipse);
    void drawCurve(int id,int n,string algorithm,int x[],int y[],Pen pen);
    void translate(int id,int dx,int dy);
    void rotate(int id,int x,int y,int r);
    void scale(int id,int x,int y,float s);
    void clip(int id,int x1,int y1,int x2,int y2,string algorithm);
    void clipCohenDDA(int x1,int y1,int x2,int y2,int X1,int Y1,int X2,int Y2,Pen pen);
    void clipCohenBresenham(int x1,int y1,int x2,int y2,int X1,int Y1,int X2,int Y2,Pen pen);

};





#endif // FUNCTION_H
