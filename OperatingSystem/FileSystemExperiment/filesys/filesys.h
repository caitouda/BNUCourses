#ifndef _FILESYS_H
#define _FILESYS_H

#define delete 		dele     		//delete 在c++里是保留字  by tangfl
//#define exit(a)		return			//建议使用 return         by tangfl

//All Defines
#define BLOCKSIZ        512			//每个块的大小
#define SYSOPENFILE     40          //系统打开文件数
#define DIRNUM          128			//一个目录下 最多 子目录数
#define DIRSIZ          12			//目录名长度 windows32下int长度为4 xiao 14->12
#define PWDSIZ 			12			//密码最大长度
#define PWDNUM 			32
#define NOFILE   		20			//用户最多打开文件数
#define NADDR  			10			//i节点内存储
#define NHINO 			128			//hash数  /* must be power of 2 */
#define USERNUM 		10
#define DINODESIZ 		52			//struct dinode大小52byte


/*filesys*/
#define DINODEBLK  		32							//i节点占用的块数
#define FILEBLK   		512							//数据块数
#define NICFREE  		50							//超级块内空闲块堆栈大小
#define NICINOD  		50							//超级块内空闲i节点数组大小
#define DINODESTART  	(2*BLOCKSIZ)				//i节点开始地址 空出1024，第一个为引导区，第二块为超级块
#define DATASTART  		((2+DINODEBLK)*BLOCKSIZ)	//数据区开始地址 DINODESTART+DINODEBLK*BLOCKSIZ	/*d:17408    0x4400*/


/*di_mode*/
#define DIEMPTY   		00000						//空权限
#define DIFILE      	01000						//类型 文件
#define DIDIR     		02000						//类型 目录

#define UDIREAD 		00001						//用户权限
#define UDIWRITE  		00002
#define UDIEXICUTE  	00004
#define GDIREAD   		00010						//用户组权限
#define GDIWRITE  		00020
#define GDIEXICUTE  	00040
#define ODIREAD  		00100						//pubilc权限
#define ODIWRITE 		00200
#define ODIEXICUTE 		00400


#define READ  			1
#define WRITE 			2
#define EXICUTE 		3


#define DEFAULTMODE 	00777					//默认权限


/* i_flag */
#define  IUPDATE  		00002

/* s_fmod */
#define SUPDATE  		00001 

/* f_flag */
#define FREAD   		00001
#define FWRITE   		00002
#define FAPPEND   		00004

//Includes
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/* error */
#define DISKFULL  		65535

/* fseek origin */
//#define SEEK_SET  		0


struct inode{//内存i节点
	struct inode  	*i_forw;//同一个hash队列里的下一个inode节点
	struct inode  	*i_back;
	char 			i_flag;
	unsigned int  	i_ino;          /*磁盘i 节点标志*/
	unsigned int  	i_count;     	/*引用计数*/
	unsigned short  di_number; 		/*关联文件数。当为0 时，则删除该文件*/
	unsigned short  di_mode;  		/*存取权限*/
	unsigned short  di_uid;//文件拥有者标识号
	unsigned short  di_gid;//文件拥有者所在组的标识号
	unsigned short  di_size;   		/*文件大小*/
	unsigned int   	di_addr[NADDR];   /*物理块号*/
};//67->72空间对齐：从di_number开始空间为5*2+4*NADDR=50

struct dinode{//磁盘 i节点：以静态形式存放文件说明信息
	unsigned short 	di_number; 		/*关联文件数*/
	unsigned short 	di_mode; 		/*存取权限*/
	unsigned short 	di_uid;
	unsigned short 	di_gid;
	unsigned short 	di_size;  		/*文件大小*/
	unsigned int 	di_addr[NADDR];   /*物理块号直接*/
};


struct direct{//目录项
	char d_name[DIRSIZ];//文件名
	unsigned int d_ino;//磁盘i节点标识符
};

struct filsys{//用来记录文件系统中盘块和磁盘 i节点的使用情况。存放在超级块
	unsigned short  	s_isize;   			/*i节点块块数*/
	unsigned long   	s_fsize;   			/*数据块块数*/
	unsigned int   		s_nfree;    		/*空闲块*/
	unsigned short  	s_pfree;  			/*空闲块指针*/
	unsigned int  		s_free[NICFREE];  	/*空闲块堆栈*/
	
	unsigned int  		s_ninode;  			/*number of free inode in s_inode*/
	short int 			s_pinode;  			/*pointer of the sinode*/
	unsigned int  		s_inode[NICINOD];   /*空闲i节点数组*/
	unsigned int 		s_rinode;    		/*remember inode*///盘块号最小的其中包含空闲i节点的盘块号

	char 				s_fmod;  			/*超级块修改标志*/
	};

struct pwd{
	unsigned short 		p_uid;
	unsigned short 		p_gid;
	char 				password [PWDSIZ];
};

struct dir{
	struct direct 		direct[DIRNUM];
	int 				size;  				/*当前目录大小*/
};

struct hinode{//每组的头指针为hinode。ihash的值即是i节点x的头指针hinode的地址。ihash(x)=&hinode［(int)(x)&128］
	struct inode 		*i_forw;   /*HASG表指针*/
};

struct file{
	char 			f_flag;    		/*文件操作标志*/
	unsigned int 	f_count;  		/*引用计数*/
	struct inode 	*f_inode; 		/*指向内存i节点*/
	unsigned long 	f_off;   		/*read/write character pointer*/
};

struct user{
	unsigned short 	u_default_mode;
	unsigned short 	u_uid;//user id
	unsigned short 	u_gid;//文件拥有者所在组的标识号
	//放系统打开文件表的下标,故<sysopenfile
	unsigned short 	u_ofile[NOFILE];   /*用户打开文件表=SYSOPENFILE+1,表示关闭一个文件*/
};
	
//all variables
extern struct hinode   hinode[NHINO];//每组头指针所以工NHINO个，同余的为一组
extern struct dir      dir;           /*当前目录(在内存中全部读入)*/
extern struct file     sys_ofile[SYSOPENFILE];
extern struct filsys   filsys;        /*内存中的超级块*/
extern struct pwd      pwd[PWDNUM];
extern struct user     user[USERNUM];
//extern struct file     *fd;           /*the file system column of all the system*/    //xiao
extern struct inode    *cur_path_inode;
extern int             user_id;
extern char            disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];//（i节点块32+数据块512+引导块1+超级块1）*块大小

// all functions
extern struct        inode *iget(unsigned int);
extern void          iput(struct inode *);
extern unsigned int  balloc(unsigned int);
extern unsigned int  balloc();
extern void          bfree(unsigned int);
extern struct inode* ialloc();
extern void          ifree(unsigned int);
extern int			 namei(char *);
extern unsigned short iname(char *);
extern unsigned int  access(unsigned int,struct inode *,unsigned short);
extern void          _dir();
extern void          mkdir(char *);
extern void          chdir(char *);
extern short		 open(int,char*,char);
extern int           creat(unsigned int,char *,unsigned short);
extern unsigned int  read(int fd, char *buf, unsigned int size);
extern unsigned int  write(int fd,char *buf,unsigned int size);
extern int           login(unsigned short, char *);
extern int           logout(unsigned short);
extern void          install();
extern void          format();
extern void          close(unsigned int,unsigned short);
extern void          halt();
extern void          delete(char *);
extern int 			 shell(int user_id,char *str);

#endif



