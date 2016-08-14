#include <stdio.h>
#include <string.h>
#include "filesys.h"


void halt(){//停止，关机
	int i,j;

	/*1. write back the current dir */
	chdir("..");//dir.cpp
	iput(cur_path_inode);//igetput.cpp

	/*2. free the u_ofile and sys_ofile and inode*/
	for(i=0; i<USERNUM; i++){
		if(user[i].u_uid != 0){//u_uid=1?begin
			for(j=0; j<NOFILE;j++){//文件最多打开文件数
				if(user[i].u_ofile[j] != SYSOPENFILE + 1){
					close(i,j);//关闭打开文件时，修改用户打开表和系统打开表中的纪录信息
					user[i].u_ofile[j] = SYSOPENFILE + 1;//修改用户打开表，若SYSOPENFILE + 1表示用户关闭文件
				}//if
			}//for
		}//if
	}//for

	/*3. write back the filesys to the disk*/
	memcpy(disk+BLOCKSIZ, &filsys, sizeof(struct filsys));

	/*4. close the file system column*/

	/*5. say GOOD BYE to all the user*/
	printf("\nGood Bye. See You Next Time. Please turn off the switch\n");
	exit(0);
}





























