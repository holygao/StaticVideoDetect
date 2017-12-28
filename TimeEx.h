// TimeEx.h: interface for the CTimeEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEEX_H__9E0170A5_03B0_4E80_A7CB_F37E0166CEB5__INCLUDED_)
#define AFX_TIMEEX_H__9E0170A5_03B0_4E80_A7CB_F37E0166CEB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once

#include <windows.h>
#include <string>
#include <time.h>


using namespace std;

class CTimeEx
{
	SYSTEMTIME m_SysTime;
	int		   m_TimeArea;
	time_t	   m_ltime;
	string     m_strRet;
public:
	int		GetYear();
	int		GetMonth();
	int		GetDay();
	int		GetHour();
	int		GetMinute();
	int		GetSecond();
	void	SetTime(time_t lTime);
	time_t	GetTime();
	void	MakeTime(int nYear, int nMonth, int nDay, int nHour, int nMinitus, int nSecond);
	string	FormatGmt();
	string	Format(LPCSTR lpszFormat);
	string  Format();
	string  FormatDate(LPCTSTR szSplit = "-");
	string  FormatTime(LPCTSTR szSplit = ":");
	void	Now();
	bool	AnalysisGmtTime(LPCSTR szTime);
	bool	Analysis(LPCSTR szTime);

	LPSYSTEMTIME GetSysTime();
	
	CTimeEx();
	virtual ~CTimeEx();
};

#endif // !defined(AFX_TIMEEX_H__9E0170A5_03B0_4E80_A7CB_F37E0166CEB5__INCLUDED_)
