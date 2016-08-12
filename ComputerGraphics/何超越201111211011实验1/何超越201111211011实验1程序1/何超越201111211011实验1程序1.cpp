/*
	w,s,a,d�ֱ��Ӧ��ı仯��
    -,=�ֱ��Ӧ�Ŵ���С��
	���������Ҽ��ֱ��Ӧ˳ʱ�룬��ʱ����ת��
	i��k,j,l�ֱ��Ӧƽ�Ƶķ���
	h,g�ֱ��Ӧ��һ��������ĳ��ֺ���ʧ��
	����һ�����������ʱ��������ԭ������һ��仯��
*/


#include<GL/glut.h>
#include<windows.h>
#include<math.h>

void GLInit();//���ڻ�����ʼ��
void GLReshape(int w,int h);//�����ڴ�С�ı�ʱ�����øú��������µĴ��ڻ������г�ʼ��
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
BOOL lighting = false; // ���ƹ��յĿ���/�ر�
BOOL light = false; // �����л�������Դ
BOOL pKeyv;
BOOL pKeyC;
// ����λ������Ļ���Ϸ��Ĺ�Դ
GLfloat light_position0[] = {1.0f, 1.0f, 1.0f, 0.0f}; // ���ù�Դ��λ��
GLfloat light_ambient0[] = {0.7f, 0.7f, 0.7f, 1.0f}; // ���廷�������ɫ
GLfloat light_diffuse0[] = {1.0f, 0.0f, 1.0f, 1.0f}; // ��������������ɫ
GLfloat light_specular0[] = {1.0f, 1.0f, 1.0f, 1.0f}; // ���徵�淴������ɫ
// ����λ������Ļ���·��Ĺ�Դ
GLfloat light_position1[] = {-1.0f, -1.0f, 1.0f, 0.0f};// ���ù�Դ��λ��
GLfloat light_ambient1[] = {0.7f, 0.7f, 0.7f, 1.0f}; // ���廷�������ɫ
GLfloat light_diffuse1[] = {1.0f, 0.0f, 1.0f, 1.0f}; // ��������������ɫ
GLfloat light_specular1[] = {1.0f, 1.0f, 1.0f, 1.0f}; // ���徵�淴������ɫ

GLfloat vx=0.0f,vy=0.0f,vz=0.0f,rot=0.0f,tx=0.0f,ty=0.0f;
GLfloat theta=0.0f,miu=0.0f;
GLfloat rotV=360.0f;
GLfloat cubeScale=1.0f,cubeScaleV=0.1f;
GLfloat cubeVertex[24][3]= //ÿ�������λ��
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

GLfloat cubeColor[24][3]=//ÿ���������ɫ
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

GLfloat cubeNormal[24][3]= //ÿ����ķ���
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

double cubeMatrix[16]= {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; //��λ����
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
            glGetDoublev(GL_MODELVIEW_MATRIX,cubeMatrix);//��cubeMatrix����ģ����ͼ�����ջ�ĵ�һ������
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

void GLDrawQuads(int *vInd,int NorInd)//����
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

void GLDrawCube()//���λ�ʮ������
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
    // ��һ����Դ
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0); // ������Դλ��
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0); // ����������
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0); // �����������
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0); // �������淴���
// �ڶ�����Դ
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1); // ������Դλ��
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1); // ����������
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1); // �����������
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1); // �������淴���
    glClearColor(0.0f,0.0f,0.0f,0.0f);								//������ɫ��ΪRGBAֵΪ0,0,0,0����ɫ������ɫ�������Ȳ��ù�ע��4��������Alphaֵ��
    glEnable(GL_DEPTH_TEST);										//������Ȳ��ԣ��ȵ���ɱ�����ҵ�󣬴�ҿ��Կ���ȥ�����Ժ����ô����Ϊʲô�������أ�
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth(1.0f);												//����Ȼ������е�ֵ��Ϊ1.0����������Ȳ���Ū�����ĺ���
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
    glViewport(0,0,w,h);//�趨�ӿڵĺ���,���ڴ�С�仯ʱ���ӿڲ����Զ����ű仯
    glMatrixMode(GL_PROJECTION);//����ǰ�����趨ΪͶӰ����Ĭ��Ϊģ����ͼ����
    glLoadIdentity();//����ǰ������Ϊ��λ����
    gluPerspective(45.0f,float(w)/float(h),0.01f,50.0f);//��ͶӰ����Ϊ��׶���ڵ�͸��ͶӰ
    glMatrixMode(GL_MODELVIEW);	//����ǰ�����趨Ϊģ����ͼ�����ƶ������Ϊ��ͼ�任���ƶ�����Ϊģ�ͱ任
    //Ϊʲô���������д��GLRender()���Ϊֻ��Ҫ�ڸı��˵�ǰ���������£�����Ҫ������ԭ����ģ����ͼ�����Ա��������ģ����ͼ�任
}


void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//�������ɫ����Ȼ�����
    glLoadIdentity();//�ѵ�ǰ��������Ϊ��λ������Ϊ��������任�ѵ�ǰ������ָ���ľ������

    //glTranslatef(0.0f,0.0f,-20.0f);

    gluLookAt(10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    /*���Բο��챦����1-3����double.c������Ҫ����һ���趨��ɫ���ģʽ�ĺ���glPolygonMode*/

    if(ok)
    {
        glPushMatrix();
        glScalef(cubeScale,cubeScale,cubeScale);
        glTranslatef(tx,ty,0);
        glRotatef(rot,vx,vy,vz);//����(vx,vy.vz)��תrot��
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

    glPushMatrix();//��ŵ�ǰ�ı任����
    glBegin(GL_LINES);//��ʼ��x��y��z����������

    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(5.0f,0.0f,0.0f);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,5.0f,0.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,5.0f);
    glEnd();//�ѵ�ǰ�ľ���Ӷ�ջ�����ȥ
    glPopMatrix();//�ָ�����ʼ�ı任���󣬱��������任�����


    glColor3f(1.0f,1.0f,1.0f);//ָ������Ҫ����ͼ�ε���ɫ��׼ȷ��˵�Ƕ������ɫ��

    glPushMatrix();
    glScalef(cubeScale,cubeScale,cubeScale);
    glBegin(GL_LINES);
    glVertex3f(-10.0f*vx,-10.0f*vy,-10.0f*vz);//����Ϊ��vx,vy,vz����
    glVertex3f(10.0f*vx,10.0f*vy,10.0f*vz);
    glEnd();
    glPopMatrix();


    if(gWireMode)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


    glPushMatrix();
    glTranslatef(tx,ty,0);
    glRotatef(rot,vx,vy,vz);//����(vx,vy.vz)��תrot��
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
    /***************************************************************************GLRender����Ҫ����ʵ�ֵĲ������˽���********************************************************************/

    glFlush();															//ǿ��ˢ�»������ĺ��������ȥ���أ�
    glutSwapBuffers();													//����ʹ����һ����������ΪʲôҪ�����أ�ԭ���ں챦���һ�¶�����һ�����в���
}


int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut�ĳ�ʼ�������������������в���������������ֻ���ס������glut�Opengl��ʾ��������Ҫ��һ�����õĺ���
    glutInitWindowPosition(500,500);								//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ�������Ͻ���������Ļ�ϵ����꣨������Ϊ��λ����ͬ��
    glutInitWindowSize(500,500);									//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ���ڵĴ�С
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//��������ʾģʽ�趨Ϊ˫���塢RGBA��ɫģʽ���Ҵ���Ȼ��棬����ɱ�����ҵ�󣬿���ȥ�������޸������������е�����һ����������ʾ�������ʲô��ͬ��Ϊʲô�������أ�
    glutCreateWindow("BNU CG Lab1_1 ����");							//����һ����Ϊ"GLUT Framework(BNU CG Lab1)"�Ĵ��ڣ��������һ�з�������������������һ��֮ǰ�أ�
    GLInit();														//��ʼ��OpenGL�ĺ���
    glutDisplayFunc(GLRender);										//��ʾ�ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLRender������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutReshapeFunc(GLReshape);										//���ڴ�С�����ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLReshape������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutIdleFunc(GLPostRedisplay);
    glutKeyboardFunc(GLKeyDown);
    glutKeyboardUpFunc(GLKeyUp);
    glutMouseFunc(GLMouse);
    glutMainLoop();													//ֻ�е���������������Ż���ֻ�ͼ����
    return 0;
}
