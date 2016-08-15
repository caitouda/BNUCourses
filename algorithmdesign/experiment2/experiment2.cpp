#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;

int chess[8][8];
const int dx[]= {1,1,-2,-2,2,2,-1,-1};//下一步的左右分量
const int dy[]= {-2,2,1,-1,1,-1,2,-2};//下一步的上下分量

struct Node
{
    int x,y,t;
};

int cmp(Node a,Node b)
{
    return a.t<b.t;
}

int get_t(int x,int y)
{
    int cnt=0;
    for(int i=0; i<8; i++)
    {
        int nx=x+dx[i];
        int ny=y+dy[i];
        if(nx<0 || ny<0 || nx>=8|| ny>=8) continue;
        if(chess[nx][ny]) continue;
        cnt++;
    }
    return cnt;
}

int dfs(int x,int y,int val)//深度優先搜索演算法,優先走方向少的位置
{
    if(val>64) return 1;
    Node s[8];
    for(int i=0; i<8; i++)//枚舉下一個位置
    {
        s[i].x=x+dx[i];
        s[i].y=y+dy[i];
        if(s[i].x<0 || s[i].y<0 || s[i].x>=8|| s[i].y>=8) s[i].t=20;
        else if(chess[s[i].x][s[i].y]) s[i].t=20;
        else s[i].t=get_t(s[i].x,s[i].y);//得到該位置,有多少個可走方向
    }
    sort(s,s+8,cmp);//排序,[可走方向少的位置]在前面
    for(int i=0; i<2 && s[i].t<20; i++)//普通dfs搜索
    {
        // 只需要跑前2個方向（終點只有一個，不能同時存在兩個方向都無法到達）
        chess[s[i].x][s[i].y]=val;
        if(dfs(s[i].x,s[i].y,val+1)) return 1;
        chess[s[i].x][s[i].y]=0;
    }
    return 0;
}

int main()
{
    int x,y;
    cout<<"Please input the original place with m&n:"<<endl;
    cin>>x>>y;
    memset(chess,0,sizeof(chess));//每個格子的步數初始化為零
    chess[x][y]=1;
    if(dfs(x,y,2))
        for(int i=0; i<8; i++)//輸出棋盤
        {
            for(int j=0; j<7; j++)
                cout<<chess[i][j]<<" ";
            cout<<chess[i][7]<<endl;
        }
    else cout<<"It can't be found."<<endl;//找不到
    return 0;
}
