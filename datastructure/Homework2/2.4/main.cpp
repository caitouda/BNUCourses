#include "seqlist.h"
#include <cstdlib>
using namespace std;
int main()
{
	int n = 8;
	SeqList<int> s1;
	for (int i = 0; i <  n; i ++) {		
        int a = rand() % 200;
		s1.Insert(i, a);
	}
	int a=23;
	s1.Insert(2, a);
	s1.Output();
	int x;
	cout<<"ÊäÈë¶¨Öµx:"<<endl;
	cin>>x;
	s1.Removex(x);
	s1.Output();
    return 0;
}
