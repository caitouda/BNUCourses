#include <iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

const int  DefaultSize = 10000;

class dataList  			//�Y�ϱ���x
{
private:
    int Vector[10000];		//�惦����Ԫ�ص�����
    int maxSize; 			//���������Ԫ�؂���
    int currentSize; 			//��ǰԪ�؂���
public:
    dataList (int num)    //���캯��
    {
        for(int i=0; i<num; i++)
        {
            Vector[i]=rand()%100;
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
    int show(int num);
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

void QuickSort (dataList& L, const int left, const int right)
{
//��Ԫ��Vector[left], ..., Vector[right]�M������,
//pivot=L.Vector[left]�ǻ���Ԫ��, ����Y��������
//λ����pivotPos, �х�����������зֳɃɲ���,
//��߅Ԫ�ص�����a��С춻�����, ��߅�������
    if (left < right)  		//Ԫ�������L�ȴ��1�r
    {
        int pivotpos = L.Partition (left, right);    //����
        QuickSort (L, left, pivotpos-1);
        QuickSort (L, pivotpos+1, right);
    }
}

int dataList::show(int num)
{
    for(int i=0; i<num; i++)
        cout<<Vector[i]<<" ";
}

int main()
{
    int n;
    cout<<"Please input the number of the data:";
    cin>>n;
    cout<<"The orinal data:"<<endl;
    dataList dl(n);
    int a=clock();
    void QuickSort (dataList& L, const int left, const int right);
    QuickSort(dl,0,n-1);
    int b=clock();
    int tt=b-a;
    cout<<"The order data:"<<endl;
    dl.show(n);
    cout<<endl<<"Time is "<<tt<<"ms"<<endl;
    return 0;
}







