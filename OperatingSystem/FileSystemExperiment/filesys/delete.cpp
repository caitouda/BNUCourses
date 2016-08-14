#include <stdio.h>
#include "filesys.h"

void delete(char *filename){
	unsigned int dirid;
	struct inode *inode;

	dirid = namei(filename);//查找文件在当前目录下对应的内存目录项的标号
	//判断该文件是否存在
	if (dirid == -1){
		printf("文件不存在，请检查!\n");
		return;
	}
	//为磁盘结点分配对应的内存结点
	inode = iget(dir.direct[dirid].d_ino);
	//判断要删除的对象是否为文件
	if(!(inode->di_mode &DIFILE)){
		printf("对象不是文件，请检查！\n");
		iput(inode);
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY;//权限置为空
	dir.size --;//当前目录大小减1
	inode->di_number--;//关联文件数减1
	iput(inode);
	return;
}











