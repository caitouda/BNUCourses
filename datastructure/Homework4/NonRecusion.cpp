#include <iostream>
#include <cstring>
using namespace  std;
int n;
int col[100];
int md[200];
int sd[200];
int q[100];
int num=1;
void print()
{
    cout<<"菴"<<num<<"意:"<<endl;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(j==q[i]) cout<<'$'<<" ";
            else cout<<"* ";
        }
        cout<<endl;
    }
    num++;
}

int main()
{
    int i=0;
    int j;
    int flag=0;
    memset(q,-1,sizeof(q));
    cout<<"?n:"<<endl;
    cin>>n;
    while(i>=0)
    {

        if(q[i]!=-1)
        {
            j=q[i];
            md[n+i-j-1]=0;
            sd[i+j]=0;
            col[j]=0;
        }
        flag=0;
        for(j=q[i]+1; j<n; j++)
        {
            if(md[n+i-j-1]==0&&sd[i+j]==0&&col[j]==0)
            {
                md[n+i-j-1]=1;
                sd[i+j]=1;
                col[j]=1;
                q[i]=j;
                i++;
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            if(q[i]!=-1)
            {
                j=q[i];
                md[n+i-j-1]=0;
                sd[i+j]=0;
                col[j]=0;
                q[i]=-1;
            }
            i--;
        }
        if(i==n)
        {
            print();
            i--;
        }
    }
    return 0;
}

