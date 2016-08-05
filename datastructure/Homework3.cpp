#include<iostream>
#include<cstring>
using namespace std;

void  move(char *f, double *s,int p)
{
    int i=0,len=strlen(f);
    for(i=p; i<len; i++)
    {
        f[i]=f[i+1];
        s[i]=s[i+1];
    }
    s[i]=s[i+1];
    f[len-1]='\0';
}

double convnum(char *c)
{
    double num=0.0;
    double a=1.0;
    int i=0,p=0,len=0;
    char temp[100];
    int tempi=0;
    int start=0;
    int f=1;
    len=strlen(c);
    if(c[0]=='-')
    {
        start=1;
        f=-1;
    }
    for(i=start; i<len; i++)
    {
        if(c[i]=='.')
        {
            p=i;
            break;
        }
        temp[tempi++]=c[i];
    }
    temp[tempi]='\0';
    if(p!=0)
    {
        for(i=p+1; i<len; i++)
        {
            a=a*0.1;
            num+=(a*(c[i]-48));
        }
    }
    a=1.0;
    len=strlen(temp);
    for(i=len-1; i>=0; i--)
    {
        num=num+(a*(temp[i]-48));
        a*=10;
    }
    num=num*f;
    return num;
}

double good(char *c)
{
    char g[100],number[30];
    char f[80];
    int fi=0;
    double s[80];
    int si=0;
    int k=0;
    int num=0,i=0;
    int cc=0;
    int jj=0;
    while(*c!='\0')
    {
        k=0;
        num=0;
        switch(*c)
        {
        case '+':
        case '-':
        case '*':
        case'/':
            f[fi++]=*c;
            if(*c=='*' || *c=='/')
                cc++;
            else
                jj++;
            if(*(c-1)!=')')
            {
                number[i]='\0';
                i=0;
                s[si++]=convnum(number);
            }
            break;
        case'(':
            k++;
            while(k>0)
            {
                c++;
                g[num]=*c;
                num++;
                if(*c==')')
                {
                    k--;
                }
                else if(*c=='(')
                {
                    k++;
                }
            }
            g[num-1]='\0';
            num=0;
            s[si++]=good(g);
            break;
        default:
            number[i++]=*c;
            if(*(c+1)=='\0')
            {
                number[i]='\0';
                s[si++]=convnum(number);
            }
            break;
        }

        c++;
    }
    f[fi]='\0';
    i=0;
    while(cc>0)
    {
        switch(f[i])
        {
        case '*':
            cc--;
            s[i+1]=s[i]*s[i+1];
            move(f,s,i);
            break;
        case '/':
            cc--;
            s[i+1]=s[i]/(float)s[i+1];
            move(f,s,i);
            break;
        default:
            i++;
            break;
        }
    }

    i=0;
    while(jj>0)
    {
        switch(f[i])
        {
        case '+':
            s[i+1]=s[i]+s[i+1];
            jj--;
            move(f,s,i);
            break;
        case '-':
            s[i+1]=s[i]-s[i+1];
            jj--;
            move(f,s,i);
            break;
        default:
            cout<<"·ûÌ–åeÕ`£¡"<<endl;
            break;
        }
    }
    return s[0];
}
int main()
{
    char str[100];
    double sum=0;
    cout<<"Ý”ÈëÖÐ¾YËãÐgß\ËãÊ½£º"<<endl;
    cin>>str;
    sum=good(str);
    cout<<str<<"="<<sum<<endl;
}
