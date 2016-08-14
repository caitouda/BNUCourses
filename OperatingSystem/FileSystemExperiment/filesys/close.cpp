#include <stdio.h>
#include "filesys.h"

/******************************************************
函数：close
功能：关闭打开文件时，修改用户打开表和系统打开表中的纪录信息
*******************************************************/
void close(unsigned int user_id, unsigned short cfd){
	struct inode *inode;
	inode = sys_ofile[user[user_id].u_ofile[cfd]].f_inode;
	if (inode==NULL)
	{
		return ;
	}
	else
	{
		iput(inode);//回收内存节点i
		sys_ofile[user[user_id] .u_ofile[cfd]].f_count -- ;//系统打开表引用计数-1
		user[user_id].u_ofile[cfd] = SYSOPENFILE+1;//用户打开表中记录系统打开表地址，故置SYSOPENFILE+1，表示没有此项没有文件打开
		return;
	}
}
	
