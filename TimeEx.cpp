// TimeEx.cpp: implementation of the CTimeEx class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "TimeEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
inline void lowercase_string(string &str)
{
    for (string::iterator i = str.begin(); i != str.end(); ++i) 
	{
		*i = tolower(static_cast<unsigned char>(*i));
    }
}

CTimeEx::CTimeEx()
{
	m_ltime = 0;
	m_SysTime.wYear  = 1900;
	m_SysTime.wMonth = 1;
	m_SysTime.wDay   = 1;
	m_SysTime.wHour  = 0;
	m_SysTime.wMinute= 0;
	m_SysTime.wSecond= 0;
	m_SysTime.wMilliseconds = 0;
	m_SysTime.wDayOfWeek    = 0;
	m_TimeArea = 8;		// 缺省为东八区
}

CTimeEx::~CTimeEx()
{

}

bool CTimeEx::AnalysisGmtTime(LPCSTR lpszTime)
{
	string strTime;
	strTime.assign(lpszTime);

	struct tm tm_time;

	int nPos = strTime.find(",", 0);
	if (nPos != strTime.npos)
	{
		strTime = strTime.substr(nPos+1, strTime.size() - 1 - nPos);
		while(strTime[0] == ' ')
		{
			strTime = strTime.substr(1, strTime.size() - 1);
		}
		
		string strDay,strMonth,strYear,strHour,strMinute,strSecond;
		string strAllMonth = "jan,feb,mar,apr,may,jan,jul,aug,sep,oct,nov,dec";
		
		nPos = strTime.find_first_of(" \t");
		if (nPos != -1)
		{
			strDay = strTime.substr(0, nPos);
			tm_time.tm_mday = atoi(strDay.c_str());

			strTime = strTime.substr(nPos+1, strTime.size());
		}
		else
		{
			return false;
		}

		strMonth = strTime.substr(0,3);
		lowercase_string(strMonth);

		nPos = strAllMonth.find(strMonth);
		if (nPos != strAllMonth.npos)
		{
//			//strMonth.Format("%d",(nPos/4));
//			//tm_time->tm_mon = atoi(strMonth.c_str());
			tm_time.tm_mon = nPos/4;
		}
		else
		{
			return false;
		}
		
		strTime = strTime.substr(4, strTime.size()-4);
		while(strTime[0] == ' ')
		{
			strTime = strTime.substr(1, strTime.size() - 1);
		}
		nPos = strTime.find_first_of(" \t");
		if (nPos != -1)
		{
			strYear = strTime.substr(0, nPos);
			tm_time.tm_year = atoi(strYear.c_str()) - 1900;
		}
		else
		{
			return false;
		}
		strTime = strTime.substr(nPos+1, strTime.size() - nPos - 1);
		strHour = strTime.substr(0, 2);
		tm_time.tm_hour = atoi(strHour.c_str());
		strMinute = strTime.substr(3,2);
		tm_time.tm_min = atoi(strMinute.c_str());
		strSecond = strTime.substr(6,2);
		tm_time.tm_sec = atoi(strSecond.c_str());
		strTime = strTime.substr(8, strTime.size() - 1);

		m_SysTime.wYear  = tm_time.tm_year + 1900;
		m_SysTime.wMonth = tm_time.tm_mon  + 1;
		m_SysTime.wDay   = tm_time.tm_mday;
		m_SysTime.wHour  = tm_time.tm_hour;
		m_SysTime.wMinute= tm_time.tm_min;
		m_SysTime.wSecond= tm_time.tm_sec;

		m_ltime = mktime(&tm_time);

		while(strTime[0] == ' ')
		{
			strTime = strTime.substr(1, strTime.size() - 1);
		}
		m_TimeArea = atoi(strTime.c_str());		
		m_ltime += 3600 * ( + m_TimeArea/100 - 8);
		return true;
	}

	return false;
}

bool CTimeEx::Analysis(LPCSTR lpszTime)
{
	// 只解析 2009-05-02 22:12:12这种类型的
	string strTime, strDate;
	strTime.assign(lpszTime);

	struct tm tm_time;
	string strDay,strMonth,strYear,strHour,strMinute,strSecond;

	// 提取YEAR
	int nPos = strTime.find("-", 0);
	if (nPos == strTime.npos) return false;
	strYear = strTime.substr(0, nPos);
	tm_time.tm_year = atoi(strYear.c_str()) - 1900;

	strTime = strTime.substr(nPos+1, strTime.size() - 1 - nPos);
	// 提取MONTH
	nPos = strTime.find("-", 0);
	if (nPos == strTime.npos) return false;
	strMonth = strTime.substr(0, nPos);
	tm_time.tm_mon = atoi(strMonth.c_str()) - 1;

	strTime = strTime.substr(nPos+1, strTime.size() - 1 - nPos);
	// 提取DAY
	nPos = strTime.find(" ", 0);
	if (nPos == strTime.npos) return false;
	strDay = strTime.substr(0, nPos);
	tm_time.tm_mday = atoi(strDay.c_str());

	strTime = strTime.substr(nPos+1, strTime.size() - nPos - 1);
	
	strHour = strTime.substr(0, 2);
	tm_time.tm_hour = atoi(strHour.c_str());
	strMinute = strTime.substr(3,2);
	tm_time.tm_min = atoi(strMinute.c_str());
	strSecond = strTime.substr(6,2);
	tm_time.tm_sec = atoi(strSecond.c_str());

	m_SysTime.wYear  = tm_time.tm_year + 1900;
	m_SysTime.wMonth = tm_time.tm_mon  + 1;
	m_SysTime.wDay   = tm_time.tm_mday;
	m_SysTime.wHour  = tm_time.tm_hour - m_TimeArea;
	m_SysTime.wMinute= tm_time.tm_min;
	m_SysTime.wSecond= tm_time.tm_sec;

	//m_ltime = mktime(&tm_time);

	return true;
}

void CTimeEx::Now()
{
	GetSystemTime(&m_SysTime);
}

string CTimeEx::Format(LPCSTR lpszFormat)
{
	m_strRet = "";
	if(m_ltime == 0) return m_strRet;

	struct tm *  tm_time;
	tm_time = localtime( &m_ltime );

	char szRet[128];
	strftime(szRet, 128, lpszFormat, tm_time);
	m_strRet.assign(szRet);

	return m_strRet;
}

string CTimeEx::Format()
{
	char ttt[100];
	sprintf(ttt, "%d-%02d-%02d %02d:%02d:%02d",
		m_SysTime.wYear,m_SysTime.wMonth,m_SysTime.wDay,
	    m_SysTime.wHour + m_TimeArea, m_SysTime.wMinute,m_SysTime.wSecond);

	m_strRet.assign(ttt);

	return m_strRet;
}

string CTimeEx::FormatDate(LPCTSTR szSplit)
{
	char ttt[100];
	sprintf(ttt, "%d%s%02d%s%02d",
		m_SysTime.wYear, szSplit, m_SysTime.wMonth, szSplit, m_SysTime.wDay);

	m_strRet.assign(ttt);

	return m_strRet;
}

string CTimeEx::FormatTime(LPCTSTR szSplit)
{
	char ttt[100];
	sprintf(ttt, "%02d%s%02d%s%02d",
		m_SysTime.wHour + m_TimeArea, szSplit, m_SysTime.wMinute, szSplit, m_SysTime.wSecond);

	m_strRet.assign(ttt);

	return m_strRet;
}

string CTimeEx::FormatGmt()
{
	struct tm *  tm_time;
	tm_time = localtime( &m_ltime );

	char szRet[128];
	strftime(szRet, 128, "%a, %d %b %Y %H:%M:%S GMT", tm_time );

	m_strRet.assign(szRet);

	return m_strRet;
}

void CTimeEx::MakeTime(int nYear, int nMonth, int nDay, int nHour, int nMinitus, int nSecond)
{
	m_SysTime.wYear  = nYear;
	m_SysTime.wMonth = nMonth;
	m_SysTime.wDay   = nDay;
	m_SysTime.wHour  = nHour;
	m_SysTime.wMinute= nMinitus;
	m_SysTime.wSecond= nSecond;
}

void CTimeEx::SetTime(time_t lTime)
{
	m_ltime = lTime;
}

time_t CTimeEx::GetTime( )
{
	return m_ltime;
}

LPSYSTEMTIME CTimeEx::GetSysTime( )
{
	return &m_SysTime;
}

int CTimeEx::GetYear()
{
	return m_SysTime.wYear;
}

int CTimeEx::GetMonth()
{
	return m_SysTime.wMonth;
}

int CTimeEx::GetDay()
{
	return m_SysTime.wDay;
}

int CTimeEx::GetHour()
{
	return m_SysTime.wHour;
}

int CTimeEx::GetMinute()
{
	return m_SysTime.wMinute;
}

int CTimeEx::GetSecond()
{
	return m_SysTime.wSecond;
}
