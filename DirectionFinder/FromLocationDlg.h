
// FromLocationDlg.h : header file
//

#pragma once


// CFromLocationDlg dialog
class CFromLocationDlg : public CPropertyPage
{
// Construction
public:
	CFromLocationDlg(CWnd* pParent = NULL);	// standard constructor

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

	BOOL EnableCntrolGroup(int GroupID, BOOL showwindow);
	CWnd* GetNextDlgGroupItemEx(CWnd* pCtrlWnd);
	int m_nLocationFormat;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	CComboBox m_cbCombo1;
	CComboBox m_cbCombo2;
	CComboBox m_cbCombo3;
	CComboBox m_cbCombo4;
	CString m_szTxLatDeg;
	CString m_szTxLatMin;
	CString m_szTxLatSec;
	CString m_szTxLonDeg;
	CString m_szTxLonMin;
	CString m_szTxLonSec;
	CString m_szRxLatDeg;
	CString m_szRxLatMin;
	CString m_szRxLatSec;
	CString m_szRxLonDeg;
	CString m_szRxLonMin;
	CString m_szRxLonSec;
	afx_msg void OnBnClickedSwaplocButton();
};
