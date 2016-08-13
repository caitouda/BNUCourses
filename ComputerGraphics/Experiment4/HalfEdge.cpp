#include"HalfEdge.h"



Id face::faceCount=0;
Id edge::edgeCount=0;
Id loop::loopCount=0;



face::face()
{
	faceno=faceCount++;
}

loop::loop()
{
	loopno=loopCount++;
}

edge::edge()
{
	edgeno=edgeCount++;
}

halfedge::halfedge()
{

}

vertex::vertex(Point pc)
{
	pcoord.X=pc.X;
	pcoord.Y=pc.Y;
	pcoord.Z=pc.Z;
}

point &point::operator =(point p)
{
	this->X = p.X;
	this->Y = p.Y;
	this->Z = p.Z;
	return *this;
}


point operator+(point p1,point p2)
{
	return Point(p1.X+p2.X,p1.Y+p2.Y,p1.Z+p2.Z);
}

point operator-(point p1,point p2)
{
	return Point(p1.X-p2.X,p1.Y-p2.Y,p1.Z-p2.Z);
}

point operator*(float w,point p)
{
	return Point(w * p.X,w * p.Y,w * p.Z);
}

bool operator==(vertex v1,vertex v2)
{
	if(v1.pcoord.X==v2.pcoord.X)
	{
		if(v1.pcoord.Y==v2.pcoord.Y)
		{
			if(v1.pcoord.Z==v2.pcoord.Z)
				return 1;
		}
	}
	return 0;
}

bool operator==(Edge e1,Edge e2)
{
	if(e1.he1->vtx==e2.he1->vtx)
	{
		if(e1.he2->vtx==e2.he2->vtx)
			return 1;
	}
	else if(e1.he2->vtx==e2.he1->vtx)
	{
		if(e1.he1->vtx==e2.he2->vtx)
			return 1;
	}
	return 0;
}

bool operator==(const HalfEdge he1,const HalfEdge he2)
{
	if(he1.vtx==he2.vtx)
	{
		return 1;
	}
	return 0;
}

bool operator==(const Loop l1,const Loop l2)
{
	if(l1.loopno==l2.loopno)
		return 1;
	return 0;
}

bool operator==(const Face f1,const Face f2)
{
	if(f1.faceno==f2.faceno)
		return 1;
	return 0;
}

Vertex *AddVertex(Vertex *v)
{
	int index=findVertex(*v);
	/*若点已添加*/
	if(-1!=index)
	{
		v=VertexAdded[index];
	}
	/*若点未添加*/
	else
	{
		VertexAdded.push_back(v);
	}
	return v;
}

void adj_HaEd_Ver(HalfEdge *he,Vertex *ver)
{
	he->vtx=ver;
	ver->vedge.push_back(he);
}

void adj_Edge_HaEd(Edge *ed,HalfEdge *he1,HalfEdge *he2)
{
	ed->he1=he1;
	ed->he2=he2;
	he1->pedge=ed;
	he2->pedge=ed;
}

void adj_Loop_HaEd(Loop *lp,HalfEdge *he,bool type)
{
	lp->ledge.push_back(he);
	if(!type)
	{
		he->wloopIn=lp;
	}
	else
	{
		he->wloopOut=lp;
	}
}

void adj_Face_Loop(Face *fa,Loop *lp,bool type)
{
	lp->lface=fa;
	if(!type)
	{
		fa->floopIn=lp;
	}
	else
	{
		fa->floopOut=lp;
	}
}

int findVertex(Vertex Verval)
{
	int i;
	for(i=0;i<VertexAdded.size();i++)
	{
		if(*VertexAdded[i]==Verval)
			return i;
	}
	return -1;
}

int findEdge(Edge Edval)
{
	int i;
	for(i=0;i<EdgeAdded.size();i++)
	{
		if(*EdgeAdded[i]==Edval)
			return i;
	}
	return -1;
}


Face *newTriangle(Vertex *v1,Vertex *v2,Vertex *v3)
{
	Vertex *VerIndex[3]={v1,v2,v3};
	HalfEdge *HeInIndex[3],*HeOutIndex[3];/*内、外环半边*/
	Loop *LoopIn,*LoopOut;				/*内、外环*/
	int j;
	for(j=0;j<3;j++)
	{
		newHE1=new HalfEdge;
		newHE2=new HalfEdge;
		newEd=new Edge;
		newHE1->vtx=VerIndex[j];
		newHE2->vtx=VerIndex[(j+1)%3];
		adj_Edge_HaEd(newEd,newHE1,newHE2);
		int index=findEdge(*newEd);
		/*若边已添加*/
		if(-1!=index)
		{
			HeInIndex[j]=EdgeAdded[index]->he2;
			HeOutIndex[j]=EdgeAdded[index]->he1;
		}
		/*若边未添加*/
		else
		{
			adj_HaEd_Ver(newHE1,VerIndex[j]);
			adj_HaEd_Ver(newHE2,VerIndex[(j+1)%3]);
			HalfEdgeAdded.push_back(newHE1);
			HalfEdgeAdded.push_back(newHE2);
			EdgeAdded.push_back(newEd);
			HeInIndex[j]=newHE1;
			HeOutIndex[j]=newHE2;
		}
	}
	newLoop1=new Loop;
	newLoop2=new Loop;
	for(j=0;j<3;j++)
	{
		adj_Loop_HaEd(newLoop1,HeInIndex[j],0);
		adj_Loop_HaEd(newLoop2,HeOutIndex[2-j],1);
	}
	LoopAdded.push_back(newLoop1);
	LoopAdded.push_back(newLoop2);
	newFace = new Face;
	adj_Face_Loop(newFace,newLoop1,0);
	adj_Face_Loop(newFace,newLoop2,1);
	TriangleAdded.push_back(newFace);
	return newFace;
}