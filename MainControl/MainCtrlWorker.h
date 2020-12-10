#pragma once

// MainCtrlWorker
#include "DataPacket.h"

class MainCtrlWorker : public CWinThread
{
	DECLARE_DYNCREATE(MainCtrlWorker)

protected:
	MainCtrlWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~MainCtrlWorker();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();



protected:
	DECLARE_MESSAGE_MAP()
public:
	int isGrab;
	int isWorking;
	CPtrArray* pResourceArray;
	CWinThread* pLineCameraWorker;
	CWinThread* pDataAnalysisWorker;
	CWinThread* pDefectManagementWorker;
	CView* pMainView;
	int ShowMode;

	DataPacket* pDataPacket;
	CImage* pScr;
	CDC* pScrCDC;
	CClientDC* pViewDC;
	HBRUSH hbr;
	HDC hdc_Scr;
	//============================
	const int PictureHight = 100;
	const int NumberOfPictures = 10; 
	void DrawScreen();
	void DrawImage(CImage* pImage, int pos=0);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void KeyDown(UCHAR nFlags);
	void init(CView*);
	//===================================
#ifdef _M_X64
#else
	PlcCtrl* pPlcCtrl;//64位注释掉
#endif 
};


