#include<iostream>
#include<cstdlib>
using namespace std;

const int  DefaultSize = 10000;

class dataList  			//資料表類定義
{
private:
    int Vector[10000];		//存儲排序元素的向量
    int maxSize; 			//向量中最大元素個數
    int currentSize; 			//當前元素個數
public:
    dataList (int num)    //構造函數
    {
        for(int i=0; i<num; i++)
        {
            cin>>Vector[i];
        }
    }
    int Length()
    {
        return currentSize;    //取表長度
    }
    void Swap (int& x, int& y)
    {
        int temp = x;
        x = y;
        y = temp;
    }
    int& operator [](int i) 	//取第i個元素
    {
        return Vector[i];
    }
    int BinarySearch(int k1, const int low, const int high );
    int BinarySearch (int k1,int n);
};

int dataList::BinarySearch(int k1, const int low, const int high )
{
//二分檢索的遞迴演算法，用到E的重載操作“<”和“>”
    int mid = 0;		        //元素序號從1開始
    if (low <= high)
    {
        mid = (low + high)/2;
        if (Vector[mid-1] < k1)     //元素序號與下標差一
            mid = BinarySearch (k1, mid +1, high);
        else if (Vector[mid-1] > k1)
            mid = BinarySearch (k1, low, mid -1);
    }
    return mid;
}

int dataList::BinarySearch (int k1,int num)
{
//二分檢索的反覆運算演算法，用到E的重載操作“<”和“>”
    int high = num,  low = 1,  mid;    //元素序號從1開始
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (Vector[mid-1] < k1) low = mid+1;
        //右縮搜索區間
        else if (Vector[mid-1] > k1) high = mid-1;
        //左縮搜索區間
        else return mid;                 //搜索成功
    }
    return 0;                                  //搜索失敗
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
