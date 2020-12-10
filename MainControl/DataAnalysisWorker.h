#pragma once
#include "DataPacket.h"

#include "ComputingManagement.h"
// DataAnalysisWorker

class DataAnalysisWorker : public CWinThread
{
	DECLARE_DYNCREATE(DataAnalysisWorker)

protected:
	DataAnalysisWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~DataAnalysisWorker();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void init(CPtrArray* presourcearray);
protected:
	DECLARE_MESSAGE_MAP()
public:
	int isWorking;
	ComputingManagement* pComputingManagement;
	CPtrArray* pResourceArray;
	DataPacket* pDataPacket;
	BYTE* pSmoothBuf;
	ImageDataSheet* m_pIDS;
	CClientDC* pClientDC;
	
	int DeelWorkload;
	void DeelPic(Picture* pPic);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	float CalDeltaTime(const SYSTEMTIME* pPre, const SYSTEMTIME* pCur);
	SYSTEMTIME BeginTime;
	double PreTime;


};


