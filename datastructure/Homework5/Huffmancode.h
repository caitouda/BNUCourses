#ifndef HUFFMANCODE_H_INCLUDE
#define HUFFMANCODE_H_INCLUDE
#include "Huffman.h"
#include "MinHeap.h"
#include <iostream>
#include <cstring>
using namespace std;
template <class E>
struct huffmancodeNode
{
    E data;
    char ch;
    char code[10];
    int len;
};

template <class E>
class huffmancode
{
public:
    huffmancode(E w[],char* cc,int n);
    void viewthecode();
    void codethestr(char * str);
    void decodethestr(char * str);
protected:
    huffmancodeNode <E> dcode[100];
    int tot;
};

template <class E>
huffmancode<E>::huffmancode(E w[],char* cc,int n)
{
    HuffmanTree<E> hft(w,cc,n);
    for(int i=1; i<=n; i++)
    {
        char *a = new char [10];
        int llen;
        char cch;
        hft.getthecode(i,a,llen,cch);
        dcode[i].ch=cch;
        dcode[i].len=llen;
        strcpy(dcode[i].code,a);
    }
    tot=n;
}

template <class E>
void huffmancode<E>::viewthecode()
{
    for(int i=1; i<=tot; i++)
    {
        cout<<dcode[i].ch<<":"<<dcode[i].code<<endl;
    }
}

template <class E>
void huffmancode<E>::codethestr(char * str)
{
    int len=strlen(str);
    cout<<"±àÂëÊÇ£º";
    for(int i=0; i<len; i++)
    {
        for(int j=1; j<=tot; j++)
            if(str[i]==dcode[j].ch)
            {
                cout<<dcode[j].code;
                break;
            }
    }
    cout<<endl;
}

template <class E>
void huffmancode<E>::decodethestr( char * str)
{
    int len=strlen(str);
    int cur=0;
    cout<<"ÒëÂëÊÇ£º";
    while(cur<len)
    {
        for(int  i=1; i<=tot; i++)
        {
            int flag=1;
            for(int j=0; j<dcode[i].len; j++)
            {
                if(str[j+cur] != dcode[i].code[j])
                {
                    flag=0;
                    break;
                }
            }
            if(flag)
            {
                cout<<dcode[i].ch;
                cur+=dcode[i].len;
                break;
            }
        }
    }
    cout<<endl;
}
#endif


