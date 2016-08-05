#include<iostream>
using namespace std;

int i,j,k;
int A[64][64],B[64][64],C[64][64];
int a11[64][64],a12[64][64],a21[64][64], a22[64][64];
int b11[64][64],b12[64][64],b21[64][64], b22[64][64];
int m1[64][64],m2[64][64],m3[64][64],m4[64][64],m5[64][64], m6[64][64], m7[64][64];
int c11[64][64],c12[64][64],c21[64][64],c22[64][64];
int x[64][64],y[64][64];


void add(int n,int a[64][64],int b[64][64],int c[64][64])//矩陣加法
{
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            c[i][j]=a[i][j]+b[i][j];
}

void sub(int n,int a[64][64],int b[64][64],int c[64][64])//矩陣減法
{
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            c[i][j]=a[i][j]-b[i][j];
}

void mul(int n,int a[64][64],int b[64][64],int c[64][64])//矩陣乘法
{
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
        {
            c[i][j]=0;
            for(k=0; k<n; k++)
                c[i][j]=c[i][j]+a[i][k]*b[k][j];
        }
}

void strassen(int n,int a[64][64],int b[64][64],int c[64][64])//strassen矩陣演算法
{
    if(n==2)mul(n,a,b,c);
    else
    {
        for(i=0; i<n/2; i++)
        {
            for(j=0; j<n/2; j++)
            {
                a11[i][j]=a[i][j];
                a12[i][j]=a[i][j+n/2];
                a21[i][j]=a[i+n/2][j];
                a22[i][j]=a[i+n/2][j+n/2];
                b11[i][j]=b[i][j];
                b12[i][j]=b[i][j+n/2];
                b21[i][j]=b[i+n/2][j];
                b22[i][j]=b[i+n/2][j+n/2];
            }
        }
        sub(n/2,b12,b22,x);//算m1
        strassen(n/2,a11,x,m1);

        add(n/2,a11,a12,x);//算m2
        strassen(n/2,x,b22,m2);

        add(n/2,a21,a22,x);//算m3
        strassen(n/2,x,b11,m3);

        sub(n/2,b21,b11,x);//Ëãm4
        strassen(n/2,a22,x,m4);

        add(n/2,a11,a22,x);//算m5
        add(n/2,b11,b22,y);
        strassen(n/2,x,y,m5);

        sub(n/2,a12,a22,x);//算m6
        add(n/2,b21,b22,y);
        strassen(n/2,x,y,m6);

        sub(n/2,a11,a21,x);//算m7
        add(n/2,b11,b12,y);
        strassen(n/2,x,y,m7);

        add(n/2,m5,m4,x);//算c11
        sub(n/2,x,m2,y);
        add(n/2,y,m6,c11);

        add(n/2,m1,m2,c12);//算c12

        add(n/2,m3,m4,c21);//算c21

        add(n/2,m5,m1,x);//算c22
        sub(n/2,x,m3,y);
        sub(n/2,y,m7,c22);
        for(i=0; i<n/2; i++)

            for(j=0; j<n/2; j++)
            {
                c[i][j]=c11[i][j];
                c[i][j+n/2]=c12[i][j];
                c[i+n/2][j]=c21[i][j];
                c[i+n/2][j+n/2]=c22[i][j];
            }
    }
}

int main()
{
    int size;
    cout<<"Please input the size of the matrix:"<<endl;
    cin>>size;
    cout<<"Please input the first matrix:"<<endl;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            cin>>A[i][j];
    cout<<"Please input the second matrix:"<<endl;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            cin>>B[i][j];
    strassen(size,A,B,C);
    cout<<"The final matrix is:"<<endl;
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            cout<<C[i][j]<<' ';
        }
        cout<<endl;
    }
    return 0;
}
