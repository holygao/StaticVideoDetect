// StaticVideoDetectDlg.h : header file
//

#if !defined(AFX_STATICVIDEODETECTDLG_H__1C99C421_538D_4B18_8F8E_09572CC13F8C__INCLUDED_)
#define AFX_STATICVIDEODETECTDLG_H__1C99C421_538D_4B18_8F8E_09572CC13F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStaticVideoDetectDlg dialog
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "CDXGraph.h"
#include <io.h>
#include "VideoDataStatistic.h"
class CStaticVideoDetectDlg : public CDialog
{
// Construction
public:
	int ReadLabelFile(char* strFilePath);

	static unsigned int StaticDetect(void *p);
	CString TruncPath(CString pathname);
	void DestroyGraph(void);
	void CreateGraph(CString vidName);
	typedef struct
	{
		CString     vidFName;
		float		timeStart;
		float		timeStop;
	}labeldata;
	
	std::vector<CString> mVideo;
	std::vector<CString> mVideoSeg;
	std::vector<CString> mSegChanl;
	std::vector<int>     msegFrameCnt;
	std::vector<labeldata> mLabelData;
	
	CDXGraph* m_FilterGraph;
	CDXGraph* m_FltSegGraph;
	double    m_VidTimeT;

	
	float		  m_TotalStaticTime;//标注静止总时间
	float		  m_FindStaticTime;//发现静止总时间
	float		  m_TotalTime;	   //视频总时间

	//设置定时器
	BOOL bTimer;
	//设置播放标志
	BOOL bPlay;
	bool bPause;
	bool bsegPause;
	bool bFindLabel;
	CStaticVideoDetectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStaticVideoDetectDlg)
	enum { IDD = IDD_STATICVIDEODETECT_DIALOG };
	CSliderCtrl	m_SliderSeg;
	CEdit	m_PlayName;
	CStatic	m_VidSegWindow;
	CSliderCtrl	m_SliderGraph;
	CStatic	m_VideoWindow;
	CProgressCtrl	m_progress;
	CButton	m_xDetect;
	CListCtrl	m_List;
	CString m_strSavePath;
	CString	m_sPath;
	CString	m_LabelFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticVideoDetectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStaticVideoDetectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnexplore2();
	afx_msg void OnBtnexplore();
	afx_msg void OnBtnlabelopen();
	afx_msg void OnBtnfileproc();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtndetect();
	afx_msg void OnStaticseg();
	afx_msg void OnStaticvid();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICVIDEODETECTDLG_H__1C99C421_538D_4B18_8F8E_09572CC13F8C__INCLUDED_)
