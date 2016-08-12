// glPoint.cpp: implementation of the glPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "glPoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

glPoint::glPoint()
{
    x = y = z = 0.0f;
}

glPoint::~glPoint()
{

}

glPoint glPoint::operator +(const glVector &vec)
{
    glPoint p;
    p.x=this->x+vec.i;
    p.y=this->y+vec.j;
    p.z=this->z+vec.k;
    return p;
}

glPoint glPoint::operator -(const glVector &vec)
{
    glPoint p;
    p.x=this->x-vec.i;
    p.y=this->y-vec.j;
    p.z=this->z-vec.k;
    return p;
}
