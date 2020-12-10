#include "pch.h"
#include "Slice.h"
#include"LinesSlice.h"
#include "Line.h"


Slice::Slice(float Center, float Left, float Right, int ID, double time, double spindleposition, double slideposition)
{
	//	CenterlineLocation100=
	pRelyOnLine = 0;
	CenterlineLocation = Center;
	LeftLocation = Left;
	RightLocation = Right;
	Width = Right - Left + 1;
	LinsSliceID = ID;
	DefectFlag = 0;
	Time = time;
	SpindlePosition = spindleposition;
	SlidePosition = slideposition + Center * 0.014;
	SlidePositionbase = slideposition;
	GapMarking = 0;
}
//调试用
Slice::Slice(float Center, float Left, float Right, int ID, int opticcellnumber, double* pcoefficients, double* pgrayvalues, double* pcols)
{//弃用

	CenterlineLocation = Center;
	LeftLocation = Left;
	RightLocation = Right;
	Width = Right - Left + 1;
	LinsSliceID = ID;
	DefectFlag = 0;
	OpticCellNumber = opticcellnumber;
	pCoefficients = pcoefficients;
	pGrayValues = pgrayvalues;
	pCols = pcols;
}



Slice::~Slice()
{
}

void Slice::EvaluateOverlap(Line* pLine, CPoint* pret)
{
	pret->x = (pLine->Left > LeftLocation) ? pLine->Left : LeftLocation;
	pret->y = (pLine->Right < RightLocation) ? pLine->Right : RightLocation;
	if (pret->x >= pret->y)
		pret->x = pret->y = 0;
}
void Slice::EvaluateOverlap(Slice* pSlice, CPoint* pret)
{
	pret->x = (pSlice->LeftLocation > LeftLocation) ? pSlice->LeftLocation : LeftLocation;
	pret->y = (pSlice->RightLocation < RightLocation) ? pSlice->RightLocation : RightLocation;
	if (pret->x >= pret->y)
		pret->x = pret->y = 0;
}

void Slice::ApplyLineAndSlice(Line* pline)
{
	Slice* pLineslice = pline->GetSliceReverse(0);
	if (pLineslice == 0)return;
	if (pLineslice->LinsSliceID >= LinsSliceID)return;

	CPoint Overlap(0, 0);
	EvaluateOverlap(pline, &Overlap);
	//Overlap.y - Overlap.x;交集宽度
	//DebugPrintf("%f  %f\n", float(Overlap.y - Overlap.x) ,pline->Width);
	if (float(Overlap.y - Overlap.x) / pline->Width > 0.5)
		LineApplyArray.Add(pline);
}
void Slice::Allocate2Line()
{
	/// <summary>
	/// 一个蛇蛋很多条蛇都想吃，于是蛇蛋把这些蛇吸引过来，加入到了
	/// 蛇蛋的ApplyArray中。蛇蛋被最近的那条蛇吃掉，如果同样近则分食。
	/// 如果蛇蛋剩下了，则孵化出一条新蛇。
	/// 
	/// </summary>
	/// <param name="T_SliceArray"></param>
	if (LineApplyArray.GetSize() == 0)return;

	float mindis = 10000, tdis;
	int i, pos = 0;
	Slice* pSlice;
	Slice* pNewSlice;

	float maxOverlap = 0, t;
	CPoint ret;
	for (i = 0; i < LineApplyArray.GetSize(); ++i)
	{
		EvaluateOverlap(LineApplyArray[i], &ret);
		if ((t = ret.y - ret.x) > maxOverlap)
		{
			maxOverlap = t;
			pos = i;
		}
	}
	LineApplyArray[pos]->AddSlice(this);
	//===每个line记住战胜自己的line========================
	int j;
	for (i = 0; i < LineApplyArray.GetSize(); ++i)
	{
		if (i == pos)continue;

		for (j = 0; j < LineApplyArray[i]->EnemyArray.GetSize(); ++j)
		{
			if (LineApplyArray[i]->EnemyArray[j] == LineApplyArray[pos])
				break;
		}
		if (j == LineApplyArray[i]->EnemyArray.GetSize())
			LineApplyArray[i]->EnemyArray.Add(LineApplyArray[pos]);
	}
	//===================================================
	LineApplyArray.RemoveAll();
}
