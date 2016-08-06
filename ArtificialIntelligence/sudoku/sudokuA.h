#ifndef _SUDOKU_A_H_
#define _SUDOKU_A_H_

#include <iostream>
using namespace std;

/*
解數獨和生成數獨的類
版本：1.0，作者：何超越，完成時間：2014.8
用法：
Csudoku():構造函數，生成一個空的題目.
Csudoku(char *filename):從文字檔讀取數獨題目,要求一共9行，每行9個數字(除了回車分行符號)，
每行每個字元代表數獨上的一個網格，如果是空格則這個網格為空。

operator <<:在控制台模式下輸出數獨題目.

SetNode(short x, short y, short value):設置網格的值.
GetNode(short x, short y):得到網格的值

DeSudoku(char *str):計算出題目的解，並保存結果，str存放解的情況，利用GetNode獲得
結果的每一個網格.

CreateRandomSudoku(int randseed):隨機生成可以解的題目，randseed為亂數種子，相同
的randseed得到相同的題目。為了加快生成速度，在某些randseed下有可能會無法生成題目，
如果成功生成則返回1，成功率大約為40%.

IsRelateNode():查看兩個網格是否關聯(是否存在約束)

GetHint(short *hnt, short px, short py):獲得提示，查看某一點可以填哪些數位，結果
存放在hnt中，並返回可以填的數位的個數。
*/


const int sudoku_backnum = 3;

class Csudoku
{
public:
	Csudoku();
	Csudoku(char *filename);
	friend ostream & operator << (ostream & os, Csudoku & sdk);
	void   SetNode(short x, short y, short value){SetNodeAndUpdate(x,y,value);}
	short  GetNode(short x, short y){return m_node[x][y];}
	int    DeSudoku(char *str = NULL);
	int    CreateRandomSudoku(int randseed);
public:
	static bool IsRelateNode(short ax, short ay, short bx, short by)
	{return (ax == bx) || (ay == by) ||
	( ((ax/3) == (bx/3)) && ((ay/3) == (by/3)) );}
	int  GetHint(short *hnt, short px, short py);
protected:
	void Init();
	bool CheckNodeValid();
	bool CheckValueValid();
protected:
	bool SetNodeAndUpdate(short x, short y, short value);
	bool UpdateLimitArray();
	short UpdateRankArray(bool getminrank = true);
	short GetOptNode(short *px, short *py);
	int  DeSudoku(short x, short y, short value, bool order);
protected:
	int  GetRandomNode(short *px, short *py, short *pvalue);
protected:
	void Backup(int id);
	void Recover(int id);
private:
	//存放節點的數位,0為沒有數位
	short  m_node[9][9];
	//存放各個節點的階:
	//某個節點的階是指在目前的狀態下，這個節點可以填入某些值
	//使得整個九宮格數字不直接互斥(即同行同列同方塊不重複)，
	//不必考慮間接互斥(填入某值後數獨無解)。
	short  m_rank[9][9];
	//存放各個節點存在的約束:
	//某個節點的約束是指在目前的狀態下，這個節點相關的節點
	//(同行同列同方塊)已經出現了的值，那麼這個節點就不能在
	//出現，因此存在約束。如果這個節點相關的節點出現了一次5，
	//那麼這個節點在值為5上的約束為1，如果出現了2次，約束為2。
	//約束只要大於1就存在約束，用一個陣列表示這個節點在所有值
	//上出現的約束，就可以很方便的知道這個節點可以填入的數字，
	//以便程式解數獨。每個節點的長度設為10是為了方便對齊，其
	//在0上的約束是毫無意義的。
	short  m_limit[9][9][10];
	//備份資料
	short  m_backup[sudoku_backnum][9][9];
	bool   m_limitupdate;
	int    m_searchtime;
};



#endif//_SUDOKU_A_H_
