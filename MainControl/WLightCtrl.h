#pragma once
#define MSG_BrightnessAdjustment             WM_USER +0
#define MSG_SaveCurrentLighten               WM_USER +1

class _declspec(dllexport) WLightCtrl
{
	//	cnComm m_comm;
public:
	WLightCtrl();
	~WLightCtrl();

	CWinThread* pThread;
	char* getVersion();
	int message(MSG* pMsg);
	int message(UINT message,WPARAM wParam,LPARAM lParam);
};

