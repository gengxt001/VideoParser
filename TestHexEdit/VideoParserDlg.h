
// VideoParserDlg.h : header file
//

#pragma once
#include "hexedit.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <list>
#include "FlvFormat.h"

using namespace std;

// CVideoParserDlg dialog
class CVideoParserDlg : public CDialogEx
{
// Construction
public:
	CVideoParserDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIDEOPARSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CHexEdit m_wndHexEdit;
	afx_msg void OnBnClickedButtonOpenfile();
	CEdit m_editFileName;
	CEdit m_editFileSize;
	CString m_strBasicInfo;
	CTreeCtrl m_treeCtrl;
	afx_msg void OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CToolTipCtrl *m_pToolTipCtrl;
	list<HEX_FORMAT> m_listFormat;
	map<int, list<CString> > m_mapTreeTip;
};
