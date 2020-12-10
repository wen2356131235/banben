#include "pch.h"
#include "WorkUnits.h"

WorkUnits::WorkUnits(UINT type, void* pdata, CWinThread* pwinthread)
{
	Type = type;
	pData = pdata;
	pWinThread=pwinthread;
}
