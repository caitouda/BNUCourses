#include<stdio.h>
using namespace std;
int n,e[100][100],jihe[100];
int main()
{
    int i,j,T,temp,ti,tj;
    printf("Please input the number of the nodes:");
    scanf("%d",&n);
    for(i=1; i<=n; i++)
    {
        jihe[i]=i;
    }
    printf("Please input the matrix of the nodes:\n");
    for(i=1; i<=n; i++)
        for(j=1; j<=n; j++)
            scanf("%d",&e[i][j]);
    T=n-1;
    while(T--)
    {
        temp=10000;
        for(i=1; i<=n; i++)
        {

            for(j=i+1; j<=n; j++)
            {
                if(e[i][j]!=0&&temp>e[i][j]&&jihe[i]!=jihe[j])
                {
                    temp=e[i][j];
                    ti=i;
                    tj=j;
                }
            }

        }
        printf("add the edge of %d and %d\n",ti,tj);
        e[ti][tj]=0;
        if(jihe[ti]<=jihe[tj])//更新加入的點所屬的集合，以較小值為准
        {
            for(i=1; i<=n; i++)
            {
                if(jihe[i]==jihe[tj])
                {
                    jihe[i]=jihe[ti];
                }
            }
        }
        else
        {
            for(i=1; i<=n; i++)
            {
                if(jihe[i]==jihe[ti])
                {
                    jihe[i]=jihe[tj];
                }
            }
        }
    }
    return 0;
}
