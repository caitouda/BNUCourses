/*
  键盘的"t/T"可以使烟花更加绚烂，再点击会恢复原样
  键盘的上下左右可以控制视点的位置
*/
#include <windows.h>
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

bool	keys[256];
bool	active=TRUE;
bool	fullscreen=true;

bool	twinkle;
bool	tp;

const	num=50;

typedef struct				// 创建一个star结构
{
    int r, g, b;			// Stars 颜色
    GLfloat dist,			// Stars 与中心的距离
            angle;			// Stars 现在的角度
}
stars;
stars star[num];

GLfloat	zoom=-15.0f;
GLfloat tilt=90.0f;
GLfloat	spin;

GLuint	loop;
GLuint	texture[1];

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

AUX_RGBImageRec *LoadBMP(char *Filename)                // 加载位图图像
{
    FILE *File=NULL;

    if (!Filename)
    {
        return NULL;
    }

    File=fopen(Filename,"r");

    if (File)
    {
        fclose(File);
        return auxDIBImageLoad(Filename);
    }
    return NULL;
}

int LoadGLTextures()                                    // 加载位图和转换为纹理
{
    int Status=FALSE;

    AUX_RGBImageRec *TextureImage[1];

    memset(TextureImage,0,sizeof(void *)*1);

    if (TextureImage[0]=LoadBMP("Data/Star.bmp"))
    {
        Status=TRUE;

        glGenTextures(1, &texture[0]);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }
    if (TextureImage[0])
    {
        if (TextureImage[0]->data)
        {
            free(TextureImage[0]->data);
        }

        free(TextureImage[0]);
    }

    return Status;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// 调整和初始化GL窗口
{
    if (height==0)
    {
        height=1;
    }

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int InitGL(GLvoid)
{
    if (!LoadGLTextures())
    {
        return FALSE;
    }

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_BLEND);

    for (loop=0; loop<num; loop++)
    {
        star[loop].angle=0.0f;
        star[loop].dist=(float(loop)/num)*5.0f;
        star[loop].r=rand()%256;
        star[loop].g=rand()%256;
        star[loop].b=rand()%256;
    }
    return TRUE;
}

int DrawGLScene(GLvoid)									// 这里所有的绘制
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for (loop=0; loop<num; loop++)
    {
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,zoom);
        glRotatef(tilt,1.0f,0.0f,0.0f);
        glRotatef(star[loop].angle,0.0f,1.0f,0.0f);
        glTranslatef(star[loop].dist,0.0f,0.0f);
        glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);
        glRotatef(-tilt,1.0f,0.0f,0.0f);

        if (twinkle)
        {
            glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( 1.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( 1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 0.0f);
            glEnd();
        }

        glRotatef(spin,0.0f,0.0f,1.0f);
        glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();

        spin+=0.01f;
        star[loop].angle+=float(loop)/num;
        star[loop].dist-=0.01f;
        if (star[loop].dist<0.0f)
        {
            star[loop].dist+=5.0f;
            star[loop].r=rand()%256;
            star[loop].g=rand()%256;
            star[loop].b=rand()%256;
        }
    }
    return TRUE;
}

GLvoid KillGLWindow(GLvoid)
{
    if (fullscreen)
    {
        ChangeDisplaySettings(NULL,0);
        ShowCursor(TRUE);
    }

    if (hRC)
    {
        if (!wglMakeCurrent(NULL,NULL))
        {
            MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC))
        {
            MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        hRC=NULL;
    }

    if (hDC && !ReleaseDC(hWnd,hDC))
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL;
    }

    if (hWnd && !DestroyWindow(hWnd))
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;
    }

    if (!UnregisterClass("OpenGL",hInstance))
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;
    }
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint		PixelFormat;
    WNDCLASS	wc;
    DWORD		dwExStyle;
    DWORD		dwStyle;
    RECT		WindowRect;
    WindowRect.left=(long)0;
    WindowRect.right=(long)width;
    WindowRect.top=(long)0;
    WindowRect.bottom=(long)height;

    fullscreen=fullscreenflag;

    hInstance			= GetModuleHandle(NULL);
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= NULL;
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= "OpenGL";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (fullscreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth	= width;
        dmScreenSettings.dmPelsHeight	= height;
        dmScreenSettings.dmBitsPerPel	= bits;
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                fullscreen=FALSE;
            }
            else
            {
                MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                return FALSE;
            }
        }
    }

    if (fullscreen)
    {
        dwExStyle=WS_EX_APPWINDOW;
        dwStyle=WS_POPUP;
        ShowCursor(FALSE);
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle=WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

    // Create The Window
    if (!(hWnd=CreateWindowEx(	dwExStyle,
                                "OpenGL",
                                title,
                                dwStyle |
                                WS_CLIPSIBLINGS |
                                WS_CLIPCHILDREN,
                                0, 0,
                                WindowRect.right-WindowRect.left,
                                WindowRect.bottom-WindowRect.top,
                                NULL,
                                NULL,
                                hInstance,
                                NULL)))
    {
        KillGLWindow();
        MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    static	PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        bits,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    if (!(hDC=GetDC(hWnd)))
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!(hRC=wglCreateContext(hDC)))
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!wglMakeCurrent(hDC,hRC))
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    ReSizeGLScene(width, height);

    if (!InitGL())
    {
        KillGLWindow();
        MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;
}

LRESULT CALLBACK WndProc(	HWND	hWnd,
                            UINT	uMsg,
                            WPARAM	wParam,
                            LPARAM	lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:
    {
        if (!HIWORD(wParam))
        {
            active=TRUE;
        }
        else
        {
            active=FALSE;
        }

        return 0;
    }

    case WM_SYSCOMMAND:
    {
        switch (wParam)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }
        break;
    }

    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_KEYDOWN:
    {
        keys[wParam] = TRUE;
        return 0;
    }

    case WM_KEYUP:
    {
        keys[wParam] = FALSE;
        return 0;
    }

    case WM_SIZE:
    {
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        return 0;
    }
    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,
                    HINSTANCE	hPrevInstance,
                    LPSTR		lpCmdLine,
                    int			nCmdShow)
{
    MSG		msg;
    BOOL	done=FALSE;

    if (!CreateGLWindow("NeHe's Animated Blended Textures Tutorial",640,480,16,0))
    {
        return 0;
    }

    while(!done)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message==WM_QUIT)
            {
                done=TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE])
            {
                done=TRUE;
            }
            else
            {
                SwapBuffers(hDC);
                if (keys['T'] && !tp)
                {
                    tp=TRUE;
                    twinkle=!twinkle;
                }
                if (!keys['T'])
                {
                    tp=FALSE;
                }

                if (keys[VK_UP])
                {
                    tilt-=0.5f;
                }

                if (keys[VK_DOWN])
                {
                    tilt+=0.5f;
                }

                if (keys[VK_PRIOR])
                {
                    zoom-=0.2f;
                }

                if (keys[VK_NEXT])
                {
                    zoom+=0.2f;
                }
            }
        }
    }

    KillGLWindow();
    return (msg.wParam);
}
