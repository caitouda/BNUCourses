#include <stdio.h>
#include <string.h>
#include "filesys.h"

void _dir(){
	unsigned int di_mode;
	int i,j,k;          //xiao  
	struct inode *temp_inode;

	printf("\n CURRENT DIRECTORY :%s\n",dir.direct[0].d_name);
	printf("当前共有%d个文件/目录\n",dir.size);
	for (i=0; i<DIRNUM; i++){
		if (dir.direct[i].d_ino != DIEMPTY){
			printf("%-14s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			/* 参数:	待分配的磁盘i结点号				*/
            /* 返回值：	指向对应的内存i结点的指针		*/
			di_mode = temp_inode->di_mode & 00777;			//DEFAULTMODE 	00777--默认权限  &按位与
			for (j=0; j<9; j++){
				if (di_mode%2){
					printf("x");
				}else{
					printf("-");
				}
				di_mode = di_mode/2;
			}
			printf("\ti_ino->%d\t",temp_inode->i_ino);
			
			if (temp_inode->di_mode & DIFILE){  //类型为文件时
				printf(" %ld ", temp_inode->di_size);
				printf("block chain:");
				j=(temp_inode->di_size%BLOCKSIZ)?1:0;
				for (k=0; k < temp_inode->di_size/BLOCKSIZ+j; k++)
					printf("%4d", temp_inode->di_addr[k]);
				printf("\n");
			}else{
				printf("<dir>\n");
			}//else
			iput(temp_inode);
		}// if (dir.direct[i].d_ino != DIEMPTY) 
	}//for 
	return;
}
void mkdir(char *dirname){
	int dirid, dirpos;
	struct inode *inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	unsigned int block;

	dirid= namei(dirname);      //要查找的文件名。返回文件对应的内存目录项标号i；如果返回-1表示没有该文件。
	if (dirid != -1){
	//	inode = iget(dirid);
		inode = iget(dir.direct[dirid].d_ino);/****************************change*************************/
		if (inode->di_mode & DIDIR)
			printf("目录%s已存在！\n", dirname); //xiao
		else
		{
			printf("%s是一个文件！\n", dirname);
		}
		iput(inode);
		return;
	}
	dirpos = iname(dirname);					//取得在addr中的空闲项位置,并将目录名写到此项里
	inode = ialloc();							//分配节点i
	dir.direct[dirpos].d_ino = inode->i_ino;	//设置该目录的磁盘节点i号
	dir.size++;									//目录数++		
	
	strcpy(buf[0].d_name,"..");					//子目录的上一层目录 当前目录
	buf[0].d_ino = cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;				//子目录的本目录 子目录

	block = balloc();
	memcpy(disk+DATASTART+block*BLOCKSIZ, buf, BLOCKSIZ);

	inode->di_size = 2*(DIRSIZ+4);//2*(12+4)=32
	inode->di_number = 1; 
	inode->di_mode = user[user_id].u_default_mode|DIDIR;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;

	iput(inode);
	return;
}


void chdir(char *dirname){  //改变当前目录
	unsigned int dirid;
	struct inode *inode;
	unsigned short block;
	int i,j,low=0, high=0;

	dirid = namei(dirname);
	if (dirid == -1){
		printf("不存在目录%s！\n", dirname);
		return;
	}	
	inode =iget(dir.direct[dirid].d_ino);  //内存节点i

	if(!(inode->di_mode&DIDIR)){
		printf("%s不是一个目录！\n");
		return;
	}
	printf("dir.size=%d\n",dir.size);

/*	for (i=0; i<dir.size; i++){
		if(dir.direct[i].d_ino == 0){
			for(j=DIRNUM-1;j>=0&&dir.direct[j].d_ino == 0;j--);
			memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ+4);  //xiao
			dir.direct[j].d_ino = 0;
		}		
	}*/
//对于有删除操作的文件系统，可能会有之前的磁盘节点i号为0 
//对于没有删除操作的文件不需要此语句

	j = cur_path_inode->di_size%BLOCKSIZ?1:0;  //di_size  文件大小
	for (i=0; i<cur_path_inode->di_size/BLOCKSIZ+j; i++){  //释放当前节点在磁盘中分配的物理块
		bfree(cur_path_inode->di_addr[i]);/////?????????
		printf("cur_path_inode->di_addr[%d]=%d\n",i,cur_path_inode->di_addr[i]);
	}
	for (i=0; i<dir.size; i+=BLOCKSIZ/(DIRSIZ+4)){
		block = balloc();
		printf("block=%d\n",block);
		cur_path_inode->di_addr[i] = block;
		memcpy(disk+DATASTART+block*BLOCKSIZ, &dir.direct[i], BLOCKSIZ);
	}
	cur_path_inode->di_size = dir.size*(DIRSIZ+4);
	iput(cur_path_inode);

	cur_path_inode = inode;

	j=0;
	for (i=0; i<inode->di_size/BLOCKSIZ+1; i++){
		memcpy(&dir.direct[j],disk+DATASTART+inode->di_addr[i]*BLOCKSIZ, BLOCKSIZ);
		j+=BLOCKSIZ/(DIRSIZ+4);
	}
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);
	for (i=dir.size; i<DIRNUM; i++){ 
		dir.direct[i].d_ino = 0;
	}
	
	//end by xiao

	return;  
} 



 
 





































