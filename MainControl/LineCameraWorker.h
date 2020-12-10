#pragma once

#include <atlimage.h>
#include "SapClassBasic.h"
#include "SapClassGui.h"


#ifdef _M_X64

#pragma comment(lib,".//win64//SapClassGui.lib")
#pragma comment(lib,".//win64//SapClassBasic.lib")
#else

#pragma comment(lib,".//win32//SapClassGui.lib")
#pragma comment(lib,".//win32//SapClassBasic.lib")
//
//#include "..\\..\\PLCControl\\PLCControl\\PlcCtrl.h"//64位注释掉
//#pragma comment(lib,"..\\..\\PLCControl\\Release\\PLCControl.lib")
#endif // COMPILE
// LineCameraWorker

class LineCameraWorker : public CWinThread
{
	DECLARE_DYNCREATE(LineCameraWorker)

protected:
	LineCameraWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~LineCameraWorker();


public:
	virtual BOOL InitInstance();
	int init();
	virtual int ExitInstance();

	BOOL CreateObjects();
	BOOL DestroyObjects();
	static void XferCallback(SapXferCallbackInfo* pInfo);
protected:
	DECLARE_MESSAGE_MAP()

protected:
	int isWorking;
	int isGrab;
	RGBQUAD* colorTable;

	CPtrArray* pResourceArray;
	CImageExWnd    m_ImageWnd;
	SapAcqDevice* m_AcqDevice;
	SapBuffer* m_Buffers;
	SapTransfer* m_Xfer;
	SapView* m_View;
#ifdef _M_X64
#else
	PlcCtrl* pPlcCtrl;//64位注释掉
#endif 
//====硬盘调试========================================
	CArray<CString> imagePath;
	int diskimagepos = 0;
	CString diskimagepath = "d:\\";
	void initDiskImagePath();
	//================================
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CImage* GetPicture();
};


