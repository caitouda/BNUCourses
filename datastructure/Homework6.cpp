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
    int st_id,en_id;//���w���_�����еĴ�̖
    int st_time[2];//��ʼ�r�g
    int  en_time[2];//�Y���r�g
    double cost;//�M��
    int number;//�����C̖
};
struct Vertex
{
    int id;//��c��id
    string name;//�������Q
};

class Graph//ֻ�惦߅����c���໥���P�Sͨ�^߅�팤��
{
private:
    int num_vertex,num_edge;
    Vertex vertex[MAX];
    Edge edge[MAX];
    int pre[MAX];//ǰ�
public:
    void Inita();//��ʼ������·���Լ�������YӍ���M�D�e��
    int get_trantime(const Edge e);//�õ��˺���ĕr�g
    int  st_search(const string s);//������c��id
    void min_time(const string n1,const string n2);//���ٕr�g
    void  min_cost(const string n1,const string n2);//��С�M��
    void min_trans(const string n1,const string n2);//�����D�Q�Δ�
    void  Dijkstra(const string s,const int u,const int v);//���·���㷨
    void path(const int u,const int fir,const int sec);//ݔ�����·��
};

void Graph::Inita()
{
    num_vertex=8,num_edge=16;//���Д��� ������
    string s[9]= {"Shanghai","Beijing","Wuhan",
                  "Lasa","Kunming","Guangzhou","Xian","Wulumuqi"
                 };
    for(int i=1; i<=num_vertex; i++)//ݔ��������Q
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
        edge[i].number=num[i];//���฽�ϙC̖
        string n1=beg[i],n2;//n1�������w���д�̖
        if(i%2==0)n2=beg[i-1];//n2�����_�����д�̖
        else n2=beg[i+1];
        for(int j=1; j<=num_vertex; j++)//�õ����г��������w���_���µĴ�̖
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
                   };//�rᘺͷ��
    int top=0;
    for(int i=1; i<=num_edge; i++)
    {
        for(int j=0; j<=1; j++)
            edge[i].st_time[j]=st[top][j];//�����16, 20,
        ++top;
        for(int j=0; j<=1; j++)
            edge[i].en_time[j]=st[top][j];//����17 ,25,
        ++top;
    }//�õ����w�r�g�͵��_�r�g
    int cost[16]= {680 ,680 ,1150 ,1150,
                   930, 930, 1320, 1320,
                   830 ,830, 890, 890,
                   1480,  1480, 810 , 810
                  };
    for(int i=1; i<=num_edge; i++)
        edge[i].cost=cost[i-1];//�õ�ÿ�N������M��
}

int Graph :: get_trantime(const Edge e)//Ӌ���@�κ��໨�M�ĕr�g
{
    int end1=e.en_time[0],end2=e.en_time[1];
    int st1=e.st_time[0],st2=e.st_time[1];
    return (end1*60+end2-st1*60-st2);
}
int  Graph::st_search(const string s)//�����������Q�ҵ������Ĵ�̖
{
    for(int i=1; i<=num_vertex; i++)
        if(vertex[i].name==s)return vertex[i].id;
    return 0;
}
void Graph::min_time(const string n1,const string n2)//��̕r�g
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//�Ҳ��������ĳ������Q
    {
        cout<<"��������ݔ��������Q���`��\n";
        return ;
    }
    Dijkstra("time",u,v);
}
void Graph:: min_cost(const string n1,const string n2)//����M��
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//�Ҳ��������ĳ������Q
    {
        cout<<"��������ݔ��������Q���`��\n";
        return ;
    }
    cout<<u<<" "<<v<<endl;
    Dijkstra("cost",u,v);
}
void Graph :: min_trans(const string n1,const string n2)//�����D�Q�Δ�
{
    int u=st_search(n1);
    int v=st_search(n2);
    if(u==0||v==0)//�Ҳ��������ĳ������Q
    {
        cout<<"��������ݔ��������Q���`��\n";
        return ;
    }
    Dijkstra("trans",u,v);
}

void Graph:: Dijkstra(const string s,const int u,const int v)//���·����Dijkstra���㷨
{
    int dis[MAX],vis[MAX],second;
    memset(pre,0,sizeof(pre));//��ʼ��
    memset(dis,INF,sizeof(dis));
    memset(vis,0,sizeof(vis));
    dis[u]=0;
    vis[u]=1;
    pre[u]=-1;
    for(int i=1; i<=num_edge; i++)//�õ�·���ϵę�ֵ
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
    for(int i=1; i<num_vertex; i++)//�x���ڼ����e�����·�����c
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
        for(int j=1; j<=num_edge; j++)//�޸�
        {
            if(edge[j].st_id==fir&&(!vis[edge[j].en_id]))
            {
                int val,sec=edge[j].en_id;
                if(s=="time")
                {
                    //̎���Ƿ��г��F�r�g���e����r
                    int fir_fir=edge[e].en_time[0],fir_sec=edge[e].en_time[1];
                    int sec_fir=edge[j].st_time[0],sec_sec=edge[j].st_time[1];
                    if(fir_fir>sec_fir||(fir_fir==sec_fir&&fir_sec>sec_sec))
                        val=24*60-fir_fir*60-fir_sec+sec_fir*60+sec_sec*60;
                }
                else if(s=="cost")val=edge[j].cost;//̎���M��
                else val=1;//̎���D�Q�Δ�
                if(dis[sec]>dis[fir]+val)
                {
                    dis[sec]=dis[fir]+val;
                    pre[sec]=fir;
                }
            }
        }
    }

    path(u,pre[v],v);//ݔ�����·��
}

void Graph::path(const int u,const int fir,const int sec)//ݔ�����·��
{
    if(fir==-1)
        return ;
    path(u,pre[fir],fir);
    cout<<"���c��"<<vertex[fir].name<<"---->"<<"�_����"<<vertex[sec].name<<":\n";
    int i;
    for(i=1; i<=num_edge; i++)
    {
        if(edge[i].st_id==fir&&edge[i].en_id==sec)
            break;
    }
    cout<<"�����C�Σ�"<<edge[i].number<<endl;
    cout<<"���l�r�g��";
    int a[4];//�õ��r�g
    a[0]=edge[i].st_time[0],a[1]=edge[i].st_time[1];
    a[2]=edge[i].en_time[0],a[3]=edge[i].en_time[1];
    for(int i=0; i<2; i++)//ݔ�����_�r�g
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
    cout<<"���_�r�g��";
    for(int i=2; i<=3; i++)
    {
        if(a[i]==0)cout<<"00";
        else if(a[i]<10)cout<<"0"<<a[i];
        else cout<<a[i];
        if(i==2)cout<<":";
    }
    cout<<endl;//ݔ�������M��
    cout<<"Ʊ�r��"<<edge[i].cost<<endl;
    cout<<endl;
}
int main(void)
{
    Graph graph;
    graph.Inita();
    cout<<"�а������й����x��Shanghai,Beijing,Wuhan,Lasa,Kunming,Guangzhou,Xian,Wulumuqi\n";
    cout<<endl;
    cout<<"�����N��Q�߷��������x��\n��1���r�g��̣�2���M����С��3�����D�Δ�����\n";
    while(1)
    {
        int n;
        string n1,n2;
        char ch;
        cout<<"Ոݔ�����x������c�c�K�c��";
        cin>>n1>>n2;
        cout<<"Ոݔ�����x��ķ�����̖��";
        cin>>n;
        if(n==1)
            graph.min_time(n1,n2);
        else if(n==2)
            graph.min_cost(n1,n2);
        else if(n==3)
            graph.min_trans(n1,n2);
        else cout<<"���x��ķ������`��"<<endl;
        cout<<"Ո����߀���ԃ�᣿��y/n):";
        cin>>ch;
        if(ch=='n'||ch=='N')
        {
            cout<<endl<<"���x���Ĳ�ԃ";
            return 0;
        }
        else continue;
    }
    return 0;
}

