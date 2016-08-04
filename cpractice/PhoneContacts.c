#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
typedef struct friends_list//���c��
{
    char xiangmu[10];
    char name[10];
    char age[8];
    char telephone[15];
    char email[20];
    char qq[15];
} member;
FILE *fp; //�����w�A�Ω󥴶}�ΫO�s�q�T�����
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
        if(Count==0)//�o�ӧP�_����ϱo�u�b�}�Y��X�@��
        {
            printf("====================================================================\n");
            printf("===============          �w��ϥΤ���q�T��          ===============\n");
            printf("=====�п�J�q�T���ɪ����ĸ��|(�Y�ɤ��s�b�h�۰ʳЫطs�q�T��)=====\n");
            printf("====================================================================\n");
            printf("=============�ͱ�����:�Цb�h�X���{�����e��7�O�s�q�T��!============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            gets(filename);
            Count=load(filename);
        }
        Sleep(1000);
        printf("====================================================================\n");
        printf("===============          ����q�T�����\��p�U        ===============\n");
        printf("===============              1:�����d��              ===============\n");
        printf("===============              2:�W�[�\��              ===============\n");
        printf("===============              3:�����\��              ===============\n");
        printf("===============              4:�d��\��              ===============\n");
        printf("===============              5:�ק�\��              ===============\n");
        printf("===============              6:�R���\��              ===============\n");
        printf("===============              7:�O�s�\��              ===============\n");
        printf("===============              0:�h�X�ϥ�              ===============\n");
        printf("====================================================================\n");
        Sleep(1000);
        printf("********************************************************************\n");
        printf("�п�ܾާ@�\��:");
        scanf("%d",&choice);
        system("cls");
        system("color 5f");
        system("color 2b");
        switch(choice)
        {
        case 1:
            printf("====================================================================\n");
            printf("===============       ����q�T���������p�U           ===============\n");
            printf("===============               1.�ˤ�                 ===============\n");
            printf("===============               2.�P��                 ===============\n");
            printf("===============               3.�Ѯv                 ===============\n");
            printf("===============               4.��L                 ===============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            printf("�п�J�n�d�ߪ������G");
            scanf("%d",&m);
            switch(m)
            {
            case 1:
                strcpy(xiangmu,"�ˤ�");
                break;
            case 2:
                strcpy(xiangmu,"�P��");
                break;
            case 3:
                strcpy(xiangmu,"�Ѯv");
                break;
            case 4:
                strcpy(xiangmu,"��L");
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
            printf("�п�J�n�d�䪺�s���H�m�W:");
            scanf("%s",name);
            search_friend(friends,name);
            break;
        case 5:
            printf("********************************************************************\n");
            printf("�п�J�n�ק諸�s���H�m�W:");
            scanf("%s",name);
            change_friend(friends,name);
            break;
        case 6:
            printf("********************************************************************\n");
            printf("�п�J�n�R�����s���H�m�W:");
            scanf("%s",name);
            delete_friend(friends,name);
            break;
        case 7:
            save(filename);
            printf("====================================================================\n");
            printf("===============               �O�s���\�I             ===============\n");
            printf("====================================================================\n");
            break;
        case 0:
            break;
        default:
            printf("====================================================================\n");
            printf("===============        ��J���~�A�Э��s��J!         ===============\n");
            printf("====================================================================\n");
            break;
        }
    }
    while(choice!=0);
    printf("Count=%d\n",Count);//���B��XCount�O���F�����R���ɡA�w�s�s���H���H�ƬO�_�����a��֤F
    printf("====================================================================\n");
    printf("==================      ���¨ϥγq�T���\��!       ==================\n");
    printf("====================================================================\n");
    return 0;

}

int load(char filename[])//���}�q�T�����
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

void save(char filename[]) //�O�s�q�T����
{
    int i;
    member *p;
    if((fp=fopen(filename,"wb"))==NULL)
    {
        printf("�L�k�O�s�I���}���%s�ɥX��\n",filename);
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
void new_friend(struct friends_list friends[])// �W�[�\��
{
    struct friends_list f;
    if(Count==100)
    {
        printf("====================================================================\n");
        printf("===============             �q�T���w��!            =================\n");
        printf("====================================================================\n");
        return ;
    }
    int flag=0;
    int i;
    int m;
    printf("====================================================================\n");
    printf("===============       ����q�T���������p�U           ===============\n");
    printf("===============               1.�ˤ�                 ===============\n");
    printf("===============               2.�P��                 ===============\n");
    printf("===============               3.�Ѯv                 ===============\n");
    printf("===============               4.��L                 ===============\n");
    printf("====================================================================\n");
    printf("********************************************************************\n");
    printf("�п�J�s�s���H�������G");
    scanf("%d",&m);
    switch(m)
    {
    case 1:
        strcpy(f.xiangmu,"�ˤ�");
        break;
    case 2:
        strcpy(f.xiangmu,"�P��");
        break;
    case 3:
        strcpy(f.xiangmu,"�Ѯv");
        break;
    case 4:
        strcpy(f.xiangmu,"��L");
        break;
    default:
        printf("====================================================================\n");
        printf("===============          �п�J���T���ާ@!           ===============\n");
        printf("====================================================================\n");
        break;
    }
    printf("�п�J�s�s���H���m�W:");
    scanf("%s",f.name);
    printf("�п�J�s�s���H���q��:");
    scanf("%s",f.telephone);
    for(i=0; i<Count; i++)
    {
        if(strcmp(f.name,friends[i].name)==0&&strcmp(f.telephone,friends[i].telephone)==0)
        {
            printf("====================================================================\n");
            printf("===============            ��ƿ��J����!             ===============\n");
            printf("===============            ���J�w�g����!             ===============\n");
            printf("====================================================================\n");
            break;
        }
        else if(i==Count-1)
            flag=1;
    }
    if(flag||Count==0)
    {
        printf("�п�J�s�s���H���~��:");
        scanf("%s",&f.age);
        printf("�п�J�s�s���H���l�c:");
        scanf("%s",&f.email);
        printf("�п�J�s�s���H��qq:");
        scanf("%s",&f.qq);
        printf("********************************************************************\n");
        friends[Count]=f;
        Count++;
    }
    system("cls");
    system("color 5f");
    system("color 2b");
}
void fen_friend(struct friends_list friends[],char *xiangmu)//�����d�ߥ\��
{
    int i,j=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            �q�T���O�Ū�!             ===============\n");
        printf("====================================================================\n");
        return ;
    }

//������X�ɷ�M�i�H���Ѯv���n�D�u��X�m�W�M�q�ܤF�A�u�ݱN��X���R�h�@��
    for(i=0; i<Count; i++)
    {
        if(strcmp(xiangmu,friends[i].xiangmu)==0)
        {
            printf("����:%s\n",friends[i].xiangmu);
            printf("�m�W:%s\n",friends[i].name);
            printf("�~��:%s\n",friends[i].age);
            printf("�q��:%s\n",friends[i].telephone);
            printf("�l�c:%s\n",friends[i].email);
            printf("qq:%s\n",friends[i].qq);
            j++;
        }
        if(i==Count-1)
        {
            if(j==0)
            {
                printf("====================================================================\n");
                printf("===============            �L�����s���H!             ===============\n");
                printf("====================================================================\n");
            }
            break;
        }
    }
}
void search_friend(struct friends_list friends[],char *name)//�d��\��
{
    int i,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            �q�T���O�Ū�!             ===============\n");
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
        printf("����:%s\n",friends[i].xiangmu);
        printf("�m�W:%s\n",friends[i].name);
        printf("�~��:%s\n",friends[i].age);
        printf("�q��:%s\n",friends[i].telephone);
        printf("�l�c:%s\n",friends[i].email);
        printf("qq:%s\n",friends[i].qq);
    }
    else
    {

        printf("====================================================================\n");
        printf("===============             �L���s���H!              ===============\n");
        printf("====================================================================\n");
    }

}
void call_friend(struct friends_list friends[],char *name)//�����\��
{
    int i,j,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            �q�T���O�Ū�!             ===============\n");
        printf("====================================================================\n");
        return ;
    }
    for(i=0; i<Count; i++)
        printf("%s  ",friends[i].name);
    printf("\n");
    printf("********************************************************************\n");
    printf("�п�J�n�����H���m�W�G");
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
        printf("���b�������A�Э@�ߵ��ݡK�K");
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
        printf("===============             �L���s���H!              ===============\n");
        printf("====================================================================\n");
    }
}
void change_friend(struct friends_list friends[],char *name)//�ק�\��
{
    int i,flag=0;
    int choice,j;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            �q�T���O�Ū�!             ===============\n");
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
            printf("===============             �ק�\��p�U             ===============\n");
            printf("===============             [1]�ק�����              ===============\n");
            printf("===============             [2]�ק�m�W              ===============\n");
            printf("===============             [3]�ק�~��              ===============\n");
            printf("===============             [4]�ק�q��              ===============\n");
            printf("===============             [5]�ק�l�c              ===============\n");
            printf("===============             [6]�ק�qq                ===============\n");
            printf("===============             [0]�h�X�ק�              ===============\n");
            printf("====================================================================\n");
            printf("********************************************************************\n");
            printf("��J���:");
            scanf("%d",&choice);
            if(choice==0)
                break;
            switch(choice)
            {
            case 1:
                printf("********************************************************************\n");
                printf("�п�J�s�������G");
                scanf("%s",friends[i].xiangmu);
                break;
            case 2:
                printf("********************************************************************\n");
                printf("�п�J�s���m�W�G");
                scanf("%s",friends[i].name);
                break;
            case 3:
                printf("********************************************************************\n");
                printf("�п�J�s���~�֡G");
                scanf("%s",&friends[i].age);
                break;
            case 4:
                printf("********************************************************************\n");
                printf("�п�J�s���q�ܸ��X�G");
                scanf("%s",friends[i].telephone);
                break;
            case 5:
                printf("********************************************************************\n");
                printf("�п�J�s��email�G") ;
                scanf("%s",friends[i].email);
                break;
            case 6:
                printf("********************************************************************\n");
                printf("�п�J�s��qq�G");
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
        printf("===============             �L���s���H!              ===============\n");
        printf("====================================================================\n");
    }
    printf("====================================================================\n");
    printf("===============               �ק令�\!              ===============\n");
    printf("====================================================================\n");

    system("cls");
    system("color 5f");
    system("color 2b");
}
void delete_friend(struct friends_list friends[],char *name)//�R���\��
{
    int i,flag=0;
    if(Count==0)
    {
        printf("====================================================================\n");
        printf("===============            �q�T���O�Ū�!             ===============\n");
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
        printf("===============            �w�R���ӳs���H!           ===============\n");
        printf("====================================================================\n");
        printf("\n");
    }
    else
    {
        printf("====================================================================\n");
        printf("===============             �L���s���H!              ===============\n");
        printf("====================================================================\n");
    }
    system("cls");
    system("color 5f");
    system("color 2b");
}
