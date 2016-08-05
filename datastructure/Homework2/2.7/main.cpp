#include "seqlist.h"
#include <cstdlib>
using namespace std;


int main()
{
	int n = 8;
	SeqList<int> s1, s2, s3;
	
	for (int i = 0; i <  n; i ++) {		
        int a = rand() % 200;
		s1.Insert(a);
	}
	for (i = 0; i <  n; i ++) {		
        int a = rand() % 200;
		s2.Insert(a);
	}
	s1.Output();
	s2.Output();
	s1.Uniontwo(s2,s3);
	s3.Output();
	return 0;
}
