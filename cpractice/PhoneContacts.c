#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
typedef struct friends_list//結構體
{
    char xiangmu[10];
    char name[10];
    char age[8];
    char telephone[15];
    char email[20];
    char qq[15];
} member;
FILE *fp; //文件指針，用於打開或保存通訊錄文件
member friends[100];

int Count=0;
void save(char filename[]);
void fen_friend(struct friends_list friends[],char *xiangmu);
void new_friend(struct friends_list friends[]);
void call_friend(struct friends_list friends[],char *name);
void search_friend(struct friends_list friends[],char *name);
void change_friend(struct friends_list friends[],char *name);
void delete_friend(struct friends_list friends[],char *name);
int main()
{
    int choice;
    int m;
    char save_j;
    char xiangmu[10];
    char name[10];
    char filename[40];
    struct friends_list friends[100];
    do
    {
        if(Count==0)//這個判斷條件使得只在開頭輸出一次
        {
            printf("====================================================================\n");
            printf("===============          歡迎使用手機通訊錄          ===============\n");
            printf("=====請輸入通訊錄檔的有效路徑(若檔不存在則自動創建新通訊錄)=====\n");
            printf("====================================================================\n");
            printf("=============友情提醒:請在退出本程式之前按7保存通訊檔!============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            gets(filename);
            Count=load(filename);
        }
        Sleep(1000);
        printf("====================================================================\n");
        printf("===============          手機通訊錄的功能如下        ===============\n");
        printf("===============              1:分類查詢              ===============\n");
        printf("===============              2:增加功能              ===============\n");
        printf("===============              3:撥號功能              ===============\n");
        printf("===============              4:查找功能              ===============\n");
        printf("===============              5:修改功能              ===============\n");
        printf("===============              6:刪除功能              ===============\n");
        printf("===============              7:保存功能              ===============\n");
        printf("===============              0:退出使用              ===============\n");
        printf("====================================================================\n");
        Sleep(1000);
        printf("********************************************************************\n");
        printf("請選擇操作功能:");
        scanf("%d",&choice);
        system("cls");
        system("color 5f");
        system("color 2b");
        switch(choice)
        {
        case 1:
            printf("====================================================================\n");
            printf("===============       手機通訊錄的分類如下           ===============\n");
            printf("===============               1.親友                 ===============\n");
            printf("===============               2.同學                 ===============\n");
            printf("===============               3.老師                 ===============\n");
            printf("===============               4.其他                 ===============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            printf("請輸入要查詢的類型：");
            scanf("%d",&m);
            switch(m)
            {
            case 1:
                strcpy(xiangmu,"親友");
                break;
            case 2:
                strcpy(xiangmu,"同學");
                break;
            case 3:
                strcpy(xiangmu,"老師");
                break;
            case 4:
                strcpy(xiangmu,"其他");
                break;
            default:
                break;
            }
            fen_friend(friends,xiangmu);
            break;
        case 2:
            new_friend(friends);
            break;
        case 3:
            call_friend(friends,name);
            break;
        case 4:
            printf("********************************************************************\n");
            printf("請輸入要查找的連絡人姓名:");
            scanf("%s",name);
            search_friend(friends,name);
            break;
        case 5:
            printf("********************************************************************\n");
            printf("請輸入要修改的連絡人姓名:");
            scanf("%s",name);
            change_friend(friends,name);
            break;
        case 6:
            printf("********************************************************************\n");
            printf("請輸入要刪除的連絡人姓名:");
            scanf("%s",name);
            delete_friend(friends,name);
            break;
        case 7:
            save(filename);
            printf("====================================================================\n");
            printf("===============               保存成功！             ===============\n");
            printf("====================================================================\n");
            break;
        case 0:
            break;
        default:
            printf("====================================================================\n");
            printf("===============        輸入有誤，請重新輸入!         ===============\n");
            printf("====================================================================\n");
            break;
        }
    }
    while(choice!=0);
    printf("Count=%d\n",Count);//此處輸出Count是為了說明刪除時，已存連絡人的人數是否相應地減少了
    printf("====================================================================\n");
    printf("==================      謝謝使用通訊錄功能!       ==================\n");
    printf("====================================================================\n");
    return 0;

}

int load(char filename[])//打開通訊錄文件
{
    member *p;
    int i=0;
    if((fp=fopen(filename,"rb"))==NULL)
        return i;
    else
    {
        while(!feof(fp))
        {
            p=(member *)malloc(sizeof(member));
            if(fread(p,sizeof(member),1,fp)==1)
            {
                strcpy(friends[i].name,p->name);
                strcpy(friends[i].age,p->age);
                strcpy(friends[i].telephone,p->telephone);
                strcpy(friends[i].xiangmu,p->xiangmu);
                strcpy(friends[i].email,p->email);
                strcpy(friends[i].qq,p->qq);
                i++;
            }
            free(p);
        }
        return i;
    }
}

void save(char filename[]) //保存通訊錄檔
{
    int i;
    member *p;
    if((fp=fopen(filename,"wb"))==NULL)
    {
        printf("無法保存！打開文件%s時出錯\n",filename);
        exit(1);
    }
    else
        for(i=0; i<Count; i++)
        {
            p=(member *)malloc(sizeof(member));
            strcpy(p->name,friends[i].name);
            strcpy(p->age,friends[i].age);
            strcpy(p->telephone,friends[i].telephone);
            strcpy(p->xiangmu,friends[i].xiangmu);
            strcpy(p->email,friends[i].email);
            strcpy(p->qq,friends[i].qq);
            fwrite(p,sizeof(member),1,fp);
            free(p);
        }
    fclose(fp);
}
void new_friend(struct friends_list friends[])// 增加功能
{
    struct friends_list f;
    if(Count==100)
    {
        printf("====================================================================\n");
        printf("===============             通訊錄已滿!            =================\n");
        printf("====================================================================\n");
        return ;
    }
    int flag=0;
    int i;
    int m;
    printf("====================================================================\n");
    printf("===============       手機通訊錄的分類如下           ===============\n");
    printf("===============               1.親友                 ===============\n");
    printf("===============               2.同學                 ===============\n");
    printf("===============               3.老師                 ===============\n");
    printf("===============               4.其他                 ===============\n");
    printf("====================================================================\n");
    printf("********************************************************************\n");
    printf("請輸入新連絡人的分類：");
    scanf("%d",&m);
    switch(m)
    {
    case 1:
        strcpy(f.xiangmu,"親友");
        break;
    case 2:
        strcpy(f.xiangmu,"同學");
        break;
    case 3:
        strcpy(f.xiangmu,"老師");
        break;
    case 4:
        strcpy(f.xiangmu,"其他");
        break;
    default:
        printf("====================================================================\n");
        printf("===============          請輸入正確的操作!           ===============\n");
        printf("====================================================================\n");
        break;
    }
    printf("請輸入新連絡人的姓名:");
    scanf("%s",f.name);
    printf("請輸入新連絡人的電話:");
    scanf("%s",f.telephone);
    for(i=0; i<Count; i++)
    {
        if(strcmp(f.name,friends[i].name)==0&&strcmp(f.telephone,friends[i].telephone)==0)
        {
            printf("====================================================================\n");
            printf("===============            資料錄入重複!             ===============\n");
            printf("===============            錄入已經取消!             ===============\n");
            printf("====================================================================\n");
            break;
        }
        else if(i==Count-1)
            flag=1;
    }
    if(flag||Count==0)
    {
        printf("請輸入新連絡人的年齡:");
        scanf("%s",&f.age);
        printf("請輸入新連絡人的郵箱:");
        scanf("%s",&f.email);
        printf("請輸入新連絡人的qq:");
        scanf("%s",&f.qq);
        printf("********************************************************************\n");
        friends[Count]=f;
        Count++;
    }
    system("cls");
    system("color 5f");
    system("color 2b");
}
void fen_friend(struct friends_list friends[],char *xiangmu)//分類查詢功能
{
    int i,j=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            通訊錄是空的!             ===============\n");
        printf("====================================================================\n");
        return ;
    }

//分類輸出時當然可以按老師的要求只輸出姓名和電話了，只需將輸出項刪去一些
    for(i=0; i<Count; i++)
    {
        if(strcmp(xiangmu,friends[i].xiangmu)==0)
        {
            printf("類型:%s\n",friends[i].xiangmu);
            printf("姓名:%s\n",friends[i].name);
            printf("年齡:%s\n",friends[i].age);
            printf("電話:%s\n",friends[i].telephone);
            printf("郵箱:%s\n",friends[i].email);
            printf("qq:%s\n",friends[i].qq);
            j++;
        }
        if(i==Count-1)
        {
            if(j==0)
            {
                printf("====================================================================\n");
                printf("===============            無此類連絡人!             ===============\n");
                printf("====================================================================\n");
            }
            break;
        }
    }
}
void search_friend(struct friends_list friends[],char *name)//查找功能
{
    int i,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            通訊錄是空的!             ===============\n");
        printf("====================================================================\n");
        return ;
    }
    for(i=0; i<Count; i++)
        if(strcmp(name,friends[i].name)==0)
        {
            flag=1;
            break;
        }
    if(flag)
    {
        printf("類型:%s\n",friends[i].xiangmu);
        printf("姓名:%s\n",friends[i].name);
        printf("年齡:%s\n",friends[i].age);
        printf("電話:%s\n",friends[i].telephone);
        printf("郵箱:%s\n",friends[i].email);
        printf("qq:%s\n",friends[i].qq);
    }
    else
    {

        printf("====================================================================\n");
        printf("===============             無此連絡人!              ===============\n");
        printf("====================================================================\n");
    }

}
void call_friend(struct friends_list friends[],char *name)//撥號功能
{
    int i,j,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            通訊錄是空的!             ===============\n");
        printf("====================================================================\n");
        return ;
    }
    for(i=0; i<Count; i++)
        printf("%s  ",friends[i].name);
    printf("\n");
    printf("********************************************************************\n");
    printf("請輸入要撥號人的姓名：");
    printf("\n");
    scanf("%s",name);
    for(i=0; i<Count; i++)
        if(strcmp(name,friends[i].name)==0)
        {
            flag=1;
            break;
        }
    if(flag)
    {
        printf("********************************************************************\n");
        printf("正在撥號中，請耐心等待……");
        for(j=0; friends[i].telephone[j]!=0; j++)
        {


            printf("%c\a",friends[i].telephone[j]);
            Sleep(300);


        }
        printf("\n");
    }
    else
    {
        printf("====================================================================\n");
        printf("===============             無此連絡人!              ===============\n");
        printf("====================================================================\n");
    }
}
void change_friend(struct friends_list friends[],char *name)//修改功能
{
    int i,flag=0;
    int choice,j;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            通訊錄是空的!             ===============\n");
        printf("====================================================================\n");
        return ;
    }
    for(i=0; i<Count; i++)
        if(strcmp(name,friends[i].name)==0)
        {
            flag=1;
            break;
        }
    if(flag)
    {
        for(j=1; j<=7; j++)
        {
            printf("====================================================================\n");
            printf("===============             修改功能如下             ===============\n");
            printf("===============             [1]修改類型              ===============\n");
            printf("===============             [2]修改姓名              ===============\n");
            printf("===============             [3]修改年齡              ===============\n");
            printf("===============             [4]修改電話              ===============\n");
            printf("===============             [5]修改郵箱              ===============\n");
            printf("===============             [6]修改qq                ===============\n");
            printf("===============             [0]退出修改              ===============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            printf("輸入選擇:");
            scanf("%d",&choice);
            if(choice==0)
                break;
            switch(choice)
            {
            case 1:
                printf("********************************************************************\n");
                printf("請輸入新的類型：");
                scanf("%s",friends[i].xiangmu);
                break;
            case 2:
                printf("********************************************************************\n");
                printf("請輸入新的姓名：");
                scanf("%s",friends[i].name);
                break;
            case 3:
                printf("********************************************************************\n");
                printf("請輸入新的年齡：");
                scanf("%s",&friends[i].age);
                break;
            case 4:
                printf("********************************************************************\n");
                printf("請輸入新的電話號碼：");
                scanf("%s",friends[i].telephone);
                break;
            case 5:
                printf("********************************************************************\n");
                printf("請輸入新的email：") ;
                scanf("%s",friends[i].email);
                break;
            case 6:
                printf("********************************************************************\n");
                printf("請輸入新的qq：");
                scanf("%s",&friends[i].qq);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        printf("====================================================================\n");
        printf("===============             無此連絡人!              ===============\n");
        printf("====================================================================\n");
    }
    printf("====================================================================\n");
    printf("===============               修改成功!              ===============\n");
    printf("====================================================================\n");

    system("cls");
    system("color 5f");
    system("color 2b");
}
void delete_friend(struct friends_list friends[],char *name)//刪除功能
{
    int i,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            通訊錄是空的!             ===============\n");
        printf("====================================================================\n");
        return ;
    }
    for(i=0; i<Count; i++)
        if(strcmp(name,friends[i].name)==0)
        {
            flag=1;
            Count--;
            break;
        }
    if(flag)
    {
        do
        {
            strcpy(friends[i].xiangmu,friends[i+1].xiangmu);
            strcpy(friends[i].name,friends[i+1].name);
            strcpy(friends[i].age,friends[i+1].age);
            strcpy(friends[i].telephone,friends[i+1].telephone);
            strcpy(friends[i].email,friends[i+1].email);
            strcpy(friends[i].qq,friends[i+1].qq);
            i++;
        }
        while(i==Count);
        printf("====================================================================\n");
        printf("===============            已刪除該連絡人!           ===============\n");
        printf("====================================================================\n");
        printf("\n");
    }
    else
    {
        printf("====================================================================\n");
        printf("===============             無此連絡人!              ===============\n");
        printf("====================================================================\n");
    }
    system("cls");
    system("color 5f");
    system("color 2b");
}
