#include "seqlist.h"
#include <cstdlib>
using namespace std;
int main()
{
	int n = 8;
	SeqList<int> s1,s2;
	int num=0;
	for (int i = 0; i <  n; i ++) {
        int a = rand() % 200;
		s1.Insert(num++,a);
		s1.Insert(num++,a);
	}
	s1.Output();
	s1.Removethesame(s2);
	s1.Output();
    return 0;
}
