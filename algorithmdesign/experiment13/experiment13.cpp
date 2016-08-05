#include <iostream>
using namespace std;

int N;
int T;
int data[100000];
int jihe[100000]; // 存儲子集元素，與輸入資料對應，用1標記

int GetJihe()
{
    int p = 0; // 指向當前值
    int temp = 0; // 當前子集合和
    while(p >= 0)
    {
        if(jihe[p]==0)
        {
            // 選中當前項
            jihe[p] = 1;
            temp += data[p];
            if(temp == T)
            {
                return 1;
            }
            else if(temp > T) //當前和超過了目標和，退而回溯
            {
                jihe[p] = 0;
                temp -= data[p];
            }
            p++;
        }
        if(p >= N) //已經搜索到最後一個數
        {
            while(jihe[p-1]==1)
            {
                p--;
                jihe[p] = 0;
                temp -= data[p];
                if(p < 1) return 0;
            }
            while(jihe[p-1] ==0)
            {
                p--;
                if(p < 1) return 0;
            }
            jihe[p-1] = 0;
            temp -= data[p-1];
        }
    }
    return 0;
}

int main()
{
    int i;
    cout<<"Please input the number of the datum:";
    cin >> N;
    cout<<"Please input the T you want to get:";
    cin >> T;
    cout<<"Please input the datum:";
    for(i = 0; i < N; i++)
    {
        cin>>data[i];
        jihe[i] = 0;
    }
    if(GetJihe())
    {
        cout<<"YES"<<endl;
        cout<<"The data choosen is:";
        for(i = 0; i < N; i++)
            if(jihe[i] == 1)
                cout<<data[i]<<" ";
        cout<<endl;
    }
    else
        cout<<"NO"<<endl;
    return 0;
}
