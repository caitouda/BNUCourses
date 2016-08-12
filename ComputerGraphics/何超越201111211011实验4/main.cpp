/*
师兄：
    你好。这次实验是在界面的初始状态包括点的初始位置、线条和背景的颜色、图案的大小等方面稍作修改完成的。
/*
					         程序说明

本程序直接在nehe搭建的opengl框架上加入绘图代码，添加的代码在程序中均已标出。

旋转观察物体：方向键，Z，X
再分：S
回到再分前的状态：R

*/


#include "HalfEdge.h"
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
wchar_t Wtitle[]=L"Nehe's OpenGL Framework";

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
/******************************************************************************************************/
/************** 以下为添加的代码【读入图形创建半边结构】 ***************/
GLfloat rotx=0.0f,roty=0.0f,rotz=0.0f;

BOOL sp=FALSE,rp=FALSE;

vector<Vertex *>VertexAdded;		//顶点指针数组：保存指向所有顶点的指针
vector<HalfEdge *>HalfEdgeAdded;	//半边指针数组：保存指向所有半边的指针
vector<Edge *>EdgeAdded;			//边指针数组：保存指向所有边的指针
vector<Loop *>LoopAdded;			//环指针数组：保存指向所有环的指针
vector<Face *>TriangleAdded;		//三角面指针数组：保存指向所有三角面的指针

/* 堆栈：存放细分后的结果 */
vector<vector<Vertex *> > VertexAddedStack;
vector<vector<HalfEdge *> > HalfEdgeAddedStack;
vector<vector<Edge *> >EdgeAddedStack;
vector<vector<Loop *> >LoopAddedStack;
vector<vector<Face *> >TriangleAddedStack;

Vertex *newVer;
HalfEdge *newHE1,*newHE2;
Edge *newEd;
Loop *newLoop1,*newLoop2;
Face *newFace;

bool buildMershes(char *filename)	//读取文件函数
{
	FILE *file;
	if(filename!=NULL)
	{
		file=fopen(filename,"r");
		if(file!=NULL)
		{
			int trianNum;
			fscanf(file,"%d",&trianNum);
			for(int i=0;i<trianNum;i++)
			{
				Vertex *VerIndex[3];
				/*添加点*/
				for(int j=0;j<3;j++)	//读取三个点
				{
					float x,y,z;	
					fscanf(file,"%f,%f,%f",&x,&y,&z);	//读取点的坐标
					newVer = AddVertex(new Vertex(Point(x,y,z))); //创建一个顶点指针，并将指针保存到VertexAdded数组
					VerIndex[j]=newVer;
				}
				newTriangle(VerIndex[0],VerIndex[1],VerIndex[2]);	//读入的三个点为一个面，并创建半边结构（参见newTriangle函数）
			}
		}
	}
	return false;
}

void push_StatusStack() //保存细分结果（将细分后的模型压入堆栈）
{
	TriangleAddedStack.push_back(TriangleAdded);
	LoopAddedStack.push_back(LoopAdded);
	EdgeAddedStack.push_back(EdgeAdded);
	HalfEdgeAddedStack.push_back(HalfEdgeAdded);
	VertexAddedStack.push_back(VertexAdded);
	TriangleAdded.clear();
	LoopAdded.clear();
	EdgeAdded.clear();
	HalfEdgeAdded.clear();
	VertexAdded.clear();
}

void pop_StatusStack() //删除栈顶模型（恢复细分前的模型）
{
	if(!TriangleAddedStack.empty())
	{
		TriangleAdded=TriangleAddedStack.back();
		LoopAdded=LoopAddedStack.back();
		EdgeAdded=EdgeAddedStack.back();
		HalfEdgeAdded=HalfEdgeAddedStack.back();
		VertexAdded=VertexAddedStack.back();
		TriangleAddedStack.pop_back();
		LoopAddedStack.pop_back();
		EdgeAddedStack.pop_back();
		HalfEdgeAddedStack.pop_back();
		VertexAddedStack.pop_back();
	}
	}

void Face_Subdivision(face* f,float w)
{
	Vertex *oldV[3];
	Vertex *newV[3];
		  int j,s;
		  Loop *floopin;
		  floopin=f->floopIn;//找到面的内环
		  HalfEdge *he,*hhe,*nhe,*nhe2,*nhhe,*cnhhe1,*c1,*c2,*c3,*c4;
		  Edge *e;
		  Loop *hhl,*chl,*chl2,*chl3,*chl4;
		  Vertex *va1,*va2,*vb1,*vb2;
		  for(j=0;j<3;j++)
		  {
			  Point a,b,c;
			  he=floopin->ledge[j];//找到内环一条半边
			  e=he->pedge;//找到这条半边对应的边
			  hhe=e->he2;//找到该边右边的半边
			 /* */if(*hhe==*he)
			  {
				  cout<<"hhe and he equale"<<endl;
				  hhe=e->he1;
			  }
			  va1=he->vtx;//找顶点
			  va2=hhe->vtx;
			  a=va1->pcoord+va2->pcoord;//计算a的两个顶点
			  
			  nhe=floopin->ledge[(j+2)%3];//找到下一条半边的下一条边
			  vb1=nhe->vtx;           
			  hhl=hhe->wloopIn;
			  for(s=0;s<3;s++)
			  {
				  if(*(hhl->ledge[s])==*hhe) break;
			  }
			  nhhe=hhl->ledge[(s+2)%3];//找到外环的下一条半边的下一条边
			  cnhhe1=hhl->ledge[(s+1)%3];
			  vb2=nhhe->vtx;
			  b=vb1->pcoord+vb2->pcoord;//计算b的两个顶点
			  
			  nhe2=floopin->ledge[(j+1)%3];//找到下一条半边
			  chl=nhe2->wloopOut;
			  for(s=0;s<3;s++)
			  {
				  if(*(chl->ledge[s])==*nhe2) break;
			  }
			  c1=chl->ledge[(s+2)%3];
			  c=c1->vtx->pcoord;
			  chl2=nhe->wloopOut;
			  for(s=0;s<3;s++)
			  {
				  if(*(chl2->ledge[s])==*nhe) break;
			  }
			  c2=chl2->ledge[(s+2)%3];
			  c=c+c2->vtx->pcoord;
			  chl3=cnhhe1->wloopOut;
			  for(s=0;s<3;s++)
			  {
				  if(*(chl3->ledge[s])==*cnhhe1) break;
			  }
			  c3=chl3->ledge[(s+2)%3];
			  c=c+c3->vtx->pcoord;
			  chl4=nhhe->wloopOut;
			  for(s=0;s<3;s++)
			  {
				  if(*(chl4->ledge[s])==*nhhe) break;
			  }
			  c4=chl4->ledge[(s+2)%3];
			  c=c+c4->vtx->pcoord;
			  Point res=0.5*a+(0.125+2*w)*b-(0.0625+w)*c;
			  if(j==0)  {
				  newV[0]=AddVertex(new Vertex(Point(res)));
				  oldV[0]=AddVertex(new Vertex(Point(va1->pcoord)));
			  }
			  else if(j==1)
			  {
				  newV[1]=AddVertex(new Vertex(Point(res)));
				  
				  oldV[1]=AddVertex(new Vertex(Point(va1->pcoord)));
			  }
			  else if(j==2)
			  {
				  newV[2]=AddVertex(new Vertex(Point(res)));
				  oldV[2]=AddVertex(new Vertex(Point(va1->pcoord)));
			  }
			  else;
		  }
		  newTriangle(newV[0],newV[1],newV[2]);
		  newTriangle(newV[0],oldV[1],newV[1]);
		  newTriangle(newV[0],newV[2],oldV[0]);
		  newTriangle(oldV[2],newV[2],newV[1]);
}/******************************************************************************************************/

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	buildMershes("Triangles.txt");
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-12.0f);
	/******************************************************************************************************/
	/************** 以下为添加的代码【绘制三角片以及描边】 ***************/
	glPushMatrix();
	glRotatef(rotx,1.0f,0.0f,0.0f);
	glRotatef(roty,0.0f,1.0f,0.0f);
	glRotatef(rotz,0.0f,0.0f,1.0f);
	/*画三角片*/
	glColor3f(0.5f,1.0f,1.0f);
	size_t i;
	for(i=0;i<TriangleAdded.size();i++)
	{
		glBegin(GL_TRIANGLES);
		for(size_t j=0;j<TriangleAdded[i]->floopIn->ledge.size();j++)
		{
			Point p=TriangleAdded[i]->floopIn->ledge[j]->vtx->pcoord;
			glVertex3f(p.X,p.Y,p.Z);
		}
		glEnd();
	}
	/*描边*/
	glColor3f(1.0f,1.0f,0.5f);
	for(i=0;i<EdgeAdded.size();i++)
	{
		Point p1=EdgeAdded[i]->he1->vtx->pcoord;
		Point p2=EdgeAdded[i]->he2->vtx->pcoord;
		glBegin(GL_LINES);
			glVertex3f(p1.X,p1.Y,p1.Z);
			glVertex3f(p2.X,p2.Y,p2.Z);
		glEnd();
	}
	/******************************************************************************************************/	
	return TRUE;									
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(wchar_t * title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								(LPCSTR)title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	fullscreen=FALSE;								// Windowed Mode

	// Create Our OpenGL Window
	if (!CreateGLWindow(Wtitle,640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
				/******************************************************************************************************/
				/************** 以下为添加的代码【响应键盘输入】 ***************/
				if(keys[VK_UP])
				{
					rotx--;
					if(rotx<0.0f)
						rotx+=360.0f;
				}
				if(keys[VK_DOWN])
				{
					rotx++;
					if(rotx>360.0f)
						rotx-=360.0f;
				}
				if(keys[VK_LEFT])
				{
					roty++;
					if(roty>360.0f)
						roty-=360.0f;
				}
				if(keys[VK_RIGHT])
				{
					roty--;
					if(roty<0.0f)
						roty+=360.0f;
				}
				if(keys['Z'])
				{
					rotz++;
					if(rotz>360.0f)
						rotz-=360.0f;
				}
				if(keys['X'])
				{
					rotz--;
					if(rotz<0.0f)
						rotz+=360.0f;
				}
				if(keys['S']&&!sp)	//判断成功后进行细分
				{
					push_StatusStack();
					for(int i=0;i<(TriangleAddedStack.back().size());i++)	//遍历所有面片
					{
						Face_Subdivision(TriangleAddedStack.back()[i],0.0625);	//对每个面片进行细分【W=0.0625】
					}
					sp=TRUE; 
				}
				if(!keys['S']&&sp==TRUE)
				{
					sp=FALSE;
				}
				if(keys['R']&&!rp)
				{
					pop_StatusStack();
					rp=TRUE; 
				}
				if(!keys['R']&&rp==TRUE)
				{
					rp=FALSE;
				}
			}
			/******************************************************************************************************/
			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow(Wtitle,640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
