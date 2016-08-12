#include<iostream> //������Բ�����е�ɨ�����㷨
#include<windows.h>
#include<math.h>
using namespace std;

void EllipsePoints(HDC hdc,int x,int y,int color)
{
    SetPixel(hdc,x,y,color);
    SetPixel(hdc,-x,y,color);
    SetPixel(hdc,-x,-y,color);
    SetPixel(hdc,x,-y,color);
}

void MidPointEllipse(HDC hdc,int a,int b,int color)
{
    int x,y,d,xP,yP,squarea,squareb;
    squarea=a*a;
    squareb=b*b;
    xP=(int)(0.5+(float)squarea/sqrt((float)(squarea+squareb)));
    yP=(int)(0.5+(float)squareb/sqrt((float)(squarea+squareb)));
    x=0;
    y=b;
    d=4*(squareb-squarea*b)+squarea;
    EllipsePoints(hdc,x,y,color);
    while(x<=xP)
    {
        if(d<=0)
            d+=4*squareb*(2*x+3);
        else
        {
            d+=4*squareb*(2*x+3)-8*squarea*(y-1);
            y--;
        }
        x++;
        EllipsePoints(hdc,x,y,color);
    }
    x=a;
    y=0;
    d=4*(squarea-a*squareb)+squareb;
    EllipsePoints(hdc,x,y,color);
    while(y<yP)
    {
        if(d<=0)
            d+=4*squarea*(2*y+3);
        else
        {
            d+=4*squarea*(2*y+3)-8*squareb*(x-1);
            x--;
        }
        y++;
        EllipsePoints(hdc,x,y,color);
    }
}

int main()
{
    int a,b;
    int color=RGB(255,255,0);
    HDC hdc=GetDC(NULL);//�����ʾ�����豸������
    SetViewportOrgEx(hdc, 600, 400, NULL);//ָ���ĸ��豸��ӳ�䵽�߼��㣨0��0���������ƶ������Ṧ�ܣ��Ӷ�ʹ�߼��㣨0��0������ָ�����Ͻ�
    cout<<"������Բ���������᳤��"<<endl;
    cin>>a>>b;
    MidPointEllipse(hdc,a,b,color);//�����е��㷨��ʾ��Բ
    return 0;
}
