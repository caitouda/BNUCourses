#include "pl0.h"

void error_info(int c)
{
	switch (c)
	{
	case 1:
		printf("常数说明中的“=”写成“:=”。\n");
		break;
	case 2:
		printf("常量说明=后应是数字。\n");
		break;
    case 3:
		printf("常量说明标识后应是=。\n");
		break;
	case 4:
		printf("const后应是标识。\n");
		break;
	case 5:
		printf("漏掉了分号\n");
		break;
	case 10:
		printf("缺少分号\n");
		break;
	case 11:
		printf("标识符未声明/过程未找到\n");
		break;
	case 12:
		printf("符号表类型不可用。\n");
		break;
	case 13:
		printf("sym!=becomes。\n");
		break;
	case 14:
		printf("call后应为标识符。\n");
		break;
	case 15:
		printf("call后标识符应为过程。\n");
		break;
    case 16:
		printf("缺少then。\n");
		break;
	case 17:
		printf("缺少end或分号。\n");
		break;
	case 18:
		printf("缺少do。\n");
		break;
	case 21:
		printf("不能为过程。\n");
		break;
	case 22:
		printf("缺少右括号。\n");
		break;
	case 23:
		printf("因子后有非法符号。\n");
		break;
	case 30:
		printf("k>nmax。\n");
		break;
	case 31:
		printf("num>amax。\n");
		break;
	case 32:
		printf("lev > levmax。\n");
		break;
	case 33:
		printf("格式错误，应是右括号。\n");
		break;
	case 34:
		printf("sym!=lparen。\n");
		break;
	}
}