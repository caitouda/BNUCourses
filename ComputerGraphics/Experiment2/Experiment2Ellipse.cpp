#include<iostream> //生成椭圆弧的中点扫描线算法
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
    HDC hdc=GetDC(NULL);//获得显示器的设备上下文
    SetViewportOrgEx(hdc, 600, 400, NULL);//指定哪个设备点映射到逻辑点（0，0），具有移动坐标轴功能，从而使逻辑点（0，0）不再指向左上角
    cout<<"输入椭圆的两个半轴长："<<endl;
    cin>>a>>b;
    MidPointEllipse(hdc,a,b,color);//利用中点算法显示椭圆
    return 0;
}
