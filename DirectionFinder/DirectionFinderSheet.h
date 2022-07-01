#pragma once
#include "DirectionFinderDlg.h"
#include "RangeAzimuthDlg.h"


// CDirectionFinderSheet

class CDirectionFinderSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDirectionFinderSheet)

public:
	CDirectionFinderSheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CDirectionFinderSheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CDirectionFinderSheet();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CDirectionFinderDlg dlgDirectionFinder;
	CRangeAzimuthDlg dlgRangeAz;

public:
	virtual BOOL OnInitDialog();
};


