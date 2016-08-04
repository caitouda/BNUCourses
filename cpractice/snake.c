#include <graphics.h>       /*C�����ϧήw*/
#include <stdlib.h>        /*�зǮw���Y�ɡA�t�Ψ��*/
#include <dos.h>          /*���Y�ɡA�̭��]�t�F�ܦhBIOS�MDOS�եΨ��*/
#define LEFT 0x4b00      /*�w�q�W�U���k�MESC��ASCII��*/
#define RIGHT 0x4d00
#define DOWN 0x5000
#define UP 0x4800
#define ESC 0x011b
#define N 200
char ch;
int i,key,choose;
int score=0;
int gamespeed;
int level=0;
struct Food
{
    int x;
    int y;
    int yes;
} food;
struct Snake
{
    int x[N];
    int y[N];
    int node;
    int direction;
    int life;           /*�D���ͩR�A0��ܬ��ۡA1��ܦ��`*/
} snake;
void GameStart(void);   /*�}�l�C���e�������A�t���ת����*/
void Init(void);        /*�C����l��*/
void Close(void);       /*�C�������ɪ�����*/
void DrawK(void);       /*�}�l�e��*/
void GameOver(void);    /*�����C��*/
void GamePlay(void);    /*���C��*/
void PrPlay(void);      /*�C�������y*/
void PrScore(void);     /*��X����*/

void main(void)
{
    int k,m=0;
    while(m==0)
    {
        Init();
        GameStart();

        DrawK();
        GamePlay();

        cleardevice();
        settextstyle(0,0,2);
        outtextxy(150,150,"DO YOU WANT PLAY AGAIN");
        outtextxy(150,200,"IF YES:HIT UP KEY");
        outtextxy(150,250,"IF NO:HIT DOWN KEY");
        k=bioskey(0);
        switch(k)
        {
        case UP:
            m=0;
            score=0;
            break;
        case DOWN:
            m=1;
            score=0;
            break;
        }
    }

    if(m==1)
    {
        Close();
    }
}

void GameStart(void)
{
    cleardevice();
    /*�M��*/
    setcolor(YELLOW);
    /*C�w��� �ϧοù����,�@�@�\��:�N��e�ϧοù�����e�����C��m��color.*/
    settextstyle(0,0,2);
    /*�\ ��: ���ϧο�X�]�m��e����r�ݩʡA�� �k: void far settextstyle (int font, int direction, char size);*/
    outtextxy(30,120,"CHOOSE THE DIFFICULTY LEVEL");/*�q��L����*/
    /*�\ ��: �b���w��m��ܤ@�r��@�� �k: void far outtextxy(int x, int y, char *textstring);*/
    outtextxy(30,160,"LEVEL 0:PLEASE HIT UP KEY");
    outtextxy(30,200,"LEVEL 1:PLEASE HIT DOWN KEY");
    outtextxy(30,240,"LEVEL 2:PLEASE HIT RIGHT KEY");
    choose=bioskey(0);
    switch(choose)
    {
    case UP:
        gamespeed=50000;
        break;
    case DOWN:
        gamespeed=20000;
        level=1;
        break;
    case RIGHT:
        gamespeed=15000;
        level=2;
        break;
    }
    cleardevice();/*�M��*/
    settextstyle(0,0,3);
    outtextxy(150,150,"ARE YOU READY");
    outtextxy(100,300,"TO START THE GAME");
    scanf("%c",&ch);
    if(ch!='n')
        setcolor(BLACK);/*�]�m�C�⬰�¦�*/
    cleardevice();
}
void Init(void)/*�ϧ��X��*/
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"c:\\tc");/*���B��turboc�����|�A�i�ھڦۤv�q���W�ɦ�m����*/
    cleardevice();/*�M��*/
}

void DrawK(void)/*�}�l�C���e��*/
{

    setcolor(4);/*�]�m�C�⬰����*/
    setlinestyle(SOLID_LINE,0,THICK_WIDTH);/*��u�F���B�S����ڷN�q�F�u�e��3�ӹϤ�*/
    /*�\��G setlinestyle() ���e�u��Ƴ]�m��e�u���A�]�A�u���B�u�ϼ˩M�u�e�C*/
    /*�Ϊk�G setlinestyle() �禡�I�s�覡��void setlinestyle(int stly,unsigned pattern,int wigth);*/
    rectangle(47,37,613,463);/*�Ϥ��e�ש��̭��W*/
    /*�\��G ���rectangle() �η�eø�Ϧ�B�u���νu�e�A�e�@�ӵ��w���W���P�k�U�����x��(����ΩΪ����)�C*/
    /*�e�x��//�Ϊk�G ���禡�I�s�覡��void rectangle(int left,int top,int right,int bottom);*/
    if(level==1)
    {
        rectangle(197,197,397,207);
        rectangle(197,277,397,287);
    }
    if(level==2)
    {
        rectangle(197,197,397,207);
        rectangle(197,277,397,287);
        rectangle(167,227,177,257);
        rectangle(417,227,427,257);
    }
}

void GamePlay(void)
{
    randomize();/*�\ ��:��l�ƶüƵo�;�     �� �k:void randomize(void);*/
    food.yes=1;/*1��ܻݭn�X�{�s�������A0��ܤw�g�s�b����*/
    snake.life=0;/*�D����*/
    snake.direction=1;/*��V�V�k*/
    snake.x[0]=100;
    snake.y[0]=100;
    snake.x[1]=110;
    snake.y[1]=100;
    snake.node=2;/*�D���`��*/
    PrPlay();
    PrScore();/*��X�o��*/

    while(1)/*�i�H���ƪ��C���A��ESC��i�H����*/
    {
        while(!kbhit())/*�A�S�����䪺���p�U�A�D�٬O�|�ۤv����*/
        {
            if(food.yes==1)/*�ݭn�X�{�s������*/
            {
                food.x=rand()%400+60;
                food.y=rand()%350+60;/*�H�����ͭ���*/
                while(food.x%10!=0)/*���������X�{�b��椺�A�o�˳D�~��Y��*/
                    food.x++;
                while(food.y%10!=0)
                    food.y++;
                if(level==1)
                {
                    /*
                       rectangle(200,200,400,210);
                       rectangle(200,280,400,290);
                    */
                    for(i=200; i<=400; i+=10)
                    {
                        if(food.x==i&&(food.y==200||food.y==210||food.y==280||food.y==290))
                        {
                            /*food.x=food.x+10;*/
                            food.y=food.y+20;
                        }
                    }

                }
                if(level==2)
                {
                    /*
                       rectangle(200,200,400,210);
                       rectangle(200,280,400,290);
                       rectangle(170,230,180,260);
                       rectangle(420,230,430,260);
                    */
                    for(i=200; i<=400; i+=10)
                    {
                        if(food.x==i&&(food.y==200||food.y==210||food.y==280||food.y==290))
                        {
                            /*food.x=food.x+10;*/
                            food.y=food.y+20;
                            food.yes=0;
                        }
                    }
                    if(food.yes==1)
                    {
                        int j;
                        for(j=230; j<=260; j+=10)
                        {
                            if(food.y==j&&(food.x==170||food.x==180||food.x==420||food.x==430))
                            {
                                food.x=food.x+20;
                            }
                        }
                    }
                }
                food.yes=0;/*�����W�������F*/
            }
            if(food.yes==0)
            {
                setcolor(YELLOW);/*�]�m�����C�⬰����*/
                rectangle(food.x,food.y,food.x+10,food.y-10);/*�e�X����*/
            }
            for(i=snake.node-1; i>0; i--)/*�D���C�@�`�����e����*/
            {
                snake.x[i]=snake.x[i-1];
                snake.y[i]=snake.y[i-1];
            }

            switch(snake.direction)/*��{�D������*/
            {
            case 1:
                snake.x[0]+=10;/*�V�k*/
                break;
            case 2:
                snake.x[0]-=10;/*�V��*/
                break;
            case 3:
                snake.y[0]-=10;/*�V�W*/
                break;
            case 4:
                snake.y[0]+=10;/*�V�U*/
                break;
            }
            for(i=3; i<snake.node; i++)/*�q�D���ĥ|�`�}�l�P�_�O�_����ۤv*/
            {
                if(snake.x[i]==snake.x[0]&&snake.y[i]==snake.y[0])
                {
                    GameOver();/*�C������*/
                    snake.life=1;/*�D���F*/
                    break;
                }
            }
            if(snake.x[0]<=40||snake.x[0]>=610||snake.y[0]<=40||snake.y[0]>=465)/*�P�_�D�O�_�������*/
            {
                GameOver();
                snake.life=1;
            }
            if(level==1)
            {
                for(i=200; i<=400; i+=10)
                {
                    /*
                      rectangle(200,200,400,210);
                      rectangle(200,280,400,290);
                                       */
                    if(snake.x[0]==i&&(snake.y[0]==200||snake.y[0]==210||snake.y[0]==280||snake.y[0]==290))
                    {
                        GameOver();
                        snake.life=1;
                        break;
                    }
                }

            }
            if(level==2)
            {
                /*
                    rectangle(200,200,400,210);
                    rectangle(200,280,400,290);
                    rectangle(170,230,180,260);
                    rectangle(420,230,430,260);
                 */
                for(i=200; i<=400; i+=10)
                {
                    if(snake.x[0]==i&&(snake.y[0]==200||snake.y[0]==210||snake.y[0]==280||snake.y[0]==290))
                    {
                        GameOver();
                        snake.life=1;
                        break;
                    }
                }
                if(snake.life==0)
                {
                    int j;
                    for(j=230; j<=260; j+=10)
                    {
                        if(snake.y[0]==j&&(snake.x[0]==170||snake.x[0]==180||snake.x[0]==420||snake.x[0]==430))
                        {
                            GameOver();
                            snake.life=1;
                            break;
                        }
                    }
                }
            }
            if(snake.life==1)
                break;/*�D���F�A�N���X�j��*/
            if(snake.x[0]==food.x&&snake.y[0]==food.y)/*�D�Y�쭹��*/
            {
                setcolor(0);/*�h���̭��W������*/
                rectangle(food.x,food.y,food.x+10,food.y-10);
                snake.x[snake.node]=-20;
                snake.y[snake.node]=-20;/*�s���@�`�����ݤ�������m�A�U���j��N���e�@�`����m*/

                snake.node++;/*�D������@�`*/
                food.yes=1;/*�s����������*/
                if(level==0)/*���Ť��P�A�Y��C�ӭ��������Ȥ��P*/
                {
                    score+=10;/*���ƼW�[*/
                }
                if(level==1)
                {
                    score+=20;
                }
                if(level==2)
                {
                    score+=30;
                }

                PrScore();/*��X����*/
            }
            setcolor(2);/*�e�X�D*/
            for(i=0; i<snake.node; i++)
                rectangle(snake.x[i],snake.y[i],snake.x[i]+10,snake.y[i]-10);

            delay(gamespeed);
            setcolor(0);/*�ζ¦�h���D���̫�@�`*/
            rectangle(snake.x[snake.node-1],snake.y[snake.node-1],snake.x[snake.node-1]+10,snake.y[snake.node-1]-10);
        }
        if(snake.life==1)/*�D���F�N���X�j��*/
            break;
        key=bioskey(0);/*�q��L�W�������O*/
        if(key==ESC)/*��ESC��h�X�C��*/
            break;
        else if(key==UP&&snake.direction!=4)
            snake.direction=3;
        else if(key==RIGHT&&snake.direction!=2)
            snake.direction=1;
        else if(key==LEFT&&snake.direction!=1)
            snake.direction=2;
        else if(key==DOWN&&snake.direction!=3)
            snake.direction=4;
    }
}

void GameOver(void)/*�C������*/
{
    char ov[10];
    cleardevice();

    setcolor(RED);/*�]�m�C�⬰����*/
    settextstyle(0,0,3);
    outtextxy(200,200,"GAME OVER");

    setcolor(10);
    settextstyle(0,0,3);
    sprintf(ov,"SCORE:%d",score);
    outtextxy(210,300,ov);

    getch();

}
void PrPlay(void)
{
    char pl[30];
    setcolor(10);
    settextstyle(0,0,2);
    sprintf(pl,"GAME PLAYING");
    outtextxy(100,20,pl);
}

void PrScore(void)/*�C�������y*/
{
    char str[10];
    setfillstyle(SOLID_FILL,BLUE);
    bar(370,15,550,35);
    /*�e�����
    �\��G ���bar()�η�e��R�ϼ˩M��R��(�`�N���O���Ϧ�)�e�X�@�ӫ��w�W���W���P�k�U������ߪ�����(������Υ����)�A���S���|����u)�C
    �Ϊk�G bar()�禡�I�s�覡��void bar(int left,int top,int right,int bottom);*/
    setcolor(10);/*�]�m�C�⬰�L���*/
    settextstyle(0,0,2);
    sprintf(str,"SCORE:%d",score);
    outtextxy(400,20,str);
}

void Close(void)/*�C���ϧε���*/
{
    getch();
    closegraph();
}
