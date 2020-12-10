#pragma once



#include "WorkUnits.h"
#include"TestDlg.h"
#include"VisualArea.h"
// ComputingServiceWorker

class ComputingServiceWorker : public CWinThread
{
	DECLARE_DYNCREATE(ComputingServiceWorker)

protected:
	ComputingServiceWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~ComputingServiceWorker();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	TestDlg* pTestBoard;
	CClientDC* pClient;
	//CArray<OpticCell*>* pOpticCellArray;
	VisualArea* pVisualArea;
	myMath* pSliceMath;


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ComputingService_LinesSlice(WorkUnits* pWorkUnits);
};


