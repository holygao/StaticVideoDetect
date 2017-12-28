// StaticDetect.h: interface for the CStaticDetect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICDETECT_H__ECD0E348_F42D_432E_B3E4_20A849515242__INCLUDED_)
#define AFX_STATICDETECT_H__ECD0E348_F42D_432E_B3E4_20A849515242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class CStaticDetect  
{
public:
	CFile file;
	void SearchGroupHead(CString segname, int *group_head_pos);
	CStaticDetect();
	virtual ~CStaticDetect();

};

#endif // !defined(AFX_STATICDETECT_H__ECD0E348_F42D_432E_B3E4_20A849515242__INCLUDED_)
