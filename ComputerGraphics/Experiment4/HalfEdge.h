#ifndef _HALFEDGE
#define _HALFEDGE

#include<vector>
#include<list>
#include<map>
#include<utility>
#include<algorithm>
using namespace std;
#define NULL 0
typedef short Id;
typedef struct face Face;
typedef struct loop Loop;
typedef struct edge Edge;
typedef struct halfedge HalfEdge;
typedef struct vertex Vertex;
typedef struct point Point;


struct point
{
	float X,Y,Z;
	point(){X=0,Y=0,Z=0;};
	point(float x,float y,float z):X(x),Y(y),Z(z){};
	point &operator=(point p);

};

struct face
{
	static Id faceCount;	/*记录已有边的个数*/
	Id faceno;				/*面的序号*/
	Loop *floopIn;			/*指向构成该面的内环*/
	Loop *floopOut;			/*指向构成该面的外环*/
	face();
};

struct loop
{
	static Id loopCount;
	Id loopno;
	vector<HalfEdge *>ledge;/*指向构成环的半边*/
	Face *lface;			/*指向该环所属的面*/
	loop();
};

struct edge
{
	static Id edgeCount;
	Id edgeno;				/*边的序号*/
	HalfEdge *he1;			/*指向左半边*/
	HalfEdge *he2;			/*指向右半边*/
	edge();
};

struct halfedge
{
	Edge *pedge;			/*指向半边的父边*/
	Vertex *vtx;			/*指向半边的起始顶点*/
	Loop *wloopIn;			/*指向半边所属的内环*/
	Loop *wloopOut;			/*指向半边所属的外环*/
	halfedge();
};

struct vertex
{
	vector<HalfEdge *>vedge;/*指向以该顶点为起点的半边*/
	Point pcoord;			/*顶点坐标*/
	vertex(Point pc);
};


Point operator+(Point p1,Point p2);
Point operator-(Point p1,Point p2);
Point operator*(float w,Point p);

/*添加新点入集合*/
Vertex *AddVertex(Vertex *v);


void adj_HaEd_Ver(HalfEdge *he,Vertex *ver);				//半边与起点建立关系

void adj_Edge_HaEd(Edge *ed,HalfEdge *he1,HalfEdge *he2);	//边与左半边和右半边建立关系

void adj_Loop_HaEd(Loop *lp,HalfEdge *he,bool type);		//环与半边建立关系

void adj_Face_Loop(Face *fa,Loop *lp,bool type);			//面与环建立关系

bool operator==(vertex v1,vertex v2);

bool operator==(Edge e1,Edge e2);

bool operator==(const HalfEdge he1,const HalfEdge he2);

bool operator==(const Loop l1,const Loop l2);

bool operator==(const Face f1,const Face f2);

extern vector<Vertex *>VertexAdded;
extern vector<HalfEdge *>HalfEdgeAdded;
extern vector<Edge *>EdgeAdded;
extern vector<Loop *>LoopAdded;
extern vector<Face *>TriangleAdded;

extern Vertex *newVer;
extern HalfEdge *newHE1,*newHE2;
extern Edge *newEd;
extern Loop *newLoop1,*newLoop2;
extern Face *newFace;

int findVertex(Vertex Verval);

int findEdge(Edge Edval);

Face *newTriangle(Vertex *v1,Vertex *v2,Vertex *v3);		//三个顶点构成一个面，建立半边结构
#endif
