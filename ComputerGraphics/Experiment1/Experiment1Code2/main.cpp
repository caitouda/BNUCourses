/*
 背景是一棵秋天的树，两只蝴蝶在翩翩起舞，几片枯叶飘落而下
*/
#include<iostream>
#include<GL/glut.h>
#include<GL/glaux.h>
#include<math.h>
#define PI 3.1415926
#define D2R (PI/180)
using namespace std;
void GLInit();
void GLReshape(int w,int h);
void GLRender();

#define	checkImageWidth 8
#define	checkImageHeight 8
GLuint texName[2];
float rot=0.0f;
float theta=0.0f,miu=0.0f;
float y=1.0f,y2=3.0f,y3=2.0f,x1=0.0f,x2=1.0f,x3=-1.0f;


AUX_RGBImageRec *rgbTex;
typedef struct													// Create A Structure
{
    GLubyte	*imageData;											// Image Data (Up To 32 Bits)
    GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
    GLuint	width;												// Image Width
    GLuint	height;												// Image Height
    GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;

TextureImage texture1,texture2,texture3;

void GLPostRedisplay()
{
    glutPostRedisplay();
}


int GLInitBMPTexture(TextureImage *texture,char *filename)
{
    AUX_RGBImageRec *bmpRec;
    bmpRec=auxDIBImageLoad(filename);
    glGenTextures(1,&texture->texID);
    glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmpRec->sizeX,bmpRec->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,bmpRec->data);
    free(bmpRec->data);
    return TRUE;
}


int GLInitTGATexture(TextureImage *texture,char *filename)
{
    GLubyte		TGAheader[12]= {0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
    GLubyte		TGAcompare[12];									// Used To Compare TGA Header
    GLubyte		header[6];										// First 6 Useful Bytes From The Header
    GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
    GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
    GLuint		temp;											// Temporary Variable
    GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

    FILE *file = fopen(filename, "rb");							// Open The TGA File

    if(	file==NULL ||											// Does File Even Exist?
            fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
            memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
            fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
    {
        if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
            return FALSE;										// Return False
        else													// Otherwise
        {
            fclose(file);										// If Anything Failed, Close The File
            return FALSE;										// Return False
        }
    }

    texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
    texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

    if(	texture->width	<=0	||									// Is The Width Less Than Or Equal To Zero
            texture->height	<=0	||									// Is The Height Less Than Or Equal To Zero
            (header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
    {
        fclose(file);											// If Anything Failed, Close The File
        return FALSE;											// Return False
    }

    texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
    imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

    texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

    if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
            fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
    {
        if(texture->imageData!=NULL)							// Was Image Data Loaded
            free(texture->imageData);							// If So, Release The Image Data

        fclose(file);											// Close The File
        return FALSE;											// Return False
    }

    for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
    {
        // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
        texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
        texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }

    fclose (file);												// Close The File

    // Build A Texture From The Data
    glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

    glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

    if (texture[0].bpp==24)										// Was The TGA 24 Bits
    {
        type=GL_RGB;											// If So Set The 'type' To GL_RGB
    }

    glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

    return true;
}

void GLInit()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);						// Black Background
    glClearDepth(1.0f);											// Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);										// Type Of Depth Testing
    glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
    glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)
    glAlphaFunc(GL_GREATER,0.1f);								// Set Alpha Testing     (disable blending)
    glEnable(GL_ALPHA_TEST);									// Enable Alpha Testing  (disable blending)
    glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
    glEnable(GL_CULL_FACE);										// Remove Back Face
    GLInitBMPTexture(&texture1,"Butterfly.bmp");
    GLInitBMPTexture(&texture3,"Leave.bmp");
    GLInitBMPTexture(&texture2,"Lab1_2.bmp");
}

void GLReshape(int w,int h)
{
    glViewport(0,0,w,h);											//设定视口的函数。...什么是视口？请大家把第3、4个参数替换成w/2和h/2体会一下，再翻看一下你们的课本就知道了^_^
    glMatrixMode(GL_PROJECTION);								//将当前需要设定的矩阵设定为投影矩阵，这个矩阵是OpenGL内部管理的
    glLoadIdentity();												//将当前矩阵设为单位矩阵，这个函数是必须的，如果去掉呢？为什么会这样？
    gluPerspective(45.0f,float(w)/float(h),0.01f,20.0f);			//对裁减体的定义函数，什么是裁减体...？
    glMatrixMode(GL_MODELVIEW);								//将当前需要设定的矩阵设定为模型观察矩阵，这个矩阵是OpenGL内部管理的
}



void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);				//对参数所指定的缓冲区进行清除，这里清除的是颜色和深度缓冲区，在完成本次作业后，可以去掉或者修改这两个参数中的任意一个，看看显示结果会有什么不同，为什么会这样呢？
    glLoadIdentity();												//将当前矩阵设为单位矩阵，这个函数是必须的，如果去掉呢？为什么会这样？

    gluLookAt(0.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    /*下面要开始绘图了，完成实验要求中的各个要求点，把下面的代码替换，为了实现交互的效果，你可以参考红宝书中1-3程序double.c，还有重点关注一下glRotatef，glScalef，至于只显示边界么... 你需要调用一个设定颜色填充模式的函数glPolygonMode*/

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture2.texID);
    glScalef(2.0f,2.0f,2.0f);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();															//表明绘图结束，glEnd与glBegin必须是成对出现的，两者之间所调用的所有glVertex3f函数，都为glBegin中指定图形的绘制而服务
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,texture1.texID);
    glPushMatrix();
    glTranslatef(sin(theta),sin(miu),1.0f);
    glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();
    glPopMatrix();
    theta+=0.01;
    miu+=0.004;
    rot+=10.0f;


    glBindTexture(GL_TEXTURE_2D,texture1.texID);
    glPushMatrix();
    glTranslatef(cos(theta),cos(miu),1.0f);
    glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();
    glPopMatrix();
    theta+=0.01;
    miu+=0.004;
    rot+=10.0f;

    glBindTexture(GL_TEXTURE_2D,texture3.texID);
    glPushMatrix();
    glTranslatef(x1,y,1.0f);
//	glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();
    glPopMatrix();
    if(y>-1.2f)
    {
        y-=0.0005f;
        x1+=0.0001f;
    }
    if(y<=-1.2f)
    {
        y=-1.2f;
    }

    glBindTexture(GL_TEXTURE_2D,texture3.texID);
    glPushMatrix();
    glTranslatef(x2,y2,1.0f);
//	glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();
    glPopMatrix();
    if(y2>-1.2f)
    {
        y2-=0.0005f;
        x2-=0.0001f;
    }
    if(y2<=-1.2f)
    {
        y2=-1.2f;
    }

    glBindTexture(GL_TEXTURE_2D,texture3.texID);
    glPushMatrix();
    glTranslatef(x3,y3,1.0f);
//	glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//表明绘图开始，参数指定了所要绘制的图形种类，这里指定的是四边形
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();
    glPopMatrix();
    y3-=0.0005f;
    if(y3<=-1.2f)y3=-1.2f;
    /***************************************************************************GLRender中需要你们实现的部分至此结束********************************************************************/

    glFlush();															//强制刷新缓冲区的函数，如果去掉呢？
    glutSwapBuffers();													//交换使用另一个缓冲区，为什么要交换呢？原理在红宝书第一章动画那一节里有阐述
}


int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut的初始化函数，负责处理命令行参数，在这里我们只需记住它是用glut搭建Opengl显示环境必须要第一个调用的函数
    glutInitWindowPosition(100,100);								//glut的窗口初始化函数，设定了绘图窗口左上角在整个屏幕上的坐标（以像素为单位，下同）
    glutInitWindowSize(500,500);									//glut的窗口初始化函数，设定了绘图窗口的大小
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//将窗口显示模式设定为双缓冲、RGBA颜色模式并且带深度缓存，在完成本次作业后，可以去掉或者修改这三个参数中的任意一个，看看显示结果会有什么不同，为什么会这样呢？
    glutCreateWindow("GLUT Framework(BNU CG Lab1_2)");				//创建一个名为"GLUT Framework(BNU CG Lab1)"的窗口，如果把这一行放在上面三个函数任意一个之前呢？
    GLInit();														//初始化OpenGL的函数
    glutDisplayFunc(GLRender);										//显示回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLRender函数，注意这里对回调函数本身的参数和返回值的要求
    glutReshapeFunc(GLReshape);										//窗口大小调整回调函数的设定，在这里我们将回调函数设定为之前已经定义的GLReshape函数，注意这里对回调函数本身的参数和返回值的要求
    glutIdleFunc(GLPostRedisplay);									//老师添加的代码　回调函数   重绘该图像
    glutMainLoop();													//只有调用了这个函数，才会出现绘图窗口
    return 0;
}
