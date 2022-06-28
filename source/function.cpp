#include "function.h"


void Canvas::resetCanvas(int width,int height)
{
    delete[] rgb;
    //cout<<width<<" "<<height<<endl;
    w=width;
    h=height;
    rgb=new RGB[width*height];
    int i;
    for(i=0;i<width*height;i++)
    {
        rgb[i].r=255;
        rgb[i].g=255;
        rgb[i].b=255;

    }
    units.clear();
    lines.clear();
    polygons.clear();
    ellipses.clear();
    curves.clear();
    //cout<<i<<endl;
    //cout<<int(rgb[width*height-1].r)<<endl;
}

void Canvas::saveCanvas(string name,char *savefile)
{
    string File=savefile;
    File+=name;
    File+=".bmp";
    char *filename=new char[100];
    strcpy(filename,File.c_str());
    MyBITMAPFILEHEADER bfh;
    MyBITMAPINFOHEADER bih;      /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(MyBITMAPFILEHEADER) + sizeof(MyBITMAPINFOHEADER)+w*h*3;
    bfh.bfOffBits = 0x36;
    bih.biSize = sizeof(MyBITMAPINFOHEADER);
    bih.biWidth = w;
    bih.biHeight = h;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Could not write file\n");
        return;
    }
    /*Write headers*/
    fwrite(&bfType,sizeof(bfType),1,file);
    fwrite(&bfh,sizeof(bfh),1, file);
    fwrite(&bih,sizeof(bih),1, file);
    for(int i=h-1;i>=0;i--)
    {
        for(int j=0;j<w;j++)
        {
            RGB rgb1;
            rgb1.r=rgb[i*w+j].r;
            rgb1.g=rgb[i*w+j].g;
            rgb1.b=rgb[i*w+j].b;
            fwrite(&rgb1,sizeof(rgb1),1,file);
        }
    }


    fclose(file);

}

void Pen::setColor(int r,int g,int b)
{
    R=r;
    G=g;
    B=b;
}

int Pen::rvalue()
{
    return R;
}

int Pen::gvalue()
{
    return G;
}

int Pen::bvalue()
{
    return B;
}

vector<pair<int,int>> lineDDA(int x1, int y1, int x2, int y2)
{
    vector<pair<int,int>> res;
    if(x1-x2==0)
    {
        if(y1<y2)
        {
            for(int i=y1;i<=y2;i++)
            {
                res.push_back(make_pair(x1,i));
            }
        }
        else
        {
            for(int i=y2;i<=y1;i++)
            {
                res.push_back(make_pair(x1,i));
            }
        }
        return res;
    }
    if(y1-y2==0)
    {
        if(x1<x2)
        {
            for(int i=x1;i<=x2;i++)
            {
                res.push_back(make_pair(i,y1));
            }
        }
        else
        {
            for(int i=x2;i<=x1;i++)
            {
                res.push_back(make_pair(i,y1));
            }
        }
        return res;
    }
    double m;
    m=double(y2-y1)/(x2-x1);
    //cout<<m<<endl;

    if(m<=1&&m>=-1){
        if(x1<x2)
        {
            int n=x2-x1+1;
            double *y=new double[n];
            y[0]=y1;
            for(int i=1;i<x2-x1;i++){
                y[i]=y[i-1]+m;
            }
            y[x2-x1]=y2;
            int k=0;
            for(int i=x1;i<=x2;i++)
            {
                res.push_back(make_pair(i,int(round(y[k]))));
                k++;
            }

        }
        else
        {
            int n=x1-x2+1;
            double *y=new double[n];
            y[0]=y1;
            for(int i=1;i<x1-x2;i++){
                y[i]=y[i-1]-m;
            }
            y[x1-x2]=y2;
            int k=0;
            for(int i=x1;i>=x2;i--)
            {
                res.push_back(make_pair(i,int(round(y[k]))));
                k++;
            }

        }
    }
    else{
        if(y1<y2)
        {
            int n=y2-y1+1;
            double *x=new double[n];
            x[0]=x1;
            for(int i=1;i<y2-y1;i++){
                x[i]=x[i-1]+1/m;
            }
            x[y2-y1]=x2;
            int k=0;
            for(int i=y1;i<=y2;i++)
            {
                res.push_back(make_pair(int(round(x[k])),i));
                k++;
            }
        }
        else
        {
            int n=y1-y2+1;
            double *x=new double[n];
            x[0]=x1;
            for(int i=1;i<y1-y2;i++){
                x[i]=x[i-1]-1/m;
            }
            x[y1-y2]=x2;
            int k=0;
            for(int i=y1;i>=y2;i--)
            {
                res.push_back(make_pair(int(round(x[k])),i));
                k++;
            }
        }
    }
    return res;
}

vector<pair<int,int>> lineBresenham(int x1, int y1, int x2, int y2)
{
    vector<pair<int,int>> res;
    if(x1-x2==0)
    {
        if(y1<y2)
        {
            for(int i=y1;i<=y2;i++)
            {
                res.push_back(make_pair(x1,i));
            }
        }
        else
        {
            for(int i=y2;i<=y1;i++)
            {
                res.push_back(make_pair(x1,i));
            }
        }
        return res;
    }
    if(y1-y2==0)
    {
        if(x1<x2)
        {
            for(int i=x1;i<=x2;i++)
            {
                res.push_back(make_pair(i,y1));
            }
        }
        else
        {
            for(int i=x2;i<=x1;i++)
            {
                res.push_back(make_pair(i,y1));
            }
        }
        return res;
    }
    double m,b;
    m=double(y2-y1)/(x2-x1);
    b=y1-m*x1;
    int dx,dy;
    dx=abs(x2-x1);
    dy=abs(y2-y1);
    int p[1000];

    if(m<=1&&m>=-1){
        if(x1>x2){
            int tmpx,tmpy;
            tmpx=x1;
            x1=x2;
            x2=tmpx;
            tmpy=y1;
            y1=y2;
            y2=tmpy;
        }
        p[0]=2*dy-dx;
        int n=x2-x1+1;
        int *y=new int[n];
        y[0]=y1;
        int r;
        if(m>0)
            r=1;
        else
            r=-1;
        int k=0;
        for(int i=x1+1;i<=x2;i++)
        {
            if(p[k]<0){
                y[k+1]=y[k];
                p[k+1]=p[k]+2*dy;
            }
            else
            {
                y[k+1]=y[k]+r;
                p[k+1]=p[k]+2*(dy-dx);
            }
            k++;
        }
        y[x2-x1]=y2;
        for(int i=x1;i<=x2;i++)
        {
            res.push_back(make_pair(i,y[i-x1]));
        }
    }
    else
    {
        if(y1>y2){
            int tmpx,tmpy;
            tmpx=x1;
            x1=x2;
            x2=tmpx;
            tmpy=y1;
            y1=y2;
            y2=tmpy;
        }
        p[0]=2*dx-dy;
        int n=y2-y1+1;
        int *x=new int[n];
        x[0]=x1;
        int r;
        if(m>0)
            r=1;
        else
            r=-1;
        int k=0;
        for(int i=y1+1;i<=y2;i++)
        {
            if(p[k]<0){
                x[k+1]=x[k];
                p[k+1]=p[k]+2*dx;
            }
            else
            {
                x[k+1]=x[k]+r;
                p[k+1]=p[k]+2*(dx-dy);
            }
            k++;
        }
        x[y2-y1]=x2;
        for(int i=y1;i<=y2;i++)
        {
            res.push_back(make_pair(x[i-y1],i));
        }
    }
    return res;
}

vector<pair<int,int>> ELLIPSE(int rx,int ry)
{
    vector<pair<int,int>> res;
    double p1=ry*ry-rx*rx*ry+rx*rx/4;
    int x1[10000]={0};
    int y1[10000]={0};
    x1[0]=0;
    y1[0]=ry;
    int i=1;
    for(;;i++)
    {
        x1[i]=x1[i-1]+1;
        if(p1<0)
        {
            y1[i]=y1[i-1];
            p1=p1+2*ry*ry*x1[i-1]+3*ry*ry;
        }
        else
        {
            y1[i]=y1[i-1]-1;
            p1=p1+2*ry*ry*x1[i-1]-2*rx*rx*y1[i-1]+2*rx*rx+3*ry*ry;
        }
        if(ry*ry*x1[i]>=rx*rx*y1[i])
            break;
    }
    double p2=ry*ry*(x1[i]+1/2)*(x1[i]+1/2)+rx*rx*(y1[i]-1)*(y1[i]-1)-rx*rx*ry*ry;
    while(y1[i]!=0){
        i++;
        y1[i]=y1[i-1]-1;
        if(p2<=0){
            p2=p2+2*ry*ry*x1[i-1]-2*rx*rx*y1[i-1]+2*ry*ry+3*rx*rx;
            x1[i]=x1[i-1]+1;
        }
        else{
            p2=p2-2*rx*rx*y1[i-1]+3*rx*rx;
            x1[i]=x1[i-1];
        }
    }
    int len=i+1;

    for(int k=0;k<len;k++)
    {

        res.push_back(make_pair(x1[k],y1[k]));
    }
    return res;
}

float Bazierpoint(int p[],double t,int n)
{
    float *p1=new float[n];
    for(int i=0;i<n;i++)
        p1[i]=float(p[i]);
    int num=n-1;
    for(int i=0;i<num;i++)
    {
        for(int j=0;j<num-i;j++)
        {
            p1[j]=(1.0-t)*p1[j]+t*p1[j+1];
        }
    }
    return p1[0];
}

vector<pair<int,int>> CurveBezier(int x[],int y[],int n){
    vector<pair<int,int>> res;
    float delta=1.0/2000;
    double t=0.0;
    int xn,yn;
    xn=x[0];
    yn=y[0];
    res.push_back(make_pair(xn,yn));
    for(int i=0;i<2000;i++)
    {
        float x1=Bazierpoint(x,t,n);
        float y1=Bazierpoint(y,t,n);
        t+=delta;
        xn=int(round(x1));
        yn=int(round(y1));
        res.push_back(make_pair(xn,yn));
    }
    return res;
}

vector<pair<int,int>> CurveBspline(int x[],int y[],int n){//3次
    vector<pair<int,int>> res;
    float *T=new float[n+4];
    for(int i=0;i<n+4;i++)
        T[i]=i;
    float **pointsx=new float *[n+3];
    float **pointsy=new float *[n+3];
    for(int i=0;i<n+3;i++)
    {
        pointsx[i]=new float[4];
        pointsy[i]=new float[4];
    }
    int xn,yn;
    for(int i=3;i<n;i++)
    {
        float delta=(T[i+1]-T[i])/100;
        for(float t=T[i];t<=T[i+1];t+=delta)
        {
            for(int k=0;k<=3;k++)
            {
                for(int j=i-3+k;j<=i;j++)
                {
                    if(k==0)
                    {
                        pointsx[j][k]=x[j];
                        pointsy[j][k]=y[j];

                    }
                    else
                    {
                        float alpha = (t-T[j]) / (T[j+4-k]-T[j]);
                        pointsx[j][k]=(1.0-alpha)*pointsx[j-1][k-1]+alpha*pointsx[j][k-1];
                        pointsy[j][k]=(1.0-alpha)*pointsy[j-1][k-1]+alpha*pointsy[j][k-1];
                        if(j==i&&k==3)
                        {

                            xn=int(round(pointsx[j][3]));
                            yn=int(round(pointsy[j][3]));
                            res.push_back(make_pair(xn,yn));

                        }
                    }
                }
            }
        }
    }
    return res;
}

void Canvas::drawLine(int id,int x1,int y1,int x2,int y2,string algorithm,Pen pen)
{
    vector<pair<int,int>> res;
    int R,G,B;
    R=pen.rvalue();
    G=pen.gvalue();
    B=pen.bvalue();
    if(algorithm=="DDA")
    { 
        res=lineDDA(x1,y1,x2,y2);
    }
    else
    {
        res=lineBresenham(x1,y1,x2,y2);
    }
    for(int i=0;i<res.size();i++)
    {
        int x=res[i].first;
        int y=res[i].second;
        rgb[w*y+x].r=R;
        rgb[w*y+x].g=G;
        rgb[w*y+x].b=B;
    }
    if(x1>x2)
    {
        int tmp1=x1;
        x1=x2;
        x2=tmp1;
        int tmp2=y1;
        y1=y2;
        y2=tmp2;
    }
    Line line(id,1,pen,x1,y1,x2,y2,algorithm);
    units.push_back(line);
    lines.push_back(line);
}

void Canvas::drawPolygon(int id,int n,string algorithm,int x[],int y[],Pen pen)
{
    vector<pair<int,int>> res;
    int R,G,B;
    R=pen.rvalue();
    G=pen.gvalue();
    B=pen.bvalue();
    if(algorithm=="DDA")
    {
        for(int i=0;i<n;i++)
        {
            if(i==n-1){
                res=lineDDA(x[i],y[i],x[0],y[0]);
            }
            else
            {
                res=lineDDA(x[i],y[i],x[i+1],y[i+1]);
            }
            for(int j=0;j<res.size();j++)
            {
                int x=res[j].first;
                int y=res[j].second;
                rgb[w*y+x].r=R;
                rgb[w*y+x].g=G;
                rgb[w*y+x].b=B;
            }
        }
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            if(i==n-1){
                res=lineBresenham(x[i],y[i],x[0],y[0]);
            }
            else
            {
                res=lineBresenham(x[i],y[i],x[i+1],y[i+1]);
            }
            for(int j=0;j<res.size();j++)
            {
                int x=res[j].first;
                int y=res[j].second;
                rgb[w*y+x].r=R;
                rgb[w*y+x].g=G;
                rgb[w*y+x].b=B;
            }
        }
    }
    Polygon polygon(id,2,pen,n,x,y,algorithm);
    units.push_back(polygon);
    polygons.push_back(polygon);
}

void Canvas::drawEllipse(int id,int x,int y,int rx,int ry,Pen pen)
{
    vector<pair<int,int>> res;
    int R,G,B;
    R=pen.rvalue();
    G=pen.gvalue();
    B=pen.bvalue();
    res=ELLIPSE(rx,ry);
    for(int i=0;i<res.size();i++)
    {
        int x1=res[i].first;
        int y1=res[i].second;
        int x2=x1;
        int y2=-y1;
        int x3=-x1;
        int y3=-y1;
        int x4=-x1;
        int y4=y1;
        x1+=x;
        x2+=x;
        x3+=x;
        x4+=x;
        y1+=y;
        y2+=y;
        y3+=y;
        y4+=y;

        rgb[w*y1+x1].r=R;
        rgb[w*y1+x1].g=G;
        rgb[w*y1+x1].b=B;

        rgb[w*y2+x2].r=R;
        rgb[w*y2+x2].g=G;
        rgb[w*y2+x2].b=B;

        rgb[w*y2+x3].r=R;
        rgb[w*y2+x3].g=G;
        rgb[w*y2+x3].b=B;

        rgb[w*y4+x4].r=R;
        rgb[w*y4+x4].g=G;
        rgb[w*y4+x4].b=B;
    }
    Ellipse ellipse(id,3,pen,x,y,rx,ry,res);
    ellipse.fill();
    units.push_back(ellipse);
    ellipses.push_back(ellipse);
}

void Ellipserotate(Ellipse &ellipse,int r)
{
    double r1=double(r)*3.1415926/180;

    for(int i=0;i<ellipse.res1.size();i++)
    {

        double tmpx1,tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4;
        tmpx1=double(ellipse.res1[i].first)*cos(r1)-double(ellipse.res1[i].second)*sin(r1);
        tmpy1=double(ellipse.res1[i].first)*sin(r1)+double(ellipse.res1[i].second)*cos(r1);
        tmpx2=double(ellipse.res2[i].first)*cos(r1)-double(ellipse.res2[i].second)*sin(r1);
        tmpy2=double(ellipse.res2[i].first)*sin(r1)+double(ellipse.res2[i].second)*cos(r1);
        tmpx3=double(ellipse.res3[i].first)*cos(r1)-double(ellipse.res3[i].second)*sin(r1);
        tmpy3=double(ellipse.res3[i].first)*sin(r1)+double(ellipse.res3[i].second)*cos(r1);
        tmpx4=double(ellipse.res4[i].first)*cos(r1)-double(ellipse.res4[i].second)*sin(r1);
        tmpy4=double(ellipse.res4[i].first)*sin(r1)+double(ellipse.res4[i].second)*cos(r1);
        ellipse.res1[i].first=int(round(tmpx1));
        ellipse.res1[i].second=int(round(tmpy1));
        ellipse.res2[i].first=int(round(tmpx2));
        ellipse.res2[i].second=int(round(tmpy2));
        ellipse.res3[i].first=int(round(tmpx3));
        ellipse.res3[i].second=int(round(tmpy3));
        ellipse.res4[i].first=int(round(tmpx4));
        ellipse.res4[i].second=int(round(tmpy4));
    }
}
void Canvas::drawellipse(Ellipse ellipse)
{
    int R,G,B;
    R=ellipse.clr.rvalue();
    G=ellipse.clr.gvalue();
    B=ellipse.clr.bvalue();
    ellipse.res1.clear();
    ellipse.res2.clear();
    ellipse.res3.clear();
    ellipse.res4.clear();
    vector<pair<int,int>> res=ELLIPSE(ellipse.rx,ellipse.ry);
    ellipse.changeres1(res);
    ellipse.fill();
    //cout<<"ellipse.r"<<endl;
    if(ellipse.r%360!=0)
    {

        Ellipserotate(ellipse,ellipse.r);

    }
    for(int i=0;i<ellipse.res1.size();i++)
    {
        int x1=ellipse.res1[i].first;
        int y1=ellipse.res1[i].second;
        int x2=ellipse.res2[i].first;
        int y2=ellipse.res2[i].second;
        int x3=ellipse.res3[i].first;
        int y3=ellipse.res3[i].second;
        int x4=ellipse.res4[i].first;
        int y4=ellipse.res4[i].second;


        x1+=ellipse.x;
        x2+=ellipse.x;
        x3+=ellipse.x;
        x4+=ellipse.x;
        y1+=ellipse.y;
        y2+=ellipse.y;
        y3+=ellipse.y;
        y4+=ellipse.y;



        rgb[w*y1+x1].r=R;
        rgb[w*y1+x1].g=G;
        rgb[w*y1+x1].b=B;

        rgb[w*y2+x2].r=R;
        rgb[w*y2+x2].g=G;
        rgb[w*y2+x2].b=B;

        rgb[w*y3+x3].r=R;
        rgb[w*y3+x3].g=G;
        rgb[w*y3+x3].b=B;

        rgb[w*y4+x4].r=R;
        rgb[w*y4+x4].g=G;
        rgb[w*y4+x4].b=B;
    }
    units.push_back(ellipse);
    ellipses.push_back(ellipse);
}

void Canvas::drawCurve(int id, int n, string algorithm, int x[], int y[], Pen pen){
    vector<pair<int,int>> point;
    int R,G,B;
    R=pen.rvalue();
    G=pen.gvalue();
    B=pen.bvalue();
    vector<pair<int,int>> res;
    if(algorithm=="Bezier")
    {
        point=CurveBezier(x,y,n);
        for(int i=1;i<point.size();i++)
        {
            res=lineDDA(point[i-1].first,point[i-1].second,point[i].first,point[i].second);
            for(int j=0;j<res.size();j++)
            {
                int x=res[j].first;
                int y=res[j].second;
                rgb[w*y+x].r=R;
                rgb[w*y+x].g=G;
                rgb[w*y+x].b=B;
            }
        }
    }
    else
    {
        point=CurveBspline(x,y,n);
        for(int i=1;i<point.size();i++)
        {
            res=lineDDA(point[i-1].first,point[i-1].second,point[i].first,point[i].second);
            for(int j=0;j<res.size();j++)
            {
                int x=res[j].first;
                int y=res[j].second;
                rgb[w*y+x].r=R;
                rgb[w*y+x].g=G;
                rgb[w*y+x].b=B;
            }
        }

    }
    Curve curve(id,4,pen,n,x,y,algorithm);
    units.push_back(curve);
    curves.push_back(curve);
}

int Line::clipCohen(int X1, int Y1, int X2, int Y2)
{
    int c1,c2;
    c1=0;
    c2=0;
    if(x1<X1){
        c1+=8;
    }
    if(x1>X2){
        c1+=4;
    }
    if(y1<Y1)
    {
        c1+=2;
    }
    if(y1>Y2)
    {
        c1+=1;
    }
    if(x2<X1){
        c2+=8;
    }
    if(x2>X2){
        c2+=4;
    }
    if(y2<Y1)
    {
        c2+=2;
    }
    if(y2>Y2)
    {
        c2+=1;
    }
    if(c1==0&&c2==0)
    {
        return 0;
    }
    else if((c1&c2)!=0)
    {
        return 1;
    }
    else
    {
        return 2;
    }

}

int Line::clipLiang(int p,int q,double *u1,double *u2)
{
   int flag=1;
   double r=double(q)/p;
   if(p<0)
   {
       if(r>*u2)
           flag=0;
       else if(r>*u1)
       {
           *u1=r;

       }
   }
   else if(p>0)
   {
       if(r<*u1)
           flag=0;
       else if(r<*u2)
       {
           *u2=r;

       }
   }
   else if(p==0&&q<0)
       flag=0;
   return flag;
}

void Canvas::clip(int id,int x1,int y1,int x2,int y2,string algorithm)
{
    for(int i=0;i<lines.size();i++)
    {
        if(lines[i].ID==id)
        {
            Line line=lines[i];
            if(algorithm=="Cohen-Sutherland")
            {
                vector<pair<int,int>> res;
                int p=line.clipCohen(x1,y1,x2,y2);
                if(p==0){
                    return;
                }
                else if(p==1)
                {
                    if(line.algo=="DDA")
                    {
                        res=lineDDA(line.x1,line.y1,line.x2,line.y2);
                    }
                    else{
                        res=lineBresenham(line.x1,line.y1,line.x2,line.y2);
                    }
                    for(int j=0;j<res.size();j++)
                    {
                        int x=res[j].first;
                        int y=res[j].second;
                        rgb[w*y+x].r=255;
                        rgb[w*y+x].g=255;
                        rgb[w*y+x].b=255;
                    }
                    lines[i].x1=0;
                    lines[i].y1=0;
                    lines[i].x2=0;
                    lines[i].y2=0;
                    lines[i].clr=Pen(255,255,255);
                    return;
                }
                else
                {
                    if(line.algo=="DDA")
                    {
                        res=lineDDA(line.x1,line.y1,line.x2,line.y2);
                    }
                    else{
                        res=lineBresenham(line.x1,line.y1,line.x2,line.y2);
                    }
                    int p=0;

                    for(int j=0;j<res.size();j++)
                    {
                        int x=res[j].first;
                        int y=res[j].second;
                        if(x<=x2&&x>=x1&&y<=y2&&y>=y1){
                            if((x==x1||y==y1)&&p==0)
                            {
                                lines[i].x1=x;
                                lines[i].y1=y;
                                p=1;

                            }
                            else if(x==x2||y==y2)
                            {
                                lines[i].x2=res[j-1].first;
                                lines[i].y2=res[j-1].second;
                            }
                            continue;
                        }

                        else{
                            rgb[w*y+x].r=255;
                            rgb[w*y+x].g=255;
                            rgb[w*y+x].b=255;
                        }
                    }
                }
            }
            else
            {
                vector<pair<int,int>> res;
                if(line.algo=="DDA")
                {
                    res=lineDDA(line.x1,line.y1,line.x2,line.y2);
                }
                else{
                    res=lineBresenham(line.x1,line.y1,line.x2,line.y2);
                }
                double u1=0;
                double u2=1.0;


                int dx=line.x2-line.x1;
                if(line.clipLiang(-dx,line.x1-x1,&u1,&u2))
                {
                    if(line.clipLiang(dx,x2-line.x1,&u1,&u2))
                    {
                        int dy=line.y2-line.y1;
                        if(line.clipLiang(-dy,line.y1-y1,&u1,&u2))
                        {
                            if(line.clipLiang(dy,y2-line.y1,&u1,&u2))
                            {
                                if(u2<1.0)
                                {
                                    line.x2=line.x1+round(double(dx)*u2);
                                    line.y2=line.y1+round(double(dy)*u2);
                                }
                                if(u1>0)
                                {
                                    line.x1=line.x1+round(double(dx)*u1);
                                    line.y1=line.y1+round(double(dy)*u1);
                                }
                                lines[i].x1=line.x1;
                                lines[i].y1=line.y1;
                                lines[i].x2=line.x2;
                                lines[i].y2=line.y2;

                                int a,b,c,d;
                                if(line.x1<line.x2)
                                {
                                    a=line.x1;
                                    b=line.x2;
                                }
                                else
                                {
                                    a=line.x2;
                                    b=line.x1;
                                }
                                if(line.y1<line.y2)
                                {
                                    c=line.y1;
                                    d=line.y2;
                                }
                                else
                                {
                                    c=line.y2;
                                    d=line.y1;
                                }

                                for(int j=0;j<res.size();j++)
                                {
                                    int x=res[j].first;
                                    int y=res[j].second;
                                    if(x>=a&&x<=b&&y>=c&&y<=d)
                                        continue;
                                    rgb[w*y+x].r=255;
                                    rgb[w*y+x].g=255;
                                    rgb[w*y+x].b=255;
                                }
                            }
                        }
                    }
                }


            }
        }
    }
}

void Line::translate(int dx, int dy)
{
    x1+=dx;
    x2+=dx;
    y1+=dy;
    y2+=dy;

}

void Polygon::translate(int dx, int dy)
{
    for(int i=0;i<n;i++)
    {
        x[i]+=dx;
        y[i]+=dy;
    }
}

void Ellipse::translate(int dx, int dy)
{
    x+=dx;
    y+=dy;
}

void Curve::translate(int dx, int dy)
{
    for(int i=0;i<n;i++)
    {
        x[i]+=dx;
        y[i]+=dy;
    }
}

void Canvas::translate(int id, int dx, int dy){
    int t;
    for(int i=0;i<units.size();i++)
    {
        if(units[i].ID==id)
        {
            t=units[i].type;
        }
    }
    if(t==1)
    {
        for(int j=0;j<lines.size();j++)
        {
            if(lines[j].ID==id)
            {
                lines[j].translate(dx,dy);
            }
        }
    }
    else if(t==2)
    {
        for(int j=0;j<polygons.size();j++)
        {
            if(polygons[j].ID==id)
            {
                polygons[j].translate(dx,dy);
            }
        }
    }
    else if(t==3)
    {
        for(int j=0;j<ellipses.size();j++)
        {
            if(ellipses[j].ID==id)
            {
                ellipses[j].translate(dx,dy);
            }
        }
    }
    else
    {
        for(int j=0;j<curves.size();j++)
        {
            if(curves[j].ID==id)
            {
                curves[j].translate(dx,dy);
            }
        }
    }
    for(int i=0;i<w*h;i++)
    {
        rgb[i].r=255;
        rgb[i].g=255;
        rgb[i].b=255;

    }
    int len1=lines.size();
    int len2=polygons.size();
    int len3=ellipses.size();
    int len4=curves.size();
    vector<Line> lines1=lines;
    vector<Polygon> polygons1=polygons;
    vector<Ellipse> ellipses1=ellipses;
    vector<Curve> curves1=curves;
    lines.clear();
    polygons.clear();
    ellipses.clear();
    curves.clear();
    for(int i=0;i<len1;i++)
    {
        Line line=lines1.back();
        lines1.pop_back();
        drawLine(line.ID,line.x1,line.y1,line.x2,line.y2,line.algo,line.clr);
    }
    for(int i=0;i<len2;i++)
    {
        Polygon polygon=polygons1.back();
        polygons1.pop_back();
        drawPolygon(polygon.ID,polygon.n,polygon.algo,polygon.x,polygon.y,polygon.clr);
    }
    for(int i=0;i<len3;i++)
    {
        Ellipse ellipse=ellipses1.back();
        ellipses1.pop_back();
        drawellipse(ellipse);
    }
    for(int i=0;i<len4;i++)
    {
        Curve curve=curves1.back();
        curves1.pop_back();
        drawCurve(curve.ID,curve.n,curve.algo,curve.x,curve.y,curve.clr);
    }
}

void Line::scale(int x, int y, float s)
{
    float tmpx1,tmpy1,tmpx2,tmpy2;
    tmpx1=float(x1)*s-float(x)*(s-1);
    tmpy1=float(y1)*s-float(y)*(s-1);
    tmpx2=float(x2)*s-float(x)*(s-1);
    tmpy2=float(y2)*s-float(y)*(s-1);
    x1=int(round(tmpx1));
    y1=int(round(tmpy1));
    x2=int(round(tmpx2));
    y2=int(round(tmpy2));
    //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
}

void Polygon::scale(int x, int y, float s)
{
    float tmpx,tmpy;
    for(int i=0;i<n;i++)
    {
        tmpx=float(this->x[i])*s-float(x)*(s-1);
        tmpy=float(this->y[i])*s-float(y)*(s-1);
        this->x[i]=int(round(tmpx));
        this->y[i]=int(round(tmpy));
    }
}

void Ellipse::scale(int x, int y, float s)
{
    float tmpx,tmpy,tmprx,tmpry;
    tmpx=float(this->x)*s-float(x)*(s-1);
    tmpy=float(this->y)*s-float(y)*(s-1);
    tmprx=float(rx)*s;
    tmpry=float(ry)*s;
    this->x=int(round(tmpx));
    this->y=int(round(tmpy));
    rx=int(round(tmprx));
    ry=int(round(tmpry));


}

void Curve::scale(int x, int y, float s)
{
    float tmpx,tmpy;
    for(int i=0;i<n;i++)
    {
        tmpx=float(this->x[i])*s-float(x)*(s-1);
        tmpy=float(this->y[i])*s-float(y)*(s-1);
        this->x[i]=int(round(tmpx));
        this->y[i]=int(round(tmpy));

    }
}

void Canvas::scale(int id, int x, int y, float s)
{

    int t;
    for(int i=0;i<units.size();i++)
    {
        if(units[i].ID==id)
        {
            t=units[i].type;
        }
    }
    if(t==1)
    {
        for(int j=0;j<lines.size();j++)
        {
            if(lines[j].ID==id)
            {
                lines[j].scale(x,y,s);
            }
        }
    }
    else if(t==2)
    {
        for(int j=0;j<polygons.size();j++)
        {
            if(polygons[j].ID==id)
            {
                polygons[j].scale(x,y,s);
            }
        }
    }
    else if(t==3)
    {
        for(int j=0;j<ellipses.size();j++)
        {
            if(ellipses[j].ID==id)
            {
                ellipses[j].scale(x,y,s);
            }
        }
    }
    else
    {
        for(int j=0;j<curves.size();j++)
        {
            if(curves[j].ID==id)
            {
                curves[j].scale(x,y,s);
            }
        }
    }

    for(int i=0;i<w*h;i++)
    {
        rgb[i].r=255;
        rgb[i].g=255;
        rgb[i].b=255;

    }
    int len1=lines.size();
    int len2=polygons.size();
    int len3=ellipses.size();
    int len4=curves.size();
    vector<Line> lines1=lines;
    vector<Polygon> polygons1=polygons;
    vector<Ellipse> ellipses1=ellipses;
    vector<Curve> curves1=curves;
    lines.clear();
    polygons.clear();
    ellipses.clear();
    curves.clear();
    for(int i=0;i<len1;i++)
    {
        Line line=lines1.back();
        lines1.pop_back();
        drawLine(line.ID,line.x1,line.y1,line.x2,line.y2,line.algo,line.clr);
    }
    for(int i=0;i<len2;i++)
    {
        Polygon polygon=polygons1.back();
        polygons1.pop_back();
        drawPolygon(polygon.ID,polygon.n,polygon.algo,polygon.x,polygon.y,polygon.clr);
    }
    for(int i=0;i<len3;i++)
    {
        Ellipse ellipse=ellipses1.back();
        ellipses1.pop_back();
        drawellipse(ellipse);
    }
    for(int i=0;i<len4;i++)
    {
        Curve curve=curves1.back();
        curves1.pop_back();
        drawCurve(curve.ID,curve.n,curve.algo,curve.x,curve.y,curve.clr);
    }
}

void Line::rotate(int x, int y, int r)
{
    double r1=double(r)*3.1415926/180;
    double tmpx1,tmpy1,tmpx2,tmpy2;
    tmpx1=double(x1)*cos(r1)-double(y1)*sin(r1)+(1.0-cos(r1))*double(x)+double(y)*sin(r1);
    tmpy1=double(x1)*sin(r1)+double(y1)*cos(r1)+(1.0-cos(r1))*double(y)-double(x)*sin(r1);
    tmpx2=double(x2)*cos(r1)-double(y2)*sin(r1)+(1.0-cos(r1))*double(x)+double(y)*sin(r1);
    tmpy2=double(x2)*sin(r1)+double(y2)*cos(r1)+(1.0-cos(r1))*double(y)-double(x)*sin(r1);
    x1=int(round(tmpx1));
    y1=int(round(tmpy1));
    x2=int(round(tmpx2));
    y2=int(round(tmpy2));

}

void Polygon::rotate(int x, int y, int r)
{
    double r1=double(r)*3.1415926/180;
    double tmpx,tmpy;
    for(int i=0;i<n;i++)
    {
        tmpx=double(this->x[i])*cos(r1)-double(this->y[i])*sin(r1)+(1.0-cos(r1))*double(x)+double(y)*sin(r1);
        tmpy=double(this->x[i])*sin(r1)+double(this->y[i])*cos(r1)+(1.0-cos(r1))*double(y)-double(x)*sin(r1);
        this->x[i]=int(round(tmpx));
        this->y[i]=int(round(tmpy));
    }
}

void Ellipse::rotate(int x, int y, int r)
{
    double r1=double(r)*3.1415926/180;
    double tmpx,tmpy;
    tmpx=double(this->x)*cos(r1)-double(this->y)*sin(r1)+(1.0-cos(r1))*double(x)+double(y)*sin(r1);
    tmpy=double(this->x)*sin(r1)+double(this->y)*cos(r1)+(1.0-cos(r1))*double(y)-double(x)*sin(r1);
    this->x=int(round(tmpx));
    this->y=int(round(tmpy));
    this->r+=r;


}

void Curve::rotate(int x, int y, int r)
{
    double r1=double(r)*3.1415926/180;
    double tmpx,tmpy;
    for(int i=0;i<n;i++)
    {
        tmpx=double(this->x[i])*cos(r1)-double(this->y[i])*sin(r1)+(1.0-cos(r1))*double(x)+double(y)*sin(r1);
        tmpy=double(this->x[i])*sin(r1)+double(this->y[i])*cos(r1)+(1.0-cos(r1))*double(y)-double(x)*sin(r1);
        this->x[i]=int(round(tmpx));
        this->y[i]=int(round(tmpy));
    }
}

void Canvas::rotate(int id, int x, int y, int r)
{

    int t;
    for(int i=0;i<units.size();i++)
    {
        if(units[i].ID==id)
        {
            t=units[i].type;
        }
    }
    if(t==1)
    {
        for(int j=0;j<lines.size();j++)
        {
            if(lines[j].ID==id)
            {
                lines[j].rotate(x,y,r);
            }
        }
    }
    else if(t==2)
    {
        for(int j=0;j<polygons.size();j++)
        {
            if(polygons[j].ID==id)
            {
                polygons[j].rotate(x,y,r);
            }
        }
    }
    else if(t==3)
    {
        for(int j=0;j<ellipses.size();j++)
        {
            if(ellipses[j].ID==id)
            {
                ellipses[j].rotate(x,y,r);
            }
        }
    }
    else
    {
        for(int j=0;j<curves.size();j++)
        {
            if(curves[j].ID==id)
            {
                curves[j].rotate(x,y,r);
            }
        }
    }

    for(int i=0;i<w*h;i++)
    {
        rgb[i].r=255;
        rgb[i].g=255;
        rgb[i].b=255;

    }
    int len1=lines.size();
    int len2=polygons.size();
    int len3=ellipses.size();
    int len4=curves.size();
    vector<Line> lines1=lines;
    vector<Polygon> polygons1=polygons;
    vector<Ellipse> ellipses1=ellipses;
    vector<Curve> curves1=curves;
    lines.clear();
    polygons.clear();
    ellipses.clear();
    curves.clear();
    for(int i=0;i<len1;i++)
    {
        Line line=lines1.back();
        lines1.pop_back();
        drawLine(line.ID,line.x1,line.y1,line.x2,line.y2,line.algo,line.clr);
    }
    for(int i=0;i<len2;i++)
    {
        Polygon polygon=polygons1.back();
        polygons1.pop_back();
        drawPolygon(polygon.ID,polygon.n,polygon.algo,polygon.x,polygon.y,polygon.clr);
    }
    for(int i=0;i<len3;i++)
    {
        Ellipse ellipse=ellipses1.back();
        ellipses1.pop_back();
        drawellipse(ellipse);
    }
    for(int i=0;i<len4;i++)
    {
        Curve curve=curves1.back();
        curves1.pop_back();
        drawCurve(curve.ID,curve.n,curve.algo,curve.x,curve.y,curve.clr);
    }
}
