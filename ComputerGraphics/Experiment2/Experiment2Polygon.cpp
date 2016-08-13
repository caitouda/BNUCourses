#include<iostream>
#include<windows.h>
#include<math.h>
using namespace std;

void MidPointLine(HDC hdc,int x0,int y0,int x1,int y1,int color)
{
    int dx,dy,incrE,incrNE,d,x,y;
    float m;
    if(x0>x1)//如果后一个坐标在前一个坐标左边，则交换两个坐标
    {
        int tempx=x1,tempy=y1;
        x1=x0,y1=y0;
        x0=tempx,y0=tempy;
    }
    //puts("3");
    dx=x1-x0;
    dy=y1-y0;
    m=(float)dy/dx;
    //cout<<m<<endl;
    if(m>=0&&m<=1)//斜率介于0到1之间
    {
        //puts("4");
        d=dx-2*dy;
        incrE=-2*dy;
        incrNE=2*(dx-dy);
        x=x0,y=y0;
        SetPixel(hdc,x,y,color);
        while(x<x1)
        {
            //cout<<"d:"<<d<<endl;
            x++;
            if(d>0)
                d+=incrE;
            else
            {
                d+=incrNE;
                y++;

            }
            //cout<<x<<" "<<y<<endl;
            SetPixel(hdc,x,y,color);
        }
    }
    if(m>1)//斜率大于1
    {
        d=dy-2*dx;
        incrE=-2*dx;
        incrNE=2*(dy-dx);
        x=x0,y=y0;
        SetPixel(hdc,x,y,color);
        while(y<y1)
        {
            y++;
            if(d>0)
                d+=incrE;
            else
            {
                d+=incrNE;
                x++;

            }
            SetPixel(hdc,x,y,color);
        }
    }
    if(m>=-1&&m<0)//斜率介于-1到0之间，以斜率介于0到1之间的为参照
    {
        y1=2*y0-y1;
        dx=x1-x0;
        dy=y1-y0;
        //puts("4");
        d=dx-2*dy;
        incrE=-2*dy;
        incrNE=2*(dx-dy);
        x=x0,y=y0;
        SetPixel(hdc,x,y,color);
        while(x<x1)
        {
            //cout<<"d:"<<d<<endl;
            x++;
            if(d>0)
                d+=incrE;
            else
            {
                d+=incrNE;
                y++;

            }
            //cout<<x<<" "<<y<<endl;
            SetPixel(hdc,x,2*y0-y,color);
        }
    }

    if(m<-1)//斜率小于-1，以斜率大于1的为参照
    {
        y1=2*y0-y1;
        dx=x1-x0;
        dy=y1-y0;
        d=dy-2*dx;
        incrE=-2*dx;
        incrNE=2*(dy-dx);
        x=x0,y=y0;
        SetPixel(hdc,x,y,color);
        while(y<y1)
        {
            y++;
            if(d>0)
                d+=incrE;
            else
            {
                d+=incrNE;
                x++;

            }
            SetPixel(hdc,x,2*y0-y,color);
        }
    }
}

int main()
{
    int i;
    HDC hdc=GetDC(NULL);
    SetViewportOrgEx(hdc, 200, 200, NULL);
//设置原点坐标

    for(i=0; i< 200; i++)
        SetPixel(hdc,200,i,RGB(255, 255, 0));
    for(i=0; i< 200; i++)
        SetPixel(hdc,i,200,RGB(255, 255, 0));
//画出黄色坐标轴

    cout<<"请输入多边形的边数："<<endl;
    int edges;
    cin>>edges;
    cout<<"请输入多边形各顶点 以空格为间隔，回车结束："<<endl;

    POINT points[20];
    //定义数组存储顶点坐标

    for(i=0; i<edges; i++)
    {
        cout<<"第"<<i+1<<"个顶点："<<endl;
        cin>>points[i].x>>points[i].y;
    }

    for(i=0; i<edges; i++)
    {
        MidPointLine(hdc,points[i].x,points[i].y,points[(i+1)%edges].x,points[(i+1)%edges].y,RGB(0,255,0));
        //通过画线的方法画出多边形
        //注意：输入顶点坐标时一定要顺序输入
    }

    return 0;
}

