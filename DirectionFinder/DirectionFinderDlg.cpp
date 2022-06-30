
// DirectionFinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirectionFinder.h"
#include "DirectionFinderDlg.h"
#include "afxdialogex.h"
#include "flightmath.h"


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


// CDirectionFinderDlg dialog



CDirectionFinderDlg::CDirectionFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirectionFinderDlg::IDD, pParent)
	, m_szRxLat(_T(""))
	, m_szTxLat(_T(""))
	, m_szRxLon(_T(""))
	, m_szTxLon(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectionFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RX_LAT_EDIT, m_szRxLat);
	DDX_Text(pDX, IDC_TX_LAT_EDIT, m_szTxLat);
	DDX_Text(pDX, IDC_RX_LON_EDIT, m_szRxLon);
	DDX_Text(pDX, IDC_TX_LON_EDIT, m_szTxLon);
}

BEGIN_MESSAGE_MAP(CDirectionFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMPUTE_BUTTON, &CDirectionFinderDlg::OnBnClickedComputeButton)
END_MESSAGE_MAP()


// CDirectionFinderDlg message handlers

BOOL CDirectionFinderDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDirectionFinderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDirectionFinderDlg::OnPaint()
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
HCURSOR CDirectionFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDirectionFinderDlg::OnBnClickedComputeButton()
{
	struct _COORD_ rxcoord, txcoord;	
	struct _POLAR_COOR_ res;
	TCHAR s[100];

	UpdateData();
	
	rxcoord.lat = _wtof(LPCTSTR(m_szRxLat));
	txcoord.lat = _wtof(LPCTSTR(m_szTxLat));
	rxcoord.lon = _wtof(LPCTSTR(m_szRxLon));
	txcoord.lon = _wtof(LPCTSTR(m_szTxLon));
	/*
	wcscpy_s(s, 100, LPCTSTR(m_szRx));
	swscanf_s(s, L"%lf, %lf", &rxcoord.lat, &rxcoord.lon);

	wcscpy_s(s, 100, LPCTSTR(m_szTx));
	swscanf_s(s, L"%lf, %lf", &txcoord.lat, &txcoord.lon);
	*/
	
	res = compute_distance_bearing_vincenty(txcoord, rxcoord);
	//res.angle = compute_bearing_deg(txcoord, rxcoord);
	//res.dist = compute_distance_haversine(txcoord, rxcoord);
	swprintf(s, 100,  L"%.2f degrees %.2f metres", res.angle, res.dist);

	SetDlgItemText(IDC_OUTPUT_STATIC, s);

}


BOOL CDirectionFinderDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::DestroyWindow();
}
