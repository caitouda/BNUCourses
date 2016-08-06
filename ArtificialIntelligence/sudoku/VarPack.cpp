#include "stdafx.h"
#include "VarPack.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#ifndef min
#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif//min

#ifndef STR_ASSERT
#define STR_ASSERT(condition,str)  \
	{ \
	if (!(condition)) \
	{ \
		cout << "Error:" << endl; \
		cout << (str) << endl; \
		exit(1); \
	} \
	} 
#endif//STR_ASSERT

#define VARPACK_READ	0x01
#define VARPACK_WRITE	0x02

#define MAX_BUF_LEN 0xffffff

VarPack::VarPack()
{
	m_datalength = 1024;
	m_buf = new char [m_datalength];
	m_nlockbuf = new int (0);
	m_pbegin = 0;
	m_pend = 0;
	m_author = VARPACK_READ | VARPACK_WRITE;
}

VarPack::VarPack(int maxdatalen)
{
	STR_ASSERT(maxdatalen >= 1, "Data length cannot less than 1!");
	STR_ASSERT(maxdatalen <= MAX_BUF_LEN, "Data length too large!");
	m_datalength = maxdatalen;
	m_buf = new char [m_datalength];
	m_nlockbuf = new int (0);
	m_pbegin = 0;
	m_pend = 0;
	m_author = VARPACK_READ | VARPACK_WRITE;
}

VarPack::VarPack(VarPack &cp)
{
	m_datalength = cp.m_datalength;
	m_pbegin = cp.m_pbegin;
	m_pend = cp.m_pend;
	m_buf = cp.m_buf;
	m_nlockbuf = cp.m_nlockbuf;
	(*m_nlockbuf)++;
	m_author = VARPACK_READ;
}

VarPack & VarPack::operator = (VarPack &cp)
{
	if (*m_nlockbuf == 0)
	{
		delete m_nlockbuf;
		delete[] m_buf;
	}
	else (*m_nlockbuf)--;

	m_datalength = cp.m_datalength;
	m_pbegin = cp.m_pbegin;
	m_pend = cp.m_pend;
	m_buf = cp.m_buf;
	m_nlockbuf = cp.m_nlockbuf;
	(*m_nlockbuf)++;
	m_author = VARPACK_READ;
	return *this;
}


VarPack::~VarPack()
{
	if (*m_nlockbuf == 0)
	{
		delete m_nlockbuf;
		delete[] m_buf;
	}
	else (*m_nlockbuf)--;
}

VarPack VarPack::MakeCopy()
{
	VarPack newvp(m_datalength);
	newvp.m_pbegin = m_pbegin;
	newvp.m_pend = m_pend;
	memcpy(newvp.m_buf, (m_buf+m_pbegin), m_pend-m_pbegin);
	return newvp;
}

void VarPack::clear()
{
	m_pbegin = 0;
	m_pend = 0;
}


bool VarPack::good()
{
	return m_buf != NULL;
}

bool VarPack::eof()
{
	return (m_pend >= m_datalength) || (m_pbegin >= m_pend);
}

void VarPack::Read(void *t, int buflen)
{
	STR_ASSERT(m_author & VARPACK_READ, 
		"Cannot access data, this packet cannot be read!");
	STR_ASSERT((m_pend-m_pbegin) >= buflen, 
		"Cannot access data of this type, please check type carefully!");
	memcpy((void *)t, (void *)(m_buf+m_pbegin), buflen);
	m_pbegin += buflen;
}

void VarPack::Write(void *t, int buflen)
{
	STR_ASSERT(m_author & VARPACK_WRITE, 
		"Cannot write data, this packet cannot be writed!");
	STR_ASSERT((m_datalength-m_pend) >= buflen, 
		"Cannot write data , please check the length of the packet!");
	memcpy((void *)(m_buf+m_pend), (void *)t, buflen);
	m_pend += buflen;
}

VarPack & operator << (VarPack & vp, const char *str)
{
	vp.Write((void *)str, (strlen(str)+1));
	return vp;
}

VarPack & operator >> (VarPack & vp, char *str)
{
	char c = ' ';
	while(c != '\0')
	{
		vp >> c;
		*(str++) = c;
	}
	return vp;
}