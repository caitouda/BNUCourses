#include "stdafx.h"
#include "sudokuA.h"
#include <fstream>

#pragma warning(disable:4996)

#include <stdlib.h>


const int max_search_time = 1000;
const int max_createnode_time = 250;
const int max_createrandom_time = 10000;
//const int 

//constructor
Csudoku::Csudoku()
{
	Init();
}

//初始化
void Csudoku::Init()
{
	int k = 0;
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
	{
		m_node[i][j] = 0;
		m_rank[i][j] = 0;
		for (k = 0;k < sudoku_backnum;k++)
			m_backup[k][i][j] = 0;
		for (k = 0;k <= 9;k++)
			m_limit[i][j][k] = 0;
	}
	m_searchtime = 0;
	m_limitupdate = true;
}


//從面板中讀取數據
Csudoku::Csudoku(char *filename)
{
	char ch = 0;
	Init();
	fstream file(filename);
	for (int i = 0;i < 9; i++)
	{
		for (int j = 0;j < 9; j++)
		{
			file.read(&ch, 1);
			if (ch == ' ')
				m_node[i][j] = 0;
			else if ('0' <= ch&& ch <= '9')
				m_node[i][j] = ch - '0';
			else
			{
				cout << "file format error!" << endl;
				exit(1);
			}
		}
		file.seekp(2, ios::cur);
	}
	file.close();
	m_limitupdate = false;
}

//output function int Win32 console mode
ostream & operator << (ostream & os, Csudoku & sdk)
{
	char ch = ' ';
	for (int i = 0;i < 9; i++)
	{
		for (int j = 0;j < 9; j++)
		{
			if (sdk.GetNode(i,j) == 0)
				ch = ' ';
			else ch = '0' + sdk.GetNode(i,j);
			os << ch << " ";
		}
		os << endl;
	}
	return os;
}

//check validness of data of all nodes
bool Csudoku::CheckNodeValid()
{
	if (!CheckValueValid())
		return false;
	//if UpdateLimitArray() return false, the nodes can not be valid
	return UpdateLimitArray();
}

//check validness of every single node
bool Csudoku::CheckValueValid()
{
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
	{
		//data out of range
		if(m_node[i][j] > 9 || m_node[i][j] < 0)
			return false;
	}
	return true;
}

//改變某個節點的變數並更新m_limit變數
bool Csudoku::SetNodeAndUpdate(short x, short y, short value)
{
	//先保存這個節點原有值
	short oldvalue = m_node[x][y];
	//如果新值和舊值一樣，沒必要進行變化
	if (oldvalue == value)
		return true;
	//如果這個節點想要改變的值已經被約束，本次修改無法進行
	//操作無效
	if ((m_limit[x][y][value] > 0) && (value > 0))
		return false;
	//修改節點值
	m_node[x][y] = value;
	//改變臨近(相關)節點的約束陣列
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
	{
		//如果相關才能改變
		if (!IsRelateNode(x,y,i,j))
			continue;
		//增加新值上的約束
		m_limit[i][j][value]++;
		//解除舊值上的約束
		m_limit[i][j][oldvalue]--;
	}
	//本身的節點不受新舊值的約束
	m_limit[x][y][oldvalue] = 0;
	m_limit[x][y][value] = 0;
	return true;
}
//更新所有節點的約束陣列
bool Csudoku::UpdateLimitArray()
{
	int i = 0, j = 0, k = 0;
	//初始化
	for (i = 0;i < 9; i++)
	for (j = 0;j < 9; j++)
	for (k = 0;k <= 9;k++)
		m_limit[i][j][k] = 0;
	//開始更新
	for (i = 0;i < 9; i++)
	for (j = 0;j < 9; j++)
	if (m_node[i][j] != 0)
	{
		//必須先把這個節點變成0，再復原才能使得SetNodeAndUpdate()函數有效
		k= m_node[i][j];
		m_node[i][j] = 0;
		if (!SetNodeAndUpdate(i,j,k))
			return false;
	}
	//更新完畢
	m_limitupdate = true;
	return true;
}
//計算所有節點的階
short Csudoku::UpdateRankArray(bool getminrank)
{
	//更新約束陣列
	if (!m_limitupdate)
		UpdateLimitArray();
	//最小階，10代表所有節點都沒有階(數獨已經完成)
	short minrank = 10;
	//最高階
	short maxrank = 0;
	//通過約束陣列計算每個節點的階
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
	{
		m_rank[i][j] = 0;
		//如果這個節點已經解決，那麼沒有階(為0)
		if (m_node[i][j] != 0)
			continue;
		//如果節點在某個值上沒有約束，那麼階加1
		for (int k = 1;k <= 9;k++)
			m_rank[i][j] += (m_limit[i][j][k] == 0);
		//獲得最小階
		minrank = min<short>(minrank, m_rank[i][j]);
		maxrank = max<short>(maxrank, m_rank[i][j]);
	}
	return getminrank ? minrank : maxrank;
}

//獲得最佳入手點
//最佳入手點的演算法:
//為了方便程式進行搜索演算法解決數獨，需要有一個最佳的節點，以便
//在這個點上進行搜索和試探能夠比較快速的找到結果。
//最佳的節點需要滿足以下兩個條件:
//1.階最小，方便搜索
//2.在這個節點上猜測數字能夠儘量多的帶來相關節點的階的改變(方便
//快速得出結果或者引起矛盾)
//針對上面兩點可以對所有的點進行量化的評分，評分最高者即為最佳點
short Csudoku::GetOptNode(short *px, short *py)
{
	//不同階的權重
	const int RankSign[] = {3,6,12,1,0,0,0,0,0,0,0,0};
	//獲得最小階
	short minrank = UpdateRankArray();
	if (minrank > 9 || minrank < 1)
		return minrank;
	//每個節點的評分，評分最高者即為最佳點
	int nodescore = 0;
	int highscore = -1;
	//逐點分析
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
	//只有階最小的點才可能最佳點
	if (m_rank[i][j] == minrank)
	{
		nodescore = 0;
		//計算此節點的評分，如果這個節點臨近(相關)的節點
		//階越低(帶來的影響越大)，那麼它的評分就越高
		for (int i2 = 0;i2 < 9; i2++)
		for (int j2 = 0;j2 < 9; j2++)
		if(IsRelateNode(i,j,i2,j2))	
			//不同的階有不同的評分
			nodescore += RankSign[m_rank[i2][j2]];
		if (nodescore > highscore)
		{
			highscore = nodescore;
			*px = i;
			*py = j;
		}
	}
	return minrank;
}

//解數獨核心演算法:
//採用不斷試探的方法(深度搜索):
//通過GetOptNode函數獲取最佳入手點，在最佳入手點上進行深度搜索。
//整個函數通過遞迴呼叫來不斷的實現深度搜索演算法。
//如果成功解出所有節點那麼返回1,如果引起矛盾,則返回-1。
//如果返回-1，那麼把這一次試探的改變還原，並返回上一級調用。
//下一級函數如果向上一級返回1，那麼試探成功，並再向上一級返回。試探的
//結果自動保存在類變數m_node陣列中；如果返回-1，代表這次試探的失敗
//在下一級函數中已經將改變還原，那麼這一級函數繼續試探，如果每次試探
//都返回的不是1，那麼這一級函數的試探全部失敗，恢復這一級的改變，向再
//上一級返回-1。
int Csudoku::DeSudoku(char *str)
{
	m_searchtime = 0;
	int state = 0;
	int i = 0,j = 0;

	Backup(0);
	if (CheckNodeValid())
		state = DeSudoku(0, 0, 0, true);
	else state = -1;
	
	if (state == 1)
	{
		Backup(1);

		Recover(0);

		DeSudoku(0, 0, 0, false);

		for (i = 0;i < 9; i++)
		for (j = 0;j < 9; j++)
		if (m_backup[1][i][j] != m_node[i][j])
			state = -3;
	}
	if (str == NULL)
		return state;
	switch (state)
	{
	case 1:sprintf(str, "成功,反覆運算次數為%d", m_searchtime/2);break;
	case -2:strcpy(str, "資料過少,無法解出!");break;
	case -1:strcpy(str, "資料矛盾,無解!");break;
	case -3:strcpy(str, "解不唯一!");break;
	default:break;
	}
	return state;
}

int Csudoku::DeSudoku(short x, short y, short value, bool order)
{
	short oldvalue = m_node[x][y];
	m_searchtime++;
	if (m_searchtime >= max_search_time)
		return -2;
	if (value != 0)
		SetNodeAndUpdate(x, y, value);

	short optx = 0;
	short opty = 0;
	short rank = GetOptNode(&optx, &opty);

	int state = -2;

	int vstart = order ? 1 : 9;
	int vend   = order ? 10 : 0;
	int vadd   = order ? 1 : -1;

	if (rank < 1)
		state = -1;
	else if (rank > 9)
		state = 1;
	else if (rank > 3)
		state = -2;
	else for (int i = vstart;i != vend;i+=vadd)
	{
		int temp = 0;
		if (m_limit[optx][opty][i] == 0)
		if ((temp=DeSudoku(optx, opty, i, order)) == 1)
		{
			state = 1;
			break;
		}
		else if (temp == -1)
			state = -1;
	}

	if (state != 1)
		SetNodeAndUpdate(x, y, oldvalue);
	return state;
}

//生成亂數種子
int Csudoku::GetRandomNode(short *px, short *py, short *pvalue)
{
	const int rankpb[] = {0,0,8,6,4,5,6,12,16,20};
	short valueArray[9];
	int   arrayLen = 0;
	int   itertime = 0;
	int   randpb = 0;
	int   i = 0;
	short maxrank = UpdateRankArray(false);
	if (maxrank <= 3)
		return -1;

	while(itertime++ <= max_createnode_time)
	{
		*px = rand() % 9;
		*py = rand() % 9;
		randpb = rand() % (rankpb[maxrank] - 1);
		if (rankpb[m_rank[*px][*py]] <= randpb)
			continue;

		for (i = 1;i <= 9;i++)
		if (m_limit[*px][*py][i] == 0)
			valueArray[arrayLen++] = i;

		*pvalue = valueArray[rand() % arrayLen];
		return 1;
	}
	return -1;
}

//生成初始狀態
int Csudoku::CreateRandomSudoku(int randseed)
{
	short addx = 0;
	short addy = 0;
	short addvalue = 0;
	short rank = 0;
	int   totaltime = 0;
	int   count = 0;
	srand(randseed);

	//step 1
	Init();
	//step2
	while(1)
	{
		count++;
		m_searchtime = 0;
		if (GetRandomNode(&addx, &addy, &addvalue) == -1)
			return -2;
		SetNodeAndUpdate(addx, addy, addvalue);
		Backup(2);
		int re = DeSudoku(0,0,0,true);
		Recover(2);
		if (re == 1)
			break;
		else if (re == -1)
		{
			count--;
			SetNodeAndUpdate(addx, addy, 0);
		}
		
		totaltime += m_searchtime;
		if (totaltime >= 5000)
			return -1;
	}
	if (UpdateRankArray() < 2)
		return -1;
	//step3
	while(1)
	{
		GetRandomNode(&addx, &addy, &addvalue);
		SetNodeAndUpdate(addx, addy, addvalue);
		rank = UpdateRankArray();
		if (rank < 2)
		{
			SetNodeAndUpdate(addx, addy, 0);
			continue;
		}
		count++;
		Backup(2);
		int re = DeSudoku(NULL);
		Recover(2);
		if (re == 1)
			break;
		else if (re == -1)
		{
			SetNodeAndUpdate(addx, addy, 0);
			count--;
		}
		totaltime += m_searchtime;
		if (totaltime >= 10000 || count > 31)
			return -1;
	}
	return totaltime;
}

void Csudoku::Backup(int id)
{
	if (id >= sudoku_backnum || id < 0)
		return;
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
		m_backup[id][i][j] = m_node[i][j];
}

void Csudoku::Recover(int id)
{
	if (id >= sudoku_backnum || id < 0)
		return;
	for (int i = 0;i < 9; i++)
	for (int j = 0;j < 9; j++)
		m_node[i][j] = m_backup[id][i][j];
	UpdateLimitArray();
}

int Csudoku::GetHint(short *hnt, short px, short py)
{
	if (hnt == NULL)
		return 0;
	int arrayLen = 0;
	for (int i = 1;i <= 9;i++)
	if (m_limit[px][py][i] == 0)
		hnt[arrayLen++] = i;
	return arrayLen;
}
