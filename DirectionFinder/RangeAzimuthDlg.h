#pragma once
#include "afxdialogex.h"


// CRangeAzimuthDlg dialog

class CRangeAzimuthDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRangeAzimuthDlg)

public:
	CRangeAzimuthDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CRangeAzimuthDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANGE_AZIMUTH_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_nLocationFormat;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	BOOL EnableCntrolGroup(int GroupID, BOOL showwindow);
	CWnd* GetNextDlgGroupItemEx(CWnd* pCtrlWnd);
	int m_nAzUnit;
	afx_msg void OnBnClickedComputeButton();
	CString m_szTxLat;
	CString m_szTxLon;
	CString m_szTxLatDeg;
	CString m_szTxLatMin;
	CString m_szTxLatSec;
	CString m_szTxLonDeg;
	CString m_szTxLonMin;
	CString m_szTxLonSec;
	CComboBox m_cbCombo1;
	CComboBox m_cbCombo2;
};
