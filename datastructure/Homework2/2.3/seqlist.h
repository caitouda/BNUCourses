#ifndef SEQLIST_H
#define SEQLIST_H

#include <iostream>
using namespace std;

template <class T>
class SeqList
{
public:
	SeqList(int sz = defaultSize); // ���캯��
	SeqList(SeqList<T> &L);   // �}�u���캯��
	~SeqList();
	SeqList<T>& operator=(SeqList<T> &L); // ���d�O���\����
	int Size() const { return maxSize; } // Ӌ��������ݼ{��헵Ă���
	int Length() const { return last + 1; } // Ӌ����L��
	int Search(T& x) const;  // ����x�ڱ���λ�ã��������ر����̖
	int Locate(int i) const;  // ��λ��i����헣��������ر����̖
	T getData(int i) const; // ȡ��i����헵�ֵ
	void setData(int i, T& x) { if (i > 0 && i <= last+1) ? data[i-1] = x; } // ��x�޸ĵ�i����헵�ֵ
	bool Insert(int i, T& x);  // ����x�ڵڂ����֮��
	bool Remove(int i, T &x);  // �h��x
	bool IsEmpty() const { return (last == -1) ? true : false; }  // �Д��շ�
	bool IsFull() const { return (last == maxSize-1) ? true : false; }  // �Д��M��
	void Input();  // ݔ��
	void Output();  // ݔ��
private:
	T* data;  // ������
	int maxSize;  // �����ݼ{��헵�헔�
	int last;   // ��ǰ�Ѵ��헵�����λ��(��0�_ʼ)
	void reSize(const int newSize);
	enum { defaultSize = 100 };
};

// ���캯����ͨ�^ָ������sz���x��е��L��
template <class T>
SeqList<T>::SeqList(int sz)
{
	if (sz > 0) 
		maxSize = sz;
	else 
		maxSize = defaultSize;
	
	last = -1;  // �ñ�Č��H�L�Ȟ��
	data = new T[maxSize];  // ��������惦���
	if (data == 0) {    // �ӑB����ʧ��
		cerr << "�惦�����e�`!" << endl;
		exit (1);
	}
}

// �}�u���캯�����Å������нo�������������ʼ���½�������
template <class T>
SeqList<T>::SeqList(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;
	
	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // �ӑB����ʧ��
		cerr << "�惦�����e�`!" << endl;
		exit (1);
	}

	for (int i = 0; i <= last; i++) {
		data[i] = L.data[i];
	}
}

// ���d�O���\����
template <class T>
SeqList<T>& SeqList<T>::operator=(SeqList<T> &L)
{
	maxSize = L.maxSize;
	last = L.last;
	
	if (data != 0) delete []data;
	data = new T[maxSize];
	if (data == 0) { // �ӑB����ʧ��
		cerr << "�惦�����e�`!" << endl;
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

// ˽�к������U������Ĵ惦��п��g��С������е�Ԫ�؂�����newSize
template <class T>
void SeqList<T>::reSize(const int newSize) {
	if (newSize <= maxSize) {   // �z�酢���ĺ�����
		cerr << "�oЧ����д�С" <<endl;
		return;
	}

	T* newArray = new T[newSize];   // ���������
	if ( newArray == 0) { // �ӑB����ʧ��
		cerr << "�惦�����e�`!" << endl;
		if (data != 0) {
			delete []data;   // ��ֹӛ���w��©
		}
		exit(1);
	}	
	
	for (int i= 0; i <= last; i++)
		newArray[i] = data[i];

	delete []data;  // �h�������

	data = newArray;
	maxSize = newSize;
}

// �����������ڱ���ѭ���ь��c�o��ֵxƥ��ı�헣��ҵ��t���������ǵڎׂ�Ԫ��
template <class T>
int SeqList<T>::Search(T& x) const
{
	for (int i = 0; i <= last; i++) {  // ѭ���ь�
		if (data[i] == x)   
			return i+1;
	}

	return 0;   // ����ʧ��		
}

// ��λ�������������ص�i(i<=i<=last+1)����헵�λ�ã���t��������0����ʾ��λʧ��
template <class T>
int SeqList<T>::Locate(int i) const 
{
	if (i >= 1&& i <= last+1)
		return i;
	else
		return 0;
}

// ȡ��i����헵�ֵ
template <class T>
T SeqList<T>:: getData(int i) const
{
	if (i > 0 && i <= last + 1)
		return data[i-1];
	else {
		cerr << "λ��" << i << "�����_" << endl;
		if (data != 0)
			delete []data;
		exit(1);
	}
}

// ���뺯��������x�ڵڂ����֮��
template <class T>
bool SeqList<T>::Insert(int i, T& x)  
{
	if (i < 0 && i > last + 1)  // ����i���������ܲ���
		return false;

	if (last == maxSize - 1)  // ��M���U���
		reSize(maxSize + 10);
	for (int j = last; j >= i; j--)  // �������ƣ��ճ���i̖λ��
		data[j+1] = data[j];
	data[i] = x;          // ����
	last ++;

	return true;
}

// �h���������ı��Єh����i(1<=i<=last+1)����헣�ͨ�^x���ر��h����Ԫ��ֵ
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
	
	last --;   // ���L�Ȝp1

	return true;
}

// ݔ�뺯������������
template <class T>
void SeqList<T>::Input()
{
	
	while (1) {
		cout << "�_ʼ��������Ոݔ�����Ԫ�؂���: ";
		cin >> last;  // ݔ��Ԫ�صĂ���
		last--;
		if ((last >= 0) && (last <= maxSize -1)) 
			break;
		cout << "��Ԫ�؂���ݔ�����`���������ܳ��^" << maxSize-1 << "���Ҳ���С�0" << endl;
	}

	cout << "Ոݔ���Ԫ�أ�" << endl;
	for (int i = 0; i <= last; i++) {  // ����ݔ���Ԫ��
		cout << "#" << i+1 << ": ";
		cin >> data[i];		
	}
}

// ݔ������
template <class T>
void SeqList<T>::Output()
{
	cout << "����飺" << endl;
	for (int i = 0; i <= last; i++)  // ����ݔ����Ԫ��
		cout << "#" << i+1 << ": " << data[i] << endl;			
}

#endif
