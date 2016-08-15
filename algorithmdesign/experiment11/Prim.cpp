#include<stdio.h>
using namespace std;
int n,e[100][100],mark[100];
int main()
{
    int i,j,T,temp,ti,tj;
    printf("Please input the number of the nodes:");
    scanf("%d",&n);
    printf("Please input the matrix of the nodes:\n");
    temp=10000;
    for(i=1; i<=n; i++)
        for(j=1; j<=n; j++)
        {
            scanf("%d",&e[i][j]);
            if(e[i][j]!=0&&temp>e[i][j])
            {
                temp=e[i][j];
                ti=i;
                tj=j;
            }
        }
    printf("add the edge of %d and %d\n",ti,tj);
    e[ti][tj]=0;
    mark[ti]=1;
    mark[tj]=1;
    T=n-2;
    while(T--)
    {
        temp=10000;
        for(i=1; i<=n; i++)//mark来标记是否已被选中
        {
            if(mark[i]==1)
                for(j=1; j<=n; j++)
                {
                    if(mark[j]==0)
                    {
                        if(e[i][j]!=0&&temp>e[i][j])
                        {
                            temp=e[i][j];
                            ti=i;
                            tj=j;
                        }
                    }

                }
        }
        printf("add the edge of %d and %d\n",ti,tj);
        e[ti][tj]=0;
        mark[ti]=1;
        mark[tj]=1;
    }
    return 0;
}
