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
	static Id faceCount;	/*��¼���бߵĸ���*/
	Id faceno;				/*������*/
	Loop *floopIn;			/*ָ�򹹳ɸ�����ڻ�*/
	Loop *floopOut;			/*ָ�򹹳ɸ�����⻷*/
	face();
};

struct loop
{
	static Id loopCount;
	Id loopno;
	vector<HalfEdge *>ledge;/*ָ�򹹳ɻ��İ��*/
	Face *lface;			/*ָ��û���������*/
	loop();
};

struct edge
{
	static Id edgeCount;
	Id edgeno;				/*�ߵ����*/
	HalfEdge *he1;			/*ָ������*/
	HalfEdge *he2;			/*ָ���Ұ��*/
	edge();
};

struct halfedge
{
	Edge *pedge;			/*ָ���ߵĸ���*/
	Vertex *vtx;			/*ָ���ߵ���ʼ����*/
	Loop *wloopIn;			/*ָ�����������ڻ�*/
	Loop *wloopOut;			/*ָ�����������⻷*/
	halfedge();
};

struct vertex
{
	vector<HalfEdge *>vedge;/*ָ���Ըö���Ϊ���İ��*/
	Point pcoord;			/*��������*/
	vertex(Point pc);
};


Point operator+(Point p1,Point p2);
Point operator-(Point p1,Point p2);
Point operator*(float w,Point p);

/*����µ��뼯��*/
Vertex *AddVertex(Vertex *v);


void adj_HaEd_Ver(HalfEdge *he,Vertex *ver);				//�������㽨����ϵ

void adj_Edge_HaEd(Edge *ed,HalfEdge *he1,HalfEdge *he2);	//�������ߺ��Ұ�߽�����ϵ

void adj_Loop_HaEd(Loop *lp,HalfEdge *he,bool type);		//�����߽�����ϵ

void adj_Face_Loop(Face *fa,Loop *lp,bool type);			//���뻷������ϵ

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

Face *newTriangle(Vertex *v1,Vertex *v2,Vertex *v3);		//�������㹹��һ���棬������߽ṹ
#endif
