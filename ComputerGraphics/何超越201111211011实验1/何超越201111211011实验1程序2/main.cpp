/*
 ������һ�������������ֻ�������������裬��Ƭ��ҶƮ�����
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
    glViewport(0,0,w,h);											//�趨�ӿڵĺ�����...ʲô���ӿڣ����Ұѵ�3��4�������滻��w/2��h/2���һ�£��ٷ���һ�����ǵĿα���֪����^_^
    glMatrixMode(GL_PROJECTION);								//����ǰ��Ҫ�趨�ľ����趨ΪͶӰ�������������OpenGL�ڲ������
    glLoadIdentity();												//����ǰ������Ϊ��λ������������Ǳ���ģ����ȥ���أ�Ϊʲô��������
    gluPerspective(45.0f,float(w)/float(h),0.01f,20.0f);			//�Բü���Ķ��庯����ʲô�ǲü���...��
    glMatrixMode(GL_MODELVIEW);								//����ǰ��Ҫ�趨�ľ����趨Ϊģ�͹۲�������������OpenGL�ڲ������
}



void GLRender()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);				//�Բ�����ָ���Ļ���������������������������ɫ����Ȼ�����������ɱ�����ҵ�󣬿���ȥ�������޸������������е�����һ����������ʾ�������ʲô��ͬ��Ϊʲô�������أ�
    glLoadIdentity();												//����ǰ������Ϊ��λ������������Ǳ���ģ����ȥ���أ�Ϊʲô��������

    gluLookAt(0.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    /*����Ҫ��ʼ��ͼ�ˣ����ʵ��Ҫ���еĸ���Ҫ��㣬������Ĵ����滻��Ϊ��ʵ�ֽ�����Ч��������Բο��챦����1-3����double.c�������ص��עһ��glRotatef��glScalef������ֻ��ʾ�߽�ô... ����Ҫ����һ���趨��ɫ���ģʽ�ĺ���glPolygonMode*/

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture2.texID);
    glScalef(2.0f,2.0f,2.0f);
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
    glTexCoord2d(0.0,0.0);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,0.0);
    glVertex3f(1.0f,-1.0f,0.0f);
    glTexCoord2d(1.0,1.0);
    glVertex3f(1.0f,1.0f,0.0f);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();															//������ͼ������glEnd��glBegin�����ǳɶԳ��ֵģ�����֮�������õ�����glVertex3f��������ΪglBegin��ָ��ͼ�εĻ��ƶ�����
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,texture1.texID);
    glPushMatrix();
    glTranslatef(sin(theta),sin(miu),1.0f);
    glRotatef(rot,0.0f,0.0f,1.0f);
    glScalef(0.05f,0.1f,0.1f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
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
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
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
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
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
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
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
    glBegin(GL_QUADS);													//������ͼ��ʼ������ָ������Ҫ���Ƶ�ͼ�����࣬����ָ�������ı���
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
    /***************************************************************************GLRender����Ҫ����ʵ�ֵĲ������˽���********************************************************************/

    glFlush();															//ǿ��ˢ�»������ĺ��������ȥ���أ�
    glutSwapBuffers();													//����ʹ����һ����������ΪʲôҪ�����أ�ԭ���ں챦���һ�¶�����һ�����в���
}


int main(int args, char **argv)
{
    glutInit(&args,argv);											//glut�ĳ�ʼ�������������������в���������������ֻ���ס������glut�Opengl��ʾ��������Ҫ��һ�����õĺ���
    glutInitWindowPosition(100,100);								//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ�������Ͻ���������Ļ�ϵ����꣨������Ϊ��λ����ͬ��
    glutInitWindowSize(500,500);									//glut�Ĵ��ڳ�ʼ���������趨�˻�ͼ���ڵĴ�С
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);			//��������ʾģʽ�趨Ϊ˫���塢RGBA��ɫģʽ���Ҵ���Ȼ��棬����ɱ�����ҵ�󣬿���ȥ�������޸������������е�����һ����������ʾ�������ʲô��ͬ��Ϊʲô�������أ�
    glutCreateWindow("GLUT Framework(BNU CG Lab1_2)");				//����һ����Ϊ"GLUT Framework(BNU CG Lab1)"�Ĵ��ڣ��������һ�з�������������������һ��֮ǰ�أ�
    GLInit();														//��ʼ��OpenGL�ĺ���
    glutDisplayFunc(GLRender);										//��ʾ�ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLRender������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutReshapeFunc(GLReshape);										//���ڴ�С�����ص��������趨�����������ǽ��ص������趨Ϊ֮ǰ�Ѿ������GLReshape������ע������Իص���������Ĳ����ͷ���ֵ��Ҫ��
    glutIdleFunc(GLPostRedisplay);									//��ʦ��ӵĴ��롡�ص�����   �ػ��ͼ��
    glutMainLoop();													//ֻ�е���������������Ż���ֻ�ͼ����
    return 0;
}
