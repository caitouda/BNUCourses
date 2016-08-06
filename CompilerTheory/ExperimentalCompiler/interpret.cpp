#include "pl0.h"
#define stacksize 500

void interpret()
{
	int p,b,t;/*指令指针，指令基址，栈顶指针*/
	struct instruction i;/*存放当前指令*/
	int s[stacksize];/*栈*/
	printf("\n运行程序：\n");
	t=0;
	b=0;
	p=0;
	s[0]=s[1]=s[2]=0;
	do{
		i=code[p];/*读当前指令*/
		p++;
		switch(i.f)
		{
			case lit:/*将a的值取到栈顶*/
				s[t]=i.a;
				t++;
				break;
			case opr:/*数字、逻辑运算*/
				switch(i.a)
				{
					case 0:
						t=b;
						p=s[t+2];
						b=s[t+1];
						break;
					case 1:
						s[t-1]=-s[t-1];
						break;
					case 2:
						t--;
						s[t-1]=s[t-1]+s[t];
						break;
					case 3:
						t--;
						s[t-1]=s[t-1]-s[t];
						break;
					case 4:
						t--;
						s[t-1]=s[t-1]*s[t];
						break;
					case 5:
						t--;
						s[t-1]=s[t-1]/s[t];
             			break;
					case 6:
						s[t-1]=s[t-1]%2;
						break;
					case 8:
						t--;
						s[t-1]=(s[t-1]==s[t]);
 						break;
					case 9:
						t--;
						s[t-1]=(s[t-1]!=s[t]);
 						break;
					case 10:
						t--;
						s[t-1]=(s[t-1]<s[t]);
 						break;
					case 11:
						t--;
						s[t-1]=(s[t-1]>=s[t]);
 						break;
					case 12:
						t--;
						s[t-1]=(s[t-1]>s[t]);
 						break;
					case 13:
						t--;
						s[t-1]=(s[t-1]<=s[t]);
 						break;
					case 14:
						printf("%d",s[t-1]);
						fprintf(file4,"%d",s[t-1]);
						t--;
						break;
					case 15:
						printf("\n");
						fprintf(file4,"\n");
						break;
					case 16:
						printf("输入程序中变量值(0:退出): ");
						fprintf(file4,"输入程序中变量值(0:退出): ");
						scanf("%d",&(s[t]));
						fprintf(file4,"%d\n",s[t]);
						t++;
						break;
				}
				break;
			case lod:/*取相对当前过程的数据基地址为ａ的内存的值到栈顶*/
				s[t]=s[base(i.l,s,b)+i.a];
				t++;
				break;
			case sto:/*栈顶的值存到相对当前过程的数据基地址为ａ的内存*/
				t--;
	            s[base(i.l,s,b)+i.a]=s[t];
				break;
			case cal:/*调用子程序*/
				s[t]=base(i.l,s,b);/*将父过程基地址入栈*/
				s[t+1]=b;/*将本过程基地址入栈，此两项用于base函数*/
				s[t+2]=p;/*将当前指令指针入栈*/
				b=t;/*改变基地址指针值为新过程的基地址*/
				p=i.a;/*跳转*/
				break;
			case inte:/*分配内存*/
				t+=i.a;
				break;
			case jmp:/*直接跳转*/
				p=i.a;
				break;
			case jpc:/*条件跳转*/
				t--;
				if(s[t]==0)
				{
					p=i.a;
				}
	    		break;
		}
	}while (p!=0);

	printf("\n运行结束\n");
}
/*通过过程基址求上1层过程的基址*/
int base(int l,int * s,int b)
{
	int b1;
	b1=b;
	 while(l>0)
	 {
		 b1=s[b1];
     	 l--;
	 }
	 return b1;
}