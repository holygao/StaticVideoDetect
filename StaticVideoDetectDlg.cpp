// StaticVideoDetectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StaticVideoDetect.h"
#include "StaticVideoDetectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticVideoDetectDlg dialog

CStaticVideoDetectDlg::CStaticVideoDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticVideoDetectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticVideoDetectDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_sPath = _T("G:\\VideoStatDetcData");
	m_strSavePath = _T("G:\\VideoDetcRes");
	m_LabelFile = _T("G:\\VideoStatDetcData\\StaticDetect.txt");
	bPlay = FALSE;
	bPause = false;
	bsegPause = false;
	bFindLabel = false ;
	m_FltSegGraph = NULL;
	m_FilterGraph = NULL;
	m_VidTimeT = 0.0;
	m_TotalStaticTime = 0.0;
	m_FindStaticTime  = 0.0;
	m_TotalTime       = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStaticVideoDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticVideoDetectDlg)
	DDX_Control(pDX, IDC_SLIDERRES, m_SliderSeg);
	DDX_Control(pDX, IDC_EDITPLAYNAME, m_PlayName);
	DDX_Control(pDX, IDC_STATICSEG, m_VidSegWindow);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderGraph);
	DDX_Control(pDX, IDC_STATICVID, m_VideoWindow);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_BTNDETECT, m_xDetect);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT5, m_strSavePath);
	DDX_Text(pDX, IDC_EDIT1, m_sPath);
	DDX_Text(pDX, IDC_EDITLABELFILE, m_LabelFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStaticVideoDetectDlg, CDialog)
	//{{AFX_MSG_MAP(CStaticVideoDetectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNEXPLORE2, OnBtnexplore2)
	ON_BN_CLICKED(IDC_BTNEXPLORE, OnBtnexplore)
	ON_BN_CLICKED(IDC_BTNLABELOPEN, OnBtnlabelopen)
	ON_BN_CLICKED(IDC_BTNFILEPROC, OnBtnfileproc)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BTNDETECT, OnBtndetect)
	ON_BN_CLICKED(IDC_STATICSEG, OnStaticseg)
	ON_BN_CLICKED(IDC_STATICVID, OnStaticvid)
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticVideoDetectDlg message handlers

BOOL CStaticVideoDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_VidSegWindow.ModifyStyle(0, WS_CLIPCHILDREN);
	m_VideoWindow.ModifyStyle(0, WS_CLIPCHILDREN);
	m_SliderGraph.SetRange(0, 1000);
	m_SliderGraph.SetPos(0);
	m_SliderSeg.SetRange(0, 1000);
	m_SliderSeg.SetPos(0);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	bTimer = FALSE;//对定时器初始化
	
	m_List.InsertColumn(0,"视频文件名",LVCFMT_CENTER,290);
    m_List.InsertColumn(1,"处理后文件名",LVCFMT_CENTER,290);
    m_List.InsertColumn(2,"标注开始",LVCFMT_CENTER,60);
    m_List.InsertColumn(3,"发现开始",LVCFMT_CENTER,60);
	m_List.InsertColumn(4,"标注结束",LVCFMT_CENTER,60);
    m_List.InsertColumn(5,"发现结束",LVCFMT_CENTER,60);
	m_List.InsertColumn(6,"总时间",LVCFMT_CENTER,60);
	m_List.InsertColumn(7,"静止时间",LVCFMT_CENTER,60);
	m_List.InsertColumn(8,"帧率",LVCFMT_CENTER,60);
	m_List.InsertColumn(9,"门限",LVCFMT_CENTER,60);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT); 
	
	if (!bTimer)
	{
		bTimer = TRUE;
		this->SetTimer(1, 200, NULL);//SetTimer(1,100,NULL);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStaticVideoDetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStaticVideoDetectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStaticVideoDetectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStaticVideoDetectDlg::OnBtnexplore2() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;				
	char dispname[MAX_PATH];  
	char pathname[50];
	ITEMIDLIST * pidl;
	
	bi.hwndOwner = 0;  
	bi.pidlRoot = 0;  
	bi.pszDisplayName = dispname;  
	bi.lpszTitle = "选择数据文件夹:";  
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = 0;  
	bi.lParam = 0;  
	bi.iImage = 0;  
	
	UpdateData(TRUE);
	if (pidl = SHBrowseForFolder(&bi)) 
	{ 
		SHGetPathFromIDList(pidl, pathname);
		m_strSavePath = pathname;
		CreateDirectory(m_strSavePath, NULL);
		UpdateData(false);
	}
}

void CStaticVideoDetectDlg::OnBtnexplore() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
	LPMALLOC pMalloc;
	int bFile = 0;
	
	if (SUCCEEDED(SHGetMalloc(&pMalloc)))//Allocate Memory
	{
		ZeroMemory(&bi, sizeof(bi));
		_TCHAR pszDirName[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszDirName;
		bi.lpszTitle = _T("选择图像数据所在目录");
		bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = IDR_MAINFRAME;
		
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//Pop up a dialog
		{
			if (::SHGetPathFromIDList(pidl, pszDirName))//Obtain the directory name
			{
				m_sPath	= pszDirName;
			}
			pMalloc->Free(pidl);			
			pMalloc->Release();
		}
		else
		{
			pMalloc->Release();
			return;
		}
		GetDlgItem(IDC_EDIT1)->SetWindowText(m_sPath);
	}
	else 
		return;
}

void CStaticVideoDetectDlg::OnBtnlabelopen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY, "txt文件(*.txt)|*.txt||",0);
	if (fd.DoModal()!=IDOK)
	{
		return;
	}
	m_LabelFile = fd.GetPathName();
	UpdateData(0);
}

void CStaticVideoDetectDlg::OnBtnfileproc() 
{
	// TODO: Add your control notification handler code here
	CFileFind tempFind; 
	char tempFileFind[MAX_PATH]; 
	CStringArray folderarray;
	
	sprintf(tempFileFind,"%s\\*.*",m_sPath);
    folderarray.Add(tempFileFind);
	
	while(folderarray.GetSize()>0)
	{
		CString nextpath=folderarray.GetAt(0);
		//TRACE("555%s\n",nextpath);
		BOOL	IsFinded=(BOOL)tempFind.FindFile(nextpath);
		folderarray.RemoveAt(0);
		while(IsFinded)
		{			
			IsFinded=tempFind.FindNextFile();
			if(tempFind.IsDirectory() && !tempFind.IsDots())
			{
				//TRACE("%s\n",tempFind.GetFilePath());
				folderarray.Add(tempFind.GetFilePath()+"\\*.*");
			}
			else
			{				
				CString filepath = tempFind.GetFilePath();
				CString filelen  = tempFind.GetLength();
				filepath.MakeLower();
				if(filepath.Right(4)==".mpg" ||
					filepath.Right(5)==".mpeg")
				{
					if (filepath.Find("seg") == -1) 
					{
						int iRow = m_List.InsertItem(9999999l,filepath,0);
						//m_List.SetItemText(iRow,1,filelen);
						mVideo.push_back(filepath);
					}
					else
					{
						mVideoSeg.push_back(filepath);
					}
					
				}				
			}			
		}
		tempFind.Close();		
	}
	UpdateData(0);
}

void CStaticVideoDetectDlg::CreateGraph(CString vidName)
{
	DestroyGraph();
	
	m_FilterGraph = new CDXGraph();
	if (m_FilterGraph->Create())
	{
		// Render the source clip
		m_FilterGraph->RenderFile(vidName);
		// Set video window and notification window
		m_FilterGraph->SetDisplayWindow(m_VideoWindow.GetSafeHwnd());
		//	m_FilterGraph->SetDisplayWindow(NULL);
		m_FilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		// Show the first frame
		m_FilterGraph->Pause();
		
	}
}

void CStaticVideoDetectDlg::DestroyGraph()
{
	if(m_FilterGraph)
	{
		// Stop the filter graph first
		m_FilterGraph->Stop();
		m_FilterGraph->SetNotifyWindow(NULL);
		
		
		delete m_FilterGraph;
		m_FilterGraph = NULL;
	}
}

CString CStaticVideoDetectDlg::TruncPath(CString pathname)
{
	int intTmp1 = 0;
	int intTmp2 = 0;
	intTmp1 = pathname.ReverseFind('\\');
	intTmp2 = pathname.GetLength();
	CString filename = pathname.Right(intTmp2 - intTmp1 -1);
	return filename;
}

void CStaticVideoDetectDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iItem;
	CRect itemrect;
	CWnd *m_wnd = GetDlgItem(IDC_STATICSEG);
	m_wnd->GetWindowRect(itemrect);
	CString segName;
	segName = m_List.GetItemText(nIndex,1);
	CString strTemp = "";
	//CreateGraph(segName);
	CString playname = TruncPath(segName);
	SetDlgItemText(IDC_EDITPLAYRESNAME,playname);

	if(m_FltSegGraph)
	{
		// Stop the filter graph first
		m_FltSegGraph->Stop();
		m_FltSegGraph->SetNotifyWindow(NULL);		
		delete m_FltSegGraph;
		m_FltSegGraph = NULL;
	}
	m_FltSegGraph = new CDXGraph();
	if (m_FltSegGraph->Create())
	{
		double duration;
		m_FltSegGraph->GetDuration(&duration); 
		double dobtmp = duration;
		strTemp.Format("%2.3f",dobtmp);		
		SetDlgItemText(IDC_EDITRESTIMET,strTemp);

		m_FltSegGraph->RenderFile(segName);
		m_FltSegGraph->SetDisplayWindow(m_VidSegWindow.GetSafeHwnd());
		m_FltSegGraph->SetNotifyWindow(this->GetSafeHwnd());
		m_FltSegGraph->Run();
		
	}
	bPlay = TRUE;	
	*pResult = 0;
}

void CStaticVideoDetectDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iItem;
	CRect itemrect;
	CWnd *m_wnd = GetDlgItem(IDC_STATICVID);
	m_wnd->GetWindowRect(itemrect);
	CString videoName;
	CString strTemp = "";
	videoName = m_List.GetItemText(nIndex,0);
	CreateGraph(videoName);
	CString playname = TruncPath(videoName);
	SetDlgItemText(IDC_EDITPLAYNAME,playname);
	double timePos;
	if (nIndex != -1)
	{		
		strTemp = m_List.GetItemText(nIndex,3); //发现时间
		if (strTemp == "")
			timePos = 0;
		timePos = atof(strTemp);
		double duration;
		m_FilterGraph->GetDuration(&duration); //单个数值用传引用方式
		m_VidTimeT = duration;
		strTemp.Format("%2.3f",duration);
		SetDlgItemText(IDC_EDITVIDTIMET,strTemp);
		m_FilterGraph->Run();
        //m_FilterGraph->SetCurrentPosition(timePos);
		bPlay = TRUE;
	}
	*pResult = 0;
}

void CStaticVideoDetectDlg::OnStaticseg()
{
	if (m_FltSegGraph)
	{
		bPause = !bPause;
		if(bPause)
		{
			m_FltSegGraph->Pause();
		}
		else
			m_FltSegGraph->Run();
	}
}

void CStaticVideoDetectDlg::OnStaticvid()
{
	if (m_FilterGraph)
	{
		bPause = !bPause;
		if(bPause)
		{
			m_FilterGraph->Pause();
		}
		else
			m_FilterGraph->Run();
	}
}

void CStaticVideoDetectDlg::OnBtndetect() 
{
	// TODO: Add your control notification handler code here
	m_xDetect.EnableWindow(0);
	
	AfxBeginThread((AFX_THREADPROC)StaticDetect, this, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
}

unsigned int CStaticVideoDetectDlg::StaticDetect(void *p)
{
	CStaticVideoDetectDlg *pDlg = (CStaticVideoDetectDlg* )p;
	
	int nVidNum = pDlg->m_List.GetItemCount();
	int nSegNum = pDlg->mVideoSeg.size();
	CString vidPath, segPath, strTemp, strTemp1;
	int frameDiff;
	float oriStaticTime = 0.0;
	unsigned char* fileData;
	BYTE MemTmp[188*100];
	pDlg->m_progress.SetRange(0,nVidNum-1);

	LPTSTR labFName=NULL;
	labFName=new TCHAR[pDlg->m_LabelFile.GetLength()+1];
	_tcscpy(labFName, pDlg->m_LabelFile);
	int lbdnum = pDlg->ReadLabelFile(labFName);
	delete []labFName;

	for(int i = 0;i<nVidNum;i++)
	{
		CVideoDataStatistic statDetec;
		CString sVideo = pDlg->m_List.GetItemText(i,0);		
		strTemp1 = pDlg->TruncPath(sVideo);
		if (strTemp1 == "CANAL_JADID_NCTV__20100823_101711_000.mpg")
		{
			int aa = 1;
		}
		for (int j = 0;j<lbdnum;j++)
		{
			strTemp = pDlg->mLabelData[j].vidFName;
			if (strTemp1.CompareNoCase(strTemp) == 0)
			{
				pDlg->bFindLabel = true;
				strTemp.Format("%2.3f",pDlg->mLabelData[j].timeStart);
				pDlg->m_List.SetItemText(i,2,strTemp);//标注静止起始时间			
				strTemp = "";

				strTemp.Format("%2.3f",pDlg->mLabelData[j].timeStop);
				pDlg->m_List.SetItemText(i,4,strTemp);//标注静止结束时间
 				strTemp = "";

				oriStaticTime = pDlg->mLabelData[j].timeStop - pDlg->mLabelData[j].timeStart;
				strTemp.Format("%2.3f",oriStaticTime);
				pDlg->m_List.SetItemText(i,6,strTemp);//标注静止时间
				pDlg->m_TotalStaticTime += oriStaticTime;
				strTemp = "";
				break;
			}
			else
				pDlg->bFindLabel = false;
		}
		if (pDlg->bFindLabel == false)  
		{			
			pDlg->m_List.SetItemText(i,2,"NoF");//没有标注静止起始时间
			pDlg->m_List.SetItemText(i,4,"NoF");//没有标注静止结束时间
 		}
		
		CString resFName = pDlg->m_strSavePath + "\\" + strTemp1;
		pDlg->m_List.SetItemText(i,1,resFName);//对应结果其文件名
		statDetec.SetResFName(sVideo,resFName);

// 		CFile file;
// 		int fileLen = 0;
// 		if (!file.Open(sVideo,CFile::modeRead))
// 		{
// 			AfxMessageBox("File Open Error!");
// 		}
// 		else
// 		{
// 			fileLen = file.GetLength();
// 			fileData = new unsigned char[fileLen];
// 			file.Read(fileData,fileLen);
// 			file.Close();
// 		}
		int startFrm = 0;
		int stopFrm = 0;
		statDetec.WriteStaticFileHeader(sVideo);
 		statDetec.GetStaticPos(startFrm,stopFrm);
		
// 		if ((startFrm !=-1)&&(stopFrm != -1)&&(stopFrm > startFrm))//片段中间静止
// 		{
// 			statDetec.ResFileWrite(0,startFrm);
// 			statDetec.ResFileWriteA(stopFrm,statDetec.mTotalFrame);
// 		}
// 		if ((startFrm !=-1)&&(stopFrm = -1))//片段尾部静止
// 		{
// 			statDetec.ResFileWrite(0,startFrm);
// 		}
		float frmrate = statDetec.GetFrameRate(sVideo);
		float timeStart = 0.0;
		float timeEnd   = 0.0;
		float findStaticTime = 0.0;
		pDlg->m_TotalTime = pDlg->m_TotalTime + statDetec.mTotalFrame*frmrate;
		if(abs(frmrate) > 0.0000001)
		{
			if (startFrm != -1)
			{
				timeStart = (float)startFrm/frmrate;
				strTemp.Format("%2.3f",timeStart);
				pDlg->m_progress.SetPos(i);
				pDlg->m_List.SetItemText(i,3,strTemp);//发现静止起始时间
				strTemp = "";
			}
			else
			{
				timeStart = (float)statDetec.mTotalFrame/frmrate;
				strTemp.Format("%2.3f",timeStart);
				pDlg->m_List.SetItemText(i,3,strTemp);//发现静止起始时间
				strTemp = "";
			}
			if (stopFrm != -1)
			{
				timeEnd = (float)stopFrm/frmrate;
				strTemp.Format("%2.3f",timeEnd);
				pDlg->m_List.SetItemText(i,5,strTemp);//发现静止结束时间
				strTemp = "";
			}
			else
			{
				timeEnd = (float)statDetec.mTotalFrame/frmrate;
				strTemp.Format("%2.3f",timeEnd);
				pDlg->m_List.SetItemText(i,5,strTemp);//发现静止结束时间
				strTemp = "";
			}

			findStaticTime = timeEnd - timeStart;
			strTemp.Format("%2.3f",findStaticTime);
			pDlg->m_List.SetItemText(i,7,strTemp);//发现静止总时间
			pDlg->m_FindStaticTime += findStaticTime;
			
			strTemp = "";
			strTemp.Format("%2.1f",frmrate);
			pDlg->m_List.SetItemText(i,8,strTemp);//帧速率

			strTemp = "";
			strTemp.Format("%1.2f",statDetec.m_threhold);
			pDlg->m_List.SetItemText(i,9,strTemp);//门限
		}
		
		if(pDlg->bFindLabel != false)
		{
			pDlg->m_TotalStaticTime = pDlg->m_TotalStaticTime + oriStaticTime;
			pDlg->m_FindStaticTime  = pDlg->m_FindStaticTime + findStaticTime;			
		}

	}

	pDlg->m_xDetect.EnableWindow(1);
// 	strTemp.Format("原有数目%d，发现数目%d，计算数目%d",pDlg->m_OriNum,pDlg->m_FindNum,pDlg->m_CalcNum);
// 	AfxMessageBox(strTemp);
	return 0;
}

int CStaticVideoDetectDlg::ReadLabelFile(char *strFilePath)
{
	labeldata lbd;
	std::fstream fs;
	
	fs.open(strFilePath,std::ios::in);
	if (!fs) 
	{
		return false;
	}

	float floTmp = -1.1;
	std::string strtmp;
	CString     cstrTemp;

	
	while( !fs.eof() )
	{
		fs >> strtmp;
		cstrTemp = strtmp.c_str();
		if (cstrTemp.Find("mpg"))
		{
			//bFind = true;
			lbd.vidFName = cstrTemp;
			fs >> strtmp;
			sscanf(strtmp.c_str(), "%f", &floTmp);
			lbd.timeStart = floTmp;
			floTmp = -1.1;
			
			fs >> strtmp;
			sscanf(strtmp.c_str(), "%f", &floTmp);
			lbd.timeStop = floTmp;
			floTmp = -1.1;
			
			mLabelData.push_back(lbd);			
		}
		else
			break;
	} 
	
	int intTemp = mLabelData.size();
	return intTemp;
}



void CStaticVideoDetectDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString strTemp = "";
	float fltemp = 0.0;
	
	if (bTimer && m_FilterGraph)
	{
		double pos = 0, duration = 1.;
		m_FilterGraph->GetCurrentPosition(&pos);
		strTemp.Format("%2.3f",pos);
		SetDlgItemText(IDC_EDITVIDTIME,strTemp);
		strTemp = "";		
		int newPos = int(pos * 1000 / m_VidTimeT);
		if (m_SliderGraph.GetPos() != newPos)
		{
			m_SliderGraph.SetPos(newPos);
		}
	}

	if (bTimer && m_FltSegGraph)
	{
		double pos = 0, duration = 1.;
		m_FltSegGraph->GetCurrentPosition(&pos);
		strTemp.Format("%2.3f",pos);
		SetDlgItemText(IDC_EDITRESTIME,strTemp);
		strTemp = "";		
		int newPos = int(pos * 1000 / m_VidTimeT);
		if (m_SliderSeg.GetPos() != newPos)
		{
			m_SliderSeg.SetPos(newPos);
		}
	}

	if (bTimer && m_FindStaticTime && m_TotalStaticTime)
	{
		strTemp.Format("%2.3f",m_FindStaticTime);
		strTemp += "秒";
		SetDlgItemText(IDC_EDITFINDSTATIC,strTemp);
		strTemp = "";

		strTemp.Format("%2.3f",m_TotalStaticTime);
		strTemp += "秒";
		SetDlgItemText(IDC_EDITORISTAIC,strTemp);
		strTemp = "";

		float fltemp = (float)m_FindStaticTime/m_TotalStaticTime;
		strTemp.Format("%2.3f",fltemp*100);
		strTemp += "%";
		SetDlgItemText(IDC_EDITRECALL,strTemp);
		strTemp = "";
	}

	if (bTimer && m_TotalTime)
	{
		strTemp.Format("%2.0f",m_TotalTime);
		strTemp += "秒";
		SetDlgItemText(IDC_EDITTOTALTIME,strTemp);
		strTemp = "";
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CStaticVideoDetectDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar->GetSafeHwnd() == m_SliderGraph.GetSafeHwnd())
	{
		if (m_FilterGraph)
		{
			// Calculate the current seeking position
			double duration = 1.;
			m_FilterGraph->GetDuration(&duration);
			double pos = duration * m_SliderGraph.GetPos() / 1000.;
			m_FilterGraph->SetCurrentPosition(pos);
		}	
		if (m_FilterGraph)
		{
			// Calculate the current seeking position
			double duration = 1.;
			m_FltSegGraph->GetDuration(&duration);
			double pos = duration * m_SliderSeg.GetPos() / 1000.;
			m_FltSegGraph->SetCurrentPosition(pos);
		}	
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
