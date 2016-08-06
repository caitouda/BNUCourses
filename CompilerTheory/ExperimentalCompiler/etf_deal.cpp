#include "pl0.h"

/*
  *表达式处理
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;/*用于保存正负号*/
	bool nxtlev[symnum];
	if(sym==plus||sym==minus)/*开头的正负号，此时当前表达式被看作一个正的或负的项*/
	{
		addop=sym;/*保存开头的正负号*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);/*处理项*/
		if(addop==minus)
		{
			gendo(opr,0,1);/*如果开头为负号生成取负指令*/
		}
	}
	else/*此时表达式被看作项的加减*/
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);/*处理项*/
	}
	while(sym==plus||sym==minus)
	{
		addop=sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);/*处理项*/
		if(addop==plus)
		{
			gendo(opr,0,2);/*生成加法指令*/
		}
		else
		{
			gendo(opr,0,3);/*生成减法指令*/
		}
	}
	return 0;
}

/*
  *项处理
*/
int term(bool*fsys,int *ptx,int lev)
{
    enum symbol mulop;/*用于保存乘除法符号*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
    factordo(nxtlev,ptx,lev);/*处理因子*/
    while(sym==times||sym==slash)
    {
		mulop=sym;
        getsymdo;
        factordo(nxtlev,ptx,lev);
        if(mulop==times)
        {
            gendo(opr,0,4);/*生成乘法指令*/
        }
        else
        {
            gendo(opr,0,5);/*生成除法指令*/
        }
    }
	return 0;
}

/*
  *因子处理
*/
int factor(bool*fsys,int *ptx,int lev)
{
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);/*检测因子的开始符好号*/
    while(inset(sym,facbegsys))/*循环直到不是因子开始符号*/
    {
        if(sym==ident)/*因子为常量或者变量*/
        {
            i=position(id,*ptx);/*查找名字*/
            if(i==0)
            {
                error(11);/*标识符未声明*/
            }
            else
            {
				switch(table[i].kind)
				{
				case constant:/*名字为常量*/
					gendo(lit,0,table[i].val);/*直接把常量的值入栈*/
					break;
				case variable:/*名字为变量*/
					gendo(lod,lev-table[i].level,table[i].adr);/*找到变量地址并将其值入栈*/
					break;
				case procedur:/*名字为过程*/
					error(21);/*不能为过程*/
					break;
				}
			}
			getsymdo;
		}
		else
		{
			if(sym==number)/*因子为数*/
			{
				if(num>amax)
				{
					error(31);
					num=0;
				}
				gendo(lit,0,num);
				getsymdo;
			}
			else
			{
				if(sym==lparen)/*因子为表达式*/
				{
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen]=true;
					expressiondo(nxtlev,ptx,lev);
					if(sym==rparen)
					{
						getsymdo;
					}
					else
					{
						error(22);/*缺少右括号*/
					}
				}
				testdo(fsys,facbegsys,23);/*因子后有非法符号*/
			}
		}
	}
	return 0;
}
