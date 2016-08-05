#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;

int chess[8][8];
const int dx[]= {1,1,-2,-2,2,2,-1,-1};//��һ�������ҷ���
const int dy[]= {-2,2,1,-1,1,-1,2,-2};//��һ�������·���

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

int dfs(int x,int y,int val)//��ȃ����������㷨,�����߷����ٵ�λ��
{
    if(val>64) return 1;
    Node s[8];
    for(int i=0; i<8; i++)//ö�e��һ��λ��
    {
        s[i].x=x+dx[i];
        s[i].y=y+dy[i];
        if(s[i].x<0 || s[i].y<0 || s[i].x>=8|| s[i].y>=8) s[i].t=20;
        else if(chess[s[i].x][s[i].y]) s[i].t=20;
        else s[i].t=get_t(s[i].x,s[i].y);//�õ�ԓλ��,�ж��ق����߷���
    }
    sort(s,s+8,cmp);//����,[���߷����ٵ�λ��]��ǰ��
    for(int i=0; i<2 && s[i].t<20; i++)//��ͨdfs����
    {
        // ֻ��Ҫ��ǰ2�����򣨽K�cֻ��һ��������ͬ�r���ڃɂ����򶼟o�����_��
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
    memset(chess,0,sizeof(chess));//ÿ�����ӵĲ�����ʼ������
    chess[x][y]=1;
    if(dfs(x,y,2))
        for(int i=0; i<8; i++)//ݔ����P
        {
            for(int j=0; j<7; j++)
                cout<<chess[i][j]<<" ";
            cout<<chess[i][7]<<endl;
        }
    else cout<<"It can't be found."<<endl;//�Ҳ���
    return 0;
}
