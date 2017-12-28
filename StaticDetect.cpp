// StaticDetect.cpp: implementation of the CStaticDetect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StaticVideoDetect.h"
#include "StaticDetect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PSPACKETSIZE	204
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticDetect::CStaticDetect()
{

}

CStaticDetect::~CStaticDetect()
{

}

void CStaticDetect::SearchGroupHead(CString segname, int *group_head_pos)
{
	std::vector<int> groupHeadVec;
	std::vector<int>::iterator iter;
	unsigned char* MemData;	
	unsigned char _FrameType;
	int filelen = 0;
	if (!file.Open(segname,CFile::modeRead|CFile::shareDenyWrite))
	{
		AfxMessageBox("file open error!");
	}
	else
		filelen = file.GetLength();
	MemData = new unsigned char[filelen];		
	file.Read(MemData,filelen);
	file.Close();

	int  i,j,k,l;
	char pes_data_header_length,syncpos;
	k=i=0;
	syncpos=-1;
	do 
	{
		if((MemData[i]==0x47)&&(MemData[i+PSPACKETSIZE]==0x47)&&(MemData[i+PSPACKETSIZE*2]==0x47)) 
			syncpos = i;
		else
			i+=PSPACKETSIZE;
	} while(syncpos<0);
	for(j=0;j<filelen-PSPACKETSIZE*2;j=j+PSPACKETSIZE) 
	{ 
		l=j+syncpos+4;	
		if (MemData[l]==0x00&&MemData[l+1]==0x00&&
			MemData[l+2]==0x01&&MemData[l+3]==0xe0)
		{
			pes_data_header_length = MemData[l+8];
			l=l+8+1;

			if (MemData[l+pes_data_header_length+88]==0x00&&MemData[l+pes_data_header_length+89]==0x00&&
				MemData[l+pes_data_header_length+90]==0x01&&MemData[l+pes_data_header_length+91]==0xb8)
			{
				groupHeadVec.push_back(l);
				k++;
			}			
		}		
	}
	delete []MemData;

	k=0;
	for (iter = groupHeadVec.begin(); iter !=  groupHeadVec.end(); iter++) 
	{
		group_head_pos[k] = *iter;
		k++;
	}
	groupHeadVec.clear();
}

