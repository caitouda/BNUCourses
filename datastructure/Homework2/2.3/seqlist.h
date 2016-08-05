#ifndef SEQLIST_H
#define SEQLIST_H

#include <iostream>
using namespace std;

template <class T>
class SeqList
{
public:
	SeqList(int sz = defaultSize); // 構造函數
	SeqList(SeqList<T> &L);   // 複製構造函數
	~SeqList();
	SeqList<T>& operator=(SeqList<T> &L); // 重載設定運算子
	int Size() const { return maxSize; } // 計算表最大可容納表項的個數
	int Length() const { return last + 1; } // 計算表長度
	int Search(T& x) const;  // 搜索x在表中位置，函數返回表項序號
	int Locate(int i) const;  // 定位第i個表項，函數返回表項序號
	T getData(int i) const; // 取第i個表項的值
	void setData(int i, T& x) { if (i > 0 && i <= last+1) ? data[i-1] = x; } // 用x修改第i個表項的值
	bool Insert(int i, T& x);  // 插入x在第個表項之後
	bool Remove(int i, T &x);  // 刪除x
	bool IsEmpty() const { return (last == -1) ? true : false; }  // 判斷表空否
	bool IsFull() const { return (last == maxSize-1) ? true : false; }  // 判斷表滿否
	void Input();  // 輸入
	void Output();  // 輸出
private:
	T* data;  // 存放陣列
	int maxSize;  // 最大可容納表項的項數
	int last;   // 當前已存表項的最後位置(從0開始)
	void reSize(const int newSize);
	enum { defaultSize = 100 };
};

// 構造函數：通過指定參數sz定義陣列的長度
template <class T>
SeqList<T>::SeqList(int sz)
{
	if (sz > 0) 
		maxSize = sz;
	else 
		maxSize = defaultSize;
	
	last = -1;  // 置表的實際長度為空
	data = new T[maxSize];  // 創建順序表存儲陣列
	if (data == 0) {    // 動態分配失敗
		cerr << "存儲分配錯誤!" << endl;
		exit (1);
	}
}

// 複製構造函數：用參數表中給出的已有順序表初始化新建的順序表
template <class T>
SeqList<T>::SeqList(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;
	
	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // 動態分配失敗
		cerr << "存儲分配錯誤!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}
}

// 重載設定運算子
template <class T>
SeqList<T>& SeqList<T>::operator=(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;
	
	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // 動態分配失敗
		cerr << "存儲分配錯誤!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}
	
	return *this;
}

// 析構函數
template <class T>
SeqList<T>::~SeqList()
{
	if (data != 0)
		delete []data; 
}

// 私有函數：擴充順序表的存儲陣列空間大小，新陣列的元素個數為newSize
template <class T>
void SeqList<T>::reSize(const int newSize) {
	if (newSize <= maxSize) {   // 檢查參數的合理性
		cerr << "無效的陣列大小" <<endl;
		return;
	}

	T* newArray = new T[newSize];   // 建立新陣列
	if ( newArray == 0) { // 動態分配失敗
		cerr << "存儲分配錯誤!" << endl;
		if (data != 0) {
			delete []data;   // 防止記憶體洩漏
		}
		exit(1);
	}	
	
	for (int i= 0; i <= last; i++)
		newArray[i] = data[i];

	delete []data;  // 刪除老陣列

	data = newArray;
	maxSize = newSize;
}

// 搜索函數：在表中循序搜尋與給定值x匹配的表項，找到則函數返回是第幾個元素
template <class T>
int SeqList<T>::Search(T& x) const
{
	for (int i = 0; i <= last; i++) {  // 循序搜尋
		if (data[i] == x)   
			return i+1;
	}

	return 0;   // 搜索失敗		
}

// 定位函數：函數返回第i(i<=i<=last+1)個表項的位置，否則函數返回0，表示定位失敗
template <class T>
int SeqList<T>::Locate(int i) const 
{
	if (i >= 1&& i <= last+1)
		return i;
	else
		return 0;
}

// 取第i個表項的值
template <class T>
T SeqList<T>:: getData(int i) const
{
	if (i > 0 && i <= last + 1)
		return data[i-1];
	else {
		cerr << "位置" << i << "不正確" << endl;
		if (data != 0)
			delete []data;
		exit(1);
	}
}

// 插入函數：插入x在第個表項之後
template <class T>
bool SeqList<T>::Insert(int i, T& x)  
{
	if (i < 0 && i > last + 1)  // 參數i不合理，不能插入
		return false;

	if (last == maxSize - 1)  // 表滿，擴充表
		reSize(maxSize + 10);
	for (int j = last; j >= i; j--)  // 依次後移，空出第i號位置
		data[j+1] = data[j];
	data[i] = x;          // 插入
	last ++;

	return true;
}

// 刪除函數：從表中刪除第i(1<=i<=last+1)個表項，通過x返回被刪除的元素值
template<class T>
bool SeqList<T>::Remove(int i, T &x)
{
	if (last == -1)  // 表空
		return false;
	if (i < 1 || i > last + 1)  // 參數i不合理
		return false;

	x = data[i-1];

	for (int j = i; j <= last; j++)
		data[j-1] = data[j];  // 依次前移
	
	last --;   // 表長度減1

	return true;
}

// 輸入函數：建立順序表
template <class T>
void SeqList<T>::Input()
{
	
	while (1) {
		cout << "開始建立順序表，請輸入表中元素個數: ";
		cin >> last;  // 輸入元素的個數
		last--;
		if ((last >= 0) && (last <= maxSize -1)) 
			break;
		cout << "表元素個數輸入有誤，範圍不能超過" << maxSize-1 << "，且不能小於0" << endl;
	}

	cout << "請輸入表元素：" << endl;
	for (int i = 0; i <= last; i++) {  // 逐個輸入表元素
		cout << "#" << i+1 << ": ";
		cin >> data[i];		
	}
}

// 輸出函數
template <class T>
void SeqList<T>::Output()
{
	cout << "順序表為：" << endl;
	for (int i = 0; i <= last; i++)  // 逐個輸出表元素
		cout << "#" << i+1 << ": " << data[i] << endl;			
}

#endif
