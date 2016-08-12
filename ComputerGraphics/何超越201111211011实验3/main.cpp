/*
	ʦ�֣���á����ʵ�����ṩ��������Ϊ���գ��Գ��ָ��õ�Ч��Ϊԭ����ɵġ�
	��Ȼ��Щ���ݲ����Ƿǳ���ȷ�������Դ��еõ��������໥��ϵ��
	�������������ã�
	��j����ʾ�ӵ����ƣ�
	��l����ʾ�ӵ����ƣ�
	��i����ʾ�ӵ����ƣ�
	��k����ʾ�ӵ����ƣ�
	��w����ʾ�ӵ���Զ��
	��s����ʾ�ӵ㿿����
	���ӵ������ƶ���Ч����Ϊ���ԣ���Ϊ����ʹ����������һ��ˮƽ�棩
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
TextureImage sphereTexMercury;//ˮ��
TextureImage sphereTexVenus;//����
TextureImage sphereTex;//����
TextureImage sphereTexMoon;//����
TextureImage sphereTexMars;//����
TextureImage sphereTexJupiter;//ľ��
TextureImage sphereTexSaturn;//����
TextureImage sphereTexUranus;//������
TextureImage sphereTexNeptune;//������
TextureImage sphereTexPluto;//ڤ����
TextureImage sphereTexSun;//̫��

void GLInit();//���ڻ�����ʼ��
void GLReshape(int w,int h);//�����ڴ�С�ı�ʱ�����øú��������µĴ��ڻ������г�ʼ��
void GLRender();
void GLKeyDown(unsigned char key,int x,int y);
void GLKeyUp(unsigned char key,int x,int y);
void GLProcessKey(float dTime);

void GLPostRedisplay();

GLfloat vx=0.0f,vy=0.0f,vz=0.0f;//�ӵ�����ֱ���
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

void GLProcessKey(float dTime)//���̵İ�������
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
    glClearColor(0.0f,0.0f,0.0f,0.0f);								//������ɫ��ΪRGBAֵΪ0,0,0,0����ɫ������ɫ�������Ȳ��ù�ע��4��������Alphaֵ��
    glEnable(GL_DEPTH_TEST);										//������Ȳ��ԣ��ȵ���ɱ�����ҵ�󣬴�ҿ��Կ���ȥ�����Ժ����ô����Ϊʲô�������أ�
    glClearDepth(1.0f);												//����Ȼ������е�ֵ��Ϊ1.0����������Ȳ���Ū�����ĺ���
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

    sphere=gluNewQuadric();	//�µĶ�������
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
    glViewport(0,0,w,h);//�趨�ӿڵĺ���,���ڴ�С�仯ʱ���ӿڲ����Զ����ű仯
    glMatrixMode(GL_PROJECTION);//����ǰ�����趨ΪͶӰ����Ĭ��Ϊģ����ͼ����
    glLoadIdentity();//����ǰ������Ϊ��λ����
    gluPerspective(45.0f,float(w)/float(h),0.01f,50.0f);//��ͶӰ����Ϊ��׶���ڵ�͸��ͶӰ
    glMatrixMode(GL_MODELVIEW);	//����ǰ�����趨Ϊģ����ͼ�����ƶ������Ϊ��ͼ�任���ƶ�����Ϊģ�ͱ任
    //Ϊʲô���������д��GLRender()���Ϊֻ��Ҫ�ڸı��˵�ǰ���������£�����Ҫ������ԭ����ģ����ͼ�����Ա��������ģ����ͼ�任
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

float a=0.0f;//��ת����
float b=0.0f;//��ת����

void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//�������ɫ����Ȼ�����
    glLoadIdentity();//�ѵ�ǰ��������Ϊ��λ������Ϊ��������任�ѵ�ǰ������ָ���ľ������

    gluLookAt(vx+10.0f,vy+10.0f,vz+10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);//�ӵ�ı仯

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glPushMatrix();//̫��
    glBindTexture(GL_TEXTURE_2D,sphereTexSun.texID);

    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(a,0,0,1.0);
    glTranslatef(0,0,0);
    gluSphere(sphere,2.0f,32,32);//��һ������Ĵ�С
    glPopMatrix();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);//����
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);//������Ȳ���
    glEnable(GL_COLOR_MATERIAL);//ָ����0�Ź�Դ��λ�ã�����̫����λ��

    glPushMatrix();//ˮ��
    glBindTexture(GL_TEXTURE_2D,sphereTexMercury.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(4.788*a/8,0,0,1.0);//��ת
    glTranslatef(1.160*2,0,0);
    glRotatef(11.363636*b,0,0,1.0);//��ת
    gluSphere(sphere,4*0.01f,32,32);
    glPopMatrix();

    glPushMatrix();//����
    glBindTexture(GL_TEXTURE_2D,sphereTexVenus.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(3.502*a/8,0,0,1.0);
    glTranslatef(2.000*2,0,0);
    glRotatef(4.464286*b,0,0,1.0);
    gluSphere(sphere,4*0.02f,32,32);
    glPopMatrix();

    glPushMatrix();//����
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(2.979*a/8,0,0,1.0);
    glTranslatef(3.000*2,0,0);//��λ�ã�������ת����
    glRotatef(2.739726*b/10,0,0,1.0);
    glBindTexture(GL_TEXTURE_2D,sphereTex.texID);
    gluSphere(sphere,4*0.054f,32,32);

    glPushMatrix();//����
    glBindTexture(GL_TEXTURE_2D,sphereTexMoon.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.05*a,0,1.0,0);
    glTranslatef(0.5,0,0);//��λ�ã�������ת����
    gluSphere(sphere,0.05f,32,32);

    glPopMatrix();

    glPopMatrix();

    glPushMatrix();//����
    glBindTexture(GL_TEXTURE_2D,sphereTexMars.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(2.413*a/8,0,0,1.0);
    glTranslatef(4.600*2,0,0);
    glRotatef(1.455605*b,0,0,1.0);
    gluSphere(sphere,4*0.034f,32,32);
    glPopMatrix();

    glPushMatrix();//ľ��
    glBindTexture(GL_TEXTURE_2D,sphereTexJupiter.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(1.306*a/8,0,0,1.0);
    glTranslatef(6.000*2,0,0);
    glRotatef(0.230840*b,0,0,1.0);
    gluSphere(sphere,4*0.160f,32,32);
    glPopMatrix();

    glPushMatrix();//����
    glBindTexture(GL_TEXTURE_2D,sphereTexSaturn.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.946*a/8,0,0,1.0);
    glTranslatef(7.000*2,0,0);
    glRotatef(0.092945*b,0,0,1.0);
    gluSphere(sphere,4*0.090f,32,32);
    glPopMatrix();

    glPushMatrix();//������
    glBindTexture(GL_TEXTURE_2D,sphereTexUranus.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.681*a/8,0,0,1.0);
    glTranslatef(7.500*2,0,0);
    glRotatef(0.032589*b,0,0,1.0);
    gluSphere(sphere,4*0.12f,32,32);
    glPopMatrix();

    glPushMatrix();//������
    glBindTexture(GL_TEXTURE_2D,sphereTexNeptune.texID);
    glRotatef (-90,1.0f, 0.0f, 0.0f);
    glRotatef(0.544*a/8,0,0,1.0);
    glTranslatef(10.000*2,0,0);
    glRotatef(0.016624*b,0,0,1.0);
    gluSphere(sphere,4*0.13f,32,32);
    glPopMatrix();

    glPushMatrix();//ڤ����
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
    /***************************************************************************GLRender����Ҫ����ʵ�ֵĲ������˽���********************************************************************/

    glFlush();															//ǿ��ˢ�»������ĺ��������ȥ���أ�
    glutSwapBuffers();													//����ʹ����һ����������ΪʲôҪ�����أ�ԭ���ں챦���һ�¶�����һ�����в���
}

int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut�ĳ�ʼ�������������������в���������������ֻ���ס������glut�Opengl��ʾ��������Ҫ��һ�����õĺ���
    glutInitWindowPosition(500,100);								//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ�������Ͻ���������Ļ�ϵ����꣨������Ϊ��λ����ͬ��
    glutInitWindowSize(500,500);									//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ���ڵĴ�С
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//��������ʾģʽ�趨Ϊ˫���塢RGBA��ɫģʽ���Ҵ���Ȼ��棬����ɱ�����ҵ�󣬿���ȥ�������޸������������е�����һ����������ʾ�������ʲô��ͬ��Ϊʲô�������أ�
    glutCreateWindow("�γ�Խ����̫��ϵ");							//����һ����Ϊ"GLUT Framework(BNU CG Lab1)"�Ĵ��ڣ��������һ�з�������������������һ��֮ǰ�أ�
    GLInit();														//��ʼ��OpenGL�ĺ���
    glutDisplayFunc(GLRender);										//��ʾ�ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLRender������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutReshapeFunc(GLReshape);										//���ڴ�С�����ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLReshape������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutKeyboardFunc(GLKeyDown);
    glutKeyboardUpFunc(GLKeyUp);
    glutIdleFunc(GLPostRedisplay);
    glutMainLoop();													//ֻ�е���������������Ż���ֻ�ͼ����
    return 0;
}
