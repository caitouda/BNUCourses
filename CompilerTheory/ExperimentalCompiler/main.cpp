#include "pl0.H"
#include <iostream>
using namespace std;

void Choose();

int main()
{
	bool nxtlev[symnum];
	fin=fopen("test.txt","r");
	if (fin)
	{
		printf("是否输出汇编代码？(Y/N)：");/*是否输出虚拟机代码*/
		scanf("%s",fname);
		listswitch=(fname[0]=='y'||fname[0]=='Y');
		printf("是否输出符号表？(Y/N)：");/*是否输出名字表*/
		scanf("%s",fname);
		tableswitch=(fname[0]=='y'||fname[0]=='Y');

		file1=fopen("file1.txt","w");
		fprintf(file1,"载入PL/0文件？");
		fprintf(file1,"%s\n", fname);

		init();//初始化
		err=0;
		cc=cx=ll=0;
		ch=' ';

		if(-1!=getsym())
		{
			file2=fopen("file2.txt","w");
			file3=fopen("file3.txt","w");
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period]=true;    
			if(-1==block(0,0,nxtlev))/*调用编译程序*/
			{
				fclose(file2);
				fclose(file1);
				fclose(file3);
				fclose(fin);
				printf("\n");
				return 0;
			}
			fclose(file2);
			fclose(file1);
			fclose(file3);
			if(sym!=period)
			{
				error(9);
			}
			if(err==0)
			{
				file4=fopen("file4.txt", "w");
				interpret();
				fclose(file4);
			}
			else
			{
				printf("Errors in pl/0 program");
			}
		}
		
		fclose(fin);
	}
    else
	{
		printf("Can't open file! \n");
	}
	printf("\n");
	
	return 0;
}	
