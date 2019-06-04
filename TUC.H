// tuc.h : main header file for the TUC application
// by Le Ngoc Thanh, EMP.1999 - Economics University of HCM City.
// To my Friends and my students.

#if !defined(AFX_TUC_H__F608A685_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_)
#define AFX_TUC_H__F608A685_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTucApp:
// See tuc.cpp for the implementation of this class
//

class CTucApp : public CWinApp
{
public:
	CTucApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTucApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTucApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TUC_H__F608A685_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_)
