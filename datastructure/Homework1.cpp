#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
class Bigint
{
private:
    int a[300];
public:
    Bigint ()
    {
        memset(a,0,sizeof(a));
    }
    friend istream& operator>>(istream& i,Bigint& m)
    {
        char ch[300];
        gets(ch);
        int length=strlen(ch);
        for(int j=0; j<length; j++)
            m.a[j]=ch[length-1-j]-'0';
        return i;
    }
    friend ostream& operator<<(ostream& o,Bigint m)
    {
        int i;
        for(i=299; i>=0; i--)
        {
            if(m.a[i])
                break;
        }
        for(; i>=0; i--)
            cout<<m.a[i];
        return o;
    }
    Bigint operator = (int m)
    {
        char ch[300];
        Bigint n;
        sprintf(ch,"%d",m);
        int length=strlen(ch);
        int i;
        for(i=0; i<length; i++)
        {
            n.a[i]=ch[length-i-1]-'0';
        }
        for(i=0; i<299; i++)
            a[i]=n.a[i];
    }
    friend Bigint operator + (const Bigint &x, const Bigint &y);
    friend Bigint operator * (const Bigint &x, const Bigint &y);
};
Bigint operator + (const Bigint &x,const Bigint &y)
{
    Bigint ands;
    int f=0;
    int num=0;
    for(int i=0; i<299; i++)
    {
        int index=x.a[i]+y.a[i]+f;
        ands.a[num++]=index%10;
        f=index/10;
    }
    return ands;
}
Bigint operator * (const Bigint &x, const Bigint &y)
{
    Bigint time;
    int plen=300;
    int qlen=300;
    while(!x.a[--plen]) {}
    while(!y.a[--qlen]) {}
    for(int i=0; i<=plen; i++)
    {
        Bigint ands;
        int f=0;
        int num=i;
        for(int j=0; j<=qlen; j++)
        {
            int index=x.a[i]*y.a[j]+f;
            ands.a[num++]=index%10;
            f=index/10;
        }
        while(f)
        {
            ands.a[num++]=f%10;
            f=f/10;
        }
        time=time+ands;
    }
    return time;
}

int main()
{
    Bigint sum;
    sum=1;
    int i;
    for( i=1; i<=100; i++)
    {
        Bigint ands;
        ands=i;
        sum=ands*sum;
    }
    cout<<sum;
    return 0;
}
