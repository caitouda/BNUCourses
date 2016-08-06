// sudokuDlg.cpp : 實現檔
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokuDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用於應用程式“關於”功能表項目的 CAboutDlg 對話方塊

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊數據
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 實現
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CsudokuDlg 對話方塊




CsudokuDlg::CsudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsudokuDlg::IDD, pParent),m_rect(30,30,336,336)
	, m_pro(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CsudokuDlg::~CsudokuDlg()
{
	delete m_pEdit;
}

void CsudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, *m_pEdit);
	DDX_Text(pDX, IDC_EDIT_PRO, m_pro);
}

BEGIN_MESSAGE_MAP(CsudokuDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT1, &CsudokuDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CsudokuDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CsudokuDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CsudokuDlg::OnBnClickedButtonCreate)
END_MESSAGE_MAP()


// CsudokuDlg 消息處理常式

BOOL CsudokuDlg::OnInitDialog()
{
	m_pEdit = new CEdit;
	m_pEdit->Create(WS_CHILD|WS_BORDER|ES_UPPERCASE|ES_LOWERCASE|ES_CENTER|ES_NUMBER,CRect(0,0,0,0),this,IDC_EDIT1);
	m_pEdit->LimitText(1);
	m_isedit = false;
	m_ixgrid = 0;
	m_iygrid = 0;
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
		m_text[ix][iy] = '\0';
	CDialog::OnInitDialog();

	// 將“關於...”功能表項目添加到系統功能表中。

	// IDM_ABOUTBOX 必須在系統命令範圍內。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設置此對話方塊的圖示。當應用程式主視窗不是對話方塊時，框架將自動
	//  執行此操作
	SetIcon(m_hIcon, TRUE);			// 設置大圖示
	SetIcon(m_hIcon, FALSE);		// 設置小圖示

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加額外的初始化代碼



	return TRUE;  // 除非將焦點設置到控制項，否則返回 TRUE
}

void CsudokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向對話方塊添加最小化按鈕，則需要下面的代碼
//  來繪製該圖示。對於使用文檔/視圖模型的 MFC 應用程式，
//  這將由框架自動完成。

void CsudokuDlg::OnPaint()
{
	CPaintDC dc(this);
	CPen pen;
	pen.CreatePen(0,1,RGB(0,0,0));
	dc.SelectObject(&pen);
	int xgridlen = m_rect.Width()/9;
	int ygridlen = m_rect.Height()/9;

	CBrush br;
	br.CreateSolidBrush(RGB(248,216,216));

	if (m_isedit)
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
	if (Csudoku::IsRelateNode(ix,iy,m_ixgrid,m_iygrid))
	{
		CRect rect(0,0,xgridlen,ygridlen);
		rect += m_rect.TopLeft();
		rect += CSize(ix*xgridlen, iy*ygridlen);
		dc.FillRect(rect,&br);
	}
	br.DeleteObject();

	for (int px = m_rect.left;px <= m_rect.right;px += xgridlen)
	{
		dc.MoveTo(px, m_rect.bottom);
		dc.LineTo(px, m_rect.top);
	}
	for (int py = m_rect.top;py <= m_rect.bottom;py += ygridlen)
	{
		dc.MoveTo(m_rect.left, py);
		dc.LineTo(m_rect.right, py);
	}
	pen.DeleteObject();

	pen.CreatePen(0,3,RGB(0,0,0));
	dc.SelectObject(&pen);
	for (int px = m_rect.left;px <= m_rect.right;px += 3*xgridlen)
	{
		dc.MoveTo(px, m_rect.bottom);
		dc.LineTo(px, m_rect.top);
	}
	for (int py = m_rect.top;py <= m_rect.bottom;py += 3*ygridlen)
	{
		dc.MoveTo(m_rect.left, py);
		dc.LineTo(m_rect.right, py);
	}

	dc.SetBkMode(0);
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
	{
		CRect rect(xgridlen/3,ygridlen/3,xgridlen,ygridlen);
		rect += m_rect.TopLeft();
		rect += CSize(ix*xgridlen, iy*ygridlen);
		CString str;
		str = m_text[ix][iy];
		dc.DrawText(str,rect,0);
	}
	pen.DeleteObject();

	if (IsIconic())
	{
		CPaintDC dc(this); // 用於繪製的設備上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使圖示在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 繪製圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//當使用者拖動最小化視窗時系統調用此函數取得游標顯示。
//
HCURSOR CsudokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CsudokuDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息處理常式代碼和/或調用預設值
	CString str;
	int xgridlen = m_rect.Width()/9;
	int ygridlen = m_rect.Height()/9;
	CRect rect(0,0,xgridlen,ygridlen);
	rect += m_rect.TopLeft();
	if (m_rect.PtInRect(point))
	{
		if (m_isedit)
		{
			m_pEdit->GetWindowTextW(str);
			m_text[m_ixgrid][m_iygrid] = str[0];
			if (m_text[m_ixgrid][m_iygrid] == '\0')
				m_core.SetNode(m_ixgrid,m_iygrid,0);
			else m_core.SetNode(m_ixgrid,m_iygrid, m_text[m_ixgrid][m_iygrid] - '0');
			CRect rect(0,0,xgridlen,ygridlen);
			rect += CSize(m_ixgrid*xgridlen, m_iygrid*ygridlen);
			InvalidateRect(rect);
			rect -= CSize(m_ixgrid*xgridlen, m_iygrid*ygridlen);
		}
		m_isedit = true;
		m_ixgrid = (point.x - m_rect.left)/xgridlen;
		m_iygrid = (point.y - m_rect.top)/ygridlen;
		rect += CSize(m_ixgrid*xgridlen, m_iygrid*ygridlen);
		str = m_text[m_ixgrid][m_iygrid];
		m_pEdit->SetWindowTextW(str);
		m_pEdit->MoveWindow(rect);
		m_pEdit->ShowWindow(SW_SHOW);
		m_pEdit->SetFocus();
		short valuearray[9];
		int num = m_core.GetHint(valuearray, m_ixgrid, m_iygrid);
		m_pro = "可輸入數位:";
		while(num--)
		{
			m_pro += wchar_t('0'+valuearray[num]);
			m_pro += wchar_t(' ');
		}
		UpdateData(false);
		Invalidate();
	}
	else
	{
		if (m_isedit)
		{
			m_pEdit->GetWindowTextW(str);
			m_text[m_ixgrid][m_iygrid] = str[0];
			CRect rect(0,0,xgridlen,ygridlen);
			rect += CSize(m_ixgrid*xgridlen, m_iygrid*ygridlen);
			InvalidateRect(rect);
			rect -= CSize(m_ixgrid*xgridlen, m_iygrid*ygridlen);
			m_isedit = false;
			m_pro = "";
			UpdateData(false);
			Invalidate();
		}
		m_pEdit->ShowWindow(SW_HIDE);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CsudokuDlg::OnEnChangeEdit1()//控制使用者輸入的是數位
{
	// TODO:  如果該控制項是 RICHEDIT 控制項，則它將不會
	// 發送該通知，除非重寫 CDialog::OnInitDialog()
	// 函數並調用 CRichEditCtrl().SetEventMask()，
	// 同時將 ENM_CHANGE 標誌“或”運算到遮罩中。

	// TODO:  在此添加控制項通知處理常式代碼
	CString str;
	m_pEdit->GetWindowTextW(str);
	if(str[0] != '\0' && (str[0] > '9' || str[0] < '1'))
	{
		str = "";
		m_pEdit->SetWindowTextW(str);
	}
}

void CsudokuDlg::OnBnClickedButtonReset()//重置
{
	// TODO: 在此添加控制項通知處理常式代碼
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
		m_text[ix][iy] = '\0';
	Invalidate();
}

void CsudokuDlg::OnBnClickedButtonAuto()
{
	// TODO: 在此添加控制項通知處理常式代碼
	OnLButtonDown(1, m_rect.BottomRight() + CSize(10,10));
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
	{
		if (m_text[ix][iy] == '\0')
			m_core.SetNode(ix,iy,0);
		else m_core.SetNode(ix,iy, m_text[ix][iy] - '0');
	}
	char str[100];
	m_core.DeSudoku(str);
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
	{
		if (m_core.GetNode(ix, iy) == 0)
			m_text[ix][iy] = '\0';
		else m_text[ix][iy] = m_core.GetNode(ix, iy) + '0';
	}
	m_pro = str;
	UpdateData(false);
	Invalidate();
}

void CsudokuDlg::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控制項通知處理常式代碼
	m_pro = "正在計算...";
	UpdateData(false);
	int randseed = time(NULL);
	int i = 0;
	const int trytime = 20;
	srand(randseed);
	for (i = 0;i < trytime;i++)
	{
		randseed += rand();
		if (m_core.CreateRandomSudoku(randseed) >= 0)
			break;
	}
	if(i < trytime)
	{
		char str[100];
		sprintf(str, "成功生成亂數種子:%d", randseed);
		m_pro = str;
	}
	else m_pro = "未完成生成工作!";
	UpdateData(false);
	for (int ix = 0;ix < 9;ix++)
	for (int iy = 0;iy < 9;iy++)
	{
		if (m_core.GetNode(ix, iy) == 0)
			m_text[ix][iy] = '\0';
		else m_text[ix][iy] = m_core.GetNode(ix, iy) + '0';
	}
	Invalidate();
}
