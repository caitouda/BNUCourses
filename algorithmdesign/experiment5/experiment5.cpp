#include<iostream>
#include<cstdlib>
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
            cin>>Vector[i];
        }
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
    int BinarySearch(int k1, const int low, const int high );
    int BinarySearch (int k1,int n);
};

int dataList::BinarySearch(int k1, const int low, const int high )
{
//���֙z�����fޒ���㷨���õ�E�����d������<���͡�>��
    int mid = 0;		        //Ԫ����̖��1�_ʼ
    if (low <= high)
    {
        mid = (low + high)/2;
        if (Vector[mid-1] < k1)     //Ԫ����̖�c�˲�һ
            mid = BinarySearch (k1, mid +1, high);
        else if (Vector[mid-1] > k1)
            mid = BinarySearch (k1, low, mid -1);
    }
    return mid;
}

int dataList::BinarySearch (int k1,int num)
{
//���֙z���ķ����\�����㷨���õ�E�����d������<���͡�>��
    int high = num,  low = 1,  mid;    //Ԫ����̖��1�_ʼ
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (Vector[mid-1] < k1) low = mid+1;
        //�ҿs�����^�g
        else if (Vector[mid-1] > k1) high = mid-1;
        //��s�����^�g
        else return mid;                 //�����ɹ�
    }
    return 0;                                  //����ʧ��
}

int main()
{
    int n;
    cout<<"Please input the size of the increasing sequence of integers L with n:";
    cin>>n;
    cout<<"Please input the element of the increasing sequence of integers L in order:"<<endl;
    dataList d1(n);
    int k;
    cout<<"Please input the element you want to find:"<<endl;
    cin>>k;
    cout<<"The place of the element is:"<<endl<<d1.BinarySearch (k,n)<<endl;
    return 0;
}
