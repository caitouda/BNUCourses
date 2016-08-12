#include<iostream>
#include<windows.h>
#include<math.h>
using namespace std;

void MidPointLine(HDC hdc,int x0,int y0,int x1,int y1,int color)
{
    int dx,dy,incrE,incrNE,d,x,y;
    float m;
    if(x0>x1)//�����һ��������ǰһ��������ߣ��򽻻���������
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
    if(m>=0&&m<=1)//б�ʽ���0��1֮��
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
    if(m>1)//б�ʴ���1
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
    if(m>=-1&&m<0)//б�ʽ���-1��0֮�䣬��б�ʽ���0��1֮���Ϊ����
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

    if(m<-1)//б��С��-1����б�ʴ���1��Ϊ����
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
//����ԭ������

    for(i=0; i< 200; i++)
        SetPixel(hdc,200,i,RGB(255, 255, 0));
    for(i=0; i< 200; i++)
        SetPixel(hdc,i,200,RGB(255, 255, 0));
//������ɫ������

    cout<<"���������εı�����"<<endl;
    int edges;
    cin>>edges;
    cout<<"���������θ����� �Կո�Ϊ������س�������"<<endl;

    POINT points[20];
    //��������洢��������

    for(i=0; i<edges; i++)
    {
        cout<<"��"<<i+1<<"�����㣺"<<endl;
        cin>>points[i].x>>points[i].y;
    }

    for(i=0; i<edges; i++)
    {
        MidPointLine(hdc,points[i].x,points[i].y,points[(i+1)%edges].x,points[(i+1)%edges].y,RGB(0,255,0));
        //ͨ�����ߵķ������������
        //ע�⣺���붥������ʱһ��Ҫ˳������
    }

    return 0;
}

