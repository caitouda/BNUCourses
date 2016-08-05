#include "seqlist.h"
#include <cstdlib>
using namespace std;
int main()
{
	int n = 8;
	SeqList<int> s1, s2;
	for (int i = 0; i <  n; i ++) {		
        int a = rand() % 200;
		s1.Insert(a);
		s2.Insert(i,a);
	}
	s2.Output();
	cout<<"ÓÐÐò";
	s1.Output();
	int s,t;
	cout<<"ÊäÈësºÍt:"<<endl;
	cin>>s>>t;
	s1.Removett(s,t);
	s1.Output();
	return 0;
}
