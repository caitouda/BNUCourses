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
    int lNumber;//页号
    int pNumber;//物理块号
    int dNumber;//在磁盘上的位置
    int write;//修改标志
    int flag;//存在标志
} page[N];

int pageQ[N];//页面循环队列,存放内存中的页数
int next=0;//循环队列下一个要置换的位置，初值为0
int blocks=-1;//给pNumber依次赋值，blocks是page的角标
int n=0;//这个n是创建的页数

void show_page(int n)//显示页表内容
{
    cout<<"当前页表内容为："<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<"page["<<i<<"].lNumber="<<page[i].lNumber<<"\t";
        cout<<"page["<<i<<"].pNumber="<<page[i].pNumber<<"\t";
        cout<<"page["<<i<<"].wite="<<page[i].write<<"\t";
        cout<<"page["<<i<<"].flag="<<page[i].flag<<endl;
    }
    cout<<endl;
}

void page_FIFO(int In)//In是被调入的page号，采用FIFO页面淘汰算法
{
    int Out=pageQ[next];//记录被置换出的page号
    pageQ[next] = In;//在队列里需要把next处的值Out换成In
    if(page[In].write)
    {
        cout<<"将"<<Out<<"页写回磁盘第"<<page[Out].dNumber<<"块"<<endl;
        page[Out].write = 0;//修改标记变为0
    }
    page[Out].flag  = 0;//已被调出内存，flag为0
    cout<<"淘汰主存块"<<page[Out].pNumber<<"中的页"<<Out<<"，从磁盘第"<<page[In].dNumber<<"块中调入页"<<In<<endl;
    page[In].flag = 1;//被调入内存，flag为1
    page[In].pNumber = page[Out].pNumber;//将调出的内存块号转给调入的
    next=(next+1)%(blocks+1);//内存总块数是blocks+1
}

void show_address(int xie,int dizhi)//显示物理地址
{
    if(dizhi>(n*1024-1))
    {
        cout<<"不存在该页！"<<endl;//查看逻辑地址是否越界
        return;
    }
    int logic_addr,ye,weiyi;
    ye=int(dizhi/1024);//页
    weiyi=dizhi%1024;//页面位移
    page[ye].write=xie;

    if (page[ye].flag==0)
    {
        cout<<"访问的"<<ye<<"页不在内存，发生缺页中断"<<endl;
        page_FIFO(ye);
    }
    logic_addr = page[ye].pNumber * 1024 + weiyi;
    cout<<"逻辑地址是："<<dizhi<<"对应的地址是："<<logic_addr<<endl;
}

int main()
{
    cout<<endl<<"输入页表的信息，创建页表（若输入为-1，则结束输入）"<<endl<<endl;
    int in_num=0;
    while(in_num!=-1)//辅存地址为-1结束
    {
        cout<<"输入第"<<n<<"页的辅存地址：";
        cin>>in_num;
        page[n].dNumber=in_num;//记录辅存，即磁盘上的位置
        page[n].lNumber=n;
        page[n].pNumber=0;
        page[n].write=0;
        page[n].flag=0;
        n++;
    }
    n=n-1;
    show_page(n);
    cout<<endl<<"输入主存块号，主存块要小于等于"<<n<<"，（以-1结束）：";

    cin>>in_num;
    while(in_num!=-1)
    {
        blocks++;//从0开始
        page[blocks].pNumber=in_num;//块号存入
        page[blocks].flag=1;
        cin>>in_num;
    }
    show_page(n);

    for(int i=0; i<=blocks; i++)//记录存入队列的块
        pageQ[i]=i;

    int in_write,in_addr;
    cout<<endl<<"输入指令性质（1-修改，0-不修改，2-结束程序运行）和逻辑地址：";
    cin>>in_write;
    cin>>in_addr;
    while(in_write!=2)//2结束程序运行
    {
        show_address(in_write,in_addr);
        //show_page(n);
        cout<<endl<<"输入指令性质（1-修改，0-不修改，2-结束程序运行）和逻辑地址：";
        cin>>in_write;
        cin>>in_addr;
    }
    return 0;
}
