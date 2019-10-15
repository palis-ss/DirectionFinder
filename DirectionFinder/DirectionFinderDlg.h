
// DirectionFinderDlg.h : header file
//

#pragma once


// CDirectionFinderDlg dialog
class CDirectionFinderDlg : public CDialogEx
{
// Construction
public:
	CDirectionFinderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DIRECTIONFINDER_DIALOG };

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
	afx_msg void OnBnClickedComputeButton();
	virtual BOOL DestroyWindow();
	CString m_szRxLat;
	CString m_szTxLat;
	CString m_szRxLon;
	CString m_szTxLon;
};
