#include <gl\glut.h>
#include <iostream>
#include "glPoint.h"
using namespace std;

#define MAXNUM 100
#define BUFSIZE 512

float T[10];
glPoint controlPoint[4];
glPoint pTmp[MAXNUM];
bool isLeftButtonDown=false;
bool isSelected=false;
GLuint selectID=0;
GLint DownX,DownY;
GLint winWidth,winHeight;
void GLIdleFunc()
{
    glutPostRedisplay();
}

void DrawControlPoint(int n,int mode);

void ProcessHits(GLint hits,GLuint buffer[]);

void MouseClick(int button,int state,int x,int y)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    if(button ==GLUT_LEFT_BUTTON)
    {
        if(state==GLUT_DOWN)
        {
            DownX=x;
            DownY=y;
            isLeftButtonDown=true;
            glGetIntegerv(GL_VIEWPORT,viewport);
            glSelectBuffer(BUFSIZE,selectBuf);
            glRenderMode(GL_SELECT);
            glInitNames();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluPickMatrix((double)x,(double)(viewport[3]-y),5.0,5.0,viewport);
            gluOrtho2D(-winWidth/2,winWidth/2,-winHeight/2,winHeight/2);
            DrawControlPoint(4,GL_SELECT);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            glFlush();
            hits=glRenderMode(GL_RENDER);
            ProcessHits(hits,selectBuf);
            glutPostRedisplay();
        }
        if(state==GLUT_UP)
        {
            isSelected=false;
            isLeftButtonDown=false;
        }
    }

}

void MouseMove(int x,int y)
{
    if(isLeftButtonDown&&isSelected)
    {
        controlPoint[selectID].x=controlPoint[selectID].x+x-DownX;
        controlPoint[selectID].y=controlPoint[selectID].y+DownY-y;
        DownX=x;
        DownY=y;
    }
}

void ProcessHits(GLint hits,GLuint buffer[])
{
    int i,j;
    GLuint *ptr=buffer;
    for(i=0; i<hits; i++)
    {
        int cnt=*ptr++;
        ptr++;
        ptr++;
        for(j=0; j<cnt; j++)
        {
            selectID=*ptr;
            isSelected=true;
            ptr++;
        }

    }
}

void BsplineDeBoorCox(glPoint P[], int k, float t, int j, glPoint &V)
{
    int i, r, temp, templ;
    glPoint Q[MAXNUM];
    float lamta;
    temp = j-k+1;
    for(i = 0; i < k; i++)
    {
        Q[i].x = P[temp+i].x;
        Q[i].y = P[temp+i].y;
        Q[i].z = P[temp+i].z;
    }
    for(r = 1; r < k; r++)
        for(i = j; i >= temp+r; i--)
        {
            lamta = (t-T[i])/(T[i+k-r]-T[i]);
            templ = i-temp;
            Q[templ].x = lamta*Q[templ].x+(1.0-lamta)*Q[templ-1].x;
            Q[templ].y = lamta*Q[templ].y+(1.0-lamta)*Q[templ-1].y;
            Q[templ].z = lamta*Q[templ].z+(1.0-lamta)*Q[templ-1].z;
        }
    V.x = Q[k-1].x;
    V.y = Q[k-1].y;
    V.z = Q[k-1].z;
}

void GLReshape(int w,int h)
{
    winWidth=w;
    winHeight=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2,w/2,-h/2,h/2,0.00,10.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLInit()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);


    controlPoint[0].x=-300.0f;
    controlPoint[1].x=-100.0f;
    controlPoint[2].x=100.0f;
    controlPoint[3].x=300.0f;
    controlPoint[0].y=0.0f;
    controlPoint[1].y=200.0f;
    controlPoint[2].y=-200.0f;
    controlPoint[3].y=0.0f;
    controlPoint[0].z=0.0f;
    controlPoint[1].z=0.0f;
    controlPoint[2].z=0.0f;
    controlPoint[3].z=0.0f;
    for(int i = 1; i <= 10; i++)
        T[i-1] = i*1.0/10;
}

void DrawControlPoint(int n,int mode)
{
    int i;
    glColor3f(0.0f,0.2f,0.6f);

    for(i=0; i<n; i++)
    {

        if(isSelected&&selectID==i)
        {
            glColor3f(1.0f,1.0f,0.0f);
        }
        else
            glColor3f(0.0f,0.5f,0.0f);
        if(mode==GL_SELECT)
        {
            glPushName(i);
        }
        glPushMatrix();
        glTranslatef(controlPoint[i].x,controlPoint[i].y,controlPoint[i].z);
        glutSolidSphere(3.0,32,32);
        glPopMatrix();

        if(mode==GL_SELECT)
            glPopName();
    }

    glColor3f(0.5f,0.6f,0.7f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for(i=0; i<n; i++)
    {
        glVertex3f(controlPoint[i].x,controlPoint[i].y,controlPoint[i].z);
    }
    glEnd();
}

void DisplayBspineCurve(glPoint P[],int n,int k,int count)
{
    int i,j;
    float deltat,t;
    glPoint V,newV;
    glColor3f(1.0f, 0.0f, 0.0f);
    deltat = (T[n] - T[k-1])/count;
    t = T[k-1];
    j = k - 1;
    BsplineDeBoorCox(P,k,t,j,V);
    for(i = 1; i <= count; i ++)
    {
        t = t + deltat;
        while(t > T[j+1])
            j++;
        BsplineDeBoorCox(P,k,t,j,newV);
        glBegin(GL_LINES);
        glVertex3f(V.x,V.y,V.z);
        glVertex3f(newV.x,newV.y,newV.z);
        glEnd();
        V.x = newV.x;
        V.y = newV.y;
        V.z = newV.z;
    }
}

void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    DrawControlPoint(4,GL_RENDER);
    DisplayBspineCurve(controlPoint, 4, 3, 100);

    glutSwapBuffers();
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("lab5_1");
    glutDisplayFunc(GLRender);
    glutReshapeFunc(GLReshape);
    glutIdleFunc(GLIdleFunc);
    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMove);
    GLInit();
    glutMainLoop();
    return 0;
}
