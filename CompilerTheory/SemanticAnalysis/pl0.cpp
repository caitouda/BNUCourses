#include"pl0.h"
#include<stdio.h>
#include<string.h>

FILE * fas;
FILE * fa;
FILE * fa1;
FILE * fa2;

bool tableswitch;
bool listswitch;
char ch;
enum symbol sym;
char id[al+1];
int  num;
int cc,ll;
int cx;
char line[81];
char a[al+1];
struct instruction code[cxmax];
char word[norw][al];
enum symbol wsym[norw];
enum symbol ssym[256];
char mnemonic[fctnum][5];//虚拟机代码指令名称
bool declbegsys[symnum];//声明开始的符号集合
bool statbegsys[symnum];//语句的开始符号集合
bool facbegsys[symnum];//因子开始的符号集合
/*------------------------------*/

struct tablestruct table[txmax];//名字表
FILE * fin;
FILE* fout;
char fname[al];
int err;//错误计数器

//漏掉空格，读取一个字符
//每次读一行，存入line缓冲区，line被getsym取空后再读一行
//被函数getsym调用

int getch()
{
	if(cc==ll)
    {
		if(feof(fin))
		{
			printf("\n program incomplete\n ");
			return -1;
		}
		ll=0;
		cc=0;
		printf("%d ",cx );
		fprintf(fa1,"%d ",cx);
		ch=' ';
		while(ch!=10)
		{
            if(EOF==fscanf(fin,"%c",&ch))
			{
				line[ll]=0;
				break;
			}
			printf("%c",ch);
			fprintf(fa1,"%c",ch);
			line[ll]=ch;
			ll++;
		}
		printf("\n");
		fprintf(fa1,"\n");
	}
	ch=line[cc];
	cc++;
	return 0;
}

int getsym()
{
	int i,j,k;
	while(ch ==' '|| ch==10 || ch==9)getchdo;//忽略空格、换号和TAB

	if (ch >= 'a' && ch <= 'z')
	{
		k=0;
		do
		{
			if (k<al)//al是符号的最大长度
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'));

		a[k] = 0;//添加\0

		strcpy(id,a);//拷贝
		i=0;
		j = norw-1 ;

		do//搜索当前字符是否是保留字
		{
			k= (i+j)/2; //折半查找
			if (strcmp(id,word[k]) <= 0)
			{
				j = k-1;
			}
			if (strcmp(id,word[k]) >= 0)
			{
				i = k+1;
			}
		} while (i <= j);

		if (i-1 > j)
		{
			sym = wsym[k];//成功找到
		}
		else
		{
			sym = ident; // 搜索失败，则是名字或数字
			//printf("3 标示符 %s\n",id);
		}
	}
	else
	{
		if (ch >= '0' && ch <= '9')
		{
			k= 0;
			num = 0;
			sym = number;
			do
			{
				num = 10*num + ch - '0';//转化为数字。
				k++;
				getchdo;
			} while (ch >= '0' && ch <= '9');
			 //printf("4 常数 %d\n",num);
			k--;
			if (k>nmax)
			{
				error(30);
			}
		}
		else
		{
			if (ch == ':')//检测赋值符号
			{
				getchdo;
				if (ch == '=')
				{
					sym = becomes;//赋值
					//printf("2 运算符 =\n");
					getchdo;
				}
				else
				{
					sym = nul;//不能识别的符号
				}
			}
			else
			{
				if (ch == '<')
				{
					getchdo;
					if (ch == '=')
					{
						sym = leq;//  less than or equal to
						// printf("2 运算符 <=\n");
						getchdo;
					}
					else
					{
						sym = lss;//less than
						//printf("2 运算符 <\n");
					}
				}
				else
				{
					if (ch == '>')
					{
						if (ch == '=')
						{
							sym = geq;//be equal or greater than
							//printf("2 运算符 >=\n");
							getchdo;
						}
						else
						{
							sym = gtr;//greater than
							//printf("2 运算符 >\n");
						}
					}
					else
					{
						sym = ssym[ch];//当符号不满足上述条件时，全部按照单字符的符号处理

						if (sym != period)
						{
							getchdo;
						}
					}
				}
			}
		}
	}
	return 0;
}

/*出错处理，打印出错位置和错误编码*/
void error(int n)//出错报告过程（error）
{
	char space[81];
	memset(space,32,81); printf("-------%c\n",ch);//32是空格
	space[cc-1]=0;//出错时当前符号已经读完，所以cc-1
	printf("****%s!%d\n",space,n);//不明白
	err++;
}

/*
  在名字表中加入一项

  k:名字种类const,var or procedure
  ptx:名字表尾指针的指针，为了可以改变名字表尾指针的数值
  lev:名字所在的层次，以后所有的lev都是这样
  pdx:为当前应分配的变量的相对地址，分配后要增加1
*/

void enter(enum object k,int *ptx,int lev, int *pdx)//登录名字表过程（enter）
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);/*全局变量id中已存有当前名字的名字*/
	table[(*ptx)].kind=k;
	switch(k)
	{
	case constant:/*常量名字*/
		if (num>amax)
		{
			error(31);
			num=0;
		}
		table[(*ptx)].val=num;
		break;
	case variable:/*变量名字*/
		table[(*ptx)].level=lev;
		table[(*ptx)].adr=(*pdx);
		(*pdx)++;
		break;/*过程名字*/
	case procedur:
		table[(*ptx)].level=lev;
		break;
	}
}

/*
  查找名字的位置
  找到则返回在名字表中的位置，否则返回0

  idt: 要查找的名字
  tx:：当前名字表尾指针
 */
int position(char *  idt,int  tx)//查询名字表函数（position）
{
	int i;
	strcpy(table[0].name,idt);
	i=tx;
	while(strcmp(table[i].name,idt)!=0)
	{
		i--;
	}
	return i;
}

/*用数组实现集合的集合运算*/
int inset(int e,bool* s)
{
    return s[e];
}

int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]||s2[i];
    }
    return 0;
}

int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&(!s2[i]);
    }
    return 0;
}

int mulset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&s2[i];
    }
    return 0;
}

//生成中间代码
int gen(enum fct x,int y,int z)//代码生成过程（gen）
{
	if(cx>=cxmax)
	{
		printf("Program too long"); /*程序过长*/
		return -1;
	}
	code[cx].f=x;
	code[cx].l=y;
	code[cx].a=z;
	cx++;
	return 0;
}

/*
  测试当前符号是否合法
  在某一部分（如一条语句，一个表达式）将要结束时时我们希望下一个符号属于某集合
  （该部分的后跟符号） test 负责这项检测，并且负责当检测不通过时的补救措施
  程序在需要检测时指定当前需要的符号集合和补救用的集合（如之前未完成部分的后跟
  符号），以及不通过时的错误号

  s1：我们需要的符号
  s2:如果不是我们需要的，则需要一个补救用的集合
  n:错误号
*/
int test(bool* s1,bool* s2,int n)//测试单词合法性及出错恢复过程（test）
{
    if(! inset(sym,s1))
    {
		error(n);
		/*当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合*/
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo;
		}
    }
    return 0;
}

/*常量声明处理*/
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	if(sym==ident)
	{
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			if(sym==becomes)
			{
				error(1);/*把=写出成了：=*/
			}
			getsymdo;
			if(sym==number)
			{
				enter(constant,ptx,lev,pdx);
				getsymdo;
			}
			else
			{
				error(2);                  /*常量说明=后应是数字*/
			}
		}
		else
		{
			error(3);                       /*常量说明标识后应是=*/
		}
	}
	else
	{
		error(4);                        /*const后应是标识*/
	}
	return 0;
}

/*变量声明处理*/
int vardeclaration(int * ptx,int lev,int * pdx)
{
	if(sym==ident)
 	{
 		enter(variable,ptx,lev,pdx);//填写名字表
 		getsymdo;
 	}
 	else
 	{
 		error(4);
    }
    return 0;
}

/*输入目标代码清单*/
void listcode(int cx0)
{
	int i;
   	if (listswitch)
   	{
   		for(i=cx0;i<cx;i++)
   	 	{
   	 		printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	 		fprintf(fa,"%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	    }
   	 }
}


