#include <iostream>
#include "MinHeap.h"
#include "Huffman.h"
#include "Huffmancode.h"
using namespace std;
int main()
{
    int n;
    while(1)
    {
        cout<<"Ոݔ���Y�ς���(��ݔ��0�t�˳�����";
        cin>>n;
        if(n==0)break;
        char str[n+1];
        int k[n+1];
        cout<<"Ոݔ���Y�ϵĴ�̖�������ę�ֵ��";
        for(int i=1; i<=n; i++)
        {
            cin>>str[i];
            cin>>k[i];
        }
        huffmancode<int> hfc(k,str,n);
        hfc.viewthecode();
        char st[100];
        cout<<"Ոݔ������Ҫ���a�Ĵ��a��";
        cin>>st;
        hfc.codethestr(st);
        cout<<"Ոݔ������Ҫ���g�Ĵ��a��";
        cin>>st;
        hfc.decodethestr(st);
    }
    return 0;
}
