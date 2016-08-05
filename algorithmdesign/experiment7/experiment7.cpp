#include <iostream>
#include<cstdlib>
#include<cstdio>
using namespace std;

const int  DefaultSize = 10000;

class dataList  			//�Y�ϱ���x
{
private:
    int Vector[10000];		//�惦����Ԫ�ص�����
    int a[10000];
    int maxSize; 			//���������Ԫ�؂���
    int currentSize; 			//��ǰԪ�؂���
public:
    dataList (int num)    //���캯��
    {
        for(int i=1; i<=num; i++)
        {
            Vector[i]=rand()%100;
            a[i]=Vector[i];
            cout<<Vector[i]<<" ";
        }
        cout<<endl;
    }
    int Length()
    {
        return currentSize;    //ȡ���L��
    }
    void Swap (int& x, int& y)
    {
        int temp = x;
        x = y;
        y = temp;
    }
    int& operator [](int i) 	//ȡ��i��Ԫ��
    {
        return Vector[i];
    }
    int Partition (const int low, const int high);
    //�������򄝷�
    int show(int num,int K);
};

int dataList::Partition (const int low, const int high)
{
//�Y�ϱ�Ĺ��к���
    int pivotpos = low;
    int pivot = Vector[low];	  //����Ԫ��
    for (int i = low+1; i <= high; i++)
        //�z�y��������, �M�Є���
        if (Vector[i] < pivot)
        {
            pivotpos++;
            if (pivotpos != i)
                Swap(Vector[pivotpos],Vector[i]);
        }				//С춻��ʵĽ��Q�����ȥ
    Vector[low] = Vector[pivotpos];
    Vector[pivotpos] = pivot;								//������Ԫ�ؾ�λ
    return pivotpos;	//���ػ���Ԫ��λ��
}

int QuickSort (dataList& L, const int left, const int right, int K)
{
//��Ԫ��Vector[left], ..., Vector[right]�M������,
//pivot=L.Vector[left]�ǻ���Ԫ��, ����Y��������
//λ����pivotPos, �х�����������зֳɃɲ���,
//��߅Ԫ�ص�����a��С춻�����, ��߅�������
    if (left < right)  		//Ԫ�������L�ȴ��1�r
    {
        int pivotpos = L.Partition (left, right);    //����
        if(K<=pivotpos-left)
            QuickSort (L, left, pivotpos-1,K-left+1);
        else
            QuickSort (L, pivotpos+1, right,K-pivotpos);
    }
}

int dataList::show(int num,int K)
{
    printf("The %d smallest data is %d\n",K,Vector[K]);
    for(int i=1; i<=num; i++)
        if(a[i]==Vector[K])cout<<"The place of the data is "<<i<<" ";
}

int main()
{
    int n;
    int k;
    cout<<"Please input the number of the data:";
    cin>>n;
    cout<<"The orinal data:"<<endl;
    dataList dl(n);
    cout<<"Please input K:"<<endl;
    cin>>k;
    int QuickSort (dataList& L, const int left, const int right, int K);
    QuickSort(dl,1,n,k);
    dl.show(n,k);
    return 0;
}
