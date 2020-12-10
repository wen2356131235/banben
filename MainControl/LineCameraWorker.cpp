// LineCameraWorker.cpp: 实现文件
//

#include "pch.h"
#include "MainControl.h"
#include "LineCameraWorker.h"


// LineCameraWorker

IMPLEMENT_DYNCREATE(LineCameraWorker, CWinThread)

LineCameraWorker::LineCameraWorker()
{
	pResourceArray = 0;

}

BOOL LineCameraWorker::CreateObjects()
{
	//CWaitCursor wait;
	// Create acquisition object
	if (m_AcqDevice && !*m_AcqDevice && !m_AcqDevice->Create() && !m_AcqDevice->Create())
	{
		DestroyObjects();
		return FALSE;
	}
	// Create buffer object
	if (m_Buffers && !*m_Buffers)
	{
		if (!m_Buffers->Create())
		{
			DestroyObjects();
			return FALSE;
		}
		// Clear all buffers
		m_Buffers->Clear();
	}
	// Set next empty with trash cycle mode for transfer
	if (m_Xfer && m_Xfer->GetPair(0))
	{
		if (!m_Xfer->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
		{
			DestroyObjects();
			return FALSE;
		}
	}
	// Create transfer object
	if (m_Xfer && !*m_Xfer && !m_Xfer->Create())
	{
		DestroyObjects();
		return FALSE;
	}
	return TRUE;
}
BOOL LineCameraWorker::DestroyObjects()
{
	// Destroy transfer object
	if (m_Xfer && *m_Xfer) m_Xfer->Destroy();

	// Destroy buffer object
	if (m_Buffers && *m_Buffers) m_Buffers->Destroy();

	// Destroy acquisition object
	if (m_AcqDevice && *m_AcqDevice) m_AcqDevice->Destroy();

	return TRUE;
}
void LineCameraWorker::XferCallback(SapXferCallbackInfo* pInfo)
{
	LineCameraWorker* pLineCameraWorker = (LineCameraWorker*)pInfo->GetContext();

	//// If grabbing in trash buffer, do not translate the image, update the
	//// appropriate number of frames on the status bar instead
	if (pInfo->IsTrash())
	{
		//	DebugPrintf("Frames acquired in trash buffer: %d", pInfo->GetEventCount());

	}
	else
	{
		CImage* pImage = pLineCameraWorker->GetPicture();
		if (pLineCameraWorker->pResourceArray)
		{
			CWinThread* pth = (CWinThread*)pLineCameraWorker->pResourceArray->GetAt(Pos_DataAnalysisWorker);
			pth->PostThreadMessage(MSG_CarryingImage, (WPARAM)pImage, 0);
		}
	}
}
LineCameraWorker::~LineCameraWorker()
{
}

BOOL LineCameraWorker::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化

	return TRUE;
}
int LineCameraWorker::init()
{

	initDiskImagePath();

	isWorking = 0;
	isGrab = 0;

#ifdef _M_X64

#else
	pPlcCtrl = new PlcCtrl;
//	pPlcCtrl=(PlcCtrl*)pGlobalResourceArray->GetAt(Pos_PLC);

#endif
	
	return TRUE;

	char pCurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pCurrentPath);
	CString currentPath(pCurrentPath);
	CString filePath = currentPath + "\\T_Linea_M2048-7um_Default_Default.ccf";

	SapLocation location(1, "Linea_M2048-7um_1", 0);
	char* configFile = filePath.GetBuffer();
	filePath.ReleaseBuffer();

	m_AcqDevice = new SapAcqDevice(location, configFile);
	m_Buffers = new SapBufferWithTrash(2, m_AcqDevice);
	m_Xfer = new SapAcqDeviceToBuf(m_AcqDevice, m_Buffers, XferCallback, this);
	//	m_AcqDevice->SetConfigFile()

		// Create all objects
	if (!CreateObjects()) { AfxMessageBox("初始化相机失败！"); return 0; }

	colorTable = new RGBQUAD[256];
	for (int i = 0; i < 256; i++)colorTable[i].rgbRed = colorTable[i].rgbGreen = colorTable[i].rgbBlue = i;

	isWorking = 1;
	//if (m_Xfer->Grab())isGrab = 1;
	return 1;
}
int LineCameraWorker::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(LineCameraWorker, CWinThread)
END_MESSAGE_MAP()


// LineCameraWorker 消息处理程序



BOOL LineCameraWorker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case MSG_SetLineCameraWorker:
		{
			pResourceArray = (CPtrArray*)pMsg->wParam;
			init();
			break;
		}
		case MSG_AskImage:
		{
			CImage* pImage = GetPicture();
			CWinThread* pth = (CWinThread*)pMsg->wParam;
			pth->PostThreadMessage(MSG_CarryingImage, (WPARAM)pImage, 0);
			break;
		}
		case MSG_LineCameraSwitch:
		{
			isGrab = (int)pMsg->wParam;
			if (isGrab ==1)
			{
				m_Xfer->Grab();
			//	isGrab = 1;
			//	DebugPrintMsgBox("start");
			}
			else
			{
				m_Xfer->Abort();
			//	isGrab = -1;
			//	DebugPrintMsgBox("end");
			}
			break;
		}
	}
	return CWinThread::PreTranslateMessage(pMsg);
}



CImage* LineCameraWorker::GetPicture()
{
	CImage* pImage = new CImage;
	//======硬盘读入图像===================
	if (1)
	if (isWorking == 0)
	{
	
		//diskimagepos
		pImage->Load(imagePath[diskimagepos]);
		diskimagepos++;
		diskimagepos %= imagePath.GetSize();
	
		//DebugPrintf("%d	\n", diskimagepos);
		return pImage;
	}
	//======硬盘读入图像结束==================================

	int srcWidth = m_Buffers->GetWidth(), srcHeight = m_Buffers->GetHeight();
	int size = srcWidth * srcHeight;

	int srcPitch = m_Buffers->GetPitch();

	pImage->Create(srcWidth, -(srcHeight + 1), 8); //多出来的那一行用来存储时间、主轴位置等信息
	int destHeight = pImage->GetHeight();
	int destPitch = pImage->GetPitch();
	int destBytePerPixel = pImage->GetBPP() / 8;

	byte* pDestBuffer = (byte*)pImage->GetBits();
	if (pImage->GetPitch() < 0)
		pDestBuffer = pDestBuffer + pImage->GetPitch() * (pImage->GetHeight() - 1);

	byte* pSrcBuffer;
	m_Buffers->GetAddress((void**)&pSrcBuffer);
	memcpy(pDestBuffer, pSrcBuffer, abs(srcPitch * srcHeight));
	int MAX = 0;
	for(int i=0;i<pImage->GetWidth();++i)
		for (int j = 0; j < pImage->GetHeight(); ++j)
		{
			if(pDestBuffer[i + j * destPitch]>MAX)
			MAX= pDestBuffer[i + j * destPitch];
		}
	((CWinThread*)pGlobalResourceArray->GetAt(Pos_DefectManagementWorker))->PostThreadMessageA(MSG_LightAdjust, (WPARAM)MAX, 0);
	//CDC* pCDC = ((CView*)pGlobalResourceArray->GetAt(Pos_MainView))->GetDC();
	//CString s;
	//s.Format("%d", MAX);
	//pCDC->TextOutA(1000, 140, s);
	//pCDC->DeleteDC();
	pImage->SetColorTable(0, 256, colorTable);

	ImageDataSheet* pImageDataSheet = (ImageDataSheet*)(pDestBuffer + size);
	GetLocalTime(&(pImageDataSheet->time));

#ifdef _M_X64

#else
	pImageDataSheet->SpindlePosition = pPlcCtrl->ReadSpindlePosition();
	pImageDataSheet->SlidePosition = pPlcCtrl->ReadSlidePosition();
#endif
	return pImage;
}

void LineCameraWorker::initDiskImagePath()
{
	diskimagepath = "D:\\work\\五五所光纤缺陷检测\\TestPicture202009251053\\测试图片1\\";
	//diskimagepath = "E:\\光纤\\picture202007081102\\";
	//diskimagepath = "D:\\work\\20200309五五所光纤缺陷检测\\上周五拍摄的图片TestPicture202009251053\\测试图片1\\";
	
	int work = 0;
	CFileFind finder;
	work = finder.FindFile(diskimagepath + "*.png");
	/*if (work)imagePath.Add(finder.GetFilePath());*/

 	while (work)
	{
		work = finder.FindNextFile();
		if (work)imagePath.Add(finder.GetFilePath());
	}
	//for (int i = 0; i < imagePath.GetSize(); i++)
	//	DebugPrintf(imagePath[i] + "\n");
}