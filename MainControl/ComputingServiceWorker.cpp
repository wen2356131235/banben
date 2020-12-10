// ComputingServiceWorker.cpp: 实现文件
//

#include "pch.h"
#include "MainControl.h"
#include "ComputingServiceWorker.h"
#include "LinesSlice.h"
// ComputingServiceWorker

IMPLEMENT_DYNCREATE(ComputingServiceWorker, CWinThread)

ComputingServiceWorker::ComputingServiceWorker()
{
}

ComputingServiceWorker::~ComputingServiceWorker()
{
	delete pSliceMath;
}

BOOL ComputingServiceWorker::InitInstance()
{
	pSliceMath = new myMath;
	double x[100];
	for (int i = 0; i < 100; ++i)
	{
		x[i] = i;
	}
	for (int i = 5; i < 100; ++i)
	{
		pSliceMath->AddFittingLineInput(x, i, 3);
	//	DebugPrintf("%d\n", ffff);
	}

	
	//=================对话框调试===================
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//pTestBoard = new TestDlg;
	//pTestBoard->Create(IDD_TestDlg);
	//pTestBoard->ShowWindow(SW_SHOW);
	//CRect* rect;
	//rect = new CRect;
	//rect->left = 0;
	//rect->top = 0;
	//rect->bottom = 700;
	//rect->right = 2100;
	//pTestBoard->MoveWindow(rect);
	//delete rect;

	//pClient = new CClientDC(pTestBoard);
	//=================================================
	// TODO:    在此执行任意逐线程初始化

	pVisualArea = new VisualArea;
	pVisualArea->CreateOpticalNeuralNetwork(2048);
	return TRUE;
}

int ComputingServiceWorker::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(ComputingServiceWorker, CWinThread)
END_MESSAGE_MAP()


// ComputingServiceWorker 消息处理程序


BOOL ComputingServiceWorker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case MSG_ComputingService_LinesSlice:
		{
			WorkUnits* p=(WorkUnits*)pMsg->wParam;
			ComputingService_LinesSlice(p);
		
			p->pWinThread->PostThreadMessage(MSG_ComputingService_complete, pMsg->wParam, (LPARAM)this);
		
			break;
		}
	}
	return CWinThread::PreTranslateMessage(pMsg);
}
void ComputingServiceWorker::ComputingService_LinesSlice(WorkUnits* pWorkUnits)
{
//	int n = rand() % 10000000;
//	for (int i = 0; i < 10000000+n; i++);
	LinesSlice * pLinesSlice=(LinesSlice*)pWorkUnits->pData;
	//int MAX = 0;
	//for (int i = 0; i < pLinesSlice->BufferSize; ++i)
	//{
	//	if (pLinesSlice->pBuffer[i] > MAX)
	//		MAX = pLinesSlice->pBuffer[i];
	//}
	
	pLinesSlice->pSliceMath = pSliceMath;
	pLinesSlice->pClient = pClient;
	pLinesSlice->pTestBoard = pTestBoard;
	pLinesSlice->deel(pVisualArea->pOpticalNeuralNetwork);
	//pLinesSlice->LinesSliceVisualization(pClient,pTestBoard); 可视化
}