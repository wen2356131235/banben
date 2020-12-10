#include "pch.h"
#include "Line.h"
#include "AlarmingData.h"
Line::Line(Slice* pSlice, myMath* pmymath)
{
	AlarmingType = 0;
	pGapLine = 0;

	KillLeft = KillRight = 0;
	Width = pSlice->Width;
	Left = pSlice->LeftLocation;
	Right = pSlice->RightLocation;
	Center = pSlice->CenterlineLocation;

	Length = 1;
	SliceArraySize = Const_LineSliceArraySize*0.9;
	SliceArray.SetSize(SliceArraySize);
	pmyMath = pmymath;
	SliceArray.SetAt(0, pSlice);
	CurrentSliceArrayPos = 1;//该位置是待更新位置
	R = G = B = 0;
	int t = 255;
	//	while (t == 0)
	//		t = rand() % 256;
	switch (rand() % 3)
	{
		case 0:
			R = t;
			break;
		case 1:
			G = t;
			break;
		case 2:
			B = t;
			break;
	}
	pSlice->R = R;
	pSlice->G = G;
	pSlice->B = B;
}

Line::~Line()
{
	int i;
	for (i = 0; i < SliceArray.GetSize(); ++i)
	{
		if (SliceArray[i])
		{
			SliceArray[i]->R = SliceArray[i]->G = SliceArray[i]->B = 0;
			SliceArray[i]->pRelyOnLine = 0;
		}
	}
	SliceArray.RemoveAll();
	if (pGapLine)delete pGapLine;
}
int Line::AddDeadLinetoKillLeftLine(Line* pLine, double threshold)
{
	Slice* pLineSlice40 = pLine->GetSliceReverse(40);
	if (pLineSlice40 == 0)
	{
		DebugPrintMsgBox("100:pLineSlice40");
		return 0;
	}
	if (KillLeftLine.GetSize() == 0)
	{
		KillLeftLine.Add(pLineSlice40);
		return 1;
	}
	Slice* pKillLeftLineLastSlice0 = KillLeftLine[KillLeftLine.GetSize() - 1];

	Slice* pLineSlice0 = pLine->GetSliceReverse(0);
	//if (pLineSlice0->LinsSliceID - pKillLeftLineLastSlice0->LinsSliceID > pLine->SliceArraySize - 1)
	//	return 0;
	Slice* pLineSlice_X = pLine->GetSliceReverse(
		pLineSlice0->LinsSliceID -
		pKillLeftLineLastSlice0->LinsSliceID);

	if (pLineSlice_X == 0)
	{
	//	DebugPrintMsgBox("100:pLineSlice_X  %d %d", pLineSlice0->LinsSliceID ,
	//		pKillLeftLineLastSlice0->LinsSliceID);
		KillLeftLine.RemoveAll();
		KillLeftLine.Add(pLineSlice40);
		return 1;
	}
	//---------判断pLineSlice_X和pKillLeftLineLastSlice0是否相邻---
	if (abs(pLineSlice_X->RightLocation - pKillLeftLineLastSlice0->LeftLocation) < threshold)
	{
		KillLeftLine.Add(pLineSlice40);
		return 1;
	}
	else
	{//判断是否报警
		return 0;
	}
	//------------------------------
	return 0;
}
int Line::AddDeadLinetoKillRightLine(Line* pLine, double threshold)
{
	Slice* pLineSlice40 = pLine->GetSliceReverse(40);
	if (pLineSlice40 == 0) 
	{
		DebugPrintMsgBox("100:pLineSlice40");
		return 0; 
	}
	if (KillRightLine.GetSize() == 0)
	{
		KillRightLine.Add(pLineSlice40);
		return 1;
	}

	Slice* pKillRightLineLastSlice0 = KillRightLine[KillRightLine.GetSize() - 1];
	if (pKillRightLineLastSlice0 == 0)
	{
		DebugPrintMsgBox("100:pKillRightLineLastSlice0");
		return 0;
	}
	Slice* pLineSlice0 = pLine->GetSliceReverse(0);
	//DebugPrintf("we:size:%d reverse:%d\n", pLine->SliceArray.GetSize(), pLineSlice0->LinsSliceID -
	//	pKillRightLineLastSlice0->LinsSliceID);

	//if (pLineSlice0->LinsSliceID - pKillRightLineLastSlice0->LinsSliceID > pLine->SliceArraySize-1)
	//	return 0;
	Slice* pLineSlice_X = pLine->GetSliceReverse(
		pLineSlice0->LinsSliceID -
		pKillRightLineLastSlice0->LinsSliceID);

	if (pLineSlice_X == 0)
	{
		//	DebugPrintMsgBox("100:pLineSlice_X  %d %d", pLineSlice0->LinsSliceID ,
		//		pKillLeftLineLastSlice0->LinsSliceID);
		KillRightLine.RemoveAll();
		KillRightLine.Add(pLineSlice40);
		return 1;
	}
	if (pLineSlice_X == 0)
	{
		DebugPrintMsgBox("100:pLineSlice_X");
		return 0;
	}
		//DebugPrintMsgBox("%d %d", pLineSlice0->LinsSliceID,
		//pKillRightLineLastSlice0->LinsSliceID);
	//---------判断pLineSlice_X和pKillRightLineLastSlice0是否相邻---

	if (abs(pLineSlice_X->LeftLocation - pKillRightLineLastSlice0->RightLocation) < threshold)
	{
		KillRightLine.Add(pLineSlice40);
		return 1;
	}
	else
	{//判断是否报警
		return 0;
	}
	//------------------------------
//	DebugPrintf("we:12\n");
	return 0;
}
float Line::CalDistancePoint2Line(Slice* pSlice)
{
	int PreCurrentSliceArrayPos = (CurrentSliceArrayPos - 1 + SliceArraySize) % SliceArraySize;

	//	if (SliceArray[PreCurrentSliceArrayPos]->LinsSliceID >= pSlice->LinsSliceID)
	//		return 0;
	return pSlice->CenterlineLocation - SliceArray[PreCurrentSliceArrayPos]->CenterlineLocation;
}
int Line::AddSlice(Slice* pSlice)
{
	Slice* pHeadSlice = GetSliceReverse(0);
	if (pSlice->LinsSliceID < pHeadSlice->LinsSliceID)return 0;

	if (pSlice->LinsSliceID > pHeadSlice->LinsSliceID)
	{
		SliceArray[CurrentSliceArrayPos] = pSlice;
		//	pSlice->LeftLocation = pHeadSlice->LeftLocation * 0.8 + pSlice->LeftLocation * 0.2;
		//	pSlice->RightLocation = pHeadSlice->RightLocation * 0.8 + pSlice->RightLocation * 0.2;
		pSlice->pRelyOnLine = this;
		pSlice->R = R;
		pSlice->G = G;
		pSlice->B = B;
		//	CalSpiralAngle();
		CurrentSliceArrayPos++;
		CurrentSliceArrayPos %= SliceArraySize;
		double t = 0.6;
		double t1 = 1 - t;
		Width = Width * t + pSlice->Width * t1;
		Left = Left * t + pSlice->LeftLocation * t1;
		Right = Right * t + pSlice->RightLocation * t1;
		Center = Center * t + pSlice->CenterlineLocation * t1;
		//	pSlice->CenterlineLocation = pSlice->CenterlineLocation * 0.7 + pHeadSlice->CenterlineLocation * 0.3;
			//	pSlice->CenterlineLocation100 = pSlice->CenterlineLocation * 0.05 + pHeadSlice->CenterlineLocation100 * 0.95;

		Length++;
	}
	if (pSlice->LinsSliceID == pHeadSlice->LinsSliceID)
	{
		//		AfxMessageBox("pSlice->LinsSliceID == pHeadSlice->LinsSliceID");
		pHeadSlice->LeftLocation =
			(pHeadSlice->LeftLocation < pSlice->LeftLocation) ? pHeadSlice->LeftLocation : pSlice->LeftLocation;
		pHeadSlice->RightLocation =
			(pHeadSlice->RightLocation > pSlice->RightLocation) ? pHeadSlice->RightLocation : pSlice->RightLocation;
		pHeadSlice->CenterlineLocation = (pHeadSlice->LeftLocation + pHeadSlice->RightLocation) / 2;
		Width = Width * 0.7 + pHeadSlice->Width * 0.3;
		Left = Left * 0.7 + pHeadSlice->LeftLocation * 0.3;
		Right = Right * 0.7 + pHeadSlice->RightLocation * 0.3;

	}
	if (pSlice->SlidePosition - pHeadSlice->SlidePosition > 0)
		pSlice->SlideDirection = 1;//滑台向右侧移动，向外
	else pSlice->SlideDirection = -1;//滑台向左侧移动，向内
	//===缝隙检测=========================
	//************************************
	//*************************************


//	if(0)
	if (pGapLine)
	{
		Slice* pGapSlice = pGapLine->GetSliceReverse(0);
		if (pSlice->LinsSliceID - pGapSlice->LinsSliceID > 3)
		{//缝隙噪声，删除
			delete pGapLine;
			pGapLine = 0;
		}
	}

	if (pSlice->GapMarking == 1)
	{
		if (pGapLine == 0)pGapLine = new Line(pSlice, pmyMath);
		else
		{
			Slice* pGapSlice = pGapLine->GetSliceReverse(0);

			pGapLine->AddSlice(pSlice);
			pSlice->GapMarking = 2;
			//****************************
			//---缝隙报警
			if (pGapLine->Length > 100)
			{//???
				AlarmingData* pAlarmingData = new AlarmingData;
				pAlarmingData->AlarmingType = 4;

				pAlarmingData->SlidePosition = pSlice->SlidePosition;
				pAlarmingData->SpindlePosition = pSlice->SpindlePosition;

				CWinThread* pth = (CWinThread*)pGlobalResourceArray->GetAt(Pos_DefectManagementWorker);
				pth->PostThreadMessage(MSG_AlarmingData, (WPARAM)pAlarmingData, 0);

			}
		}
	}

	//===========================
//	判断跨匝不恰当报警

	if (KillLeftLine.GetSize() != 0)
	{
		Slice* pKillLeftLineLastSlice0 = KillLeftLine[KillLeftLine.GetSize() - 1];
		if (abs(pKillLeftLineLastSlice0->CenterlineLocation - pSlice->CenterlineLocation) > pSlice->Width * 2)
		{
			if (KillLeftLine.GetSize() != 2)
			{//报警！！！！！
				AlarmingData* pAlarmingData = new AlarmingData;
				pAlarmingData->AlarmingType = 2;

				pAlarmingData->SlidePosition = pSlice->SlidePosition;
				pAlarmingData->SpindlePosition = pSlice->SpindlePosition;

				CWinThread* pth = (CWinThread*)pGlobalResourceArray->GetAt(Pos_DefectManagementWorker);
				pth->PostThreadMessage(MSG_AlarmingData, (WPARAM)pAlarmingData, 0);
			}
			KillLeftLine.RemoveAll();
		}
	}
	//-------------------------------------------------
	if (KillRightLine.GetSize() != 0)
	{
		Slice* pKillRightLineLastSlice0 = KillRightLine[KillRightLine.GetSize() - 1];
		if (abs(pKillRightLineLastSlice0->CenterlineLocation - pSlice->CenterlineLocation) > pSlice->Width * 2)
		{
			if (KillRightLine.GetSize() != 2)
			{//报警！！！！！
				AlarmingData* pAlarmingData = new AlarmingData;
				pAlarmingData->AlarmingType = 5;

				pAlarmingData->SlidePosition = pSlice->SlidePosition;
				pAlarmingData->SpindlePosition = pSlice->SpindlePosition;

				CWinThread* pth = (CWinThread*)pGlobalResourceArray->GetAt(Pos_DefectManagementWorker);
				pth->PostThreadMessage(MSG_AlarmingData, (WPARAM)pAlarmingData, 0);
			}
			KillRightLine.RemoveAll();
		}
	}
	return 1;
}
void Line::CalSpiralAngle()
{
	//if (Length < 10)return;
	//Slice* pHeadSlice1 = GetSliceReverse(0);
	//Slice* pHeadSlice2 = GetSliceReverse(9);
	//pHeadSlice1->SpiralAngle = atan((pHeadSlice1->CenterlineLocation - pHeadSlice2->CenterlineLocation) / (pHeadSlice1->LinsSliceID - pHeadSlice2->LinsSliceID)) * 180 / 3.1415926;
	//
	//Slice* ps = GetSliceReverse(0);
	//double t = ps->SpiralAngle;
	//if (AlarmingType != 0)
	//	DebugPrintf("[%d,lineID %d type %d]Angle	%lf\n", ps->LinsSliceID,ID, AlarmingType, t);

}
void Line::draw(HDC hDC, int ScrHight)
{
	return;
	//to 当前现实存在
	//from:0,to:--
	int i, j;
	int n = SliceArray.GetSize() - 1;
	//	int from_t=-1;
	//	for (i = n; i >= 0 && SliceArray[i]->LinsSliceID != from; --i);
	for (i = n, j = 0; i >= 0 && j < ScrHight; ++j, --i)
	{

		::SetPixel(hDC, SliceArray[i]->LeftLocation, i, RGB(0, 0, 255));
		::SetPixel(hDC, SliceArray[i]->RightLocation, i, RGB(0, 0, 255));
		::SetPixel(hDC, SliceArray[i]->CenterlineLocation, i, RGB(0, 255, 255));
		if (SliceArray[i]->DefectFlag == 1)
		{
			::SetPixel(hDC, SliceArray[i]->CenterlineLocation - 1, i, RGB(255, 0, 0));
			::SetPixel(hDC, SliceArray[i]->CenterlineLocation, i, RGB(255, 0, 0));
			::SetPixel(hDC, SliceArray[i]->CenterlineLocation + 1, i, RGB(255, 0, 0));
		}
	}
}
Slice* Line::GetSliceReverse(int pos)
{
	if (pos > SliceArraySize - 1)
		return 0;
		
	//	DebugPrintf("GetSliceReverse %d\n",(CurrentSliceArrayPos - 1 - pos + SliceArraySize) % SliceArraySize);
	return SliceArray[(CurrentSliceArrayPos - 1 - pos + SliceArraySize) % SliceArraySize];
}