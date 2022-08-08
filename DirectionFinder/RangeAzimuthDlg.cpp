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
	, m_szRange(_T(""))
	, m_szAzimuth(_T(""))
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
	DDX_Text(pDX, IDC_RANGE, m_szRange);
	DDX_Text(pDX, IDC_AZIMUTH, m_szAzimuth);
}


BEGIN_MESSAGE_MAP(CRangeAzimuthDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO1, &CRangeAzimuthDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CRangeAzimuthDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_COMPUTE_BUTTON, &CRangeAzimuthDlg::OnBnClickedComputeButton)
END_MESSAGE_MAP()


// CRangeAzimuthDlg message handlers


void CRangeAzimuthDlg::OnBnClickedRadio1()
{
	double dec;	

	EnableCntrolGroup(IDC_DMS_GROUP, FALSE);
	EnableCntrolGroup(IDC_DECIMAL_GROUP, TRUE);

	UpdateData();
	if (!m_szTxLatDeg.IsEmpty() && !m_szTxLatMin.IsEmpty() && !m_szTxLatSec.IsEmpty())
	{
		dec = _wtof(LPCTSTR(m_szTxLatDeg)) + _wtof(LPCTSTR(m_szTxLatMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLatSec)) / 3600.0;
		if (m_cbCombo1.GetCurSel() != 0)
			dec *= -1;

		m_szTxLat.Format(L"%.6f", dec);
	}
	else
		m_szTxLat.Empty();

	if (!m_szTxLonDeg.IsEmpty() && !m_szTxLonMin.IsEmpty() && !m_szTxLonSec.IsEmpty())
	{
		dec = _wtof(LPCTSTR(m_szTxLonDeg)) + _wtof(LPCTSTR(m_szTxLonMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLonSec)) / 3600.0;
		if (m_cbCombo2.GetCurSel() != 0)
			dec *= -1;
		m_szTxLon.Format(L"%.6f", dec);
	}
	else
		m_szTxLon.Empty();

	UpdateData(FALSE);
}


void CRangeAzimuthDlg::OnBnClickedRadio2()
{
	double deg;
	struct _DMS_ dms = {};

	EnableCntrolGroup(IDC_DECIMAL_GROUP, FALSE);
	EnableCntrolGroup(IDC_DMS_GROUP, TRUE);

	UpdateData();
	if (!m_szTxLat.IsEmpty())
	{
		deg = _wtof(LPCTSTR(m_szTxLat));
		dms = DECtoDMS(deg);
		m_szTxLatDeg.Format(L"%.0f", dms.deg);
		m_szTxLatMin.Format(L"%.0f", dms.min);
		m_szTxLatSec.Format(L"%2.2f", dms.sec);
		
		if (deg >= 0)
			m_cbCombo1.SetCurSel(0);
		else
			m_cbCombo1.SetCurSel(1);
	}
	else
	{
		m_szTxLatDeg.Empty();
		m_szTxLatMin.Empty();
		m_szTxLatSec.Empty();
	}

	if (!m_szTxLon.IsEmpty())
	{
		deg = _wtof(LPCTSTR(m_szTxLon));
		dms = DECtoDMS(deg);
		m_szTxLonDeg.Format(L"%.0f", dms.deg);
		m_szTxLonMin.Format(L"%.0f", dms.min);
		m_szTxLonSec.Format(L"%2.2f", dms.sec);
		if (deg >= 0)
			m_cbCombo2.SetCurSel(0);
		else
			m_cbCombo2.SetCurSel(1);
	}
	else
	{
		m_szTxLonDeg.Empty();
		m_szTxLonMin.Empty();
		m_szTxLonSec.Empty();
	}
	UpdateData(FALSE);
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
	struct _COORD_ txcoord = {}, res = {};
	struct _POLAR_COOR_ raz = {};
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

	switch (m_nAzUnit)
	{
	case 0:
		raz.angle = _wtof(LPCTSTR(m_szAzimuth));
		break;
	case 1:
		raz.angle = _wtof(LPCTSTR(m_szAzimuth)) * RAD2DEG;
		break;
	case 2:
		raz.angle = _wtof(LPCTSTR(m_szAzimuth)) / 17.7777777777;
		break;
	}

	raz.dist = _wtof(LPCTSTR(m_szRange));
	res = compute_coord_vincenty(txcoord, raz.angle, raz.dist);
	struct _DMS_ latdms, londms;

	latdms = DECtoDMS(res.lat);
	londms = DECtoDMS(res.lon);
	swprintf(s, 100, L"%.6f, %.6f\n= %.0f°%02.0f'%02.2f\"%c,  %.0f°%02.0f'%02.2f\"%c", res.lat, res.lon,
		latdms.deg, latdms.min, latdms.sec, res.lat > 0 ? L'N' : L'S', londms.deg, londms.min, londms.sec, res.lon > 0 ? L'E' : L'W');

	SetDlgItemText(IDC_OUTPUT_STATIC, s);
}


BOOL CRangeAzimuthDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cbCombo1.SetCurSel(0);
	m_cbCombo2.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
