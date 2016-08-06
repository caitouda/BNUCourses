#include "pl0.h"

int flag;

/*
  *编译程序主体
  *
  *lev:当前分程序所在层
  *tx:名字表当前尾指针
  *fsys:当前模块后跟符号集合
*/
int block(int lev,int tx,bool* fsys)
{
    int i;
    int dx;/*名字分配到的相对地址*/
    int tx0;/*保留初始tx*/
    int cx0;/*保留初始cx*/
	bool nxtlev[symnum];//在下级函数的参数中，符号集合均为值参，但由于使用数组
						//实现，传递进来的是指针，为防止下级函数改变上级函数的								
						//集合，开辟新的空间传递给下级函数
	dx=3;
	tx0=tx;/*记录本层名字的初始位置*/
	table[tx].adr=cx;
	gendo(jmp,0,0);
	if(lev > levmax)
	{
		error(32);                                                                                                                                         
	}
	do{
		if(sym==constsym)/*收到常量声明符号，开始处理常量声明*/
		{
			getsymdo;
			do{
			constdeclarationdo(&tx,lev,&dx);/*dx的值会被constdeclaration改变，使用指针*/
			while(sym==comma)
			{
				getsymdo;
				constdeclarationdo(&tx,lev,&dx);
			}
			if(sym==semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);/*漏掉了逗号或者分号*/
			}
			}while(sym==ident);
		}
		if(sym==varsym)/*收到变量声名符号，开始处理变量声名*/
		{
			getsymdo;
			do{
				vardeclarationdo(&tx,lev,&dx);
				while(sym==comma)
				{
					getsymdo;
					vardeclarationdo(&tx,lev,&dx);
				}
				if(sym==semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5);
				}
			}while(sym==ident);
		}
		while(sym==procsym)/*收到过程声名符号，开始处理过程声名*/
		{
			getsymdo;
			if(sym==ident)
			{
				enter(procedur,&tx,lev,&dx);/*记录过程名字*/
				getsymdo;
			}
			else
			{
				error(4);/*procedure后应为标识符*/
			}
			if(sym==semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);/*漏掉了分号*/
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
			nxtlev[semicolon]=true;
			if(-1==block(lev+1,tx,nxtlev))
			{
				return -1;/*递归调用*/
			}
            if(sym==semicolon)
            {
                getsymdo;
                memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
                nxtlev[ident]=true;
                nxtlev[procsym]=true;
                testdo(nxtlev,fsys,6);
			}
			else
			{
				error(5);/*漏掉了分号*/
			}
        }

		memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
		nxtlev[ident]=true;
		nxtlev[period]=true;
		testdo(nxtlev,declbegsys,7);

	}while(inset(sym,declbegsys));/*直到没有声明符号*/
	
	code[table[tx0].adr].a=cx;/*开始生成当前过程代码*/
	table[tx0].adr=cx;/*当前过程代码地址*/
	table[tx0].size=dx;/*声明部分中每增加一条声明都会给dx增加1,声明部分已经结束,dx就是当前过程数据的size*/
	
    if(flag==1)
		cx0=cx;
	else
		cx0=0;
	flag=1;
	gendo(inte,0,dx);/*生成分配内存代码*/
	if(tableswitch)/*输出名字表*/
	{
		printf("符号表：\n");
		if(tx0+1>tx)
		{
			printf("NULL\n");
		}
		for(i=tx0+1;i<=tx;i++)
		{
			switch(table[i].kind)
			{
			case constant:
				printf("%d const\t%s\t",i,table[i].name);
				printf("val=%d\n",table[i].val);
				fprintf(file3,"%d const\t %s\t",i,table[i].name);
				fprintf(file3,"val=%d\n",table[i].val);
				break;
			case variable:
				printf("%d var\t%s\t",i,table[i].name);
				printf("lev=%d\t addr=%d\n",table[i].level,table[i].adr);
				fprintf(file3,"%d var\t%s\t",i,table[i].name);
				fprintf(file3,"lev=%d\t addr=%d\n",table[i].level,table[i].adr);
				break;
			case procedur:
				printf("%d proc\t%s\t",i,table[i].name);
				printf("lev=%d\t addr=%d\t size=%d\n",table[i].level,table[i].adr,table[i].size);
				fprintf(file3,"%d proc\t%s\t",i,table[i].name);
				fprintf(file3,"lev=%d\t adr=%d\t size=%d \n",table[i].level,table[i].adr,table[i].size);
				break;
			}
		}
		printf("\n");
	}
	/*语句后跟符号为分号或end*/
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);/*每个后跟符号集和都包含上层后跟符号集和，以便补救*/
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	statementdo(nxtlev,&tx,lev);
	gendo(opr,0,0);/*每个过程出口都要使用的释放数据段命令*/
	memset(nxtlev,0,sizeof(bool)*symnum);/*分程序没有补救集合*/
	test(fsys,nxtlev,8);/*检测后跟符号正确性*/
	listcode(cx0);/*输出代码*/
	return 0;
}
