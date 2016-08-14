#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include<windows.h>
#include<conio.h>
#include<dos.h>
#define N 64
using namespace std;

struct node
{
    int lNumber;//ҳ��
    int pNumber;//������
    int dNumber;//�ڴ����ϵ�λ��
    int write;//�޸ı�־
    int flag;//���ڱ�־
} page[N];

int pageQ[N];//ҳ��ѭ������,����ڴ��е�ҳ��
int next=0;//ѭ��������һ��Ҫ�û���λ�ã���ֵΪ0
int blocks=-1;//��pNumber���θ�ֵ��blocks��page�ĽǱ�
int n=0;//���n�Ǵ�����ҳ��

void show_page(int n)//��ʾҳ������
{
    cout<<"��ǰҳ������Ϊ��"<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<"page["<<i<<"].lNumber="<<page[i].lNumber<<"\t";
        cout<<"page["<<i<<"].pNumber="<<page[i].pNumber<<"\t";
        cout<<"page["<<i<<"].wite="<<page[i].write<<"\t";
        cout<<"page["<<i<<"].flag="<<page[i].flag<<endl;
    }
    cout<<endl;
}

void page_FIFO(int In)//In�Ǳ������page�ţ�����FIFOҳ����̭�㷨
{
    int Out=pageQ[next];//��¼���û�����page��
    pageQ[next] = In;//�ڶ�������Ҫ��next����ֵOut����In
    if(page[In].write)
    {
        cout<<"��"<<Out<<"ҳд�ش��̵�"<<page[Out].dNumber<<"��"<<endl;
        page[Out].write = 0;//�޸ı�Ǳ�Ϊ0
    }
    page[Out].flag  = 0;//�ѱ������ڴ棬flagΪ0
    cout<<"��̭�����"<<page[Out].pNumber<<"�е�ҳ"<<Out<<"���Ӵ��̵�"<<page[In].dNumber<<"���е���ҳ"<<In<<endl;
    page[In].flag = 1;//�������ڴ棬flagΪ1
    page[In].pNumber = page[Out].pNumber;//���������ڴ���ת�������
    next=(next+1)%(blocks+1);//�ڴ��ܿ�����blocks+1
}

void show_address(int xie,int dizhi)//��ʾ�����ַ
{
    if(dizhi>(n*1024-1))
    {
        cout<<"�����ڸ�ҳ��"<<endl;//�鿴�߼���ַ�Ƿ�Խ��
        return;
    }
    int logic_addr,ye,weiyi;
    ye=int(dizhi/1024);//ҳ
    weiyi=dizhi%1024;//ҳ��λ��
    page[ye].write=xie;

    if (page[ye].flag==0)
    {
        cout<<"���ʵ�"<<ye<<"ҳ�����ڴ棬����ȱҳ�ж�"<<endl;
        page_FIFO(ye);
    }
    logic_addr = page[ye].pNumber * 1024 + weiyi;
    cout<<"�߼���ַ�ǣ�"<<dizhi<<"��Ӧ�ĵ�ַ�ǣ�"<<logic_addr<<endl;
}

int main()
{
    cout<<endl<<"����ҳ�����Ϣ������ҳ��������Ϊ-1����������룩"<<endl<<endl;
    int in_num=0;
    while(in_num!=-1)//�����ַΪ-1����
    {
        cout<<"�����"<<n<<"ҳ�ĸ����ַ��";
        cin>>in_num;
        page[n].dNumber=in_num;//��¼���棬�������ϵ�λ��
        page[n].lNumber=n;
        page[n].pNumber=0;
        page[n].write=0;
        page[n].flag=0;
        n++;
    }
    n=n-1;
    show_page(n);
    cout<<endl<<"���������ţ������ҪС�ڵ���"<<n<<"������-1��������";

    cin>>in_num;
    while(in_num!=-1)
    {
        blocks++;//��0��ʼ
        page[blocks].pNumber=in_num;//��Ŵ���
        page[blocks].flag=1;
        cin>>in_num;
    }
    show_page(n);

    for(int i=0; i<=blocks; i++)//��¼������еĿ�
        pageQ[i]=i;

    int in_write,in_addr;
    cout<<endl<<"����ָ�����ʣ�1-�޸ģ�0-���޸ģ�2-�����������У����߼���ַ��";
    cin>>in_write;
    cin>>in_addr;
    while(in_write!=2)//2������������
    {
        show_address(in_write,in_addr);
        //show_page(n);
        cout<<endl<<"����ָ�����ʣ�1-�޸ģ�0-���޸ģ�2-�����������У����߼���ַ��";
        cin>>in_write;
        cin>>in_addr;
    }
    return 0;
}
