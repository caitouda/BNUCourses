﻿#include "seqlist.h"
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
    s1.Output();
    int j,x;
	cout<<"怀i睿x:"<<endl;
	cin>>j>>x;
    s1.Insert(j,x);
	s1.Output();
	return 0;
}
