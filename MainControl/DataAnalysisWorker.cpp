// DataAnalysisWorker.cpp: 实现文件
//

#include "pch.h"
#include "MainControl.h"
#include "DataAnalysisWorker.h"


// DataAnalysisWorker
#include "LinesSlice.h"
IMPLEMENT_DYNCREATE(DataAnalysisWorker, CWinThread)

DataAnalysisWorker::DataAnalysisWorker()
{
	isWorking = 0;
	pComputingManagement = 0;
}

DataAnalysisWorker::~DataAnalysisWorker()
{

}

BOOL DataAnalysisWorker::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	
	return TRUE;
}

int DataAnalysisWorker::ExitInstance()
{

	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

void DataAnalysisWorker::init(CPtrArray* presourcearray)
{
	pResourceArray = presourcearray;
	pDataPacket = (DataPacket*)pResourceArray->GetAt(Pos_DataPacket);
	pComputingManagement = new ComputingManagement;
	pSmoothBuf = new BYTE[2048];
	isWorking = 1;
	DeelWorkload = 10;//通过自动调整，逐渐减少，增加负载
	//pClientDC = new CClientDC((CView*)pGlobalResourceArray->GetAt(Pos_MainView));
}

BEGIN_MESSAGE_MAP(DataAnalysisWorker, CWinThread)
END_MESSAGE_MAP()


// DataAnalysisWorker 消息处理程序


void DataAnalysisWorker::DeelPic(Picture* pPic)
{
	CImage* pI = pPic->pCImage;
	int n = pI->GetHeight() - 1;
	int i;
	DeelWorkload += pComputingManagement->GetTaskStress();
	if (DeelWorkload < 1)DeelWorkload = 1;
//	DebugPrintCDC("%d\n", DeelWorkload);
	//CDC*pCDC=((CView*)pGlobalResourceArray->GetAt(Pos_MainView))->GetDC();
	//CString s;
	//s.Format("%d", DeelWorkload);
	//pCDC->TextOutA(1000, 120, s);
	//pCDC->DeleteDC();
	
	LinesSlice* pLinesSlice;
	byte* pbuf;
	int width = pI->GetWidth();
	ImageDataSheet* pIDS =(ImageDataSheet*) pI->GetPixelAddress(0,n);
	
	double DeltaSlidePos, DeltaSpindlePos, EverySlidePos, EverySpindlePos, Timing,DeltaTime,EveryTime;

	
	
	if (pPic->ID == 0)
	{
		m_pIDS = pIDS;
		BeginTime = pIDS->time;
		DeltaSlidePos = DeltaSpindlePos = EverySlidePos = EverySpindlePos = Timing = DeltaTime = PreTime = EveryTime = 0;
	}
	else 
	{
		DeltaSlidePos = pIDS->SlidePosition - m_pIDS->SlidePosition;
		DeltaSpindlePos = pIDS->SpindlePosition - m_pIDS->SpindlePosition;
		Timing = CalDeltaTime( &BeginTime, & pIDS->time);
		DeltaTime = Timing - PreTime;
		EveryTime = DeltaTime / Const_linesPerPicture;		
		EverySlidePos = DeltaSlidePos / Const_linesPerPicture;
		EverySpindlePos = DeltaSpindlePos / Const_linesPerPicture;
	}

	for (i = 0; i <n; ++i)
	{
		pbuf = (byte*)pI->GetPixelAddress(0, i);
		if (pDataPacket->LinesSliceIDcount == 0)
			memcpy(pSmoothBuf, pbuf, width);
		else
			for (int i = 0; i < width; ++i)pSmoothBuf[i] = pbuf[i] * 0.05 + pSmoothBuf[i] * 0.95;

		//-----------------------------
		if (i % DeelWorkload == 0)
		{
			pLinesSlice = new LinesSlice(pSmoothBuf, width, pDataPacket->LinesSliceIDcount,PreTime+(i+1)*EveryTime, m_pIDS->SpindlePosition + (i + 1) * EverySpindlePos, m_pIDS->SlidePosition + (i + 1) * EverySlidePos/*,pClientDC*/);
			//DebugPrintf(" %4d time:%12f Spindle:%12f Slide:%12f\n", i, PreTime + (i + 1) * EveryTime, m_pIDS->SpindlePosition+(i + 1) * EverySpindlePos, m_pIDS->SlidePosition+(i + 1) * EverySlidePos);
			//DebugPrintf("%4d time:%f Spindle:%f Slide:%f\n", i, pLinesSlice->Time, pLinesSlice->SpindlePosition, pLinesSlice->SlidePosition);
			pDataPacket->AddLinesSlice(pLinesSlice);
			WorkUnits* pWorkUnits = new WorkUnits(MSG_ComputingService_LinesSlice, pLinesSlice, (CWinThread*)pResourceArray->GetAt(Pos_DataAnalysisWorker));
			pComputingManagement->AddWorkUnits(pWorkUnits);
		}

	}
	//DebugPrintf(" time:%f,time:%02d:%02d:%02d:%03d  Spindle:%f Slide:%f\n", Timing, pIDS->time.wHour, pIDS->time.wMinute, pIDS->time.wSecond, pIDS->time.wMilliseconds,pIDS->SpindlePosition, pIDS->SlidePosition);
	if (pIDS->SlidePosition> m_pIDS->SlidePosition)
		pDataPacket->SlideDirection = 1;
	if (pIDS->SlidePosition == m_pIDS->SlidePosition)
		pDataPacket->SlideDirection = 0;
	if (pIDS->SlidePosition < m_pIDS->SlidePosition)
		pDataPacket->SlideDirection = -1;

	m_pIDS = pIDS;
	PreTime = Timing;
}

BOOL DataAnalysisWorker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case MSG_SetDataAnalysisWorker:
		{
			init((CPtrArray*)pMsg->wParam);
			break;
		}
		case MSG_CarryingImage:
		{
			Picture* p = new Picture((CImage*)pMsg->wParam);
			if (!pDataPacket->AddPicture(p))
			{//注释掉的是拍摄用代码
			//	CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_LineCameraWorker);
			//	if (pth)pth->PostThreadMessage(MSG_LineCameraSwitch, 0, 0);
			}
			DeelPic(p);
			//====拍摄打开


		//	CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_MainCtrlWorker);
		//	if (pth)pth->PostThreadMessage(MSG_drawScreen, 0, 0);


			break;
		}
		case MSG_ComputingService_complete:
		{
			WorkUnits* p=(WorkUnits*)pMsg->wParam;
			LinesSlice* pL = (LinesSlice*)p->pData;
			pL->Flag = 1;
			pComputingManagement->CompleteWorkUnits(p, (CWinThread*)pMsg->lParam);
			pDataPacket->LineGrowUp();
			break;
		}
		case MSG_SavePictureData:
		{
			for (int i = 0; i < pDataPacket->CurrentPicPos; ++i)
				pDataPacket->PictureArray[i]->SaveImage();
			break;
		}
	}
	return CWinThread::PreTranslateMessage(pMsg);
}

float DataAnalysisWorker::CalDeltaTime(const SYSTEMTIME* pPre, const SYSTEMTIME* pCur)
{
	CTime TimeOne(*pPre);
	CTime TimeTwo(*pCur);
	CTimeSpan DeltaTime = TimeTwo - TimeOne;
	int DeltaMiliSeconds = pCur->wMilliseconds - pPre->wMilliseconds;
	int Delta[] = { DeltaTime.GetDays() ,DeltaTime.GetHours() ,DeltaTime.GetMinutes() ,DeltaTime.GetSeconds(),DeltaMiliSeconds };
	int binary[] = { 24,60,60,60,1000 };
	for (int i = 4; i >= 1; --i)
	{
		if (Delta[i] < 0)
		{
			Delta[i] += binary[i];
			Delta[i - 1] -= 1;
		}
	}
	int total = 0;

	total += Delta[0] * 86400000 + Delta[1] * 3600000 + Delta[2] * 60000 + Delta[3] * 1000 + Delta[4];

	return total;
}
