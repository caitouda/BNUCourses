// glVector.cpp: implementation of the glVector class.
//
//////////////////////////////////////////////////////////////////////

#include "glVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

glVector::glVector(GLfloat i,GLfloat j,GLfloat k)
{
    this->i = i;
    this->j = j;
    this->k = k;
}

glVector::~glVector()
{

}

void glVector::operator *=(GLfloat scalar)
{
    i *= scalar;
    j *= scalar;
    k *= scalar;
}

glVector glVector::operator +(const glVector &glVec)
{
    glVector res;
    res.i = this->i+glVec.i;
    res.j = this->j+glVec.j;
    res.k = this->k+glVec.k;
    return res;
}

glVector glVector::operator -(const glVector &glVec)
{
    glVector res;
    res.i = this->i-glVec.i;
    res.j = this->j-glVec.j;
    res.k = this->k-glVec.k;
    return res;
}

glVector glVector::operator *(const GLfloat &dig)
{
    glVector res;
    res.i = this->i*dig;
    res.j = this->j*dig;
    res.k = this->k*dig;
    return res;
}

glVector glVector::operator /(const GLfloat &dig)
{
    glVector res;
    res.i = this->i/dig;
    res.j = this->j/dig;
    res.k = this->k/dig;
    return res;
}
