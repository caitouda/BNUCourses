#include<iostream>
using namespace std;
#define Num 4
#define cNoEdge 99999

template<class Type>
class Traveling
{
    friend void main(void);
public:
    Type BBTSP(int v[]);
private:
    int n;		//圖G的頂點數
    Type **a,	//圖G的鄰接矩陣
         NoEdge,//圖G的無邊標誌
         cc,	//當前費用
         bestc;	//當前最小費用
};

template<class Type>
class MinHeapNode
{
    friend Traveling<Type>;
public:
    operator Type() const
    {
        return lcost;
    }
private:
    Type lcost,	//子樹費用下界
         cc,	//當前費用
         rcost;	//x[s:n - 1]中頂點最小的出邊費用
    int  s,		//根結點到當前結點的路徑為x[0:s]
         *x;	//需要進一步搜索的頂點是x[s + 1:n - 1]
};

template<typename Type>
class MinHeap
{
public:
    MinHeap(int size):m_nMaxSize(size)
        ,m_pheap(new Type[m_nMaxSize]),m_ncurrentsize(0) {}
    ~MinHeap()
    {
        delete[] m_pheap;
    }

public:
    bool Insert(const Type item);	//insert element
    bool DeleteMin(Type &item);	//delete element

private:
    //adjust the elements of the child tree with the root of start from top to bottom
    void Adjust(const int start, const int end);

private:
    const int m_nMaxSize;
    Type *m_pheap;
    int m_ncurrentsize;
};

template<typename Type>
void MinHeap<Type>::Adjust(const int start, const int end)
{
    int i = start,j = i*2+1;    //get the position of the child of i
    Type temp=m_pheap[i];
    while(j <= end)
    {
        if(j<end && (m_pheap[j]) > (m_pheap[j+1]))    //left>right
        {
            j++;
        }
        if((temp) <= (m_pheap[j]))  //adjust over
        {
            break;
        }
        else    //change the parent and the child, then adjust the child
        {
            m_pheap[i] = m_pheap[j];
            i = j;
            j = 2*i+1;
        }
    }
    m_pheap[i] = temp;
}

template<typename Type>
bool MinHeap<Type>::Insert(const Type item)
{
    if(m_ncurrentsize == m_nMaxSize)
    {
        return 0;
    }
    m_pheap[m_ncurrentsize] = item;
    int j = m_ncurrentsize, i = (j-1)/2;    //get the position of the parent of j
    Type temp = m_pheap[j];
    while(j > 0)    //adjust from bottom to top
    {
        if(m_pheap[i] <= temp)
        {
            break;
        }
        else
        {
            m_pheap[j] = m_pheap[i];
            j = i;
            i = (j-1)/2;
        }
    }
    m_pheap[j] = temp;
    m_ncurrentsize++;
    return 1;
}

template<typename Type>
bool MinHeap<Type>::DeleteMin(Type &item)
{
    if(0 == m_ncurrentsize)
    {
        return 0;
    }
//	for(int i=0; i<m_ncurrentsize; i++){
//		if( m_pheap[i] == item){
//			m_pheap[i] = m_pheap[m_ncurrentsize-1]; //filled with the last element
//			Adjust(i,m_ncurrentsize-2);     //adjust the tree with start of i
    item = m_pheap[0];
    m_pheap[0] = m_pheap[m_ncurrentsize-1];
    Adjust(0,m_ncurrentsize-2);
    m_ncurrentsize--;
//			i=0;
//		}
//	}
    return 1;
}

template<class Type>
Type Traveling<Type>::BBTSP(int v[])
{
    int i,j;
    //定義最小堆容量為1000
    MinHeap<MinHeapNode<Type> >H(1000);
    Type *MinOut=new Type [n+1];
    //計算MinOut[i] = 頂點i的最小出邊費用
    Type MinSum=0; //最小出邊費用和
    for(i = 1; i <= n; i++)
    {
        Type Min = NoEdge;
        for(j=1; j <= n; j++)
            if(a[i][j] != NoEdge && (a[i][j] < Min || Min == NoEdge))
                Min = a[i][j];
        if(Min == NoEdge) return NoEdge;
        MinOut[i] = Min;
        MinSum += Min;
    }
    //初始化
    MinHeapNode<Type> E;
    E.x=new int[n];
    for(i = 0; i < n; i++ )
        E.x[i] = i + 1;
    E.s = 0;
    E.cc = 0;
    E.rcost = MinSum;
    Type bestc = NoEdge;
    //搜索排列空間樹
    while (E.s < n - 1) //非葉結點
    {
        if (E.s == n - 2) //當前擴展結點是葉結點的父結點
        {
            //再加2條邊構成回路
            //所構成回路是否優於當前最優解
            if(a[E.x[n - 2]][E.x[n - 1]] != NoEdge &&
                    a[E.x[n - 1]][1] != NoEdge && (E.cc +
                                                   a[E.x[n - 2]][E.x[n - 1]] + a[E.x[n - 1]][1]
                                                   < bestc || bestc == NoEdge))
            {
                //費用更小回路
                bestc = E.cc + a[E.x[n - 2]][E.x[n - 1]] + a[E.x[n - 1]][1];
                E.cc = bestc;
                E.s++;
                H.Insert(E);
            }
            else delete [] E.x;
        }//捨棄擴展結點
        else //產生當前擴展結點的兒子結點
        {
            for(int i = E.s + 1; i < n; i++)
                if(a[E.x[E.s]][E.x[i]] != NoEdge)
                {
                    //可行兒子結點
                    Type cc = E.cc + a[E.x[E.s]][E.x[i]];
                    Type rcost = E.rcost - MinOut[E.x[E.s]];
                    Type b = cc + rcost;//下界
                    if(b < bestc || bestc == NoEdge)
                    {
                        //兒子可能含最優解
                        //結點插入最小堆
                        MinHeapNode< Type > N;
                        N.x = new int [n];
                        for (j = 0; j < n; j++)
                            N.x[j] =E.x[j];
                        N.x[E.s + 1] = E.x[i];
                        N.x[i] = E.x[E.s + 1];
                        N.cc = cc;
                        N.s = E.s + 1;
                        N.lcost = b;
                        N.rcost = rcost;
                        H.Insert(N);
                    }
                }
            delete [] E.x;
        }//完成結點擴展
//		try {H.DeleteMin(E);}
//		catch(OutOfBounds){break;}
        if(H.DeleteMin(E) == 0)
            break;
    }
    if(bestc == NoEdge) return NoEdge;//無回路
    //將最優級解複製到v[1:n]
    for(i = 0; i < n; i++)
        v[i + 1] = E.x[i];
    while(true) //釋放最小堆中所有結點
    {
        delete [] E.x;
//		try{H.DeleteMin(E);}
//		catch(OutOfBounds){break;}
        if(H.DeleteMin(E) == 0)
            break;
    }
    return bestc;
}

void main()
{
    int v[Num + 1],i;
    Traveling<int> Travel;
    Travel.NoEdge = cNoEdge;
    Travel.n = Num;
    Travel.a = new int *[Num];
    for(i = 0; i <= Num; i++)
    {
        Travel.a[i] = new int [Num];
    }
    Travel.a[1][1] = Travel.NoEdge;
    Travel.a[1][2] = 30;
    Travel.a[1][3] = 6;
    Travel.a[1][4] = 4;
    Travel.a[2][1] = 30;
    Travel.a[2][2] = Travel.NoEdge;
    Travel.a[2][3] = 5;
    Travel.a[2][4] = 10;
    Travel.a[3][1] = 6;
    Travel.a[3][2] = 5;
    Travel.a[3][3] = Travel.NoEdge;
    Travel.a[3][4] = 20;
    Travel.a[4][1] = 4;
    Travel.a[4][2] = 10;
    Travel.a[4][3] = 20;
    Travel.a[4][4] = Travel.NoEdge;
    if((i = Travel.BBTSP(v)) != Travel.NoEdge)
    {
        cout << "The optimal value is:" << i << endl;
        cout << "The optimal solution is:";
        for(i = 1; i <=	Num; i++)
            cout << v[i] << "-->";
        cout << "1" << endl;
    }
    else
        cout << "Free loop" << endl;
    getchar();
}
