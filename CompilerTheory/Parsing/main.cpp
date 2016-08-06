#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<map>
using namespace std;

typedef struct t
{
    string name,kind;
    int val,lev,adr,size;
} Table;

bool isChengXu(int lev);
bool isFenChengXu(int lev);
bool isChengXuShouBu();
bool isChangLiangShuoMing();
bool isBianLiangShuoMing();
bool isGuoChengShuoMing(int lev);
bool isYuJuBuFen();
bool isChangLiangDingYi();
bool isGuoChengShouBu(int lev);
bool isYuJu();
bool isFuHeYuJu();
bool isFuZhiYuJu();
bool isWhileDoYuJu();
bool isIfYuJu();
bool isCallYuJu();
bool isWriteYuJu();
bool isReadYuJu();
bool isTiaoJian();
bool isYinZi();
bool isXiang();
bool isBiaoDaShi();
string find(string s);
int stoi(string s);//字符串转化成数字
string itos(int n);//数字转化成字符串
int error(int e,int eline);//指出在eline行有错误e
bool isNumber(string s);//判断一个字符串是不是一个数字
string strtoupper(string s);//把字符串转换为大写
string strtolower(string s);//把字符串转换为小写
int lvkongge(string mSourse,int &i);//虑空格，遇到文件末尾结束，返回-1
string getnext(string mSourse,int &i);//从源文件中识别一个单词并返回
bool cifafenxi(string mSourse);//词法分析的主函数，执行虑空格操作，然后用getnext函数获取下一个单词进行分析，依次循环知道lvkongge函数返回-1
void initmp();//初始化保留字界符的查找表，
bool nexteql(string s);//从词法分析的结果顶部取出一个单词，判断s是否与之相等，不相等返回FALSE，相等返回TRUE并且顶部指针指向词法分析结果的下一个单词

map<string,string> words;//保留字，界符等等，的对照表

string wQueue[200][2];//词法分析的最终结果，模拟成一个队列【wQueue[i][0]是第i个位置的单词，wQueue[i][1]是第i个单词的类别】
int whead=0,wtail=0;//队列的头，尾指针

int wline[200]= {0},line=1;//单词的行号，与wQueue对应

string name;//每一次调用nexteqls函数都会改变，具体看nexteqls函数

int etop=0;

Table table[100];
int tabletop=0,padr=1,pradr=3,plev=-1,ltx=0;

string find(string s)
{
    int i=0;
    while(i<tabletop)
    {
        if(table[i].name==s)
        {
            return table[i].kind;
        }
        i++;
    }
    return "@_@";
}

int stoi(string s)//把字符串转换成整数
{
    int i=-1;
    int r=0;
    while(s[++i])
    {
        r*=10;
        r+=s[i]-'0';
    }
    return r;
}

string itos(int n)//把整数n转换成string型
{
    string s;
    char c[10]="";
    int l=8;
    c[9]=0;
    while(n>=10)
    {
        c[l--]=n%10+'0';
        n/=10;
    }
    c[l--]=n+'0';
    s=c+(++l);
    return s;
}

int error(int e,int eline)//错误处理
{
    switch(e)
    {
    case 0:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法字符"<<endl;
        break;
    }
    case 1:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有超过14个字母的单词"<<endl;
        break;
    }
    case 2:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有数字开头的标识符"<<endl;
        break;
    }
    case 3:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有超过10个字母的标识符"<<endl;
        break;
    }
    case 4:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行程序结尾没有."<<endl;
        break;
    }
    case 5:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行程序首部标识符后面没有;"<<endl;
        break;
    }
    case 6:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行程序首部PRAGRAM后面没有标识符"<<endl;
        break;
    }
    case 7:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行没有程序首部"<<endl;
        break;
    }
    case 8:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行分程序后缺少语句部分"<<endl;
        break;
    }
    case 9:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行常量说明的,字符后面有错误"<<endl;
        break;
    }
    case 10:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行常量说明后面没有;结尾"<<endl;
        break;
    }
    case 11:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行常量定义有错误"<<endl;
        break;
    }
    case 12:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行变量说明多了,"<<endl;
        break;
    }
    case 13:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行变量说明没有;结尾"<<endl;
        break;
    }
    case 14:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行VAR后面没有定义的标识符"<<endl;
        break;
    }
    case 15:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行分程序后面没有;结尾"<<endl;
        break;
    }
    case 16:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行过程说明后面没有分程序"<<endl;
        break;
    }
    case 17:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行过程首部后面没有;结尾"<<endl;
        break;
    }
    case 18:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行常量定义没有指定数字"<<endl;
        break;
    }
    case 19:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行常量定义没有=字符"<<endl;
        break;
    }
    case 20:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行过程首部PROCEDURE后面并没有标识符"<<endl;
        break;
    }
    case 21:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行语句后面没有;结尾"<<endl;
        break;
    }
    case 22:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行没有END结尾，或者此行语句缺少;结尾"<<endl;
        break;
    }
    case 23:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行复合语句的BEGIN后面不是语句"<<endl;
        break;
    }
    case 24:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行赋值语句后面不是表达式"<<endl;
        break;
    }
    case 25:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行赋值语句没有:=赋值号"<<endl;
        break;
    }
    case 26:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行READ语句参数中有非法,字符"<<endl;
        break;
    }
    case 27:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行READ语句缺少)字符"<<endl;
        break;
    }
    case 28:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行READ语句缺少标识符"<<endl;
        break;
    }
    case 29:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行READ语句缺少(字符"<<endl;
        break;
    }
    case 30:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WRITE语句参数中有非法,字符"<<endl;
        break;
    }
    case 31:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WRITE语句缺少)字符"<<endl;
        break;
    }
    case 32:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WRITE语句缺少标识符"<<endl;
        break;
    }
    case 33:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WRITE语句缺少(字符"<<endl;
        break;
    }
    case 34:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行CALL语句没有过程名字"<<endl;
        break;
    }
    case 35:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行IF语句的THEN后面不是合法语句"<<endl;
        break;
    }
    case 36:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行IF语句的条件后面没有THEN"<<endl;
        break;
    }
    case 37:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行IF语句后面不是合法条件"<<endl;
        break;
    }
    case 38:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WHILE语句的DO后面不是合法语句"<<endl;
        break;
    }
    case 39:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WHILE语句没有DO"<<endl;
        break;
    }
    case 40:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行WHILE语句后面不是合法的条件"<<endl;
        break;
    }
    case 41:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行表达式的右边缺少)字符"<<endl;
        break;
    }
    case 42:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法表达式"<<endl;
        break;
    }
    case 43:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法因子"<<endl;
        break;
    }
    case 44:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法项"<<endl;
        break;
    }
    case 45:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法项"<<endl;
        break;
    }
    case 46:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法项"<<endl;
        break;
    }
    case 47:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行有非法项"<<endl;
        break;
    }
    case 48:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行关系运算符有非法表达式"<<endl;
        break;
    }
    case 49:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行ODD有非法表达式"<<endl;
        break;
    }
    case 101:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行给非变量赋值或变量没有定义"<<endl;
        break;
    }
    case 102:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行read语句读入非变量或变量没有定义"<<endl;
        break;
    }
    case 103:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行read语句读入非变量或变量没有定义"<<endl;
        break;
    }
    case 106:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行call语句后面不是过程名称或过程没有定义"<<endl;
        break;
    }
    case 107:
    {
        etop++;
        cout<<itos(e)<<": "<<itos(eline)<<"行因子出错"<<endl;
        break;
    }
    }
    return e;
}

bool isNumber(string s)//判断s是不是数字
{
    int i=0;
    while(s[i])
    {
        if(s[i]<'0'||s[i]>'9')
        {
            return false;
        }
        i++;
    }
    return true;
}

string strtoupper(string s)//转化成大写
{
    return s;
    int i=0;
    while(s[i])
    {
        if(s[i]>='a'&&s[i]<='z')
            s[i]-=32;
        i++;
    }
    return s;

}

string strtolower(string s)//转化成小写
{
    return s;
    int i=0;
    while(s[i])
    {
        if(s[i]>='A'&&s[i]<='Z')
            s[i]+=32;
        i++;
    }
    return s;
}

//虑空格
int lvkongge(string mSourse,int &i)
{
    if(i<0)return -1;
    while(mSourse[i]==32||mSourse[i]==9||mSourse[i]==10)
    {
        if(mSourse[i]=='\n')
            line++;
        i++;
    }
    if(mSourse[i]==0)return -1;
    return i;
}

//获得下一个单词
string getnext(string mSourse,int &i)
{
    string word;
    while(mSourse[i]!=' '&&mSourse[i]!='\n')
    {
        if((mSourse[i]>='a'&&mSourse[i]<='z')||(mSourse[i]>='A'&&mSourse[i]<='Z')||(mSourse[i]>='0'&&mSourse[i]<='9'))
        {
            word+=mSourse[i];
            i++;
            continue;
        }
        switch(mSourse[i])
        {
        case ';':
        {
            return word==""?(i++,";"):word;
            break;
        }
        case '.':
        {
            return word==""?(i=-1,"."):word;
            break;
        }
        case ',':
        {
            return word==""?(i++,","):word;
            break;
        }
        case '(':
        {
            return word==""?(i++,"("):word;
            break;
        }
        case ')':
        {
            return word==""?(i++,")"):word;
            break;
        }
        case '<':
        {
            if(mSourse[i+1]=='='&&word=="")return (i+=2,"<=");
            return word==""?(i++,"<"):word;
            break;
        }
        case '>':
        {
            if(mSourse[i+1]=='='&&word=="")return (i+=2,">=");
            return word==""?(i++,">"):word;
            break;
        }
        case ':':
        {
            if(mSourse[i+1]=='='&&word=="")return (i+=2,":=");
            return word==""?(i++,":"):word;
            break;
        }
        case '=':
        {
            return word==""?(i++,"="):word;
            break;
        }
        case '+':
        {
            return word==""?(i++,"+"):word;
            break;
        }
        case '-':
        {
            return word==""?(i++,"-"):word;
            break;
        }
        case '*':
        {
            return word==""?(i++,"*"):word;
            break;
        }
        case '/':
        {
            return word==""?(i++,"/"):word;
            break;
        }
        case '#':
        {
            return word==""?(i++,"#"):word;
            break;
        }
        default:
        {
            return "@_@";//如果是非法字符
        }
        }
        word+=mSourse[i];
        i++;
    }
    return word;
}

//词法分析函数
bool cifafenxi(string mSourse)
{
    int i=0;
    string word;
    while(1)
    {
        if(lvkongge(mSourse,i)==-1)break;
        word=getnext(mSourse,i);
        if(word=="@_@")
        {
            error(0,line);//cout<<line<<"行有非法字符!"<<endl;
            return false;
        }
        //word=strtoupper(word);
        if(words.find(word)!=words.end())//词法分析把结果存入队列
        {
            wQueue[wtail][0]=word;
            wline[wtail]=line;
            wQueue[wtail++][1]=words[word];
        }
        else
        {
            //word=strtolower(word);
            if(word.length()>14)
            {
                word=word.substr(0,14);//名称过长
                if(isNumber(word))
                {
                    word="0";
                }
                error(1,line);//cout<<line<<"行有超过14个字母的单词，已经被截断"<<endl;
            }
            if(isNumber(word))
            {
                wQueue[wtail][0]=word;
                wline[wtail]=line;
                wQueue[wtail++][1]="NUMBER";
            }
            else
            {
                if(word[0]>='0'&&word[0]<='9')
                {
                    error(2,line);//数字开头的标识符非法
                    return false;
                }
                if(word.length()>10)
                {
                    word=word.substr(0,10);//名称过长
                    error(3,line);//cout<<line<<"行有超过10个字母的单词，已经被截断"<<endl;
                }
                wQueue[wtail][0]=word;
                wline[wtail]=line;
                wQueue[wtail++][1]="IDENT";
            }
        }
    }
    return true;
}

//初始化保留字、界符对照表
void initmp()
{
    words["BEGIN"]="BEGINSYM";
    words["CALL"]="CALLSYM";
    words["CONST"]="CONSTSYM";
    words["DO"]="DOSYM";
    words["END"]="ENDSYM";
    words["IF"]="IFSYM";
    words["ODD"]="ODDSYM";
    words["PROCEDURE"]="PROCSYM";
    words["PROGRAM"]="PROGSYM";
    words["READ"]="READSYM";
    words["THEN"]="THENSYM";
    words["VAR"]="VARSYM";
    words["WHILE"]="WHILESYM";
    words["WRITE"]="WRITESYM";
    words[":="]="BECOMES";
    words["+"]="PLUS";
    words["-"]="MINUS";
    words["*"]="TIMES";
    words["/"]="SLASH";
    words["("]="LPAREN";
    words[")"]="RPLREN";
    words["<"]="LQT";
    words[">"]="RQT";
    words["<="]="LEQ";
    words[">="]="REQ";
    words["="]="EQL";
    words[","]="COMMA";
    words["."]="PERIED";
    words["#"]="NEQ";
    words[";"]="SEMICOLON";
}

bool nexteql(string s)//判断wQueue中的whead位置的字符是否等于s，如果等于则whead++,返回true并且更改全局变量name的值
{
    string tmp=wQueue[whead][1];
    line=wline[whead];
    name=wQueue[whead++][0];
    if(s=="guanxiyunsuan")
    {
        if(tmp=="NEQ"||tmp=="RQT"||tmp=="REQ"||tmp=="EQL"||tmp=="LEQ"||tmp=="LQT")
        {
            return true;
        }
    }
    else if(s=="*/")
    {
        if(tmp=="TIMES"||tmp=="SLASH")
        {
            return true;
        }
    }
    else if(s=="+-")
    {
        if(tmp=="PLUS"||tmp=="MINUS")
        {
            return true;
        }
    }
    else if(tmp==s)
    {
        return true;
    }
    whead--;
    if(whead!=0)
        line=wline[whead-1];
    if(whead==wtail-1)
    {
        table[ltx].size=table[tabletop-1].adr==-1?3:(1+table[tabletop-1].adr);
    }
    return false;
}

bool isChengXu(int lev)
{
    if(isChengXuShouBu())
    {
        if(isFenChengXu(lev))
        {
            if(nexteql("PERIED"))
            {
                return true;
            }
            else
            {
                error(4,line);//程序没有.结尾
            }
        }
    }
    return false;
}

bool isChengXuShouBu()
{
    if(nexteql("PROGSYM"))
    {
        if(nexteql("IDENT"))
        {
            string name1=name;
            if(nexteql("SEMICOLON"))
            {
                //addtotable(name1,"PROGRAM",0,0,0,3);
                return true;
            }
            else
            {
                error(5,line);//程序首部没有;结尾
            }
        }
        else
        {
            error(6,line);//PROGSYM后面没有标识符
        }
    }
    else
    {
        error(7,line);//程序首部没有PROGSYM声明
    }
    return false;
}

bool isFenChengXu(int lev)
{
    while(isChangLiangShuoMing());
    while(isBianLiangShuoMing());
    while(isGuoChengShuoMing(lev));
    if(isYuJuBuFen())
    {
        return true;
    }
    else
    {
        error(8,line);//没有语句说明部分
    }
    return false;
}

bool isChangLiangShuoMing()
{
    if(nexteql("CONSTSYM"))
    {
        if(isChangLiangDingYi())
        {
            while(nexteql("COMMA"))
            {
                if(isChangLiangDingYi())
                {
                    continue;
                }
                else
                {
                    error(9,line);//常量定义错误
                }
            }
            if(nexteql("SEMICOLON"))
            {
                return true;
            }
            else
            {
                error(10,line);//没有;结尾
            }
        }
        else
        {
            error(11,line);//常量定义错误
        }
    }
    else
    {
        return false;//不是常量说明
    }
    return false;
}

bool isBianLiangShuoMing()
{
    if(nexteql("VARSYM"))
    {
        if(nexteql("IDENT"))
        {
            //addtotable(name,"VARIABLE",0,plev+1,pradr++,-1);
            while(nexteql("COMMA"))
            {
                if(nexteql("IDENT"))
                {
                    //addtotable(name,"VARIABLE",0,plev+1,pradr++,-1);
                    continue;
                }
                else
                {
                    error(12,line);//逗号后面不是标识符
                }
            }
            if(nexteql("SEMICOLON"))
            {
                return true;
            }
            else
            {
                error(13,line);//没有;结尾
            }
        }
        else
        {
            error(14,line);//VAR后面没有标识符
        }
    }
    else
    {
        return false;
    }
    return false;
}

bool isGuoChengShuoMing(int lev)
{
    if(isGuoChengShouBu(lev))
    {
        if(nexteql("SEMICOLON"))
        {
            if(isFenChengXu(lev+1))
            {
                if(nexteql("SEMICOLON"))
                {
                    return true;
                }
                else
                {
                    error(15,line);//分程序后面没有;结尾
                }
            }
            else
            {
                error(16,line);//过程说明后面没有分程序
            }
        }
        else
        {
            error(17,line);//过程首部后面缺少分号
        }
    }
    else
    {
        return false;//不是过程说明
    }
    return false;
}

bool isYuJuBuFen()
{
    if(isYuJu())
    {
        return true;
    }
    else if(isFuHeYuJu())
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool isChangLiangDingYi()
{
    if(nexteql("IDENT"))
    {
        string s=name;
        if(nexteql("EQL"))
        {
            if(nexteql("NUMBER"))
            {
                //addtotable(s,"CONST",stoi(name),-1,-1,-1);
                return true;
            }
            else
            {
                error(18,line);//常量定义中标识符后面没有数字
            }
        }
        else
        {
            error(19,line);//没有等号
        }
    }
    else
    {
        return false;//不是常量定义
    }
    return false;
}

bool isGuoChengShouBu(int lev)
{
    if(nexteql("PROCSYM"))
    {
        if(nexteql("IDENT"))
        {
            //addtotable(name,"PROCEDURE",0,lev,padr++,3);
            return true;
        }
        else
        {
            error(20,line);//过程首部没有标识符
        }
    }
    else
    {
        return false;//不是过程首部
    }
    return false;
}

bool isYuJu()
{
    if(isFuZhiYuJu())
    {
        return true;
    }
    else if(isIfYuJu())
    {
        return true;
    }
    else if(isWhileDoYuJu())
    {
        return true;
    }
    else if(isCallYuJu())
    {
        return true;
    }
    else if(isWriteYuJu())
    {
        return true;
    }
    else if(isReadYuJu())
    {
        return true;
    }
    else if(isFuHeYuJu())
    {
        return true;
    }
    else
    {
        return false;//不是语句
    }
    return false;
}

bool isFuHeYuJu()
{
    if(nexteql("BEGINSYM"))
    {
        if(isYuJu())
        {
            while(nexteql("SEMICOLON"))
            {
                if(isYuJu())
                {
                    continue;
                }
                else
                {
                    error(21,line);//没有;结尾
                }
            }
            if(nexteql("ENDSYM"))
            {
                return true;
            }
            else
            {
                error(22,line);//没有END结尾
            }
        }
        else
        {
            error(23,line);//BEGIN后面不是语句；
        }
    }
    else
    {
        return false;//不是复合语句
    }
    return false;
}

bool isFuZhiYuJu()
{
    int x=whead;
    if(nexteql("IDENT"))
    {
        if(find(name)!="VARIABLE")
        {
            error(101,line);
            return false;
        }
        if(nexteql("BECOMES"))
        {
            if(isBiaoDaShi())
            {
                //printYuFa(x,whead,"赋值");
                return true;
            }
            else
            {
                error(24,line);//不是表达式
            }
        }
        else
        {
            error(25,line);//没有BECOMES符号
        }
    }
    else
    {
        return false;//不是赋值语句
    }
    return false;
}

bool isReadYuJu()
{
    int x=whead;
    if(nexteql("READSYM"))
    {
        if(nexteql("LPAREN"))
        {
            if(nexteql("IDENT"))
            {
                if(find(name)!="VARIABLE")
                {
                    error(102,line);
                    return false;
                }
                while(nexteql("COMMA"))
                {
                    if(nexteql("IDENT"))
                    {
                        if(find(name)!="VARIABLE")
                        {
                            error(103,line);
                            return false;
                        }
                        continue;
                    }
                    else
                    {
                        error(26,line);//逗号后没有标识符
                    }
                }
                if(nexteql("RPLREN"))
                {
                    //printYuFa(x,whead,"READ");
                    return true;
                }
                else
                {
                    error(27,line);//没有)
                }
            }
            else
            {
                error(28,line);//没有标识符
            }
        }
        else
        {
            error(29,line);//没有(
        }
    }
    else
    {
        return false;//不是read语句
    }
    return false;
}

bool isWriteYuJu()
{
    int x=whead;
    if(nexteql("WRITESYM"))
    {
        if(nexteql("LPAREN"))
        {
            if(isBiaoDaShi())
            {
                while(nexteql("COMMA"))
                {
                    if(isBiaoDaShi())
                    {
                        continue;
                    }
                    else
                    {
                        error(30,line);//逗号后没有标识符
                    }
                }
                if(nexteql("RPLREN"))
                {
                    //printYuFa(x,whead,"WRITE");
                    return true;
                }
                else
                {
                    error(31,line);//没有)
                }
            }
            else
            {
                error(32,line);//没有标识符
            }
        }
        else
        {
            error(33,line);//没有(
        }
    }
    else
    {
        return false;//不是read语句
    }
    return false;
}

bool isCallYuJu()
{
    int x=whead;
    if(nexteql("CALLSYM"))
    {
        if(nexteql("IDENT"))
        {
            if(find(name)!="PROCEDURE")
            {
                error(106,line);
                return false;
            }
            //printYuFa(x,whead,"CALL");
            return true;
        }
        else
        {
            error(34,line);//call没有过程名字
        }
    }
    else
    {
        return false;//不是Call语句
    }
    return false;
}

bool isIfYuJu()
{
    int x=whead;
    if(nexteql("IFSYM"))
    {
        if(isTiaoJian())
        {
            if(nexteql("THENSYM"))
            {
                if(isYuJu())
                {
                    //printYuFa(x,whead,"IF");
                    return true;
                }
                else
                {
                    error(35,line);//THEN后面不是语句
                }
            }
            else
            {
                error(36,line);//没有THEN语句
            }
        }
        else
        {
            error(37,line);//IF后面不是条件
        }
    }
    else
    {
        return false;//不是if语句
    }
    return false;
}

bool isWhileDoYuJu()
{
    int x=whead;
    if(nexteql("WHILESYM"))
    {
        if(isTiaoJian())
        {
            if(nexteql("DOSYM"))
            {
                if(isYuJu())
                {
                    //printYuFa(x,whead,"WHILE");
                    return true;
                }
                else
                {
                    error(38,line);//DO后面不是语句
                }
            }
            else
            {
                error(39,line);//没有DO
            }
        }
        else
        {
            error(40,line);//while后不是条件
        }
    }
    else
    {
        return false;//不是while语句
    }
    return false;
}

bool isYinZi()
{
    if(nexteql("IDENT"))
    {
        if(find(name)!="VARIABLE"&&find(name)!="CONST")
        {
            error(107,line);
            return false;
        }
        return true;
    }
    else if(nexteql("NUMBER"))
    {
        return true;
    }
    else if(nexteql("LPAREN"))
    {
        if(isBiaoDaShi())
        {
            if(nexteql("RPLREN"))
            {
                return true;
            }
            else
            {
                error(41,line);//没有)
            }
        }
        else
        {
            error(42,line);//不是表达式
        }
    }
    else
    {
        return false;//不是因子
    }
    return false;
}

bool isXiang()
{
    if(isYinZi())
    {
        while(nexteql("*/"))
        {
            if(isYinZi())
            {
                continue;
            }
            else
            {
                error(43,line);//乘法运算符后不是项
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;//不是项
    }
    return false;
}

bool isBiaoDaShi()
{
    if(nexteql("+-"))
    {
        if(isXiang())
        {
            while(nexteql("+-"))
            {
                if(isXiang())
                {
                    continue;
                }
                else
                {
                    error(44,line);//有非法项
                    return false;
                }
            }
            return true;
        }
        else
        {
            error(45,line);//不是项
        }
    }
    else
    {
        if(isXiang())
        {
            while(nexteql("+-"))
            {
                if(isXiang())
                {
                    continue;
                }
                else
                {
                    error(46,line);//有非法项
                    return false;
                }
            }
            return true;
        }
        else
        {
            error(47,line);//不是项
        }
    }
    return true;
}

bool isTiaoJian()
{
    if(isBiaoDaShi())
    {
        if(nexteql("guanxiyunsuan"))
        {
            if(isBiaoDaShi())
            {
                return true;
            }
            else
            {
                error(48,line);//关系运算符后面不是表达式
            }
        }
    }
    else if(nexteql("ODDSYM"))
    {
        if(isBiaoDaShi())
        {
            return true;
        }
        else
        {
            error(49,line);//ODD后面不是表达式
        }
    }
    else
    {
        return false;//不是条件
    }
    return false;
}

int main()
{
    freopen("in.txt","r",stdin);
    string mSourse="";
    string lines;
    initmp();
    while(getline(cin,lines))
    {
        mSourse+=lines+" \n ";
    }//读取文件中的PL0代码
    cifafenxi(mSourse);//词法分析
    isChengXu(0);//语法分析错误报告
    if(etop==0)
        cout<<"0 error,Success!"<<endl;
    else
        cout<<etop<<"errors"<<endl;
    return 0;
}

