// DefectManagementWorker.cpp: 实现文件
//

#include "pch.h"
#include "MainControl.h"
#include "DefectManagementWorker.h"
#include "AlarmingData.h"

// DefectManagementWorker

IMPLEMENT_DYNCREATE(DefectManagementWorker, CWinThread)

DefectManagementWorker::DefectManagementWorker()
{
	LightFlag = 0;
	
#ifdef _M_X64
#else
	pPlcCtrl = 0;
#endif
}

DefectManagementWorker::~DefectManagementWorker()
{
	delete pWLightCtrl;
}

BOOL DefectManagementWorker::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	pWLightCtrl = new WLightCtrl;
	LightValue = 50;
	Step = 1;
	Timer100=SetTimer(0, 0, 20, 0);

	PixelMax = -1;
//	DebugPrintf("xx:%x\n", Timer100);
	return TRUE;
}

int DefectManagementWorker::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(DefectManagementWorker, CWinThread)
END_MESSAGE_MAP()


// DefectManagementWorker 消息处理程序

void DefectManagementWorker::init()
{
#ifdef _M_X64
#else
//	pPlcCtrl = (PlcCtrl*)pGlobalResourceArray->GetAt(Pos_PLC);
	pPlcCtrl = new PlcCtrl;
#endif
	CView * p=(CView*)pGlobalResourceArray->GetAt(Pos_MainView);
	pViewDC = new CClientDC(p);
}
BOOL DefectManagementWorker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case MSG_SetDefectManagementWorker:
		{
			init();
			break;
		}
		case MSG_AlarmingData:
		{
			AlarmingData* p = (AlarmingData*)pMsg->wParam;
			DeelAlarmingData(p);
			break;
		}
		case MSG_LightAdjust:
		{
			PixelMax = (int)pMsg->wParam;
			

			break;
		}
		case MSG_SetInitialLight:
		{
			
			MSG msg;
			msg.message = MSG_BrightnessAdjustment;
			msg.wParam = (WPARAM)0;
			msg.lParam = 0;
			pWLightCtrl->message(&msg);
		

			msg.wParam = (WPARAM)100;
			msg.lParam = 0;
			pWLightCtrl->message(&msg);
			//MSG msg;
			//msg.message = MSG_BrightnessAdjustment;
			msg.wParam = (WPARAM)LightValue;
			msg.lParam = 0;
			pWLightCtrl->message(&msg);
			break;
		}
		case WM_TIMER:
		{				
		/*	DebugPrintf("Timer100:%x\n ", Timer100);
			DebugPrintf("wParam:%x\n ", pMsg->wParam);
			DebugPrintf("lParam:%x\n", pMsg->lParam);*/
			Timer(pMsg->wParam);
			break;
		}
	}
	return CWinThread::PreTranslateMessage(pMsg);
}
void DefectManagementWorker::DeelAlarmingData(AlarmingData* pAlarmingData)
{
	//=======回叠
	if (pAlarmingData->AlarmingType == 1
		|| pAlarmingData->AlarmingType == 2
		|| pAlarmingData->AlarmingType == 3
		)
	{
		AlarmingData* pt;
		//---AlarmingType == 2，已经处理完2的问题--------------
		if (pt = FindSimilarAlarmingData(pAlarmingData))
		{//FindSimilarAlarmingData的时候，已经把正常跨匝的加入存储
			//发现此次报警时以前的正常跨在，不报警
		//不论AlarmingType等于1，2，3只要发现他的前身是2就忽略
			delete pAlarmingData;
			return;
		}
		//------------------------------
		if (pAlarmingData->AlarmingType == 1)
		{//回叠报警******    1     ******
	//		DebugPrintf("回叠报警 SlidePosition %lf,SpindlePosition %lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
#ifdef _M_X64
			DebugPrintf("回叠报警 SlidePosition %lf,SpindlePosition %lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
#else
			pPlcCtrl->FoldbackAlert(pAlarmingData);
			CString s; s.Format("回叠报警 滑台 %0.2lf, 主轴 %0.2lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
			pViewDC->TextOut(1020,20,s);
#endif 
			
		}
	}
	//======================================
	//缝隙报警 ****    2     *****
	if (pAlarmingData->AlarmingType == 4)
	{
#ifdef _M_X64
		DebugPrintf("缝隙报警 SlidePosition %lf,SpindlePosition %lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
#else
		pPlcCtrl->GapAlert(pAlarmingData);
		CString s;
		s.Format("缝隙报警 滑台 %0.2lf, 滑台 %0.2lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
		pViewDC->TextOut(1020, 60, s);
		DebugPrintf("缝隙报警 SlidePosition %lf,SpindlePosition %lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);

#endif 	
	}
	//===========================================
	//跨匝不正确报警 ****    3     *****
	if (pAlarmingData->AlarmingType == 5)
	{
#ifdef _M_X64
		DebugPrintf("跨匝不正确 SlidePosition %lf,SpindlePosition %lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
#else
	//	pPlcCtrl->CrossTurnAlert(pAlarmingData);
		CString s;
		s.Format("跨匝不正确 滑台 %0.2lf, 主轴 % 0.2lf\n", pAlarmingData->SlidePosition, pAlarmingData->SpindlePosition);
		pViewDC->TextOut(1020, 100, s);
#endif 
	}
	//===========================================
	if(pAlarmingData->AlarmingType!=2)
	delete pAlarmingData;
}
AlarmingData* DefectManagementWorker::FindSimilarAlarmingData(AlarmingData* pAlarmingData)
{
//	DebugPrintf("*******\nAlarmingDataArray.GetSize()%d\n*************\n", AlarmingDataArray.GetSize());
	double slideposUp = pAlarmingData->SlidePosition + 1;
	int pos = -1, i;
	for (i = 0; i < AlarmingDataArray.GetSize(); ++i)
	{
		if (pAlarmingData->SlidePosition < AlarmingDataArray[i]->SlidePosition)
		{
			pos = i;
			break;
		}
	}
	if (pos == -1)//没有发现之前在该位置报警
	{
		if (pAlarmingData->AlarmingType == 2)
			AlarmingDataArray.Add(pAlarmingData);
		return 0;
	}
	int posup = pos;
	for (; i < AlarmingDataArray.GetSize(); ++i)
	{//向前搜寻更大一些的空间
		if (slideposUp < AlarmingDataArray[i]->SlidePosition)
		{
			posup = i;
			break;
		}
	}
	for (i = posup; i >= 0; --i)
	{
		if (AlarmingDataArray[i]->SlidePosition < slideposUp - 3)break;
		if (abs(AlarmingDataArray[i]->SlidePosition - pAlarmingData->SlidePosition) < 0.2 &&
			abs(AlarmingDataArray[i]->SpindlePosition - pAlarmingData->SpindlePosition) < 0.028)//10度
		{
			return AlarmingDataArray[i];//发现相同位置报警
		}
	}
	if (pAlarmingData->AlarmingType == 2)//没有相同位置报警，如果是正常跨匝，则加入存储队列
		AlarmingDataArray.InsertAt(pos, pAlarmingData);
	return 0;//返回0，传入的已经存储
}
int DefectManagementWorker::AddAlarmingData(AlarmingData* pAlarmingData)
{
	int pos = pAlarmingData->SlidePosition;


	return 0;
}

void DefectManagementWorker::AdjustBrightness(int MAX)
{
	if (MAX < 0)
		return;
//	DebugPrintf("400:%d\n", MAX);
	if (LightFlag == 0)
	{
		//DebugPrintf("200-240:LightValue %d MAX:%d LightFlag:%d\n", LightValue, MAX,LightFlag);
		if (MAX < 100)
		{
			LightValue += Step;
		}
		else if (MAX > 140)
		{
			LightValue -= Step;
		}
		else
		{
			//PixelMax = -1;
			return;
		}
		MakeValueInRange();
		//DebugPrintf("%d\n", LightValue);
		MSG msg;
	
		msg.message = MSG_BrightnessAdjustment;
		msg.wParam = (int)LightValue;
		msg.lParam = 0;
		pWLightCtrl->message(&msg);
		LightFlag = 1;
		//PixelMax = -1;

	}
	else
	{
	//	DebugPrintf("220-230:LightValue %d MAX:%d LightFlag:%d\n", LightValue, MAX,LightFlag);
		if (MAX > 130)
		{
			LightValue -= Step;
		}
		else if (MAX < 120)
		{
			LightValue += Step;
		}
		else
		{
			LightFlag = 0;
		//	PixelMax = -1;
			return;
		}
		MakeValueInRange();
		DebugPrintf("%d\n", LightValue);
		MSG msg;
		
		msg.message = MSG_BrightnessAdjustment;
		msg.wParam = (int)LightValue;
		msg.lParam = 0;
		pWLightCtrl->message(&msg);
	//	PixelMax = -1;
	

	}

}

void DefectManagementWorker::Timer(UINT_PTR nEvent)
{
	if (nEvent == Timer100)
	{
		
		AdjustBrightness(PixelMax);
		//*****************************
#ifdef _M_X64
#else
		//PLCTimer();
#endif 
		//*******************************
	}
}
void DefectManagementWorker::PLCTimer()
{
#ifdef _M_X64
#else

	if (pPlcCtrl&&pPlcCtrl->isWork&&pPlcCtrl->GetStartState())
	{
		pPlcCtrl->ClearStartState();
		CWinThread* pth = (CWinThread*)pGlobalResourceArray->GetAt(Pos_LineCameraWorker);
		if (pth)pth->PostThreadMessage(MSG_LineCameraSwitch, (WPARAM)1, 0);
	}
	if (pPlcCtrl && pPlcCtrl->isWork && pPlcCtrl->GetResetState())
	{
		pPlcCtrl->ClearResetState();
		CWinThread* pth = (CWinThread*)pGlobalResourceArray->GetAt(Pos_LineCameraWorker);
		if (pth)pth->PostThreadMessage(MSG_LineCameraSwitch, (WPARAM)-1, 0);
	}
#endif
}
void DefectManagementWorker::MakeValueInRange()
{
	if (LightValue < 0)
		LightValue =0 ;
	if (LightValue > 90)
		LightValue = 90;
		
}
