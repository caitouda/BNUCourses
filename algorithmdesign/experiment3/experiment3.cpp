#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

int main()
{
    int i;
    int count;//�yԇ�Ĵ��b����
    int temp;//ÿֻ���ĵȴ��r�g���R�rֵ
    int allwaittime;//���ȴ��r�g
    int waitcost;//���ȴ��M��
    float becount=0;//��ԓ���ĴΔ�
    float allcount=0;//�yԇ���Δ�
    float percent;//�yԇ����
    //int buildcost;
    cout<<"Please input the amount of the ships you want to test:";
    cin>>count;
    cout<<endl;
    int waittime[count];//�ȴ��r�g
    waittime[0]=0;//��һ�b���ĵȴ��r�g����
    int staytime[count];//ͣ���r�g
    int reachtime[count];//���b�����_�r�g��w��ǰһ�Ҵ���
    while(1)
    {
        for(i=0; i<count; i++)//�S�C�a��ͣ���r�g�͵��_�r�g��
        {
            staytime[i]=4+rand()%40;
            reachtime[i]=15+rand()%30;
        }
        allwaittime=0;
        for(i=1; i<count; i++)//ÿֻ���ĵȴ��r�g���ǰһ�b���ĵȴ��r�g��ͣ���r�g�p���_�r�g��
        {
            temp=waittime[i-1]+staytime[i-1]-reachtime[i-1];
            if(temp>0)waittime[i]=temp;
            else waittime[i]=0;
            allwaittime=allwaittime+waittime[i];
        }
        //cout<<allwaittime<<endl;
        waitcost=allwaittime*1000*24*12*5/count;//�ÿ����b���c�yԇ���b���ı�������ÿ��ȴ��M��
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

