#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

const static char IP[64] =//初始置换
{
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const static char EP1[56] =//密钥置换（原64位去掉奇偶校验位后）
{
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

const static char LOOP[16] =//左移
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const static char EP2[48] =//选择子密钥
{
    14, 17, 11, 24,   1,   5,
    3,  28, 15,  6,  21,  10,
    23, 19, 12,  4,  26,   8,
    16,  7, 27, 20,  13,   2,
    41, 52, 31, 37,  47,  55,
    30, 40, 51, 45,  33,  48,
    44, 49, 39, 56,  34,  53,
    46, 42, 50, 36,  29,  32
};

static const char EC[48] =//放大换位
{
    32,   1,   2,   3,   4,   5,
    4,   5,   6,   7,   8,   9,
    8,   9,  10,  11,  12,  13,
    12,  13,  14,  15,  16,  17,
    16,  17,  18,  19,  20,  21,
    20,  21,  22,  23,  24,  25,
    24,  25,  26,  27,  28,  29,
    28,  29,  30,  31,  32,   1
};

const static char SBox[8][4][16] =//8个S盒
{
    {
        //S1
        {  14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7   },
        {   0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8   },
        {   4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0   },
        {  15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13   }
    },
    {
        //S2
        {  15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10   },
        {   3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5   },
        {   0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15   },
        {  13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9   }
    },
    {
        //S3
        {  10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8   },
        {  13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1   },
        {  13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7   },
        {   1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12   }
    },
    {
        //S4
        {   7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15   },
        {  13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9   },
        {  10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4   },
        {   3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14   }
    },
    {
        //S5
        {   2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9   },
        {  14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6   },
        {   4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14   },
        {  11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3   }
    },
    {
        //S6
        {  12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11   },
        {  10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8   },
        {   9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6   },
        {   4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13   }
    },
    {
        //S7
        {   4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1   },
        {  13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6   },
        {   1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2   },
        {   6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12   }
    },
    {
        //S8
        {  13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7   },
        {   1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2   },
        {   7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8   },
        {   2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11   }
    }
};

const static char PP[32] =//P盒置换
{
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26,5, 18, 31, 10,
    2, 8, 24, 14,32, 27, 3, 9,
    19, 13, 30, 6,22, 11, 4, 25,
};

const static char LP[64] =//末置换
{
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

static bool M[64], tmp[32], *Li = &M[0], *Ri = &M[32];
static bool SubKey[16][48];//16个子密钥

class CDES//定义DES类
{
public:
    void Encryption(char out[8],char In[8]);//加密函数
    void Decryption(char out[8],char In[8]);//解密函数
};

void ByteToBit(bool *Out, const char *In, int bits)//字符转换成字节
{
    int i;

    for(i=0; i<bits; i++)
    {
        //In[i]的第N位右移N位并和0x01按位"与"运算(N=1~8)
        Out[i] = (In[i>>3]>>(i&7)) & 1;
    }
}

void BitToByte(char *Out,const bool *In,int bits)//字节转换成字符
{
    int i;
    memset(Out,0,(bits+7)/8);
    for(i=0; i<bits; i++)
    {
        Out[i>>3] |= In[i]<<(i&7);
    }
}

void RotateL(bool*In,int len,int loop)//循环左移
{
    static bool tmp[256];
    memcpy(tmp,In,loop);
    memcpy(In,In+loop,len-loop);
    memcpy(In+len-loop,tmp,loop);
}

void Xor(bool*InA,const bool*InB,int len)//异或
{
    int i;
    for(i=0; i<len; i++)
    {
        InA[i]^=InB[i];
    }
}

void Transform(bool*Out,bool*In,const char*Table,int len)//各个置换转换
{
    int i;
    static bool tmp[256];
    for(i=0; i<len; i++)
    {
        tmp[i]=In[Table[i]-1];
    }
    memcpy(Out,tmp,len);
}

void S_func(bool Out[32],const bool In[48])//将48位转换成32位
{
    int j,m,n;
    //膨胀后的比特串分为8组，每组6比特。
    for(j=0; j<8; j++,In+=6,Out+=4)
    {
        m = (In[0]*2)+In[5];
        n = (In[1]*8)+(In[2]*4)+(In[3]*2)+In[4];
        ByteToBit(Out,&SBox[j][m][n],4);
    }
}

void F_func(bool In[32],const bool Ki[48])
{
    static bool MR[48];
    Transform(MR,In,EC,48);
    Xor(MR, Ki, 48);
    //膨胀后的比特串分为8组，每组6比特。各组经过各自的S盒后，又变为4比特，合并后又成为32比特。
    S_func(In, MR);
    //该32比特经过P变换后，输出的比特串才是32比特的f(Ri-1,Ki)。
    Transform(In, In, PP, 32);
}

void SetKey(char key[8])//生成子密钥
{
    int i;
    static bool K[64], *KL = &K[0], *KR = &K[28];
    ByteToBit(K,key,64);//转换为二进制
    Transform(K,K,EP1,56);//64比特的密钥K，经过EP1后，生成56比特的串。
    //生成16个子密钥
    for(i=0; i<16; i++)
    {
        //循环左移，合并
        RotateL(KL,28,LOOP[i]);
        RotateL(KR,28,LOOP[i]);
        Transform(SubKey[i],K,EP2,48);
    }
}

bool CheckKey(char* key)
{
    SetKey(key);
    char A[6],B[6];
    for(int i=1,j=16; i<=16,j>=1; i++,j--)
    {
        BitToByte(B,SubKey[i],48);
        BitToByte(A,SubKey[j],48);
        //若16个子密钥完全相同，则为弱密钥

        if (memcmp((void*)A,(void*)B,6) ) return 1;

        else return 0;

    }
}

void CDES::Encryption(char out[8],char In[8])//加密函数
{
    ByteToBit(M,In,64);//转换为二进制
    Transform(M,M,IP,64);
    for(int i=0; i<16; i++)
    {
        memcpy(tmp,Ri,32);
        F_func(Ri,SubKey[i]);
        Xor(Ri,Li,32);//将所得结果与明文的左32位进行异或
        memcpy(Li,tmp,32);//将明文的左右32位交换
    }
    Transform(M, M, LP, 64);

    BitToByte(out, M, 64);
}

void CDES::Decryption(char out[8],char In[8])//解密函数
{
    ByteToBit(M,In,64);//转换为二进制
    Transform(M,M,IP,64);
    for(int i=15; i>=0; i--)
    {
        memcpy(tmp,Li,32);
        F_func(Li,SubKey[i]);
        Xor(Li,Ri,32);
        memcpy(Ri,tmp,32);
    }
    Transform(M, M, LP, 64);
    BitToByte(out, M, 64);
}

int main(void)
{
    int c;
    char key[10];
    char str[128];
    char str1[128];
    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("1.txt","r");
    if(fp1)
    {
        fscanf(fp1,"%s",str);
        cout<<"已打开“1.txt”中的明文"<<endl;
    }
    else
    {
        cout<<"“1.txt”不存在！请先创建“1.txt”，并输入明文！"<<endl;
        exit(0);
    }
    fp2=fopen("key.txt","r");
    if(fp2)
    {
        fscanf(fp2,"%s",key);
        cout<<"已打开“key.txt”中的密钥"<<endl;
    }
    else
    {
        cout<<"“key.txt”不存在！请先创建“key.txt”，并输入明文！"<<endl;
        exit(0);
    }
    c=CheckKey(key);
    if(c)
        printf("这是非弱密钥！\n");
    else
        printf("这是弱密钥！\n");
    SetKey(key);
    memset(str1,0,sizeof(str1));
    CDES des;
    des.Encryption(str1,str);
    fp3=fopen("2.txt","w+");
    fprintf(fp3,"%s",str1);
    cout<<"密文已输入“2.txt”中"<<endl;
    cout<<"是否解密？(1.是;0.否)";
    int n;
    cin>>n;
    if(n==1)
    {
        while(1)
        {
            char sec[10];
            cout<<"请输入密码：";
            cin>>sec;
            if(strcmp(sec,key)==0)
            {
                memset(str,0,sizeof(str));
                des.Decryption(str,str1);
                fp4=fopen("3.txt","w+");
                fprintf(fp4,"%s",str);
                cout<<"密文已输入“3.txt”中"<<endl;
                break;
            }
            else
                cout<<"密码错误！"<<endl;
        }
    }
    else
    {
        cout<<"DES加密解密过程结束！"<<endl;
        return 0;
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    return 0;
}
