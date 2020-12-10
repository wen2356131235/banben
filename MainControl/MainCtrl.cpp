#include "pch.h"
#include "MainCtrl.h"
#include"MainCtrlWorker.h"
MainCtrl::MainCtrl(CView* pView)
{
	//pCamera = new LineCamera;
	pMainView = pView;

	pMainCtrlThread = AfxBeginThread(RUNTIME_CLASS(MainCtrlWorker));
	Sleep(10);
	pMainCtrlThread->PostThreadMessage(MSG_SetMainCtrlWorker, (WPARAM)pMainView, 0);
	//AfxMessageBox("");
}
MainCtrl::~MainCtrl()
{
	//delete pCamera;
}
void MainCtrl::TranslateMessage(MSG* pMsg)
{
	pMainCtrlThread->PostThreadMessage(pMsg->message, (WPARAM)pMsg->wParam, pMsg->lParam);
}


