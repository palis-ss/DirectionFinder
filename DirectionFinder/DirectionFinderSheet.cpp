// DirectionFinderSheet.cpp : implementation file
//

#include "stdafx.h"
#include "DirectionFinder.h"
#include "DirectionFinderSheet.h"


// CDirectionFinderSheet

IMPLEMENT_DYNAMIC(CDirectionFinderSheet, CPropertySheet)

CDirectionFinderSheet::CDirectionFinderSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDirectionFinderSheet::CDirectionFinderSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{		
	AddPage(&dlgDirectionFinder);
	AddPage(&dlgRangeAz);
}

CDirectionFinderSheet::~CDirectionFinderSheet()
{
}


BEGIN_MESSAGE_MAP(CDirectionFinderSheet, CPropertySheet)
END_MESSAGE_MAP()


BOOL CDirectionFinderSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	SetDlgItemText(IDOK, L"Close");
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);

	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDHELP)->EnableWindow(FALSE);
	

	return bResult;
}
