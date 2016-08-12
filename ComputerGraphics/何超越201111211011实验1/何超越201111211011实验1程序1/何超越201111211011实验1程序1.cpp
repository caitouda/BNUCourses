/*
	w,s,a,d分别对应轴的变化，
    -,=分别对应放大缩小，
	鼠标左键，右键分别对应顺时针，逆时针旋转，
	i，k,j,l分别对应平移的方向，
	h,g分别对应另一个立方体的出现和消失，
	当另一个立方体出现时，可以与原立方体一起变化。
*/


#include<GL/glut.h>
#include<windows.h>
#include<math.h>

void GLInit();//窗口环境初始化
void GLReshape(int w,int h);//当窗口大小改变时，调用该函数，对新的窗口环境进行初始化
void GLRender();
void GLMouse(int button,int state,int x,int y);
void GLKeyDown(unsigned char key,int x,int y);
void GLKeyUp(unsigned char key,int x,int y);
void GLProcessKey(float dTime);
void GLProcessMouse(float dTime);
void GLDrawQuads(int *vInd,int NorInd);
void GLDrawCube();
void GLPostRedisplay();

bool ok=0;
bool gKeys[256]= {false};
bool gLeftButtonDown=false;
bool gRightButtonDown=false;
bool gWireMode=false;
BOOL lighting = false; // 控制光照的开启/关闭
BOOL light = false; // 用以切换两个光源
BOOL pKeyv;
BOOL pKeyC;
// 定义位置在屏幕右上方的光源
GLfloat light_position0[] = {1.0f, 1.0f, 1.0f, 0.0f}; // 设置光源的位置
GLfloat light_ambient0[] = {0.7f, 0.7f, 0.7f, 1.0f}; // 定义环境光的颜色
GLfloat light_diffuse0[] = {1.0f, 0.0f, 1.0f, 1.0f}; // 定义漫反射光的颜色
GLfloat light_specular0[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 定义镜面反射光的颜色
// 定义位置在屏幕左下方的光源
GLfloat light_position1[] = {-1.0f, -1.0f, 1.0f, 0.0f};// 设置光源的位置
GLfloat light_ambient1[] = {0.7f, 0.7f, 0.7f, 1.0f}; // 定义环境光的颜色
GLfloat light_diffuse1[] = {1.0f, 0.0f, 1.0f, 1.0f}; // 定义漫反射光的颜色
GLfloat light_specular1[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 定义镜面反射光的颜色

GLfloat vx=0.0f,vy=0.0f,vz=0.0f,rot=0.0f,tx=0.0f,ty=0.0f;
GLfloat theta=0.0f,miu=0.0f;
GLfloat rotV=360.0f;
GLfloat cubeScale=1.0f,cubeScaleV=0.1f;
GLfloat cubeVertex[24][3]= //每个顶点的位置
{
    {-2.0f,-2.0f,2.0f},//0
    {2.0f,-2.0f,2.0f},//1
    {0.0f,0.0f,0.0f},//2
    {0.0f,0.0f,0.0f},//3
    {-2.0f,-2.0f,-2.0f},//4
    {2.0f,-2.0f,-2.0f},//5
    {0.0f,0.0f,0.0f},//6
    {0.0f,0.0f,0.0f},//7
    {-2.0f,2.0f,2.0f},//8
    {2.0f,2.0f,2.0f},//9
    {2.0f,2.0f,-2.0f},//10
    {-2.0f,2.0f,-2.0f},//11
    {0.0f,-2.0f,0.0f},//0
    {0.0f,-2.0f,0.0f},//1
    {2.0f,0.0f,2.0f},//2
    {-2.0f,0.0f,2.0f},//3
    {0.0f,-2.0f,0.0f},//4
    {0.0f,-2.0f,0.0f},//5
    {2.0f,0.0f,-2.0f},//6
    {-2.0f,0.0f,-2.0f},//7
    {0.0f,2.0f,0.0f},//8
    {0.0f,2.0f,0.0f},//9
    {0.0f,2.0f,0.0f},//10
    {0.0f,2.0f,0.0f}//11

};

GLfloat cubeColor[24][3]=//每个顶点的颜色
{
    {1.0f,0.0f,0.0f},//0
    {1.0f,1.0f,0.0f},//1
    {1.0f,1.0f,0.5f},//2
    {0.0f,1.0f,1.0f},//3
    {0.0f,1.0f,1.0f},//4
    {0.0f,1.0f,0.0f},//5
    {0.5f,0.0f,1.0f},//6
    {0.0f,0.0f,1.0f},//7
    {0.0f,1.0f,0.0f},//8
    {1.0f,1.0f,1.0f},//9
    {1.0f,1.0f,1.0f},//10
    {1.0f,0.5f,0.0f},//11
    {1.0f,0.0f,0.0f},//0
    {1.0f,1.0f,0.0f},//1
    {1.0f,1.0f,0.5f},//2
    {0.0f,1.0f,1.0f},//3
    {0.0f,1.0f,1.0f},//4
    {0.0f,1.0f,0.0f},//5
    {0.5f,0.0f,1.0f},//6
    {0.0f,0.0f,1.0f},//7
    {0.0f,1.0f,0.0f},//8
    {1.0f,1.0f,1.0f},//9
    {1.0f,1.0f,1.0f},//10
    {1.0f,0.5f,0.0f}//11
};

GLfloat cubeNormal[24][3]= //每个面的法向
{
    {0.0f,1.0f,1.0f},
    {0.0f,1.0f,-1.0f},
    {1.0f,1.0f,0.0f},
    {-1.0f,1.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {0.0f,-1.0f,0.0f},//
    {0.0f,-1.0f,1.0f},
    {0.0f,-1.0f,-1.0f},
    {-1.0f,-1.0f,0.0f},
    {1.0f,-1.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {0.0f,-1.0f,0.0f},
    /*	{0.0f,0.0f,1.0f},
    	{0.0f,0.0f,-1.0f},
    	{1.0f,0.0f,0.0f},
    	{-1.0f,0.0f,0.0f}*/
    {0.0f,-1.0f,1.0f},
    {0.0f,-1.0f,-1.0f},
    {-1.0f,-1.0f,0.0f},
    {1.0f,-1.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {0.0f,-1.0f,0.0f},//

    {0.0f,1.0f,1.0f},
    {0.0f,1.0f,-1.0f},
    {1.0f,1.0f,0.0f},
    {-1.0f,1.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {0.0f,-1.0f,0.0f}

};

int cubeVerIndex[24][4]=
{
    {0,1,2,3},
    {4,5,6,7},
    {1,5,6,2},
    {0,4,7,3},
    {2,3,7,6},
    {1,0,4,5},

    {2,3,8,9},
    {6,7,11,10},
    {2,6,10,9},
    {3,7,11,8},
    {8,9,10,11},
    {2,3,7,6},
    /*{0,1,9,8},
    {4,5,10,11},
    {1,5,10,9},
    {0,4,9,8},*/
    {12,13,14,15},
    {16,17,18,19},
    {13,17,18,14},
    {12,16,19,15},
    {14,15,19,18},
    {13,12,16,17},//
    {14,15,21,21},
    {18,19,23,22},
    {14,18,22,21},
    {15,19,23,20},
    {14,21,22,23},
    {14,15,19,18}
};

double cubeMatrix[16]= {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; //单位矩阵
GLfloat ambient[4]= {0.3f,0.3f,0.3f,0.3f};
GLfloat diffuse[4]= {0.5f,0.5f,0.5f,0.5f};
GLfloat position[4]= {10.0f,10.0f,10.0f,0.0f};


void GLMouse(int button,int state,int x,int y)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN&&gLeftButtonDown==false)
            gLeftButtonDown=true;
        if(state==GLUT_UP&&gLeftButtonDown==true)
        {
            gLeftButtonDown=false;
            glPushMatrix();
            glLoadIdentity();
            glRotatef(rot,vx,vy,vz);
            glMultMatrixd(cubeMatrix);
            glGetDoublev(GL_MODELVIEW_MATRIX,cubeMatrix);//让cubeMatrix等于模型视图矩阵堆栈的第一个矩阵
            glPopMatrix();
            rot=0.0f;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if(state==GLUT_DOWN&&gRightButtonDown==false)
            gRightButtonDown=true;
        if(state==GLUT_UP&&gRightButtonDown==true)
        {
            gRightButtonDown=false;
            glPushMatrix();
            glLoadIdentity();
            glRotatef(rot,vx,vy,vz);
            glMultMatrixd(cubeMatrix);
            glGetDoublev(GL_MODELVIEW_MATRIX,cubeMatrix);
            glPopMatrix();
            rot=0.0f;
        }
    }
}

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

void GLProcessKey(float dTime)
{
    if(gKeys['w'])
    {
        theta+=0.01f;
        if(theta>=360.0f)
            theta-=360.0f;
    }
    if(gKeys['s'])
    {
        theta-=0.01f;
        if(theta<=-360.0f)
            theta+=360.0f;
    }
    if(gKeys['a'])
    {
        miu+=0.01f;
        if(miu>=360.0f)
            miu-=360.0f;
    }
    if(gKeys['d'])
    {
        miu-=0.01f;
        if(miu<=-360.0f)
            miu+=360.0f;
    }
    if(gKeys['-'])
    {
        cubeScale-=dTime*cubeScale;
        if(cubeScale<0.0f)
            cubeScale=0.0f;
    }
    if(gKeys['='])
    {
        cubeScale+=dTime*cubeScale;
        if(cubeScale>3.0f)
            cubeScale=3.0f;
    }
    if(gKeys['j'])
    {
        tx-=0.5f;
    }
    if(gKeys['l'])
    {
        tx+=0.5f;
    }
    if(gKeys['i'])
    {
        ty+=0.5f;
    }
    if(gKeys['k'])
    {
        ty-=0.5f;
    }
    if(gKeys['h'])
    {

        /* cubeScale=0.0f;*/
        /*glPushMatrix();
        glBegin();
        	GLDrawQuads(cubeVerIndex[12],12);
            GLDrawQuads(cubeVerIndex[13],13);
            GLDrawQuads(cubeVerIndex[14],14);
            GLDrawQuads(cubeVerIndex[15],15);
            GLDrawQuads(cubeVerIndex[16],16);
            GLDrawQuads(cubeVerIndex[17],17);
        	GLDrawQuads(cubeVerIndex[18],18);
            GLDrawQuads(cubeVerIndex[19],19);
            GLDrawQuads(cubeVerIndex[20],20);
            GLDrawQuads(cubeVerIndex[21],21);
            GLDrawQuads(cubeVerIndex[22],22);
            GLDrawQuads(cubeVerIndex[23],23);
        	glEnd();
            glPopMatrix();*/
        ok=1;

        /*cubeScale+=dTime*cubeScale;
               if(cubeScale>3.0f)
                   cubeScale=3.0f;*/
    }

    if(gKeys['g'])
    {
        //cubeScale=1.0f;
        ok=0;
    }
};

void GLProcessMouse(float dTime)
{
    if(gLeftButtonDown)
        rot+=rotV*dTime;
    if(gRightButtonDown)
        rot-=rotV*dTime;
}

void GLDrawQuads(int *vInd,int NorInd)//画面
{
    glBegin(GL_QUADS);
    glNormal3fv(cubeNormal[NorInd]);
    for(int i=0; i<4; i++)
    {
        glColor3fv(cubeColor[vInd[i]]);
        glVertex3fv(cubeVertex[vInd[i]]);
    }

    glEnd();
}

void GLDrawCube()//依次画十二个面
{
    GLDrawQuads(cubeVerIndex[0],0);
    GLDrawQuads(cubeVerIndex[1],1);
    GLDrawQuads(cubeVerIndex[2],2);
    GLDrawQuads(cubeVerIndex[3],3);
    GLDrawQuads(cubeVerIndex[4],4);
    GLDrawQuads(cubeVerIndex[5],5);
    GLDrawQuads(cubeVerIndex[6],6);
    GLDrawQuads(cubeVerIndex[7],7);
    GLDrawQuads(cubeVerIndex[8],8);
    GLDrawQuads(cubeVerIndex[9],9);
    GLDrawQuads(cubeVerIndex[10],10);
    GLDrawQuads(cubeVerIndex[11],11);

    /*	GLDrawQuads(cubeVerIndex[12],12);

    		GLDrawQuads(cubeVerIndex[13],13);
        GLDrawQuads(cubeVerIndex[14],14);
        GLDrawQuads(cubeVerIndex[15],15);
        GLDrawQuads(cubeVerIndex[16],16);
        GLDrawQuads(cubeVerIndex[17],17);
    	GLDrawQuads(cubeVerIndex[18],18);
        GLDrawQuads(cubeVerIndex[19],19);
        GLDrawQuads(cubeVerIndex[20],20);
        GLDrawQuads(cubeVerIndex[21],21);
        GLDrawQuads(cubeVerIndex[22],22);
        GLDrawQuads(cubeVerIndex[23],23);*/
}


void GLPostRedisplay()
{
    glutPostRedisplay();
}


void GLInit()
{
    // 第一个光源
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0); // 创建光源位置
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0); // 创建环境光
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0); // 创建漫反射光
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0); // 创建镜面反射光
// 第二个光源
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1); // 创建光源位置
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1); // 创建环境光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1); // 创建漫反射光
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1); // 创建镜面反射光
    glClearColor(0.0f,0.0f,0.0f,0.0f);								//将背景色置为RGBA值为0,0,0,0的颜色，即黑色，这里先不用关注第4个参数（Alpha值）
    glEnable(GL_DEPTH_TEST);										//开启深度测试，等到完成本次作业后，大家可以看看去掉它以后会怎么样，为什么会这样呢？
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth(1.0f);												//将深度缓冲区中的值设为1.0，这里可以先不用弄清它的含义
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /*
    	glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
    	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
    */
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
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


void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清除的颜色和深度缓冲区
    glLoadIdentity();//把当前矩阵设置为单位矩阵，因为绝大多数变换把当前矩阵与指定的矩阵相乘

    //glTranslatef(0.0f,0.0f,-20.0f);

    gluLookAt(10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    /*可以参考红宝书中1-3程序double.c，你需要调用一个设定颜色填充模式的函数glPolygonMode*/

    if(ok)
    {
        glPushMatrix();
        glScalef(cubeScale,cubeScale,cubeScale);
        glTranslatef(tx,ty,0);
        glRotatef(rot,vx,vy,vz);//向量(vx,vy.vz)旋转rot角
        //	glBegin();
        GLDrawQuads(cubeVerIndex[12],12);
        GLDrawQuads(cubeVerIndex[13],13);
        GLDrawQuads(cubeVerIndex[14],14);
        GLDrawQuads(cubeVerIndex[15],15);
        GLDrawQuads(cubeVerIndex[16],16);
        GLDrawQuads(cubeVerIndex[17],17);
        GLDrawQuads(cubeVerIndex[18],18);
        GLDrawQuads(cubeVerIndex[19],19);
        GLDrawQuads(cubeVerIndex[20],20);
        GLDrawQuads(cubeVerIndex[21],21);
        GLDrawQuads(cubeVerIndex[22],22);
        GLDrawQuads(cubeVerIndex[23],23);
        //glEnd();
        glPopMatrix();
    }


    vx=sin(theta)*cos(miu);
    vy=cos(theta);
    vz=sin(theta)*sin(miu);

    glPushMatrix();//存放当前的变换矩阵
    glBegin(GL_LINES);//开始画x、y、z三个坐标轴

    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(5.0f,0.0f,0.0f);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,5.0f,0.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,5.0f);
    glEnd();//把当前的矩阵从堆栈里清出去
    glPopMatrix();//恢复到开始的变换矩阵，避免连环变换的情况


    glColor3f(1.0f,1.0f,1.0f);//指定即将要画的图形的颜色（准确地说是顶点的颜色）

    glPushMatrix();
    glScalef(cubeScale,cubeScale,cubeScale);
    glBegin(GL_LINES);
    glVertex3f(-10.0f*vx,-10.0f*vy,-10.0f*vz);//方向为（vx,vy,vz）轴
    glVertex3f(10.0f*vx,10.0f*vy,10.0f*vz);
    glEnd();
    glPopMatrix();


    if(gWireMode)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


    glPushMatrix();
    glTranslatef(tx,ty,0);
    glRotatef(rot,vx,vy,vz);//向量(vx,vy.vz)旋转rot角
    glMultMatrixd(cubeMatrix);
    glScalef(cubeScale,cubeScale,cubeScale);
    GLDrawCube();
    glPopMatrix();


    static signed __int64 lastTickCount=GetTickCount();
    signed __int64 TickCount=GetTickCount();
    float dt=(TickCount-lastTickCount)/1000.0f;
    lastTickCount=TickCount;

    GLProcessKey(dt);
    GLProcessMouse(dt);
    if(rot>=360.0f)rot-=360.0f;
    /***************************************************************************GLRender中需要你们实现的部分至此结束********************************************************************/

    glFlush();															//强制刷新缓冲区的函数，如果去掉呢？
    glutSwapBuffers();													//交换使用另一个缓冲区，为什么要交换呢？原理在红宝书第一章动画那一节里有阐述
}


int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut的初始化函数，负责处理命令行参数，在这里我们只需记住它是用glut搭建Opengl显示环境必须要第一个调用的函数
    glutInitWindowPosition(500,500);								//glut的窗口初始化函数，设定了绘图窗口左上角在整个屏幕上的坐标（以像素为单位，下同）
    glutInitWindowSize(500,500);									//glut的窗口初始化函数，设定了绘图窗口的大小
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//将窗口显示模式设定为双缓冲、RGBA颜色模式并且带深度缓存，在完成本次作业后，可以去掉或者修改这三个参数中的任意一个，看看显示结果会有什么不同，为什么会这样呢？
    glutCreateWindow("BNU CG Lab1_1 例程");							//创建一个名为"GLUT Framework(BNU CG Lab1)"的窗口，如果把这一行放在上面三个函数任意一个之前呢？
    GLInit();														//初始化OpenGL的函数
    glutDisplayFunc(GLRender);										//显示回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLRender函数，注意这里对回调函数本身的参数和返回值的要求
    glutReshapeFunc(GLReshape);										//窗口大小调整回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLReshape函数，注意这里对回调函数本身的参数和返回值的要求
    glutIdleFunc(GLPostRedisplay);
    glutKeyboardFunc(GLKeyDown);
    glutKeyboardUpFunc(GLKeyUp);
    glutMouseFunc(GLMouse);
    glutMainLoop();													//只有调用了这个函数，才会出现绘图窗口
    return 0;
}
