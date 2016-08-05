#include <iostream>
#include<cstdlib>
#include<ctime>
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
            Vector[i]=rand()%100;
            cout<<Vector[i]<<" ";
        }
        cout<<endl;
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
    int Partition (const int low, const int high);
    //快速排序劃分
    int show(int num);
};

int dataList::Partition (const int low, const int high)
{
//資料表類的共有函數
    int pivotpos = low;
    int pivot = Vector[low];	  //基準元素
    for (int i = low+1; i <= high; i++)
        //檢測整個序列, 進行劃分
        if (Vector[i] < pivot)
        {
            pivotpos++;
            if (pivotpos != i)
                Swap(Vector[pivotpos],Vector[i]);
        }				//小於基準的交換到左側去
    Vector[low] = Vector[pivotpos];
    Vector[pivotpos] = pivot;								//將基準元素就位
    return pivotpos;	//返回基準元素位置
}

void QuickSort (dataList& L, const int left, const int right)
{
//對元素Vector[left], ..., Vector[right]進行排序,
//pivot=L.Vector[left]是基準元素, 排序結束後它的
//位置在pivotPos, 把參加排序的序列分成兩部分,
//左邊元素的排序碼都小於或等於它, 右邊都大於它
    if (left < right)  		//元素序列長度大於1時
    {
        int pivotpos = L.Partition (left, right);    //劃分
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







