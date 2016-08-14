#include <stdio.h>
#include <string.h>
#include "filesys.h"

static struct dinode block_buf[BLOCKSIZ/DINODESIZ];		//存放i节点的临时数组占一个盘块,盘块中放最多能放的磁盘节点个数
/*****************************************************
函数：ialloc
功能：分配磁盘i节点，返回相应的内存i节点指针
******************************************************/

struct inode * ialloc(){
	struct inode *temp_inode;
	unsigned int cur_di;
	int i,count, block_end_flag;

	//I界点分配时从低位到高位使用，并且分配的i节点也是由低到高
	if (filsys.s_pinode == NICINOD){//空闲i节点数组为空,此数组相当于栈
		i=0;
		block_end_flag = 1;
		//放入一组新的或放入未满一组的新的，设置指针，直至最后为栈底
		count = filsys.s_pinode = filsys.s_ninode>NICINOD?0:(NICINOD-filsys.s_ninode);
		cur_di = filsys.s_rinode;//记录当前inode节点（未读）的编号，初始为NICINOD + 4
		while (count < NICINOD){//调入空闲i节点，直到空闲i节点栈满		
			if (block_end_flag){//临时栈空，更新临时栈
				memcpy(block_buf, disk+DINODESTART + cur_di*DINODESIZ, BLOCKSIZ);//读一个盘块的磁盘inode节点到临时数组
				block_end_flag = 0;
				i = 0;
			}
		//此为原来从此开始，到while (count < NICINOD)结束括号之间的代码，有错误并缺失代码
		/*while (block_buf[IUPDATE].di_mode != DIEMPTY){	//临时数组为空，则读到空闲i节点数组中
				cur_di ++;
				i++;
			}
			if (i == NICINOD){		//空闲i节点数组已满
				block_end_flag = 1;
				continue;
			}
			filsys.s_inode[count++] = cur_di;*/
         //修改如下
            while (block_buf[i].di_mode != DIEMPTY){	//若该磁盘i节点不为空，读下一个i节点
				cur_di ++;
				i++;
				if(i==NICINOD)break;
			}
			if (i ==  BLOCKSIZ/DINODESIZ){		//缓冲区已读完
				block_end_flag = 1;
				continue;
			}
			filsys.s_inode[count++] = cur_di;//放入空权限的inode号
			i++;
			cur_di++;
			if (i == NICINOD)block_end_flag=1;//缓冲区已读完
		}
		filsys.s_rinode = cur_di;	//更新下一个未读入空闲inode栈的磁盘inode号
	}
	/*分配空闲i节点*/
	temp_inode = iget(filsys.s_inode[filsys.s_pinode]);
	memcpy(disk+DINODESTART+filsys.s_inode[filsys.s_pinode]*DINODESIZ,
	             &temp_inode->di_number, sizeof(struct dinode));
	filsys.s_pinode ++;//栈顶指针栈中下一个节点
	filsys.s_ninode --; //空闲i节点个数-1
	filsys.s_fmod = SUPDATE; 
	return temp_inode;
} 


void ifree(unsigned int dinodeid){//释放磁盘i节点
	//filsys.s_ninode --;	
	filsys.s_ninode ++;//空闲磁盘i节点数+1
	if (filsys.s_pinode != 0){		//空闲i节点数组未满
		filsys.s_pinode--;
		filsys.s_inode[filsys.s_pinode] = dinodeid;
	}
	else{//空闲i节点数组满
		if (dinodeid < filsys.s_rinode){//回收磁盘inode号比下一个未读inode号小，置下一个为当前回收的号	
			//filsys.s_inode[NICINOD] = dinodeid;		
			filsys.s_rinode = dinodeid;
		}
	}
	return;
}















































