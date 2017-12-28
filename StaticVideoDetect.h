// StaticVideoDetect.h : main header file for the STATICVIDEODETECT application
//

#if !defined(AFX_STATICVIDEODETECT_H__6FE44A25_A890_4C22_AF3D_B9A0607AD2B4__INCLUDED_)
#define AFX_STATICVIDEODETECT_H__6FE44A25_A890_4C22_AF3D_B9A0607AD2B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStaticVideoDetectApp:
// See StaticVideoDetect.cpp for the implementation of this class
//

class CStaticVideoDetectApp : public CWinApp
{
public:
	CStaticVideoDetectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticVideoDetectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStaticVideoDetectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICVIDEODETECT_H__6FE44A25_A890_4C22_AF3D_B9A0607AD2B4__INCLUDED_)
