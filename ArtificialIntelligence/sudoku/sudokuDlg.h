// sudokuDlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"

#include "sudokuA.h"

// CsudokuDlg 對話方塊
class CsudokuDlg : public CDialog
{
// 構造
public:
	CsudokuDlg(CWnd* pParent = NULL);	// 標準構造函數
	~CsudokuDlg();

// 對話方塊數據
	enum { IDD = IDD_SUDOKU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 實現
protected:
	HICON m_hIcon;
	CEdit *m_pEdit;
	int m_ixgrid;
	int m_iygrid;
	wchar_t m_text[9][9];
	bool m_isedit;
	CRect m_rect;
	Csudoku m_core;

	// 生成的消息映射函數
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	afx_msg void OnEnChangeEdit1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButtonReset();
public:
	afx_msg void OnBnClickedButtonAuto();
public:
	afx_msg void OnBnClickedButtonCreate();
public:
	CString m_pro;
};
