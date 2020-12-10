#include "pch.h"
#include "ComputingManagement.h"
#include "ComputingServiceWorker.h"
ComputingManagement::ComputingManagement()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	//CString s;
	//s.Format("计算机核心数：%d,内存：%I64d-%I64d= %d",
	//	si.dwNumberOfProcessors,
	//	long long(si.lpMaximumApplicationAddress) / 1024 / 1024 / 1024 / 1024,
	//	long long(si.lpMinimumApplicationAddress),
	//	(long long(si.lpMaximumApplicationAddress) - long long(si.lpMinimumApplicationAddress)) / 1024 / 1024 / 1024);
	ThreadSize = si.dwNumberOfProcessors - 1;
	if (ThreadSize < 1)ThreadSize = 1;
  //  ThreadSize = 1;
	//DebugPrintf("ThreadSize  %d", ThreadSize);
	//============================
	ThreadStateArray.SetSize(ThreadSize);
	WinThreadArray.SetSize(ThreadSize);
	CWinThread* pth;
	for (int i = 0; i < ThreadSize; ++i)
	{
		pth = AfxBeginThread(RUNTIME_CLASS(ComputingServiceWorker));
		WinThreadArray.SetAt(i, pth);
		ThreadStateArray.SetAt(i, 0);
	}
}

ComputingManagement::~ComputingManagement()
{
	int i;
	for (i = 0; i < ThreadSize; ++i)
		WinThreadArray[i]->Delete();
	WinThreadArray.RemoveAll();
	ThreadStateArray.RemoveAll();

	for (i = 0; i < ThreadSize; ++i) delete WorkUnitsArray[i];
	WorkUnitsArray.RemoveAll();
}

int ComputingManagement::GetTaskStress()
{
	int n = WorkUnitsArray.GetSize();
	if (n < ThreadSize)return -1;
	if (n >= ThreadSize && n <= 2*ThreadSize)return 0;
	else return 1;
}

void ComputingManagement::AddWorkUnits(WorkUnits* pWorkUnits)
{
	if (PerformWorkUnits(pWorkUnits) == 0)
		WorkUnitsArray.Add(pWorkUnits);
}

int ComputingManagement::PerformWorkUnits(WorkUnits* pWorkUnits)
{
	int i;
	for (i = 0; i < ThreadSize; i++)
	{
		if (ThreadStateArray[i] == 0)
		{
			ThreadStateArray[i] = 1;
			WinThreadArray[i]->PostThreadMessage(pWorkUnits->Type, (WPARAM)pWorkUnits, 0);
			return 1;
		}
	}
	return 0;
}
int ComputingManagement::PerformWorkUnits()
{
	if (WorkUnitsArray.GetSize() == 0)return 0;
	PerformWorkUnits(WorkUnitsArray[0]);
	WorkUnitsArray.RemoveAt(0);
	return 1;
}

//#include "LinesSlice.h"
void ComputingManagement::CompleteWorkUnits(WorkUnits* pWorkUnits, CWinThread* pthread)
{
	int i;
	for (i = 0; i < ThreadSize; ++i)
	{
		if (WinThreadArray[i] == pthread)
		{
			ThreadStateArray[i] = 0;
			break;
		}
	}
	delete pWorkUnits;
	PerformWorkUnits();
}