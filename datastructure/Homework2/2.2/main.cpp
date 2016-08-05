#include "seqlist.h"
#include <cstdlib>
using namespace std;
int main()
{
	int n = 8;
	SeqList<int> s1;
	int mi;
	for (int i = 0; i <  n; i ++) {
        int a = rand() % 200;
		s1.Insert(i, a);
	}
    s1.Output();
	int j;
    cout<<"ÊäÈëi:"<<endl;
	cin>>j;
	s1.Remove(j,mi);
	s1.Output();
	cout<<mi<<endl;
	return 0;
}
