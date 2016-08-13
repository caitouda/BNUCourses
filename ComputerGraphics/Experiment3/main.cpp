/*
	师兄，你好。这次实验以提供的数据作为参照，以呈现更好的效果为原则完成的。
	虽然有些数据并不是非常精确，但可以从中得到基本的相互关系。
	几个按键的作用：
	“j”表示视点左移；
	“l”表示视点右移；
	“i”表示视点上移；
	“k”表示视点下移；
	“w”表示视点拉远；
	“s”表示视点靠近；
	（视点上下移动的效果最为明显，因为可以使所有星球处于一个水平面）
*/

#include<GL/glut.h>
#include<windows.h>
#include<math.h>
#include <stdio.h>
#include "glPoint.h"
#include "glVector.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define D2R (PI/180.0)

GLfloat fLowLight[] = { 0.0f, 0.0f, 0.25f, 0.50f };

// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

typedef struct													// Create A Structure
{
    GLubyte	*imageData;											// Image Data (Up To 32 Bits)
    GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
    GLuint	width;												// Image Width
    GLuint	height;												// Image Height
    GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;

GLUquadricObj *sphere;
TextureImage sphereTexMercury;//水星
TextureImage sphereTexVenus;//金星
TextureImage sphereTex;//地球
TextureImage sphereTexMoon;//月球
TextureImage sphereTexMars;//火星
TextureImage sphereTexJupiter;//木星
TextureImage sphereTexSaturn;//土星
TextureImage sphereTexUranus;//天王星
TextureImage sphereTexNeptune;//海王星
TextureImage sphereTexPluto;//冥王星
TextureImage sphereTexSun;//太阳

void GLInit();//窗口环境初始化
void GLReshape(int w,int h);//当窗口大小改变时，调用该函数，对新的窗口环境进行初始化
void GLRender();
void GLKeyDown(unsigned char key,int x,int y);
void GLKeyUp(unsigned char key,int x,int y);
void GLProcessKey(float dTime);

void GLPostRedisplay();

GLfloat vx=0.0f,vy=0.0f,vz=0.0f;//视点的三种变量
bool gKeys[256]= {false};
bool gWireMode=false;

void GLKeyDown(unsigned char key,int x,int y)
{
    if(!gKeys[key])
        gKeys[key]=true;
}

void GLKeyUp(unsigned char key,int x,int y)
{
    if(gKeys[key])
        gKeys[key]=false;

    if(key=='p')
        gWireMode=!gWireMode;
}

void GLProcessKey(float dTime)//键盘的按键设置
{
    if(gKeys['j'])
    {
        vx=vx-dTime;
    }
    if(gKeys['l'])
    {
        vx=vx+dTime;
    }
    if(gKeys['i'])
    {
        vy=vy+dTime;
    }
    if(gKeys['k'])
    {
        vy=vy-dTime;
    }
    if(gKeys['w'])
    {
        vz=vz+dTime;
    }
    if(gKeys['s'])
    {
        vz=vz-dTime;
    }
};

void GLPostRedisplay()
{
    glutPostRedisplay();
}

int GLInitBMPTexture(TextureImage *texture,char *filename)
{
    AUX_RGBImageRec *bmpRec;
    FILE *file;
    if(!(file=fopen(filename,"r")))
    {
        return FALSE;
    }
    fclose(file);

    if(!(bmpRec=auxDIBImageLoad(filename)))
        return FALSE;
    glGenTextures(1,&texture->texID);
    glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmpRec->sizeX,bmpRec->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,bmpRec->data);
    free(bmpRec->data);
    return TRUE;
}

void GLInit()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);								//将背景色置为RGBA值为0,0,0,0的颜色，即黑色，这里先不用关注第4个参数（Alpha值）
    glEnable(GL_DEPTH_TEST);										//开启深度测试，等到完成本次作业后，大家可以看看去掉它以后会怎么样，为什么会这样呢？
    glClearDepth(1.0f);												//将深度缓冲区中的值设为1.0，这里可以先不用弄清它的含义
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

    sphere=gluNewQuadric();	//新的二次曲面
    gluQuadricTexture(sphere,GL_TRUE);
    GLInitBMPTexture(&sphereTexMercury,"Mercury.bmp");
    GLInitBMPTexture(&sphereTexVenus,"Venus.bmp");
    GLInitBMPTexture(&sphereTex,"Earth.bmp");
    GLInitBMPTexture(&sphereTexMoon,"Moon.bmp");
    GLInitBMPTexture(&sphereTexMars,"Mars.bmp");
    GLInitBMPTexture(&sphereTexJupiter,"Jupiter.bmp");
    GLInitBMPTexture(&sphereTexSaturn,"Saturn.bmp");
    GLInitBMPTexture(&sphereTexUranus,"Uranus.bmp");
    GLInitBMPTexture(&sphereTexNeptune,"Neptune.bmp");
    GLInitBMPTexture(&sphereTexPluto,"Pluto.bmp");
    GLInitBMPTexture(&sphereTexSun,"Sun.bmp");
}

void GLReshape(int w,int h)
{
    glViewport(0,0,w,h);//设定视口的函数,窗口大小变化时，视口不会自动跟着变化
    glMatrixMode(GL_PROJECTION);//将当前矩阵设定为投影矩阵，默认为模型视图矩阵
    glLoadIdentity();//将当前矩阵设为单位矩阵
    gluPerspective(45.0f,float(w)/float(h),0.01f,50.0f);//将投影设置为视锥体内的透视投影
    glMatrixMode(GL_MODELVIEW);	//将当前矩阵设定为模型视图矩阵，移动照相机为视图变换，移动物体为模型变换
    //为什么不将该语句写在GLRender()里？因为只需要在改变了当前矩阵的情况下，才需要将其变回原来的模型视图矩阵，以便后面运用模型视图变换
}

void DrawSphere(float radius,int slices,int stacks)
{
    int i,j;
    GLfloat miu,theta;
    glPoint *pointArray;
    glPoint *texCoorArray;
    pointArray = new glPoint[stacks+1];
    texCoorArray = new glPoint[stacks+1];

    glBegin(GL_TRIANGLE_STRIP);

    for(i=0; i<=stacks; i++)
    {
        pointArray[i].x=0.0f;
        pointArray[i].y=radius;
        pointArray[i].z=0.0f;
        texCoorArray[i].x = i*(1.0/stacks);
        texCoorArray[i].y = 1.0f;
    }

    for(j=1; j<=slices; j++)
    {
        miu=(j*180.0/slices)*D2R;
        for(i=0; i<=stacks; i++)
        {
            glPoint pTmp;
            glPoint texCoordTmp;
            theta=(i*360.0/stacks)*D2R;
            pTmp.x=radius*sin(miu)*cos(theta);
            pTmp.y=radius*cos(miu);
            pTmp.z=radius*sin(miu)*sin(theta);

            texCoordTmp.x = i*(1.0/stacks);
            texCoordTmp.y = 1.0f - j*(1.0/slices);

            glTexCoord2f(texCoorArray[i].x,texCoorArray[i].y);
            glVertex3f(pointArray[i].x,pointArray[i].y,pointArray[i].z);
            pointArray[i]=pTmp;
            texCoorArray[i]=texCoordTmp;
            glTexCoord2f(texCoorArray[i].x,texCoorArray[i].y);
            glVertex3f(pointArray[i].x,pointArray[i].y,pointArray[i].z);
        }
    }
    glEnd();
    delete []pointArray;
    delete []texCoorArray;
}

float a=0.0f;//公转变量
float b=0.0f;//自转变量

void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清除的颜色和深度缓冲区
    glLoadIdentity();//把当前矩阵设置为单位矩阵，因为绝大多数变换把当前矩阵与指定的矩阵相乘

    gluLookAt(vx+10.0f,vy+10.0f,vz+10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);//视点的变化

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glPushMatrix();//太阳
    glBindTexture(GL_TEXTURE_2D,sphereTexSun.texID);

    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(a,0,0,1.0);
    glTranslatef(0,0,0);
    gluSphere(sphere,2.0f,32,32);//第一个坐标改大小
    glPopMatrix();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);//开灯
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);//启用深度测试
    glEnable(GL_COLOR_MATERIAL);//指定第0号光源的位置，就是太阳的位置

    glPushMatrix();//水星
    glBindTexture(GL_TEXTURE_2D,sphereTexMercury.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(4.788*a/8,0,0,1.0);//公转
    glTranslatef(1.160*2,0,0);
    glRotatef(11.363636*b,0,0,1.0);//自转
    gluSphere(sphere,4*0.01f,32,32);
    glPopMatrix();

    glPushMatrix();//金星
    glBindTexture(GL_TEXTURE_2D,sphereTexVenus.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(3.502*a/8,0,0,1.0);
    glTranslatef(2.000*2,0,0);
    glRotatef(4.464286*b,0,0,1.0);
    gluSphere(sphere,4*0.02f,32,32);
    glPopMatrix();

    glPushMatrix();//地球
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(2.979*a/8,0,0,1.0);
    glTranslatef(3.000*2,0,0);//改位置，放在旋转后面
    glRotatef(2.739726*b/10,0,0,1.0);
    glBindTexture(GL_TEXTURE_2D,sphereTex.texID);
    gluSphere(sphere,4*0.054f,32,32);

    glPushMatrix();//月球
    glBindTexture(GL_TEXTURE_2D,sphereTexMoon.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.05*a,0,1.0,0);
    glTranslatef(0.5,0,0);//改位置，放在旋转后面
    gluSphere(sphere,0.05f,32,32);

    glPopMatrix();

    glPopMatrix();

    glPushMatrix();//火星
    glBindTexture(GL_TEXTURE_2D,sphereTexMars.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(2.413*a/8,0,0,1.0);
    glTranslatef(4.600*2,0,0);
    glRotatef(1.455605*b,0,0,1.0);
    gluSphere(sphere,4*0.034f,32,32);
    glPopMatrix();

    glPushMatrix();//木星
    glBindTexture(GL_TEXTURE_2D,sphereTexJupiter.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(1.306*a/8,0,0,1.0);
    glTranslatef(6.000*2,0,0);
    glRotatef(0.230840*b,0,0,1.0);
    gluSphere(sphere,4*0.160f,32,32);
    glPopMatrix();

    glPushMatrix();//土星
    glBindTexture(GL_TEXTURE_2D,sphereTexSaturn.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.946*a/8,0,0,1.0);
    glTranslatef(7.000*2,0,0);
    glRotatef(0.092945*b,0,0,1.0);
    gluSphere(sphere,4*0.090f,32,32);
    glPopMatrix();

    glPushMatrix();//天王星
    glBindTexture(GL_TEXTURE_2D,sphereTexUranus.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.681*a/8,0,0,1.0);
    glTranslatef(7.500*2,0,0);
    glRotatef(0.032589*b,0,0,1.0);
    gluSphere(sphere,4*0.12f,32,32);
    glPopMatrix();

    glPushMatrix();//海王星
    glBindTexture(GL_TEXTURE_2D,sphereTexNeptune.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.544*a/8,0,0,1.0);
    glTranslatef(10.000*2,0,0);
    glRotatef(0.016624*b,0,0,1.0);
    gluSphere(sphere,4*0.13f,32,32);
    glPopMatrix();

    glPushMatrix();//冥王星
    glBindTexture(GL_TEXTURE_2D,sphereTexPluto.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.475*a/8,0,0,1.0);
    glTranslatef(119/5,0,0);
    glRotatef(0.015*b,0,0,1.0);
    gluSphere(sphere,4*0.0099f,32,32);
    glPopMatrix();

    static signed __int64 lastTickCount=GetTickCount();
    signed __int64 TickCount=GetTickCount();
    float dt=(TickCount-lastTickCount)/500.0f;
    lastTickCount=TickCount;

    GLProcessKey(dt);

    a=a+1;
    b=b+4;
    /***************************************************************************GLRender中需要你们实现的部分至此结束********************************************************************/

    glFlush();															//强制刷新缓冲区的函数，如果去掉呢？
    glutSwapBuffers();													//交换使用另一个缓冲区，为什么要交换呢？原理在红宝书第一章动画那一节里有阐述
}

int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut的初始化函数，负责处理命令行参数，在这里我们只需记住它是用glut搭建Opengl显示环境必须要第一个调用的函数
    glutInitWindowPosition(500,100);								//glut的窗口初始化函数，设定了绘图窗口左上角在整个屏幕上的坐标（以像素为单位，下同）
    glutInitWindowSize(500,500);									//glut的窗口初始化函数，设定了绘图窗口的大小
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//将窗口显示模式设定为双缓冲、RGBA颜色模式并且带深度缓存，在完成本次作业后，可以去掉或者修改这三个参数中的任意一个，看看显示结果会有什么不同，为什么会这样呢？
    glutCreateWindow("何超越——太阳系");							//创建一个名为"GLUT Framework(BNU CG Lab1)"的窗口，如果把这一行放在上面三个函数任意一个之前呢？
    GLInit();														//初始化OpenGL的函数
    glutDisplayFunc(GLRender);										//显示回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLRender函数，注意这里对回调函数本身的参数和返回值的要求
    glutReshapeFunc(GLReshape);										//窗口大小调整回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLReshape函数，注意这里对回调函数本身的参数和返回值的要求
    glutKeyboardFunc(GLKeyDown);
    glutKeyboardUpFunc(GLKeyUp);
    glutIdleFunc(GLPostRedisplay);
    glutMainLoop();													//只有调用了这个函数，才会出现绘图窗口
    return 0;
}
