#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

int main()
{
    int i;
    int count;//測試的船隻數量
    int temp;//每只船的等待時間的臨時值
    int allwaittime;//總等待時間
    int waitcost;//總等待費用
    float becount=0;//應該建的次數
    float allcount=0;//測試總次數
    float percent;//測試概率
    //int buildcost;
    cout<<"Please input the amount of the ships you want to test:";
    cin>>count;
    cout<<endl;
    int waittime[count];//等待時間
    waittime[0]=0;//第一隻船的等待時間為零
    int staytime[count];//停留時間
    int reachtime[count];//兩隻船到達時間差，歸到前一艘船中
    while(1)
    {
        for(i=0; i<count; i++)//隨機產生停留時間和到達時間差
        {
            staytime[i]=4+rand()%40;
            reachtime[i]=15+rand()%30;
        }
        allwaittime=0;
        for(i=1; i<count; i++)//每只船的等待時間等於前一隻船的等待時間加停留時間減到達時間差
        {
            temp=waittime[i-1]+staytime[i-1]-reachtime[i-1];
            if(temp>0)waittime[i]=temp;
            else waittime[i]=0;
            allwaittime=allwaittime+waittime[i];
        }
        //cout<<allwaittime<<endl;
        waitcost=allwaittime*1000*24*12*5/count;//用總船隻數與測試船隻數的比例來求得總等待費用
        //cout<<waitcost<<endl;
        printf("The %0.f test:",allcount+1);
        printf("The waitcost is %d yuan\n",waitcost);
        allcount++;
        if(waitcost>13500000)
        {
            //cout<<"The dock should be built."<<endl;
            becount++;
        }
        // else
        //cout<<"The dock should't be built."<<endl;*/
        if(allcount==1000)break;
    }
    printf("The count of the dock being built is %.0f\n",becount);
    percent=becount/allcount;
    printf("The probability of the dock being built is %.2f\n",percent);
    if(percent<0.50)printf("The dock should't be built.\n");
    else printf("The dock should be built.\n");
    return 0;
}

