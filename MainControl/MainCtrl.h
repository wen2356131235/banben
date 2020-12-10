#pragma once

//#include"..\\CameraDriverLibrary\\Camera.h"
//#pragma comment(lib,"..\\Debug\\CameraDriverLibrary.lib")
//

class _declspec(dllexport) MainCtrl
{
public:
	MainCtrl(CView* pView);
	~MainCtrl();
	void TranslateMessage(MSG* pMsg);

	//=======================

	CWinThread* pMainCtrlThread;
	//==============




	//================
	//LineCamera* pCamera;
	CView* pMainView;
};

