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

glPoint glPoint::operator +(const glPoint &p)
{
    glPoint np;
    np.x=this->x+p.x;
    np.y=this->y+p.y;
    np.z=this->z+p.z;
    return np;
}

glPoint glPoint::operator -(const glVector &vec)
{
    glPoint p;
    p.x=this->x-vec.i;
    p.y=this->y-vec.j;
    p.z=this->z-vec.k;
    return p;
}

glPoint glPoint::operator -(const glPoint &p)
{
    glPoint np;
    np.x=this->x-p.x;
    np.y=this->y-p.y;
    np.z=this->z-p.z;
    return np;
}

glPoint glPoint::operator *(const double &scale)
{
    glPoint p;
    p.x=this->x*scale;
    p.y=this->y*scale;
    p.z=this->z*scale;
    return p;
}
