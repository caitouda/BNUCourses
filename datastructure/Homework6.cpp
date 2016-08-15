#include <iostream>
#include <algorithm>
#include <cstring>
#include <queue>
#include <stack>

using namespace std;

const int MAX=100;
const int INF=1000000;
struct Edge
{
    int st_id,en_id;//起飛和開往城市的代號
    int st_time[2];//起始時間
    int  en_time[2];//結束時間
    double cost;//費用
    int number;//航班機號
};
struct Vertex
{
    int id;//頂點的id
    string name;//城市名稱
};

class Graph//只存儲邊和頂點，相互的關係通過邊來尋找
{
private:
    int num_vertex,num_edge;
    Vertex vertex[MAX];
    Edge edge[MAX];
    int pre[MAX];//前驅
public:
    void Inita();//初始化，把路線以及航班的資訊存進圖裡面
    int get_trantime(const Edge e);//得到乘航班的時間
    int  st_search(const string s);//尋找頂點的id
    void min_time(const string n1,const string n2);//最少時間
    void  min_cost(const string n1,const string n2);//最小費用
    void min_trans(const string n1,const string n2);//最少轉換次數
    void  Dijkstra(const string s,const int u,const int v);//最短路演算法
    void path(const int u,const int fir,const int sec);//輸出最短路徑
};

void Graph::Inita()
{
    num_vertex=8,num_edge=16;//城市數量 航班數
    string s[9]= {"Shanghai","Beijing","Wuhan",
                  "Lasa","Kunming","Guangzhou","Xian","Wulumuqi"
                 };
    for(int i=1; i<=num_vertex; i++)//輸入城市名稱
    {
        vertex[i].name=s[i-1];
        vertex[i].id=i;
    }
    int num[17]= {0,6320,6320 ,2104,2104,201,201,
                  2323 ,2323 ,173 ,173 ,3304 ,3304,
                  82 ,82 ,4723 ,4723
                 };
    string  beg[17]= {"","Beijing","Shanghai","Beijing",
                      "Wulumuqi","Beijing","Xian","Xian","Guangzhou","Lasa","Kunming",
                      "Lasa","Wuhan","Wulumuqi","Kunming","Wuhan","Guangzhou",
                     };
    for(int i=1; i<=num_edge; i++)
    {
        edge[i].number=num[i];//航班附上機號
        string n1=beg[i],n2;//n1附上起飛城市代號
        if(i%2==0)n2=beg[i-1];//n2附上開往城市代號
        else n2=beg[i+1];
        for(int j=1; j<=num_vertex; j++)//得到所有城市在起飛和開往下的代號
        {
            if(n1==vertex[j].name)
            {
                edge[i].st_id=vertex[j].id;
            }
        }
        for(int j=1; j<=num_vertex; j++)
            if(n2==vertex[j].name)edge[i].en_id=vertex[j].id;
    }
    int st[32][2]= {16, 20,
                    17 ,25,
                    18 ,0,
                    19 ,5,
                    8 , 0,
                    9  ,55,
                    10 ,45,
                    11 ,40,
                    15 ,25,
                    17 ,0,
                    12 ,35,
                    14 ,15,
                    7  ,15,
                    9  ,35,
                    10 ,15,
                    11 ,35,
                    10 ,20,
                    11 ,45,
                    12 ,35,
                    14 ,0,
                    14 ,15,
                    15 ,45,
                    16 ,25,
                    17 ,55,
                    9  ,30,
                    12 ,15,
                    13 ,5,
                    15 ,50,
                    7  ,5,
                    8 ,45,
                    11 ,25,
                    13 ,5
                   };//時針和分針
    int top=0;
    for(int i=1; i<=num_edge; i++)
    {
        for(int j=0; j<=1; j++)
            edge[i].st_time[j]=st[top][j];//比如存16, 20,
        ++top;
        for(int j=0; j<=1; j++)
            edge[i].en_time[j]=st[top][j];//比如17 ,25,
        ++top;
    }//得到起飛時間和到達時間
    int cost[16]= {680 ,680 ,1150 ,1150,
                   930, 930, 1320, 1320,
                   830 ,830, 890, 890,
                   1480,  1480, 810 , 810
                  };
    for(int i=1; i<=num_edge; i++)
        edge[i].cost=cost[i-1];//得到每種航班的費用
}

int Graph :: get_trantime(const Edge e)//計算這次航班花費的時間
{
    int end1=e.en_time[0],end2=e.en_time[1];
    int st1=e.st_time[0],st2=e.st_time[1];
    return (end1*60+end2-st1*60-st2);
}
int  Graph::st_search(const string s)//根據城市名稱找到對應的代號
{
    for(int i=1; i<=num_vertex; i++)
        if(vertex[i].name==s)return vertex[i].id;
    return 0;
}
void Graph::min_time(const string n1,const string n2)//最短時間
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//找不到對應的城市名稱
    {
        cout<<"對不起，您輸入城市名稱有誤！\n";
        return ;
    }
    Dijkstra("time",u,v);
}
void Graph:: min_cost(const string n1,const string n2)//最短費用
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//找不到對應的城市名稱
    {
        cout<<"對不起，您輸入城市名稱有誤！\n";
        return ;
    }
    cout<<u<<" "<<v<<endl;
    Dijkstra("cost",u,v);
}
void Graph :: min_trans(const string n1,const string n2)//最少轉換次數
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//找不到對應的城市名稱
    {
        cout<<"對不起，您輸入城市名稱有誤！\n";
        return ;
    }
    Dijkstra("trans",u,v);
}

void Graph:: Dijkstra(const string s,const int u,const int v)//最短路徑的Dijkstra演算法
{
    int dis[MAX],vis[MAX],second;
    memset(pre,0,sizeof(pre));//初始化
    memset(dis,INF,sizeof(dis));
    memset(vis,0,sizeof(vis));
    dis[u]=0;
    vis[u]=1;
    pre[u]=-1;
    for(int i=1; i<=num_edge; i++)//得到路徑上的權值
    {
        int en=edge[i].en_id;
        if(edge[i].st_id==u)
        {
            if(s=="time")
                dis[en]=get_trantime(edge[i]);
            else if(s=="cost")
                dis[en]=edge[i].cost;
            else dis[en]=1;
            pre[en]=u;
        }

    }
    for(int i=1; i<num_vertex; i++)//選擇不在集合裡有最短路徑的點
    {
        int min=INF,fir=u;
        for(int j=1; j<=num_vertex; j++)
        {
            if(!vis[j]&&dis[j]<min)
            {
                fir=j;
                min=dis[j];
            }
        }
        vis[fir]=1;
        int e;
        for(int q=1; q<=num_edge; q++)
        {
            if(edge[q].st_id==u&&edge[q].en_id==fir)
            {
                e=q;
                break;
            }
        }
        for(int j=1; j<=num_edge; j++)//修改
        {
            if(edge[j].st_id==fir&&(!vis[edge[j].en_id]))
            {
                int val,sec=edge[j].en_id;
                if(s=="time")
                {
                    //處理是否有出現時間交錯的情況
                    int fir_fir=edge[e].en_time[0],fir_sec=edge[e].en_time[1];
                    int sec_fir=edge[j].st_time[0],sec_sec=edge[j].st_time[1];
                    if(fir_fir>sec_fir||(fir_fir==sec_fir&&fir_sec>sec_sec))
                        val=24*60-fir_fir*60-fir_sec+sec_fir*60+sec_sec*60;
                }
                else if(s=="cost")val=edge[j].cost;//處理費用
                else val=1;//處理轉換次數
                if(dis[sec]>dis[fir]+val)
                {
                    dis[sec]=dis[fir]+val;
                    pre[sec]=fir;
                }
            }
        }
    }

    path(u,pre[v],v);//輸出最短路徑
}

void Graph::path(const int u,const int fir,const int sec)//輸出最短路徑
{
    if(fir==-1)
        return ;
    path(u,pre[fir],fir);
    cout<<"起點："<<vertex[fir].name<<"---->"<<"開往："<<vertex[sec].name<<":\n";
    int i;
    for(i=1; i<=num_edge; i++)
    {
        if(edge[i].st_id==fir&&edge[i].en_id==sec)
            break;
    }
    cout<<"航班機次："<<edge[i].number<<endl;
    cout<<"出發時間：";
    int a[4];//得到時間
    a[0]=edge[i].st_time[0],a[1]=edge[i].st_time[1];
    a[2]=edge[i].en_time[0],a[3]=edge[i].en_time[1];
    for(int i=0; i<2; i++)//輸出到達時間
    {

        if(a[i]==0)
        {
            cout<<"00";
        }
        else if(a[i]<10)
        {
            cout<<"0"<<a[i];
        }
        else cout<<a[i];
        if(i==0)cout<<":";
    }
    cout<<endl;
    cout<<"到達時間：";
    for(int i=2; i<=3; i++)
    {
        if(a[i]==0)cout<<"00";
        else if(a[i]<10)cout<<"0"<<a[i];
        else cout<<a[i];
        if(i==2)cout<<":";
    }
    cout<<endl;//輸出所花費用
    cout<<"票價："<<edge[i].cost<<endl;
    cout<<endl;
}
int main(void)
{
    Graph graph;
    graph.Inita();
    cout<<"有八座城市供您選擇：Shanghai,Beijing,Wuhan,Lasa,Kunming,Guangzhou,Xian,Wulumuqi\n";
    cout<<endl;
    cout<<"有三種最優決策方案供您選擇：\n（1）時間最短（2）費用最小（3）中轉次數最少\n";
    while(1)
    {
        int n;
        string n1,n2;
        char ch;
        cout<<"請輸入你選擇的起點與終點：";
        cin>>n1>>n2;
        cout<<"請輸入您選擇的方案編號：";
        cin>>n;
        if(n==1)
            graph.min_time(n1,n2);
        else if(n==2)
            graph.min_cost(n1,n2);
        else if(n==3)
            graph.min_trans(n1,n2);
        else cout<<"你選擇的方案有誤！"<<endl;
        cout<<"請問您還想查詢嗎？（y/n):";
        cin>>ch;
        if(ch=='n'||ch=='N')
        {
            cout<<endl<<"感謝您的查詢";
            return 0;
        }
        else continue;
    }
    return 0;
}

