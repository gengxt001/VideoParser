
// TestHexEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestHexEdit.h"
#include "TestHexEditDlg.h"
#include "afxdialogex.h"

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


// CTestHexEditDlg dialog




CTestHexEditDlg::CTestHexEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestHexEditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestHexEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndHexEdit);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, m_editFileName);
	DDX_Control(pDX, IDC_EDIT_FILE_SIZE, m_editFileSize);
}

BEGIN_MESSAGE_MAP(CTestHexEditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CTestHexEditDlg::OnBnClickedButtonOpenfile)
END_MESSAGE_MAP()


// CTestHexEditDlg message handlers

BOOL CTestHexEditDlg::OnInitDialog()
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


	FILE *pFile;
	pFile = fopen("ReadMe.txt", "rb");
	if (pFile != NULL)
	{
		int ret = fread(testArray, 1, nSize, pFile);

		m_wndHexEdit.SetData(ret, (BYTE*)testArray);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestHexEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestHexEditDlg::OnPaint()
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
HCURSOR CTestHexEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestHexEditDlg::OnBnClickedButtonOpenfile()
{
	// TODO: Add your control notification handler code here
	//CFileDialog dlg(TRUE, _T("文件的后缀，例如（.txt）"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.txt)|*.TXT"),NULL); 
	CFileDialog dlg(true, _T("mp3"), _T("*.mp3"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("mp3 Files (*.mp3)|*.mp3|wav File(*.wav)|*.wav|All File (*.*)|*.*||"), NULL); 

	const int nSize = 0xffff;
	BYTE *testArray = (BYTE*)malloc(nSize);
	if (testArray == NULL)
	{
		static const TCHAR szNoMem[] = _T("Out of memory : Insufficient memory to resize edit buffer.");
		AfxMessageBox(szNoMem);
	}

	if (dlg.DoModal() == IDOK) 
	{
		CString fileName;  
		fileName = dlg.GetPathName();  
		m_editFileName.SetWindowText(fileName); 

		CString strFileSize;
		strFileSize.Format(_T("%d"), fileName.GetLength());
		m_editFileSize.SetWindowText(strFileSize);

		FILE *pFile;
		pFile = _tfopen(fileName,_T("rb"));//fopen((const char*)fileName.GetBuffer(), "rb");
		if (pFile != NULL)
		{
			
			int ret = fread(testArray, 1, nSize, pFile);

			m_wndHexEdit.SetData(ret, (BYTE*)testArray);
		}
	}
}
