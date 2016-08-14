#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h"

/* 函数名：	iget							*/
/* 作用：	为磁盘i结点分配对应的内存i结点	*/
/* 参数:	待分配的磁盘i结点号				*/
/* 返回值：	指向对应的内存i结点的指针		*/
struct inode * iget(unsigned int dinodeid)
{
	int existed = 0, inodeid;//inodeid哪个哈希组
	long addr;
	struct inode *temp, *newinode;


	int i;


	inodeid = dinodeid % NHINO;//计算内存结点应该在第几个哈希队列里
	if (hinode[inodeid].i_forw == NULL)//若该哈希队列为空，内存结点一定未被创建
		existed = 0;
	else//若不为空，从该哈希队列头开始查找
	{
		temp = hinode[inodeid].i_forw;//temp=inode节点指针 
		while (temp)                           
		{
			if (temp->i_ino == dinodeid)//若找到磁盘i节点对应
			{
				existed = 1;
				temp->i_count ++;//引用计数+1
				return temp;//返回该内存结点指针
			}
			else
				temp = temp->i_forw;//查找同一个hash队列里的下一个inode节点

		}
	}

	/* 若没有找到 */   
	/* 1. 计算该磁盘i结点在文件卷中的位置 */
	addr = DINODESTART + dinodeid * DINODESIZ;

	/* 2. 分配一个内存i结点 */
	newinode = (struct inode *)malloc(sizeof(struct inode));

	/* 3. 用磁盘i结点初始化内存i结点 */
	//由于inode节点从di_number开始的内容与dionde中内容一样
	memcpy(&(newinode->di_number), disk+addr, DINODESIZ);

	/* 4. 将内存i结点链入相应的哈希队列里*/
	//每次新的inode节点插入在哈希队列的头部
	newinode->i_forw = hinode[inodeid].i_forw;
	hinode[inodeid].i_forw = newinode; 
	newinode->i_back = newinode;//头节点的父指针为自己
	if (newinode->i_forw)//如果头结点的子节点不为空
		newinode->i_forw->i_back = newinode;//子节点的父指针为头结点

	/*5. 初始化内存i结点的其他数据项 */
	newinode->i_count = 1;
	newinode->i_flag = 0;   /* 表示未更新 */
	newinode->i_ino = dinodeid;

	return newinode;
}

/* 函数名：	iput							*/
/* 作用：	回收内存i结点					*/
/* 参数:	指向待回收的内存i结点指针		*/
/* 返回值：	无								*/
void iput(struct inode *pinode)
{
	long addr;
	unsigned int block_num;
	int i;

	if (pinode->i_count > 1)//若引用计数>1
	{
		pinode->i_count --;

		return;
	}
	else
	{
		if (pinode->di_number != 0)//若联结计数不为0
		{
			/* 把内存i结点的内容写回磁盘i结点 */
			addr = DINODESTART + pinode->i_ino *DINODESIZ;
			memcpy(disk+addr, &pinode->di_number, DINODESIZ);
		}
		else
		{
			/* 删除磁盘i结点和文件对应的物理块 */
			block_num = pinode->di_size/BLOCKSIZ;
			for (i=0; i<block_num; i++)
				bfree(pinode->di_addr[i]);
			ifree(pinode->i_ino);
		}

		/* 释放内存i结点 */
		{
			int inodeid;
			inodeid = (pinode->i_ino)  % NHINO ;//找到所在的哈希队列
			
			/* 从该哈希队列里删除 */
			//待删除节点为哈希队列头部
			if (hinode[inodeid].i_forw == pinode)
			{
				hinode[inodeid].i_forw = pinode->i_forw;
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_forw;
			}
			else
			{
				pinode->i_back->i_forw = pinode->i_forw;//调整父节点子节点指针
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_back;//调整子节点父节点指针
			}
		}
		free(pinode);//释放i节点空间
			
	}

	return;
}













