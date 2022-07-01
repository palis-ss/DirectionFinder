// RangeAzimuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirectionFinder.h"
#include "afxdialogex.h"
#include "RangeAzimuthDlg.h"
#include "flightmath.h"


// CRangeAzimuthDlg dialog

IMPLEMENT_DYNAMIC(CRangeAzimuthDlg, CPropertyPage)

CRangeAzimuthDlg::CRangeAzimuthDlg(CWnd* pParent /*=nullptr*/)
	: CPropertyPage(IDD_RANGE_AZIMUTH_DLG)
	, m_nLocationFormat(FALSE)
	, m_nAzUnit(0)
	, m_szTxLat(_T(""))
	, m_szTxLon(_T(""))
	, m_szTxLatDeg(_T(""))
	, m_szTxLatMin(_T(""))
	, m_szTxLatSec(_T(""))
	, m_szTxLonDeg(_T(""))
	, m_szTxLonMin(_T(""))
	, m_szTxLonSec(_T(""))
{

}

CRangeAzimuthDlg::~CRangeAzimuthDlg()
{
}

void CRangeAzimuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nLocationFormat);
	DDX_Radio(pDX, IDC_RADIO3, m_nAzUnit);
	DDX_Text(pDX, IDC_TX_LAT_EDIT, m_szTxLat);
	DDX_Text(pDX, IDC_TX_LON_EDIT, m_szTxLon);
	DDX_Text(pDX, IDC_TX_LAT_DEG_EDIT, m_szTxLatDeg);
	DDX_Text(pDX, IDC_TX_LAT_MIN_EDIT, m_szTxLatMin);
	DDX_Text(pDX, IDC_TX_LAT_SEC_EDIT, m_szTxLatSec);
	DDX_Text(pDX, IDC_TX_LON_DEG_EDIT, m_szTxLonDeg);
	DDX_Text(pDX, IDC_TX_LON_MIN_EDIT, m_szTxLonMin);
	DDX_Text(pDX, IDC_TX_LON_SEC_EDIT, m_szTxLonSec);
	DDX_Control(pDX, IDC_NS_COMBO1, m_cbCombo1);
	DDX_Control(pDX, IDC_NS_COMBO2, m_cbCombo2);
}


BEGIN_MESSAGE_MAP(CRangeAzimuthDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO1, &CRangeAzimuthDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CRangeAzimuthDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_COMPUTE_BUTTON, &CRangeAzimuthDlg::OnBnClickedComputeButton)
END_MESSAGE_MAP()


// CRangeAzimuthDlg message handlers


void CRangeAzimuthDlg::OnBnClickedRadio1()
{
	EnableCntrolGroup(IDC_DMS_GROUP, FALSE);
	EnableCntrolGroup(IDC_DECIMAL_GROUP, TRUE);
}


void CRangeAzimuthDlg::OnBnClickedRadio2()
{
	EnableCntrolGroup(IDC_DECIMAL_GROUP, FALSE);
	EnableCntrolGroup(IDC_DMS_GROUP, TRUE);
}


BOOL CRangeAzimuthDlg::EnableCntrolGroup(int GroupID, BOOL showwindow)
{
	CWnd* pCtrlWnd;


	if ((pCtrlWnd = GetDlgItem(GroupID)) == NULL)
	{
		return FALSE;
	}

	do
	{
		pCtrlWnd->ShowWindow(showwindow ? SW_SHOW : SW_HIDE);
	} while ((pCtrlWnd = GetNextDlgGroupItemEx(pCtrlWnd)) != NULL);

	return TRUE;
}

CWnd* CRangeAzimuthDlg::GetNextDlgGroupItemEx(CWnd* pCtrlWnd)
{
	CWnd* pWnd;

	if (pCtrlWnd == NULL
		|| pCtrlWnd->GetDlgCtrlID() == 0
		|| (pWnd = pCtrlWnd->GetWindow(GW_HWNDNEXT)) == NULL
		|| (pWnd->GetStyle() & WS_GROUP))
	{
		return NULL;
	}

	return pWnd;
}



void CRangeAzimuthDlg::OnBnClickedComputeButton()
{
	struct _COORD_ txcoord = {};
	struct _POLAR_COOR_ res;
	TCHAR s[100];

	UpdateData();

	switch (m_nLocationFormat)
	{
	case 0:		
		txcoord.lat = _wtof(LPCTSTR(m_szTxLat));		
		txcoord.lon = _wtof(LPCTSTR(m_szTxLon));
		break;
	case 1:		
		txcoord.lat = _wtof(LPCTSTR(m_szTxLatDeg)) + _wtof(LPCTSTR(m_szTxLatMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLatSec)) / 3600.0;
		txcoord.lon = _wtof(LPCTSTR(m_szTxLonDeg)) + _wtof(LPCTSTR(m_szTxLonMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLonSec)) / 3600.0;

		txcoord.lat *= m_cbCombo1.GetCurSel() == 0 ? 1.0 : -1.0;
		txcoord.lon *= m_cbCombo2.GetCurSel() == 0 ? 1.0 : -1.0;
		break;
	}

	swprintf(s, 100, L"%.2f degrees %.2f m", res.angle, res.dist);

	SetDlgItemText(IDC_OUTPUT_STATIC, s);
}
