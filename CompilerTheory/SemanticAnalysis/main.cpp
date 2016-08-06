#include"pl0.H"
#include<iostream>
using namespace std;

void Choose();

int main()
{
    bool nxtlev[symnum];
    fin=fopen("f.txt","r");
    if (fin)
    {
        printf("是否输出汇编代码？(Y/N)："); /*是否输出虚拟机代码*/
        scanf("%s",fname);
        listswitch=(fname[0]=='y'||fname[0]=='Y');

        fa1=fopen("fa1.tmp","w");
        fprintf(fa1,"Iput pl/0 file ?");
        fprintf(fa1,"%s\n", fname);

        init();//初始化
        err=0;
        cc=cx=ll=0;
        ch=' ';

        if(-1!=getsym())
        {
            fa=fopen("fa.tmp","w");
            fas=fopen("fas.tmp","w");
            addset(nxtlev,declbegsys,statbegsys,symnum);
            nxtlev[period]=true;
            if(-1==block(0,0,nxtlev))/*调用编译程序*/
            {
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);
            if(sym!=period)
            {
                error(9);
            }
            if(err==0)
            {
                fa2=fopen("fa2.tmp", "w");
                fclose(fa2);
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

