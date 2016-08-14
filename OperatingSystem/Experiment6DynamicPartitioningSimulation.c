#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<dos.h>
#define n 10//假设系统允许的最大作业数为n，n值为10
#define m 10//假设系统允许的空闲区表最大为m，m值为10*/
#define minisize 100//空闲分区被分配时的比较量，避免碎片太多

struct
{
    float address;//起始地址
    float length;//分区长度，单位为字节
    int flag;//登记栏标志，"0"表示空
} used_table[n];//已分配区表

struct
{
    float address;//起始地址
    float length;//空闲区长度，单位为字节、
    int flag;//登记栏标志，"0"表示空，"1"表示未分配
} free_table[m];//空闲区表

void sort()//首次适应算法的空闲分区链
{
    int i,j,k;
    for(i=0; i<m-1; i++)
    {
        k=i;
        if(free_table[k].flag==1)
        {
            for(j=i+1; j<m; j++)
                if(free_table[k].address<free_table[j].address&&free_table[j].flag==1)
                {
                    k=j;
                }
            float temp1=free_table[i].address;
            float temp2=free_table[i].length;
            free_table[i].address=free_table[k].address;
            free_table[i].length=free_table[k].length;
            free_table[k].address=temp1;
            free_table[k].length=temp2;
        }
    }
}

void allocate(int J,float xk)//给J作业分配xk大小的空间
{
    int i,k;
    float ad;
    k=-1;
    for(i=0; i<m; i++)
        if(free_table[i].length>=xk&&free_table[i].flag==1)
            if(k==-1)
            {
                k=i;
                break;
            }
    if(k==-1)//未找到可用空闲区，返回
    {
        printf("无可用空闲区\n");
        return;
    }
    //找到可用空闲区，开始分配
    if(free_table[k].length-xk<=minisize)
    {
        free_table[k].flag=0;
        ad=free_table[k].address;
        xk=free_table[k].length;
    }
    else
    {
        free_table[k].length=free_table[k].length-xk;
        ad=free_table[k].address+free_table[k].length;
    }
    //修改已分配区表
    i=0;
    while(used_table[i].flag!=0&&i<n)//寻找空表目
        i++;

    if(i>=n)//无表目可填写已分配分区
    {
        printf("无表目填写已分分区，错误\n");
        //修正空闲区表
        if(free_table[k].flag==0)
            //前面找到的是整个空闲分区
            free_table[k].flag=1;
        else//前面找到的是某个空闲分区的一部分
        {
            free_table[k].length=free_table[k].length+xk;
            return;
        }
    }
    else//修改已分配表
    {
        used_table[i].address=ad;
        used_table[i].length=xk;
        used_table[i].flag=J;
    }
    sort();
    return;
}//主存分配函数结束

void reclaim(int J)//回收J作业
{
    int i,k,j,s,t;
    float S,L;
    //寻找已分配表中对应登记项
    s=0;
    while((used_table[s].flag!=J||used_table[s].flag==0)&&s<n)
        s++;
    if(s>=n)//在已分配表中找不到J作业
    {
        printf("找不到该作业\n");
        return;
    }
    //修改已分配表
    used_table[s].flag=0;
    //取得归还分区的起始地址S和长度L
    S=used_table[s].address;
    L=used_table[s].length;
    j=-1;
    k=-1;
    i=0;
    //寻找回收分区的空闲上下邻，上邻表目k，下邻表目j
    while(i<m&&(j==-1||k==-1))
    {
        if(free_table[i].flag==1)
        {
            if(free_table[i].address+free_table[i].length==S)k=i;//找到上邻
            if(free_table[i].address==S+L)j=i;//找到下邻
        }
        i++;
    }

    if(k!=-1)
        if(j!=-1)//上邻空闲区，下邻空闲区，三项合并
        {
            free_table[k].length=free_table[j].length+free_table[k].length+L;
            free_table[j].flag=0;
        }
        else
            //上邻空闲区，下邻非空闲区，与上邻合并
            free_table[k].length=free_table[k].length+L;
    else if(j!=-1)//上邻非空闲区，下邻为空闲区，与下邻合并
    {
        free_table[j].address=S;
        free_table[j].length=free_table[j].length+L;
    }
    else//上下邻均为非空闲区，回收区域直接填入
    {
        //在空闲区表中寻找空栏目
        t=0;
        while(free_table[t].flag==1&&t<m)
            t++;
        if(t>=m)//空闲区表满,回收空间失败,将已分配表复原
        {
            printf("主存空闲表没有空间,回收空间失败\n");
            used_table[s].flag=J;
            return;
        }
        free_table[t].address=S;
        free_table[t].length=L;
        free_table[t].flag=1;
    }
    sort();
    return;
}//主存回收函数结束

int main( )
{
    int i,a;
    float xk;
    int J;
    //空闲分区表初始化
    free_table[0].address=10240;//起始地址为10240
    free_table[0].length=102400;//长度为102400
    free_table[0].flag=1;//初始空闲区为一个整体空闲区
    for(i=1; i<m; i++)
        free_table[i].flag=0;//其余空闲分区表项未被使用
    //已分配表初始化
    for(i=0; i<n; i++)
        used_table[i].flag=0;//初始时均未分配
    while(1)
    {
        printf("功能选择项(0-退出;1-分配主存;2-回收主存;3-显示主存)\n");
        printf("请选择相应功能(0-3):");
        scanf("%d",&a);
        switch(a)
        {
        case 1://a=1分配主存空间
            printf("输入作业名(请输入数字）: ");
            scanf("%d",&J);
            printf("\n");
            printf("输入作业所需长度xk: ");
            scanf("%f",&xk);
            allocate(J,xk);//分配主存空间
            printf("内存已分配，按任意键继续...\n");
            getch();
            break;
        case 2://a=2回收主存空间
            printf("输入要回收分区的作业名：");
            scanf("%d",&J);
            reclaim(J);//回收主存空间
            printf("内存已回收，按任意键继续...\n");
            getch();
            break;
        case 3://a=3显示主存情况
            //输出空闲区表和已分配表的内容
            printf("输出空闲区表：\n起始地址 分区长度 标志\n");
            for(i=0; i<m; i++)
                printf("%6.0f%9.0f%6d\n",free_table[i].address,free_table[i].length, free_table[i].flag);
            printf("按任意键,输出已分配区表...\n");
            getch();
            printf("输出已分配区表：\n起始地址 分区长度 标志\n");
            for(i=0; i<n; i++)
                if(used_table[i].flag!=0)
                    printf("%6.0f%9.0f%6d\n",used_table[i].address,used_table[i].length, used_table[i].flag);
                else
                    printf("%6.0f%9.0f%6d\n",used_table[i].address,used_table[i].length, used_table[i].flag);
            printf("按任意键继续...\n");
            getch();
            break;
        case 4:
            exit(0);//a=4程序结束
        default:
            printf("没有该选项\n");
        }
    }
    return 1;
}

