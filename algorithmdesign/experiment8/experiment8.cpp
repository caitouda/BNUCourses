#include<stdio.h>
#include<algorithm>
using namespace std;
struct wupin//用來存放物品的標號、體積和價值
{
    int m,w,v;
};
struct wupin wp[1000];
int dp[1000][1000],top;
void show(int N,int CC)//依次選擇的物品的標號
{
    if(N==0) return;
    if(dp[N-1][CC-wp[N].w]+wp[N].v==dp[N][CC])
    {
        top++;
        wp[top].m=N-1;
        show(N-1,CC-wp[N].w);
    }
    else show(N-1,CC);
}
int main()
{
    int n,C,i,j;
    printf("Please input the number of the goods:");
    scanf("%d",&n);
    printf("Please input the whole volumn of the bag:");
    scanf("%d",&C);
    printf("Please input the volumn and value of the goods in turn by one line:");
    printf("\n");
    for(i=1; i<=n; i++)
    {
        printf("The %d goods is:",i);
        scanf("%d%d",&wp[i].w,&wp[i].v);
        wp[i].m=i;
    }
    for(i=1; i<=n; i++)
    {
        for(j=1; j<=C; j++)
        {
            dp[i][j]=dp[i-1][j];
            if(j>=wp[i].w)
            {
                if(dp[i][j]<dp[i-1][j-wp[i].w]+wp[i].v)
                {
                    dp[i][j]=dp[i-1][j-wp[i].w]+wp[i].v;
                }
            }
        }
    }
    printf("The largest value of the bag is:");
    printf("%d\n",dp[n][C]);
    top=0;
    show(n,C);
    printf("The order of the goods is:");
    for(i=top; i>1; i--)
    {
        printf("%d ",wp[i].m+1);
    }
    printf("%d\n",wp[1].m+1);
    return 0;
}

