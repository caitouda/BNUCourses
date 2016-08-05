#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED
#include "MinHeap.h"
#include <iostream>
#include<cstring>
using namespace std;
template <class E>
struct HuffmanNode
{
    E data;
    char ch;
    HuffmanNode<E> *leftChild, *rightChild,*parent;
    HuffmanNode () :leftChild(NULL),rightChild(NULL), parent(NULL)  { }
    HuffmanNode (E elem,char cc,HuffmanNode<E> *left = NULL,
                 HuffmanNode<E> *right = NULL,HuffmanNode<E> *pr = NULL)
        : data (elem),ch(cc),leftChild (left),  rightChild (right), parent (pr) { }
    friend bool operator< (HuffmanNode<E> & hf1,HuffmanNode<E> & hf2)
    {
        return hf1.data<hf2.data;
    }
    friend bool operator> (HuffmanNode<E> & hf1,HuffmanNode<E> & hf2)
    {
        return hf1.data>hf2.data;
    }
    friend bool operator<= (HuffmanNode<E> & hf1,HuffmanNode<E> & hf2)
    {
        if (hf1.data<hf2.data||hf1.data==hf2.data) return 1;
        else return 0;
    }
};
template <class E>
struct treecode
{
    E data;
    char code[10];
    int len;
    char ch;
    treecode():len(0) {}
};
template <class E>
class HuffmanTree
{
public:
    HuffmanTree (E w[], char* cc,int n);
    ~HuffmanTree()
    {
        del(root);
    }
    void viewtree()
    {
        viewTree(root);
    }
    void getthecode(int i,char* & str,int & llen,char & cch);
protected:
    HuffmanNode<E> *root;
    treecode<E> tt[10];
    int size;
    void viewTree(HuffmanNode<E> * ptr);
    void del(HuffmanNode<E> *ptr);
    void mergeTree (HuffmanNode<E>& ht1,HuffmanNode<E>& ht2,HuffmanNode<E> *& parent);
    void Codethetree(HuffmanNode<E>* ptr,char* a,int i);
};
template <class E>
HuffmanTree<E>::HuffmanTree(E w[],char* cc,int n)
{
    MinHeap<HuffmanNode<E> > hp;
    HuffmanNode <E> *parent;
    HuffmanNode <E> fisrt;
    HuffmanNode <E> second;
    HuffmanNode <E> *NodeList = new HuffmanNode<E>[n];

    for(int i=0; i<n; i++)
    {
        NodeList[i].data = w[i+1];
        NodeList[i].ch=cc[i+1];
        NodeList[i].leftChild = NULL;
        NodeList[i].rightChild = NULL;
        NodeList[i].parent = NULL;
        hp.Insert(NodeList[i]);
    }

    for(int i=0; i<n-1; i++)
    {
        hp.RemoveMin(fisrt);
        hp.RemoveMin(second);
        HuffmanNode <E> *a = new HuffmanNode<E> (fisrt);
        HuffmanNode <E> *b = new HuffmanNode<E> (second);
        mergeTree(*a,*b,parent);
        hp.Insert(*parent);
    }
    root = parent;
    size=1;
    char a[10];
    Codethetree(root,a,0);
}
template <class E>
void HuffmanTree<E>::mergeTree (HuffmanNode<E>& ht1,HuffmanNode<E>& ht2, HuffmanNode<E> *& parent)
{
    parent = new HuffmanNode<E>;
    if(ht1.data<=ht2.data)
    {
        parent->leftChild = &ht1;
        parent->rightChild = &ht2;
    }
    else
    {
        parent->leftChild = &ht2;
        parent->rightChild = &ht1;
    }
    parent->data =ht1.data+ht2.data;
    ht1.parent = ht2.parent = parent;
};
template <class E>
void HuffmanTree<E>::getthecode(int i,char* & str,int & llen,char & cch)
{
    if(i>size) cout<<"warning,no such one"<<endl;
    strcpy(str,tt[i].code);
    llen=tt[i].len;
    cch=tt[i].ch;
}


template <class E>
void HuffmanTree<E>::del(HuffmanNode <E> * ptr)
{
    if(ptr->leftChild != NULL)
    {
        del(ptr->leftChild);
    }
    if(ptr->rightChild != NULL)
    {
        del(ptr->rightChild);
    }
    delete ptr;
}
template <class E>
void HuffmanTree<E>::Codethetree(HuffmanNode<E>* ptr,char* a,int i)
{
    if(ptr->leftChild == NULL && ptr->rightChild == NULL)
    {
        tt[size].data=ptr->data;
        a[i]=0;
        strcpy(tt[size].code,a);
        tt[size].len=i;
        tt[size].ch=ptr->ch;
        size++;
        return ;
    }
    if(ptr->leftChild != NULL)
    {
        a[i] = '0';
        Codethetree(ptr->leftChild,a,i+1);
    }
    if(ptr->rightChild != NULL)
    {
        a[i] = '1';
        Codethetree(ptr->rightChild,a,i+1);
    }

}

template <class E>
void HuffmanTree<E>::viewTree(HuffmanNode<E> * ptr)
{
    cout<<ptr->ch;
    cout<<"(";

    if(ptr->leftChild != NULL ) viewTree(ptr->leftChild);
    else cout<<"*";

    cout<<",";

    if(ptr->rightChild != NULL ) viewTree((ptr->rightChild));
    else cout<<"*";

    cout<<")";
}


#endif


