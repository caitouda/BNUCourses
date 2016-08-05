#include<stdio.h>
using namespace std;
const int Max=1000000;
int cost[1000][1000],prev[1000],dist[1000],mark[1000];

int main()
{
    int i,j,n,start,end;
    printf("Please input the number of the points:");
    scanf("%d",&n);
    printf("Please input the length of the points in order:\n");
    for(i=1; i<=n; i++)
        for(j=1; j<=n; j++)
        {
            scanf("%d",&cost[i][j]);
            if(cost[i][j]==0)cost[i][j]=Max;
        }
    printf("Please input the start point:");
    scanf("%d",&start);
    mark[start]=1;
    for(i=1; i<=n; i++)
    {
        if(mark[i]==0)
        {
            prev[i]=start;
            dist[i]=cost[start][i];
        }
    }
    for(i=1; i<=n; i++)
    {
        int u=start;
        int temp=Max;
        for(j=1; j<=n; j++)
        {
            if(mark[j]==0&&temp>dist[j])
            {
                u=j;
                temp=dist[j];
            }
        }
        mark[u]=1;
        for(j=1; j<=n; j++)
        {
            if(mark[j]==0&&cost[u][j]<Max)
            {
                int newdist=dist[u]+cost[u][j];
                if(newdist<dist[j])
                {
                    dist[j]=newdist;
                    prev[j]=u;
                }
            }
        }
    }
    printf("Please input the end point:");
    scanf("%d",&end);
    printf("The shortest length is:%d\n",dist[end]);
    printf("The opposite order is:");
    printf("%d ",end);
    while(prev[end]!=0)
    {
        printf("%d ",prev[end]);
        end=prev[end];
    }
    printf("\n");
    return 0;
}
