#pragma once
class WorkUnits
{
public:
	WorkUnits(UINT type, void* pdata, CWinThread* pwinthread);
	UINT Type;
	void* pData;
	CWinThread* pWinThread;
};

