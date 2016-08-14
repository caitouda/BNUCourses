#include <stdio.h>
#include "filesys.h"

unsigned int read(int fd, char *buf, unsigned int size){
	unsigned long off;
	int block, block_off, i, j;
	struct inode *inode;
	char *temp_buf;

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;
	//检查用户对该文件是否有读取权限
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD)){
		printf("\nthe file is not opened for read\n");
		return 0;
	}
	temp_buf = buf;
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;//获取文件的读指针
	if((off+size) > inode->di_size){
		size = inode->di_size - off;
	}
	block_off = off % BLOCKSIZ;//获取块内偏移量
	block = off / BLOCKSIZ;//获取文件逻辑块号
	//没有超过一个盘块
	if (block_off+size<BLOCKSIZ){
		//读取size大小的字节到buf所指向的缓冲区
		memcpy(buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, size);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size;
		return size;
	}
	//数据大小超过一个盘块
	memcpy(temp_buf, disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,BLOCKSIZ-block_off);
	temp_buf += BLOCKSIZ - block_off;
	j = (inode->di_size-off-block_off) / BLOCKSIZ;
	//for循环：对数据进行分解分成多个块读
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){
		memcpy(temp_buf, disk+DATASTART+inode->di_addr[j+i]*BLOCKSIZ, BLOCKSIZ);
		temp_buf += BLOCKSIZ;
	}
	//对不足一个block的数据进行块操作
	block_off = (size-(BLOCKSIZ-block_off))% BLOCKSIZ;
	memcpy(temp_buf, disk+DATASTART+i*BLOCKSIZ, block_off);
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;//记录文件读指针现在的偏移
	return size;
}


unsigned int write(int fd, char *buf, unsigned int size){
	unsigned long off;
	int block, block_off, i, j;
	struct inode *inode;
	char *temp_buf;

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;
	if  (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE)){
		printf("\nthe file is not opened for write\n");
		return 0;
	}
	//add by liwen to check the filesize and alloc the BLOCK
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;//获取文件的写指针
    //计算要写入size字节的数据需要多少盘块
	block = ((off+size)-inode->di_size)/BLOCKSIZ;
	if(((off+size)-inode->di_size)%BLOCKSIZ)
		block++;
	//判断剩余空闲块是否够写
	if(filsys.s_nfree < block){
		printf("Not enough space to write so much bytes!\n");
		return 0;
	}
	//判断写数据所需盘块数是否超过内存节点限制
	j = inode->di_size/BLOCKSIZ;
	if(inode->di_size % BLOCKSIZ){
		j ++;
	}
	if(j+block>NADDR){
		printf("To write so much bytes will exceed the file limit!!\n");
		return 0;
	}

    //根据写数据大小所需盘块数分配空闲数据块
	for(i=j;i<j+block;i++){
		inode->di_addr[i]=balloc();
	}
	inode->di_size += size;
	//end add
	temp_buf = buf;

	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;
	block_off = off % BLOCKSIZ;
	block = off/BLOCKSIZ;
    //没有超过一个盘块
	if (block_off+size<BLOCKSIZ){
		memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off, buf, size);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size;
		return size;
	}
	//超过一个盘块
	memcpy(disk+DATASTART+inode->di_addr[block]*BLOCKSIZ+block_off,temp_buf,BLOCKSIZ-block_off);
	temp_buf += BLOCKSIZ-block_off;
	//对数据进行分解分成多个块写入
	for (i=0; i<(size-(BLOCKSIZ-block_off))/BLOCKSIZ; i++){
		memcpy(disk+DATASTART+inode->di_addr[block+1+i]*BLOCKSIZ, temp_buf, BLOCKSIZ);
		temp_buf += BLOCKSIZ;
	}
	//对不足一个block的数据进行块操作
	block_off = (size-(BLOCKSIZ-block_off)) % BLOCKSIZ;
	memcpy(disk+DATASTART+block*BLOCKSIZ, temp_buf, block_off);
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;//记录文件写指针现在的偏移
	return size;
}


