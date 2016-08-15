#ifndef SEQLIST_H
#define SEQLIST_H

#include <iostream>
using namespace std;

template <class T>
class SeqList
{
public:
	SeqList(int sz = defaultSize); // 构造函数
	SeqList(SeqList<T> &L);   // 复制构造函数
	~SeqList();
	SeqList<T>& operator=(SeqList<T> &L); // 重载赋值运算符
	int Size() const { return maxSize; } // 计算表最大可容纳表项的个数
	int Length() const { return last + 1; } // 计算表长度
	int Search(T& x) const;  // 搜索x在表中位置，函数返回表项序号
	int Locate(int i) const;  // 定位第i个表项，函数返回表项序号
	T getData(int i) const; // 取第i个表项的值
	void setData(int i, T& x) { if (i > 0 && i <= last+1) data[i-1] = x; }; // 用x修改第i个表项的值
	bool Insert(int i, T& x);  // 插入x在第个表项之后
	bool Insert(T &x);
	bool Remove(int i, T &x);  // 删除x
	bool IsEmpty() const { return (last == -1) ? true : false; }  // 判断表空否
	bool IsFull() const { return (last == maxSize-1)  true : false; }  // 判断表满否
	void Input();  // 输入
	void Output();  // 输出
	void Removestot(int s,int t);
	void Removett(T &x,T &y);
private:
	T* data;  // 存放数组
	int maxSize;  // 最大可容纳表项的项数
	int last;   // 当前已存表项的最后位置(从0开始)
	void reSize(const int newSize);
	enum { defaultSize = 100 };
};

// 构造函数：通过指定参数sz定义数组的长度
template <class T>
SeqList<T>::SeqList(int sz)
{
	if (sz > 0)
		maxSize = sz;
	else
		maxSize = defaultSize;

	last = -1;  // 置表的实际长度为空
	data = new T[maxSize];  // 创建顺序表存储数组
	if (data == 0) {    // 动态分配失败
		cerr << "存储分配错误!" << endl;
		exit (1);
	}
}

// 复制构造函数：用参数表中给出的已有顺序表初始化新建的顺序表
template <class T>
SeqList<T>::SeqList(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;

	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // 动态分配失败
		cerr << "存储分配错误!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}
}

// 重载赋值运算符
template <class T>
SeqList<T>& SeqList<T>::operator=(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;

	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // 动态分配失败
		cerr << "存储分配错误!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}

	return *this;
}

// 析构函数
template <class T>
SeqList<T>::~SeqList()
{
	if (data != 0)
		delete []data;
}

// 私有函数：扩充顺序表的存储数组空间大小，新数组的元素个数为newSize
template <class T>
void SeqList<T>::reSize(const int newSize) {
	if (newSize <= maxSize) {   // 检查参数的合理性
		cerr << "无效的数组大小" <<endl;
		return;
	}

	T* newArray = new T[newSize];   // 建立新数组
	if ( newArray == 0) { // 动态分配失败
		cerr << "存储分配错误!" << endl;
		if (data != 0) {
			delete []data;   // 防止内存泄漏
		}
		exit(1);
	}

	for (int i= 0; i <= last; i++)
		newArray[i] = data[i];

	delete []data;  // 删除老数组

	data = newArray;
	maxSize = newSize;
}

// 搜索函数：在表中顺序搜索与给定值x匹配的表项，找到则函数返回是第几个元素
template <class T>
int SeqList<T>::Search(T& x) const
{
	for (int i = 0; i <= last; i++) {  // 顺序搜索
		if (data[i] == x)
			return i+1;
	}

	return 0;   // 搜索失败
}

// 定位函数：函数返回第i(i<=i<=last+1)个表项的位置，否则函数返回0，表示定位失败
template <class T>
int SeqList<T>::Locate(int i) const
{
	if (i >= 1&& i <= last+1)
		return i;
	else
		return 0;
}

// 取第i个表项的值
template <class T>
T SeqList<T>:: getData(int i) const
{
	if (i > 0 && i <= last + 1)
		return data[i-1];
	else {
		cerr << "位置" << i << "不正确" << endl;
		if (data != 0)
			delete []data;
		exit(1);
	}
}

// 插入函数：插入x在第个表项之后
template <class T>
bool SeqList<T>::Insert(int i, T& x)
{
	if (i < 0 && i > last + 1)  // 参数i不合理，不能插入
		return false;

	if (last == maxSize - 1)  // 表满，扩充表
		reSize(maxSize + 10);
	for (int j = last; j >= i; j--)  // 依次后移，空出第i号位置
		data[j+1] = data[j];
	data[i] = x;          // 插入
	last ++;

	return true;
}

template <class T>
bool SeqList<T>::Insert(T &x)
{
	int i; 
	if(last==-1)
	{
		i=0;
	}
	else
	{
	for( i=0; i<=last; i++)
		{
			if(data[i]>x) break;
		}
	}
	//cout<<i<<endl;
	return Insert(i,x);
}

// 删除函数：从表中删除第i(1<=i<=last+1)个表项，通过x返回被删除的元素值
template<class T>
bool SeqList<T>::Remove(int i, T &x)
{
	if (last == -1)  // 表空
		return false;
	if (i < 1 || i > last + 1)  // 参数i不合理
		return false;

	x = data[i-1];

	for (int j = i; j <= last; j++)
		data[j-1] = data[j];  // 依次前移

	last --;   // 表长度减1

	return true;
}

// 输入函数：建立顺序表
template <class T>
void SeqList<T>::Input()
{

	while (1) {
		cout << "开始建立顺序表，请输入表中元素个数: ";
		cin >> last;  // 输入元素的个数
		last--;
		if ((last >= 0) && (last <= maxSize -1))
			break;
		cout << "表元素个数输入有误，范围不能超过" << maxSize-1 << "，且不能小于0" << endl;
	}

	cout << "请输入表元素：" << endl;
	for (int i = 0; i <= last; i++) {  // 逐个输入表元素
		cout << "#" << i+1 << ": ";
		cin >> data[i];
	}
}

// 输出函数
template <class T>
void SeqList<T>::Output()
{
	cout << "顺序表为：" << endl;
	for (int i = 0; i <= last; i++)  // 逐个输出表元素
		cout << "#" << i+1 << ": " << data[i] << endl;
}

template <class T>
void SeqList<T>::Removestot(int s,int t)
{
	if(s>t)
	{
		cout<<"error:the order of s and t!!!"<<endl;
		return;
	}
	T mm;
	for(int i=t;i>=s;i--)
		Remove(i,mm);

}


template <class T>
void SeqList<T>::Removett(T &x,T &y)
{
	if(last==-1)
	{
		cout<<"error: null!!!"<<endl;
		return;
	}
	int s,t;
	
	for( s=0;s<=last;s++)
	{
		if(data[s]==x) break;

	}
	
	for(t=0; t<=last; t++)
	{
		if(data[t]==y) break;
	}
	if(s>last) 
	{
		cout<<"error:no such data as t!!!"<<endl;
		return ;
	}
	if(t>last) 
	{
		cout<<"error:no such data as t!!!"<<endl;
		return ;
	}
	Removestot(s+1,t+1);
}

#endif
