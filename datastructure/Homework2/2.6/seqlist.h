#ifndef SEQLIST_H
#define SEQLIST_H

#include <iostream>
using namespace std;

template <class T>
class SeqList
{
public:
	SeqList(int sz = defaultSize); // ���캯��
	SeqList(SeqList<T> &L);   // ���ƹ��캯��
	~SeqList();
	SeqList<T>& operator=(SeqList<T> &L); // ���ظ�ֵ�����
	int Size() const { return maxSize; } // ������������ɱ���ĸ���
	int Length() const { return last + 1; } // �������
	int Search(T& x) const;  // ����x�ڱ���λ�ã��������ر������
	int Locate(int i) const;  // ��λ��i������������ر������
	T getData(int i) const; // ȡ��i�������ֵ
	void setData(int i, T& x) { if (i > 0 && i <= last+1) data[i-1] = x; }; // ��x�޸ĵ�i�������ֵ
	bool Insert(int i, T& x);  // ����x�ڵڸ�����֮��
	bool Insert(T &x);
	bool Remove(int i, T &x);  // ɾ��x
	bool IsEmpty() const { return (last == -1) ? true : false; }  // �жϱ�շ�
	bool IsFull() const { return (last == maxSize-1)  true : false; }  // �жϱ�����
	void Input();  // ����
	void Output();  // ���
	void Removestot(int s,int t);
	void Removett(T &x,T &y);
private:
	T* data;  // �������
	int maxSize;  // �������ɱ��������
	int last;   // ��ǰ�Ѵ��������λ��(��0��ʼ)
	void reSize(const int newSize);
	enum { defaultSize = 100 };
};

// ���캯����ͨ��ָ������sz��������ĳ���
template <class T>
SeqList<T>::SeqList(int sz)
{
	if (sz > 0)
		maxSize = sz;
	else
		maxSize = defaultSize;

	last = -1;  // �ñ��ʵ�ʳ���Ϊ��
	data = new T[maxSize];  // ����˳���洢����
	if (data == 0) {    // ��̬����ʧ��
		cerr << "�洢�������!" << endl;
		exit (1);
	}
}

// ���ƹ��캯�����ò������и���������˳����ʼ���½���˳���
template <class T>
SeqList<T>::SeqList(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;

	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // ��̬����ʧ��
		cerr << "�洢�������!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}
}

// ���ظ�ֵ�����
template <class T>
SeqList<T>& SeqList<T>::operator=(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;

	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // ��̬����ʧ��
		cerr << "�洢�������!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}

	return *this;
}

// ��������
template <class T>
SeqList<T>::~SeqList()
{
	if (data != 0)
		delete []data;
}

// ˽�к���������˳���Ĵ洢����ռ��С���������Ԫ�ظ���ΪnewSize
template <class T>
void SeqList<T>::reSize(const int newSize) {
	if (newSize <= maxSize) {   // �������ĺ�����
		cerr << "��Ч�������С" <<endl;
		return;
	}

	T* newArray = new T[newSize];   // ����������
	if ( newArray == 0) { // ��̬����ʧ��
		cerr << "�洢�������!" << endl;
		if (data != 0) {
			delete []data;   // ��ֹ�ڴ�й©
		}
		exit(1);
	}

	for (int i= 0; i <= last; i++)
		newArray[i] = data[i];

	delete []data;  // ɾ��������

	data = newArray;
	maxSize = newSize;
}

// �����������ڱ���˳�����������ֵxƥ��ı���ҵ����������ǵڼ���Ԫ��
template <class T>
int SeqList<T>::Search(T& x) const
{
	for (int i = 0; i <= last; i++) {  // ˳������
		if (data[i] == x)
			return i+1;
	}

	return 0;   // ����ʧ��
}

// ��λ�������������ص�i(i<=i<=last+1)�������λ�ã�����������0����ʾ��λʧ��
template <class T>
int SeqList<T>::Locate(int i) const
{
	if (i >= 1&& i <= last+1)
		return i;
	else
		return 0;
}

// ȡ��i�������ֵ
template <class T>
T SeqList<T>:: getData(int i) const
{
	if (i > 0 && i <= last + 1)
		return data[i-1];
	else {
		cerr << "λ��" << i << "����ȷ" << endl;
		if (data != 0)
			delete []data;
		exit(1);
	}
}

// ���뺯��������x�ڵڸ�����֮��
template <class T>
bool SeqList<T>::Insert(int i, T& x)
{
	if (i < 0 && i > last + 1)  // ����i���������ܲ���
		return false;

	if (last == maxSize - 1)  // �����������
		reSize(maxSize + 10);
	for (int j = last; j >= i; j--)  // ���κ��ƣ��ճ���i��λ��
		data[j+1] = data[j];
	data[i] = x;          // ����
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

// ɾ���������ӱ���ɾ����i(1<=i<=last+1)�����ͨ��x���ر�ɾ����Ԫ��ֵ
template<class T>
bool SeqList<T>::Remove(int i, T &x)
{
	if (last == -1)  // ���
		return false;
	if (i < 1 || i > last + 1)  // ����i������
		return false;

	x = data[i-1];

	for (int j = i; j <= last; j++)
		data[j-1] = data[j];  // ����ǰ��

	last --;   // ���ȼ�1

	return true;
}

// ���뺯��������˳���
template <class T>
void SeqList<T>::Input()
{

	while (1) {
		cout << "��ʼ����˳������������Ԫ�ظ���: ";
		cin >> last;  // ����Ԫ�صĸ���
		last--;
		if ((last >= 0) && (last <= maxSize -1))
			break;
		cout << "��Ԫ�ظ����������󣬷�Χ���ܳ���" << maxSize-1 << "���Ҳ���С��0" << endl;
	}

	cout << "�������Ԫ�أ�" << endl;
	for (int i = 0; i <= last; i++) {  // ��������Ԫ��
		cout << "#" << i+1 << ": ";
		cin >> data[i];
	}
}

// �������
template <class T>
void SeqList<T>::Output()
{
	cout << "˳���Ϊ��" << endl;
	for (int i = 0; i <= last; i++)  // ��������Ԫ��
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
