#include"pl0.h"

void error_info(int c)
{
	switch (c)
	{
	case 1:
		printf("常数说明中的“=”写成“:=”。\n");
		break;	
	}
}