#include <graphics.h>       /*C中的圖形庫*/
#include <stdlib.h>        /*標準庫標頭檔，系統函數*/
#include <dos.h>          /*標頭檔，裡面包含了很多BIOS和DOS調用函數*/
#define LEFT 0x4b00      /*定義上下左右和ESC的ASCII值*/
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
    int life;           /*蛇的生命，0表示活著，1表示死亡*/
} snake;
void GameStart(void);   /*開始遊戲前的介面，含難度的選擇*/
void Init(void);        /*遊戲初始化*/
void Close(void);       /*遊戲結束時的介面*/
void DrawK(void);       /*開始畫面*/
void GameOver(void);    /*結束遊戲*/
void GamePlay(void);    /*玩遊戲*/
void PrPlay(void);      /*遊戲結束語*/
void PrScore(void);     /*輸出分數*/

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
    /*清屏*/
    setcolor(YELLOW);
    /*C庫函數 圖形螢幕函數,　　功能:將當前圖形螢幕的當前筆劃顏色置為color.*/
    settextstyle(0,0,2);
    /*功 能: 為圖形輸出設置當前的文字屬性，用 法: void far settextstyle (int font, int direction, char size);*/
    outtextxy(30,120,"CHOOSE THE DIFFICULTY LEVEL");/*從鍵盤接收*/
    /*功 能: 在指定位置顯示一字串　用 法: void far outtextxy(int x, int y, char *textstring);*/
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
    cleardevice();/*清屏*/
    settextstyle(0,0,3);
    outtextxy(150,150,"ARE YOU READY");
    outtextxy(100,300,"TO START THE GAME");
    scanf("%c",&ch);
    if(ch!='n')
        setcolor(BLACK);/*設置顏色為黑色*/
    cleardevice();
}
void Init(void)/*圖形驅動*/
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"c:\\tc");/*此處為turboc的路徑，可根據自己電腦上檔位置改變*/
    cleardevice();/*清屏*/
}

void DrawK(void)/*開始遊戲畫面*/
{

    setcolor(4);/*設置顏色為紅色*/
    setlinestyle(SOLID_LINE,0,THICK_WIDTH);/*實線；此處沒有實際意義；線寬為3個圖元*/
    /*功能： setlinestyle() 為畫線函數設置當前線型，包括線型、線圖樣和線寬。*/
    /*用法： setlinestyle() 函式呼叫方式為void setlinestyle(int stly,unsigned pattern,int wigth);*/
    rectangle(47,37,613,463);/*圖元寬度往裡面凹*/
    /*功能： 函數rectangle() 用當前繪圖色、線型及線寬，畫一個給定左上角與右下角的矩形(正方形或長方形)。*/
    /*畫矩形//用法： 此函式呼叫方式為void rectangle(int left,int top,int right,int bottom);*/
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
    randomize();/*功 能:初始化亂數發生器     用 法:void randomize(void);*/
    food.yes=1;/*1表示需要出現新的食物，0表示已經存在食物*/
    snake.life=0;/*蛇活著*/
    snake.direction=1;/*方向向右*/
    snake.x[0]=100;
    snake.y[0]=100;
    snake.x[1]=110;
    snake.y[1]=100;
    snake.node=2;/*蛇的節數*/
    PrPlay();
    PrScore();/*輸出得分*/

    while(1)/*可以重複玩遊戲，按ESC鍵可以結束*/
    {
        while(!kbhit())/*再沒有按鍵的情況下，蛇還是會自己移動*/
        {
            if(food.yes==1)/*需要出現新的食物*/
            {
                food.x=rand()%400+60;
                food.y=rand()%350+60;/*隨機產生食物*/
                while(food.x%10!=0)/*食物必須出現在整格內，這樣蛇才能吃到*/
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
                food.yes=0;/*介面上有食物了*/
            }
            if(food.yes==0)
            {
                setcolor(YELLOW);/*設置食物顏色為黃色*/
                rectangle(food.x,food.y,food.x+10,food.y-10);/*畫出食物*/
            }
            for(i=snake.node-1; i>0; i--)/*蛇的每一節都往前移動*/
            {
                snake.x[i]=snake.x[i-1];
                snake.y[i]=snake.y[i-1];
            }

            switch(snake.direction)/*實現蛇的移動*/
            {
            case 1:
                snake.x[0]+=10;/*向右*/
                break;
            case 2:
                snake.x[0]-=10;/*向左*/
                break;
            case 3:
                snake.y[0]-=10;/*向上*/
                break;
            case 4:
                snake.y[0]+=10;/*向下*/
                break;
            }
            for(i=3; i<snake.node; i++)/*從蛇的第四節開始判斷是否撞到自己*/
            {
                if(snake.x[i]==snake.x[0]&&snake.y[i]==snake.y[0])
                {
                    GameOver();/*遊戲結束*/
                    snake.life=1;/*蛇死了*/
                    break;
                }
            }
            if(snake.x[0]<=40||snake.x[0]>=610||snake.y[0]<=40||snake.y[0]>=465)/*判斷蛇是否撞到邊界*/
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
                break;/*蛇死了，就跳出迴圈*/
            if(snake.x[0]==food.x&&snake.y[0]==food.y)/*蛇吃到食物*/
            {
                setcolor(0);/*去掉屏面上的食物*/
                rectangle(food.x,food.y,food.x+10,food.y-10);
                snake.x[snake.node]=-20;
                snake.y[snake.node]=-20;/*新的一節先放到看不見的位置，下次迴圈就取前一節的位置*/

                snake.node++;/*蛇身體長一節*/
                food.yes=1;/*新的食物產生*/
                if(level==0)/*等級不同，吃到每個食物的分值不同*/
                {
                    score+=10;/*分數增加*/
                }
                if(level==1)
                {
                    score+=20;
                }
                if(level==2)
                {
                    score+=30;
                }

                PrScore();/*輸出分數*/
            }
            setcolor(2);/*畫出蛇*/
            for(i=0; i<snake.node; i++)
                rectangle(snake.x[i],snake.y[i],snake.x[i]+10,snake.y[i]-10);

            delay(gamespeed);
            setcolor(0);/*用黑色去除蛇的最後一節*/
            rectangle(snake.x[snake.node-1],snake.y[snake.node-1],snake.x[snake.node-1]+10,snake.y[snake.node-1]-10);
        }
        if(snake.life==1)/*蛇死了就跳出迴圈*/
            break;
        key=bioskey(0);/*從鍵盤上接受指令*/
        if(key==ESC)/*按ESC鍵退出遊戲*/
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

void GameOver(void)/*遊戲結束*/
{
    char ov[10];
    cleardevice();

    setcolor(RED);/*設置顏色為紅色*/
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

void PrScore(void)/*遊戲結束語*/
{
    char str[10];
    setfillstyle(SOLID_FILL,BLUE);
    bar(370,15,550,35);
    /*畫條函數
    功能： 函數bar()用當前填充圖樣和填充色(注意不是給圖色)畫出一個指定上左上角與右下角的實心長條形(長方塊或正方塊)，但沒有四條邊線)。
    用法： bar()函式呼叫方式為void bar(int left,int top,int right,int bottom);*/
    setcolor(10);/*設置顏色為淺綠色*/
    settextstyle(0,0,2);
    sprintf(str,"SCORE:%d",score);
    outtextxy(400,20,str);
}

void Close(void)/*遊戲圖形結束*/
{
    getch();
    closegraph();
}
