// sudoku.h : PROJECT_NAME 應用程式的主標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此檔之前包含“stdafx.h”以生成 PCH 檔"
#endif

#include "resource.h"		// 主符號


// CsudokuApp:
// 有關此類的實現，請參閱 sudoku.cpp
//

class CsudokuApp : public CWinApp
{
public:
	CsudokuApp();

// 重寫
	public:
	virtual BOOL InitInstance();

// 實現

	DECLARE_MESSAGE_MAP()
};

extern CsudokuApp theApp;
