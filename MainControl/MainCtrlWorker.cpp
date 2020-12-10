// MainCtrlWorker.cpp: 实现文件
//

#include "pch.h"
#include "MainControl.h"
#include "MainCtrlWorker.h"

// MainCtrlWorker

#include "LineCameraWorker.h"
#include "DataAnalysisWorker.h"
#include "DefectManagementWorker.h"
IMPLEMENT_DYNCREATE(MainCtrlWorker, CWinThread)
//extern CPtrArray* pGlobalResourceArray;
MainCtrlWorker::MainCtrlWorker()
{
	isWorking = 0;
	isGrab = -1;
	pResourceArray = 0;
	pDataPacket = new DataPacket;
	ShowMode = 2;
}

MainCtrlWorker::~MainCtrlWorker()
{

}

BOOL MainCtrlWorker::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	return TRUE;
}

int MainCtrlWorker::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}
//time_t tt0=0,tt1;
void MainCtrlWorker::DrawScreen()
{

	//LinesSliceArray[CurrentLinesSlicePos];
	LinesSlice* pLinesSlice;
	int i;
	for (i = 0; i < 1000; ++i)
	{
		pLinesSlice=pDataPacket->GetLinesSliceReverse(i); 
		if (pLinesSlice)
		{
			pLinesSlice->SetShowMode(ShowMode);
			pLinesSlice->draw((byte*)pScr->GetPixelAddress(0, i));
		}
	}

	SetStretchBltMode(pViewDC->GetSafeHdc(), HALFTONE);
	pScr->Draw(pViewDC->GetSafeHdc(), 0, 0, 1000, 750);
	
//	CString s; s.Format("w	%lf", pDataPacket->AverageSliceWidth);
//	pViewDC->TextOut(1000,0,s);
//	s.Format("w	%lf", pDataPacket->AverageSliceWidth_half);
//	pViewDC->TextOut(1000, 40, s);

//	s.Format("w	%lf", pDataPacket->TripleStandardDeviation);
//	pViewDC->TextOut(1000, 80, s);
}
void MainCtrlWorker::DrawImage(CImage* pImage,int pos)
{
	if (!pImage)return;

	pImage->Draw(hdc_Scr,0,pos*(pImage->GetHeight()-1),pImage->GetWidth(),pImage->GetHeight()-1 ,0, 0, pImage->GetWidth(), pImage->GetHeight() - 1);
	
}

BEGIN_MESSAGE_MAP(MainCtrlWorker, CWinThread)
END_MESSAGE_MAP()


// MainCtrlWorker 消息处理程序


BOOL MainCtrlWorker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		{//？？？？？跟keydown传flag有什么区别
			UCHAR* p = (UCHAR*)&pMsg->lParam;
			KeyDown(p[2]);
			break;
		}
		case MSG_drawScreen:
		{
			DrawScreen();
			break;
		}
		case MSG_SetMainCtrlWorker:
		{
			init((CView*)pMsg->wParam);
			break;
		}
		default:
			break;
	}


	return CWinThread::PreTranslateMessage(pMsg);
}
void MainCtrlWorker::KeyDown(UCHAR nFlags)
{
	switch (nFlags)
	{
		case 47://v
		{
			if (isWorking == 0)break;
			if (isGrab == -1)break;
			CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_LineCameraWorker);
			if(pth)pth->PostThreadMessage(MSG_AskImage, (WPARAM)pDataAnalysisWorker, 0);
			break;
		}
		case 57://空格
		{
			if (isWorking == 0)break;
			isGrab *= -1;
			//CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_LineCameraWorker);
			//if (pth)pth->PostThreadMessage(MSG_LineCameraSwitch, (WPARAM)isGrab, 0);
			break;
		}
		case 31://s
		{
			if (isWorking == 0)break;
			CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_DataAnalysisWorker);
			if (pth)pth->PostThreadMessage(MSG_SavePictureData, 0, 0);
			break;
		}
		case 37://k
		{
			if (isWorking == 0)break;
			pScr->Save("d:\\001.png");
			break;
		}
		default:
		{
			if (nFlags > 1 && nFlags < 4)
			{
				ShowMode = nFlags;
			}
			//DebugPrintMsgBox("%d", nFlags);
			break;
		}

	}
}
//CPtrArray* pGlobalResourceArray;
void MainCtrlWorker::init(CView* pView)
{
	pResourceArray = new CPtrArray;
	pGlobalResourceArray = pResourceArray;
	pMainView = pView;
	pDataPacket->pResourceArray = pResourceArray;

	pViewDC = new CClientDC(pView);
	pScr = new CImage;
	pScr->Create(2048, PictureHight * NumberOfPictures, 24);

	hdc_Scr = pScr->GetDC();
	hbr = CreateSolidBrush(RGB(0, 0, 255));//delete?
	pResourceArray->SetSize(ResourceArraySize);

	pResourceArray->SetAt(Pos_MainView, pMainView);
	pResourceArray->SetAt(Pos_DataPacket, pDataPacket);
	pResourceArray->SetAt(Pos_MainCtrlWorker, this);

	pLineCameraWorker = AfxBeginThread(RUNTIME_CLASS(LineCameraWorker));
	pResourceArray->SetAt(Pos_LineCameraWorker, pLineCameraWorker);

	pDataAnalysisWorker = AfxBeginThread(RUNTIME_CLASS(DataAnalysisWorker));
	pResourceArray->SetAt(Pos_DataAnalysisWorker, pDataAnalysisWorker);

	pDefectManagementWorker = AfxBeginThread(RUNTIME_CLASS(DefectManagementWorker));
	pResourceArray->SetAt(Pos_DefectManagementWorker, pDefectManagementWorker);

#ifdef _M_X64

#else
	pPlcCtrl = new PlcCtrl;
	pResourceArray->SetAt(Pos_PLC, pPlcCtrl);
#endif

	//====================================================

//=====================================================
	Sleep(100);
	pDefectManagementWorker->PostThreadMessage(MSG_SetDefectManagementWorker,0,0);
	pLineCameraWorker->PostThreadMessage(MSG_SetLineCameraWorker, (WPARAM)pResourceArray, 0);
	pDataAnalysisWorker->PostThreadMessage(MSG_SetDataAnalysisWorker, (WPARAM)pResourceArray, 0);
	((CWinThread*)pResourceArray->GetAt(Pos_DefectManagementWorker))->PostThreadMessageA(MSG_SetInitialLight, 0, 0);

	//=======================================
	//CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_LineCameraWorker);
	//pth->PostThreadMessage(MSG_AskImage, (WPARAM)this, 0);
	isWorking = 1;
	

}
