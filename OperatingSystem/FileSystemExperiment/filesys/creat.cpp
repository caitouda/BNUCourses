#include <stdio.h>
#include "filesys.h"
/*********************************************************************
函数：creat
功能：创建文件，存在且可写则覆盖，否则申请i节点，并打开该文件，返回文件指针
**********************************************************************/
int creat(unsigned int user_id, char *filename, unsigned short mode){
	struct inode *inode;
	int dirid,di_ith;
	int i,j;

	dirid = namei(filename);//查找文件在当前目录下对应的内存目录项的标号
	if (dirid != -1){//如果存在同名文件/目录
		inode = iget(dir.direct[dirid].d_ino);
		if(!(inode->di_mode&DIFILE)){//如果不是文件
			printf("存在同名目录！\n");
		}
		if (access(user_id,inode,WRITE) == 0){//判断用户对该文件能否进行写操作
			iput(inode);
			printf("\n creat access not allowed \n");
			return -1;
		}
         //释放旧文件的block组更新当前文件系统的指针
		j = inode->di_size%512?1:0;
		for (i=0; i<inode->di_size/BLOCKSIZ+j; i++)
			bfree(inode->di_addr[i]);

		for (i=0; i<SYSOPENFILE; i++){
			if (sys_ofile[i].f_count != 0 && sys_ofile[i].f_inode == inode){
				sys_ofile[i].f_off = 0;
			}
		}
		iput(inode);//回收内存节点
		return open(user_id,filename,WRITE);
	}
	else{
		inode = ialloc();//分配磁盘节点  返回相应的内存节点指针
		di_ith = iname(filename);//为当前文件分配目录项
		
		
			
		dir.size++; //当前目录大小加1
		dir.direct[di_ith].d_ino = inode->i_ino;//将磁盘节点赋值给存储目录项的信息节点
		//更改磁盘节点的相关数据项的信息
		inode->di_mode = mode;
		inode->di_uid = user[user_id].u_uid;
		inode->di_gid = user[user_id].u_gid; 
		inode->di_size = 0;
		inode->di_number = 1;			//liwen change to 1
		iput(inode);
		return open(user_id,filename,WRITE);
	}
	return 0;
}








