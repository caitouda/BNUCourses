#include<windows.h>
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<string>
#include<ctime>
#include<algorithm>
#include<functional>
#include<cstdlib>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//宏定义
#define STANDARD 1
#define MOUSE 2

//gluNurbsSurface函数的各参数的定义
const GLint sorder=4;
const GLint torder=4;
const GLint s_count=4;
const GLint t_count=4;
const GLint sknot_count=8;
const GLint tknot_count=8;
const int max_count=16;
int cnt=0;
GLfloat ctlpoints[s_count][t_count][3];
int height=600;
int width=800;

GLUnurbsObj *theNurb;

void init_surface(void)
{
    for(int i=0; i<s_count; i++)
    {
        for(int j=0; j<t_count; j++)
        {
            ctlpoints[i][j][0]=250;
            ctlpoints[i][j][1]=250;
            ctlpoints[i][j][2]=0;
        }
    }
}

void init(void)
{
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glClearColor (0.2, 0.4, 0.8, 0.0);             
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    init_surface();

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void drawscreen()
{
    int i, j;
    float knots[100];
    for(i=0; i<2*s_count; i++)
    {
        knots[i]=i*0.1;
    }
    glPushMatrix();
    glScalef (0.01, 0.01, 0.01);
    glTranslatef(-250.0f,-250.0f,0.0f);

    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, sknot_count, knots, tknot_count, knots,  t_count * 3, 3, &ctlpoints[0][0][0], sorder, torder, GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);

    glPointSize(5.0);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for (i = 0; i < s_count; i++)
    {
        for (j = 0; j < t_count; j++)
        {
            glVertex3f(ctlpoints[i][j][0],ctlpoints[i][j][1], ctlpoints[i][j][2]);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawscreen();

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
}

void MouseClick(int button,int state,int x,int y)//鼠标点击
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int i=cnt/s_count,j=cnt%s_count;

        ctlpoints[i][j][0]=x;
        ctlpoints[i][j][1]=height-y;
        ctlpoints[i][j][2]=rand()%100;

        cnt++;
        cnt%=max_count;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (150, 100);
    glutCreateWindow("");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(MouseClick);
    glutMainLoop();
    return 0;
}
