#include<iostream>
using namespace std;

#define	MAX 10

int Path[MAX];
int tempPath[MAX];
int Cost[MAX][MAX];
int allCost;
bool mark[MAX];//��ӛ�����Ƿ����x

int Input()//ݔ����Д�Ŀ�̓ɳ���֮�g���M��
{
    int num;
    cout<<"Please input the number of the citys:";
    cin>>num;
    cout<<"Please input the n * n matrix expression between any two cities' cost:"<<endl;
    for(int i=0; i<num; i++)
    {
        for(int j=0; j<num; j++)
        {
            cin>>Cost[i][j];
        }
    }
    return num;
}

void Output(int cityNum)//ݔ�����·���c��С�M��
{
    cout<<"The path costs least is:"<<endl;
    for(int i=0; i<cityNum; i++)
    {
        cout<<Path[i]<<" ";
    }
    cout<<endl<<"The least cost is:"<<allCost<<"yuan"<<endl;
}

void shortestPath(int Num,int cityNum)//���·�����㷨
{
    int tempCost=0;
    for(int i=0; i<cityNum; i++)
    {
        if(!mark[i])
        {
            mark[i]=true;
            tempPath[cityNum-Num]=i;
            if(Num>1)
            {
                shortestPath(Num-1,cityNum);//�fޒ
            }
            else
            {
                for(int j=0; j<cityNum-1; j++)
                {
                    tempCost+=Cost[tempPath[j]][tempPath[j+1]];//���M��
                }

                if(tempCost<allCost || allCost==0)//�ҵ���С�M�õ�·��
                {
                    allCost=tempCost;
                    for(int k=0; k<cityNum; k++)
                    {
                        Path[k]=tempPath[k];
                    }
                }
            }
            mark[i]=false;
        }

    }
}

int main()
{
    int cityNum = Input();
    shortestPath(cityNum,cityNum);
    Output(cityNum);
    return 0;
}
