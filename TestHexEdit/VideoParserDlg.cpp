
// VideoParserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoParser.h"
#include "VideoParserDlg.h"
#include "afxdialogex.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoParserDlg dialog




CVideoParserDlg::CVideoParserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoParserDlg::IDD, pParent)
	, m_strBasicInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndHexEdit);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, m_editFileName);
	DDX_Control(pDX, IDC_EDIT_FILE_SIZE, m_editFileSize);
	DDX_Text(pDX, IDC_BASIC_INFO, m_strBasicInfo);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}

BEGIN_MESSAGE_MAP(CVideoParserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CVideoParserDlg::OnBnClickedButtonOpenfile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CVideoParserDlg::OnSelchangedTree1)
END_MESSAGE_MAP()


// CVideoParserDlg message handlers

BOOL CVideoParserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	const int nSize = 10000;
	BYTE *testArray = (BYTE*)malloc(nSize);
	if (testArray == NULL)
	{
	static const TCHAR szNoMem[] = _T("Out of memory : Insufficient memory to resize edit buffer.");
	AfxMessageBox(szNoMem);
	}


	m_pToolTipCtrl = new CToolTipCtrl;
    if (!m_pToolTipCtrl->Create(this, TTS_ALWAYSTIP))
    {
        TRACE(_T("Unable To create ToolTip\n"));
        return FALSE;
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoParserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoParserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoParserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoParserDlg::OnBnClickedButtonOpenfile()
{
	// TODO: Add your control notification handler code here
	//CFileDialog dlg(TRUE, _T("文件的后缀，例如（.txt）"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.txt)|*.TXT"),NULL); 
	CFileDialog dlg(true, _T("flv"), _T("*.flv"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("mp3 Files (*.mp3)|*.mp3|wav File(*.wav)|*.wav|flv Files(*.flv)|*.flv|All File (*.*)|*.*||"), NULL); 

	const int nSize = 0xffff;
	BYTE *testArray = (BYTE*)malloc(nSize);
	if (testArray == NULL)
	{
		static const TCHAR szNoMem[] = _T("Out of memory : Insufficient memory to resize edit buffer.");
		AfxMessageBox(szNoMem);
	}

	CString fileName;  

	FILE *pFile;
	if (dlg.DoModal() == IDOK) 
	{
		
		fileName = dlg.GetPathName();  
		m_editFileName.SetWindowText(fileName); 

		CString strFileSize;
		strFileSize.Format(_T("%d"), fileName.GetLength());
		m_editFileSize.SetWindowText(strFileSize);

		pFile = _tfopen(fileName,_T("rb"));//fopen((const char*)fileName.GetBuffer(), "rb");
		if (pFile != NULL)
		{
			
			int ret = fread(testArray, 1, nSize, pFile);

			m_wndHexEdit.SetData(ret, (BYTE*)testArray);
		}

		//go to analyze the file to get basic infomation
		FlvFormat *pFlvFormat = new FlvFormat(fileName);

		m_listFormat = pFlvFormat->parseFile();

		int maxLength = 0;

		for(list<HEX_FORMAT>::iterator ite = m_listFormat.begin();
			ite!= m_listFormat.end();
			ite++)
		{
			
			if(maxLength++ > 100)
				break;
			CString strFormat;
			strFormat.Format(_T("(%d--%d)"),  ite->begin_pos, ite->end_pos);
			
			m_treeCtrl.InsertItem(*ite->strType+strFormat, NULL);
		}
			/*
		list<CString> listStrRtn = pFlvFormat->getBasicFormatInfo();

		//m_strBasicInfo = strRtn;
		for(list<CString>::iterator ite= listStrRtn.begin();
			ite!= listStrRtn.end(); ite++)
			m_strBasicInfo += *ite;
			*/
		UpdateData(FALSE);
	}
}


void CVideoParserDlg::OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strText; // 树节点的标签文本字符串   
       
    // 获取当前选中节点的句柄   
    HTREEITEM hItem = m_treeCtrl.GetSelectedItem();   
    // 获取选中节点的标签文本字符串   
    strText = m_treeCtrl.GetItemText(hItem);   
	
	int begin_pos = 0;
	int end_pos = 0;
	CString beginStr, endStr;
	int a = strText.Find('(');
	int b = strText.Find('-');
	int c = strText.Find(')');

	if(a>0 && b>0)
		beginStr = strText.Mid(a+1, b-1);
	
	if(b>0&& c>0)
		endStr = strText.Mid(b+2, c-1);

	begin_pos = _ttoi(beginStr);
	end_pos = _ttoi(endStr);
	m_wndHexEdit.SetPosition(begin_pos);
	//SetSelection 选择的是一个区间，[begin_pos, end_pos),不包含end_pos，所以下面的end_pos后面需要加1
	m_wndHexEdit.SetSelection(begin_pos, end_pos+1);

	//显示具体的信息
	
}
