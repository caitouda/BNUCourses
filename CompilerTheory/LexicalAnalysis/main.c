/*
  程序开始的时候，需要输入读入文件的名字
  输出在“nout.c”中
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

enum symbol
{
    nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,//0-9
    lss,leq,gtr,geq,lparen,rparen,comma,semicolon,period,becomes,//10-19
    beginsym,endsym,ifsym,thensym,whilesym,writesym,//20-25
    readsym,dosym,callsym,constsym,varsym,procsym//26-31
};

int ssym[256];
char word[13][10];
int wsym[13];

void init()
{
    int i;

    strcpy(&(word[0][0]),"begin");
    strcpy(&(word[1][0]),"call");
    strcpy(&(word[2][0]),"const");
    strcpy(&(word[3][0]),"do");
    strcpy(&(word[4][0]),"end");
    strcpy(&(word[5][0]),"if");
    strcpy(&(word[6][0]),"odd");
    strcpy(&(word[7][0]),"procedure");
    strcpy(&(word[8][0]),"read");
    strcpy(&(word[9][0]),"then");
    strcpy(&(word[10][0]),"var");
    strcpy(&(word[11][0]),"while");
    strcpy(&(word[12][0]),"write");

    wsym[0]=beginsym;
    wsym[1]=callsym;
    wsym[2]=constsym;
    wsym[3]=dosym;
    wsym[4]=endsym;
    wsym[5]=ifsym;
    wsym[6]=oddsym;
    wsym[7]=procsym;
    wsym[8]=readsym;
    wsym[9]=thensym;
    wsym[10]=varsym;
    wsym[11]=whilesym;
    wsym[12]=writesym;

//设置单字符符号
    for(i=0; i<=255; i++)
    {
        ssym[i]=nul;
    }
    ssym['+']=plus;
    ssym['-']=minus;
    ssym['*']=times;
    ssym['/']=slash;
    ssym['(']=lparen;
    ssym[')']=rparen;
    ssym['=']=eql;
    ssym[',']=comma;
    ssym['.']=period;
    ssym['#']=neq;
    ssym[';']=semicolon;

}

//根据ID找出单词类别
int find_sym(char a[],int ID)
{
    if(ID==3)
    {
        int temp=zheban(a,0,12);
        if(temp==0)//不是保留字
            return ident;
        else
            return temp;
    }
    if(ID==5)  return number;
    if(ID==7)  return becomes;
    if(ID==9)  return leq;
    if(ID==10) return lss;
    if(ID==12) return geq;
    if(ID==13) return gtr;
    if(ID==14)
    {
        if(ssym[(int)a[0]] != nul)
            return ssym[(int)a[0]];
        else
            return 100;
    }
}

//设置保留字名字
int zheban(char a[],int left,int right)//不能比较前字母，只能比较整个
{
    if(left>right) return 0;
    int middle=(left+right)/2;
    if(a[0]==word[middle][0])
    {
        if(a[1]==word[middle][1])
        {
            if(strcmp(a,word[middle])==0)
            {
                return wsym[middle];
            }
            return 0;
        }
        else if(a[1]<word[middle][1])
            return zheban(a,left,middle-1);
        else
            return zheban(a,middle+1,right);
    }
    else if(a[0]<word[middle][0])
        return zheban(a,left,middle-1);
    else
        return zheban(a,middle+1,right);
}

//判断是不是数字
int Is_Number(char ch)
{
    return ch >= '0' && ch <= '9';
}

//判断是不是字母
int Is_Char(char ch)
{
    return ch >= 'a' && ch <= 'z';
}

void error()
{
    printf("something wrong with your code T_T ...\n");
}

//文件读入与分析
void file_analysis(char fn[])
{
    FILE *fp = NULL;
    FILE *fin = NULL;
    char ch;

    if((fin=fopen("nout.c","w"))==NULL)
    {
        printf("File open error!\n");
        exit(0);
    }

    if((fp=fopen(fn,"r"))==NULL)
    {
        printf("File open error!\n");
        exit(0);
    }

    int next = 1;//1表示从文件读入一个字符，0则表示不读入

    while(1)//读入字符
    {
        char A[10];

        if(next)
        {
            if((ch = fgetc(fp)) == EOF) break;
        }

        int k=0;
        int id;

        if(ch == ' ' || ch == '\n')//问题，不知道PL/0是否允许一个单词被回车分开？
        {
            next = 1;
            continue;
        }

        if(Is_Char(ch))
        {
            id = 3;
            while( Is_Number(ch) || Is_Char(ch))
            {
                A[k++] = ch;
                ch = fgetc(fp);
            }
            next = 0;
        }

        else if(Is_Number(ch))
        {
            id = 5;
            while( Is_Number(ch) )
            {
                A[k++] = ch;
                ch = fgetc(fp);
            }
            next = 0;
        }

        else if(ch == ':')
        {
            id = 7;
            A[k++] = ch;
            ch = fgetc(fp);
            if(ch == '=')
            {
                A[k++] = ch;
                next = 1;
            }
            else
            {
                error();
                break;
            }
        }

        else if(ch == '<')
        {
            A[k++] = ch;
            ch = fgetc(fp);
            if(ch == '=')
            {
                A[k++] = ch;
                next = 1;
                id = 9;
            }
            else
            {
                id = 10;
                next = 0;
            }
        }

        else if(ch == '>')
        {
            A[k++] = ch;
            ch = fgetc(fp);
            if(ch == '=')
            {
                A[k++] = ch;
                next = 1;
                id = 12;
            }
            else
            {
                id = 13;
                next = 0;
            }
        }

        else
        {
            A[k++] = ch;
            id = 14;
            next = 1;
        }

        A[k] = 0;
        int sym;
        sym = find_sym(A,id);
        fputs(A,fin);
        fputs(",",fin);
        fprintf(fin,"%d\n",sym);
    }
}

int main()
{

    init();
    char name[20];

    printf("please input the file's name:");
    scanf("%s",name);
    file_analysis(name);

    return 0;
}
