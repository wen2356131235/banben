#pragma once
#include "WorkUnits.h"
class ComputingManagement
{
	CArray<CWinThread*> WinThreadArray;
	CArray<int>	ThreadStateArray;
	int ThreadSize;

	CArray<WorkUnits* > WorkUnitsArray;
public:
	ComputingManagement();
	~ComputingManagement();
	int GetTaskStress();
	void AddWorkUnits(WorkUnits*);
	int PerformWorkUnits(WorkUnits* pWorkUnits);
	int PerformWorkUnits();
	void CompleteWorkUnits(WorkUnits* pWorkUnits, CWinThread* pthread);
};

