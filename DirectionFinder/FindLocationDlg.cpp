
// FindLocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirectionFinder.h"
#include "FindLocationDlg.h"
#include "afxdialogex.h"
#include "flightmath.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindLocationDlg dialog

CFindLocationDlg::CFindLocationDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFindLocationDlg::IDD)
	, m_szRxLat(_T(""))
	, m_szTxLat(_T(""))
	, m_szRxLon(_T(""))
	, m_szTxLon(_T(""))
	, m_nLocationFormat(0)
	, m_szTxLatDeg(_T(""))
	, m_szTxLatMin(_T(""))
	, m_szTxLatSec(_T(""))
	, m_szTxLonDeg(_T(""))
	, m_szTxLonMin(_T(""))
	, m_szTxLonSec(_T(""))
	, m_szRxLatDeg(_T(""))
	, m_szRxLatMin(_T(""))
	, m_szRxLatSec(_T(""))
	, m_szRxLonDeg(_T(""))
	, m_szRxLonMin(_T(""))
	, m_szRxLonSec(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindLocationDlg::DoDataExchange(CDataExchange* pDX)
{	
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_RX_LAT_EDIT, m_szRxLat);
	DDX_Text(pDX, IDC_TX_LAT_EDIT, m_szTxLat);
	DDX_Text(pDX, IDC_RX_LON_EDIT, m_szRxLon);
	DDX_Text(pDX, IDC_TX_LON_EDIT, m_szTxLon);
	DDX_Radio(pDX, IDC_RADIO1, m_nLocationFormat);
	DDX_Control(pDX, IDC_NS_COMBO1, m_cbCombo1);
	DDX_Control(pDX, IDC_NS_COMBO2, m_cbCombo2);
	DDX_Control(pDX, IDC_NS_COMBO3, m_cbCombo3);
	DDX_Control(pDX, IDC_NS_COMBO4, m_cbCombo4);
	DDX_Text(pDX, IDC_TX_LAT_DEG_EDIT, m_szTxLatDeg);
	DDX_Text(pDX, IDC_TX_LAT_MIN_EDIT, m_szTxLatMin);
	DDX_Text(pDX, IDC_TX_LAT_SEC_EDIT, m_szTxLatSec);
	DDX_Text(pDX, IDC_TX_LON_DEG_EDIT, m_szTxLonDeg);
	DDX_Text(pDX, IDC_TX_LON_MIN_EDIT, m_szTxLonMin);
	DDX_Text(pDX, IDC_TX_LON_SEC_EDIT, m_szTxLonSec);
	DDX_Text(pDX, IDC_RX_LAT_DEG_EDIT, m_szRxLatDeg);
	DDX_Text(pDX, IDC_RX_LAT_MIN_EDIT, m_szRxLatMin);
	DDX_Text(pDX, IDC_RX_LAT_SEC_EDIT, m_szRxLatSec);
	DDX_Text(pDX, IDC_RX_LON_DEG_EDIT, m_szRxLonDeg);
	DDX_Text(pDX, IDC_RX_LON_MIN_EDIT, m_szRxLonMin);
	DDX_Text(pDX, IDC_RX_LON_SEC_EDIT, m_szRxLonSec);
}

BEGIN_MESSAGE_MAP(CFindLocationDlg, CPropertyPage)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMPUTE_BUTTON, &CFindLocationDlg::OnBnClickedComputeButton)
	ON_BN_CLICKED(IDC_RADIO2, &CFindLocationDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CFindLocationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_SWAPLOC_BUTTON, &CFindLocationDlg::OnBnClickedSwaplocButton)
END_MESSAGE_MAP()


// CFindLocationDlg message handlers

BOOL CFindLocationDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	m_cbCombo1.SetCurSel(0);
	m_cbCombo2.SetCurSel(0);
	m_cbCombo3.SetCurSel(0);
	m_cbCombo4.SetCurSel(0);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindLocationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CPropertyPage::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindLocationDlg::OnPaint()
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
		CPropertyPage::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindLocationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFindLocationDlg::OnBnClickedComputeButton()
{
	struct _COORD_ rxcoord = {}, txcoord = {};
	struct _POLAR_COOR_ res;
	TCHAR s[100];

	UpdateData();

	switch (m_nLocationFormat)
	{	
	case 0:
		rxcoord.lat = _wtof(LPCTSTR(m_szRxLat));
		txcoord.lat = _wtof(LPCTSTR(m_szTxLat));
		rxcoord.lon = _wtof(LPCTSTR(m_szRxLon));
		txcoord.lon = _wtof(LPCTSTR(m_szTxLon));
		break;
	case 1:
		rxcoord.lat = _wtof(LPCTSTR(m_szRxLatDeg)) + _wtof(LPCTSTR(m_szRxLatMin)) / 60.0 + _wtof(LPCTSTR(m_szRxLatSec)) / 3600.0;
		txcoord.lat = _wtof(LPCTSTR(m_szTxLatDeg)) + _wtof(LPCTSTR(m_szTxLatMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLatSec)) / 3600.0;
		rxcoord.lon = _wtof(LPCTSTR(m_szRxLonDeg)) + _wtof(LPCTSTR(m_szRxLonMin)) / 60.0 + _wtof(LPCTSTR(m_szRxLonSec)) / 3600.0;
		txcoord.lon = _wtof(LPCTSTR(m_szTxLonDeg)) + _wtof(LPCTSTR(m_szTxLonMin)) / 60.0 + _wtof(LPCTSTR(m_szTxLonSec)) / 3600.0;

		txcoord.lat *= m_cbCombo1.GetCurSel() == 0 ? 1.0 : -1.0;
		txcoord.lon *= m_cbCombo2.GetCurSel() == 0 ? 1.0 : -1.0;
		rxcoord.lat *= m_cbCombo3.GetCurSel() == 0 ? 1.0 : -1.0;
		rxcoord.lon *= m_cbCombo4.GetCurSel() == 0 ? 1.0 : -1.0;
		break;
	}

	res = compute_distance_bearing_vincenty(txcoord, rxcoord);
	//res.angle = compute_bearing_deg(txcoord, rxcoord);
	//res.dist = compute_distance_haversine(txcoord, rxcoord);
	swprintf(s, 100, L"%.4f degrees\t%.2f m\n= %.2f radians\t\t= %.2f km\n= %.2f mils", res.angle, res.dist, res.angle * DEG2RAD, res.dist/1000.0,res.angle * 17.777777777);

	SetDlgItemText(IDC_OUTPUT_STATIC, s);

}


BOOL CFindLocationDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::DestroyWindow();
}

BOOL CFindLocationDlg::EnableCntrolGroup(int GroupID, BOOL showwindow)
{	
	CWnd* pCtrlWnd;


	if ((pCtrlWnd = GetDlgItem(GroupID)) == NULL)
	{
		return FALSE;
	}

	do
	{
		pCtrlWnd->ShowWindow(showwindow?SW_SHOW:SW_HIDE);
	} while ((pCtrlWnd = GetNextDlgGroupItemEx(pCtrlWnd)) != NULL);

	return TRUE;
}

CWnd* CFindLocationDlg::GetNextDlgGroupItemEx(CWnd * pCtrlWnd)
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

void CFindLocationDlg::OnBnClickedRadio1()
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

	if (!m_szRxLatDeg.IsEmpty() && !m_szRxLatMin.IsEmpty() && !m_szRxLatSec.IsEmpty())
	{
		dec = _wtof(LPCTSTR(m_szRxLatDeg)) + _wtof(LPCTSTR(m_szRxLatMin)) / 60.0 + _wtof(LPCTSTR(m_szRxLatSec)) / 3600.0;
		if (m_cbCombo3.GetCurSel() != 0)
			dec *= -1;

		m_szRxLat.Format(L"%.6f", dec);
	}
	else
		m_szRxLat.Empty();

	if (!m_szRxLonDeg.IsEmpty() && !m_szRxLonMin.IsEmpty() && !m_szRxLonSec.IsEmpty())
	{
		dec = _wtof(LPCTSTR(m_szRxLonDeg)) + _wtof(LPCTSTR(m_szRxLonMin)) / 60.0 + _wtof(LPCTSTR(m_szRxLonSec)) / 3600.0;
		if (m_cbCombo4.GetCurSel() != 0)
			dec *= -1;
		m_szRxLon.Format(L"%.6f", dec);
	}
	else
		m_szRxLon.Empty();

	UpdateData(FALSE);
}


void CFindLocationDlg::OnBnClickedRadio2()
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
		m_szTxLatDeg.Format(L"%d", (int)dms.deg);
		m_szTxLatMin.Format(L"%d", (int)dms.min);
		m_szTxLatSec.Format(L"%d", (int)dms.sec);

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
		m_szTxLonDeg.Format(L"%d", (int)dms.deg);
		m_szTxLonMin.Format(L"%d", (int)dms.min);
		m_szTxLonSec.Format(L"%d", (int)dms.sec);
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

	if (!m_szRxLat.IsEmpty())
	{
		deg = _wtof(LPCTSTR(m_szRxLat));
		dms = DECtoDMS(deg);
		m_szRxLatDeg.Format(L"%d", (int)dms.deg);
		m_szRxLatMin.Format(L"%d", (int)dms.min);
		m_szRxLatSec.Format(L"%d", (int)dms.sec);

		if (deg >= 0)
			m_cbCombo3.SetCurSel(0);
		else
			m_cbCombo3.SetCurSel(1);
	}
	else
	{
		m_szRxLatDeg.Empty();
		m_szRxLatMin.Empty();
		m_szRxLatSec.Empty();
	}

	if (!m_szRxLon.IsEmpty())
	{
		deg = _wtof(LPCTSTR(m_szRxLon));
		dms = DECtoDMS(deg);
		m_szRxLonDeg.Format(L"%d", (int)dms.deg);
		m_szRxLonMin.Format(L"%d", (int)dms.min);
		m_szRxLonSec.Format(L"%d", (int)dms.sec);
		if (deg >= 0)
			m_cbCombo4.SetCurSel(0);
		else
			m_cbCombo4.SetCurSel(1);
	}
	else
	{
		m_szRxLonDeg.Empty();
		m_szRxLonMin.Empty();
		m_szRxLonSec.Empty();
	}

	UpdateData(FALSE);
}


void CFindLocationDlg::OnBnClickedSwaplocButton()
{
	CString sztx, szrx;
	int txhem, rxhem;

	UpdateData();
	switch (m_nLocationFormat)
	{
	case 0:
		GetDlgItemText(IDC_TX_LAT_EDIT, sztx);
		GetDlgItemText(IDC_RX_LAT_EDIT, szrx);
		SetDlgItemText(IDC_TX_LAT_EDIT, szrx);
		SetDlgItemText(IDC_RX_LAT_EDIT, sztx);

		GetDlgItemText(IDC_TX_LON_EDIT, sztx);
		GetDlgItemText(IDC_RX_LON_EDIT, szrx);
		SetDlgItemText(IDC_TX_LON_EDIT, szrx);
		SetDlgItemText(IDC_RX_LON_EDIT, sztx);
		
		break;
	case 1:
		GetDlgItemText(IDC_TX_LAT_DEG_EDIT, sztx);
		GetDlgItemText(IDC_RX_LAT_DEG_EDIT, szrx);
		SetDlgItemText(IDC_TX_LAT_DEG_EDIT, szrx);
		SetDlgItemText(IDC_RX_LAT_DEG_EDIT, sztx);

		GetDlgItemText(IDC_TX_LON_DEG_EDIT, sztx);
		GetDlgItemText(IDC_RX_LON_DEG_EDIT, szrx);
		SetDlgItemText(IDC_TX_LON_DEG_EDIT, szrx);
		SetDlgItemText(IDC_RX_LON_DEG_EDIT, sztx);

		GetDlgItemText(IDC_TX_LAT_MIN_EDIT, sztx);
		GetDlgItemText(IDC_RX_LAT_MIN_EDIT, szrx);
		SetDlgItemText(IDC_TX_LAT_MIN_EDIT, szrx);
		SetDlgItemText(IDC_RX_LAT_MIN_EDIT, sztx);

		GetDlgItemText(IDC_TX_LON_MIN_EDIT, sztx);
		GetDlgItemText(IDC_RX_LON_MIN_EDIT, szrx);
		SetDlgItemText(IDC_TX_LON_MIN_EDIT, szrx);
		SetDlgItemText(IDC_RX_LON_MIN_EDIT, sztx);

		GetDlgItemText(IDC_TX_LAT_SEC_EDIT, sztx);
		GetDlgItemText(IDC_RX_LAT_SEC_EDIT, szrx);
		SetDlgItemText(IDC_TX_LAT_SEC_EDIT, szrx);
		SetDlgItemText(IDC_RX_LAT_SEC_EDIT, sztx);

		GetDlgItemText(IDC_TX_LON_SEC_EDIT, sztx);
		GetDlgItemText(IDC_RX_LON_SEC_EDIT, szrx);
		SetDlgItemText(IDC_TX_LON_SEC_EDIT, szrx);
		SetDlgItemText(IDC_RX_LON_SEC_EDIT, sztx);

		txhem = m_cbCombo1.GetCurSel();
		rxhem = m_cbCombo3.GetCurSel();
		m_cbCombo1.SetCurSel(rxhem);
		m_cbCombo3.SetCurSel(txhem);

		txhem = m_cbCombo2.GetCurSel();
		rxhem = m_cbCombo4.GetCurSel();
		m_cbCombo2.SetCurSel(rxhem);
		m_cbCombo4.SetCurSel(txhem);
		break;
	default:
		break;
	}
}
