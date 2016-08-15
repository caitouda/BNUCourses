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
        cout<<"請輸入資料個數(當輸入0則退出）：";
        cin>>n;
        if(n==0)break;
        char str[n+1];
        int k[n+1];
        cout<<"請輸入資料的代號及對應的權值：";
        for(int i=1; i<=n; i++)
        {
            cin>>str[i];
            cin>>k[i];
        }
        huffmancode<int> hfc(k,str,n);
        hfc.viewthecode();
        char st[100];
        cout<<"請輸入你需要編碼的代碼：";
        cin>>st;
        hfc.codethestr(st);
        cout<<"請輸入你需要翻譯的代碼：";
        cin>>st;
        hfc.decodethestr(st);
    }
    return 0;
}
