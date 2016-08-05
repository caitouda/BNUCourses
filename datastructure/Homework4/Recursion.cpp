#include <iostream>
#include <cstring>
using namespace std;
int n;
int col[100];
int md[200];
int sd[200];
int q[100];
int num=1;
void print()
{
    cout<<"µÚ"<<num<<"·NÇé›r:"<<endl;
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
void Queen(int i)
{
    for(int j=0; j<n; j++)
    {
        if(md[n+i-j-1]==0&&sd[i+j]==0&&col[j]==0)
        {
            q[i]=j;
            if(i==n-1) print();
            else
            {
                md[n+i-j-1]=1;
                sd[i+j]=1;
                col[j]=1;
                Queen(i+1);
            }
            md[n+i-j-1]=0;
            sd[i+j]=0;
            col[j]=0;
        }
    }
}
int main()
{

    memset(col,0,sizeof(col));
    memset(md,0,sizeof(md));
    memset(sd,0,sizeof(sd));
    cout<<"İ”Èën:"<<endl;
    cin>>n;
    Queen(0);
    return 0;
}

