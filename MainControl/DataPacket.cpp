#include "pch.h"
#include "DataPacket.h"
#include "AlarmingData.h"
DataPacket::DataPacket()
{
	PictureArraySize = Const_PictureArraySize;
	PictureArray.SetSize(PictureArraySize);
	CurrentPicPos = 0;
	picIDcount = 0;

	LineVitality = 5;
	LinesIDcount = 0;
	//-----------------
	LinesSliceArraySize = Const_LineSliceArraySize;
	LinesSliceArray.SetSize(LinesSliceArraySize);
	CurrentLinesSlicePos = 0;
	LinesSliceIDcount = 0;
	CompleteSliceID = 0;
	CompleteLinesSliceArrayPos = 10;



	pmyMath = new myMath;
	double px[] = { 0,1,2,3,4 };
	myMathindex5_2 = pmyMath->AddFittingLineInput(px, 5, 2);
	AverageSliceWidth = 0.0;
	AverageSliceWidth_half = 0.0;

	TripleStandardDeviation = 0.0;
	SliceWidthAverageError = 0.0;
}

DataPacket::~DataPacket()
{
}
//void DataPacket::AddReLinesSlice(LinesSlice* plinesslice)
//{
//	if (ReLinesSliceArray[ReCurrentLinesSlicePos])
//		delete ReLinesSliceArray[ReCurrentLinesSlicePos];
//	ReLinesSliceArray[ReCurrentLinesSlicePos] = plinesslice;
//	ReCurrentLinesSlicePos++;
//	plinesslice->ID = ReLinesSliceIDcount;
//	ReLinesSliceIDcount++;
//	ReCurrentLinesSlicePos = ReCurrentLinesSlicePos % ReLinesSliceArraySize;
//}
void DataPacket::AddLinesSlice(LinesSlice* plinesslice)
{
	if (LinesSliceArray[CurrentLinesSlicePos])
		delete LinesSliceArray[CurrentLinesSlicePos];
	LinesSliceArray[CurrentLinesSlicePos] = plinesslice;
	CurrentLinesSlicePos++;
	plinesslice->ID = LinesSliceIDcount;
	LinesSliceIDcount++;
	CurrentLinesSlicePos = CurrentLinesSlicePos % LinesSliceArraySize;
}

int DataPacket::AddPicture(Picture* pPic)
{
	//拍摄用代码 =======↓=======
//	if (CurrentPicPos == Const_PictureArraySize-1)
//		return 0;
	//====拍摄用代码====↑=======
	if (PictureArray[CurrentPicPos])
		delete PictureArray[CurrentPicPos];
	PictureArray[CurrentPicPos] = pPic;
	CurrentPicPos++;
	pPic->ID = picIDcount;
	picIDcount++;
	CurrentPicPos = CurrentPicPos % PictureArraySize;
	return 1;
}

Picture* DataPacket::GetPictureReverse(int pos)
{
	return PictureArray[(CurrentPicPos - pos - 1 + PictureArraySize) % PictureArraySize];
	//反向取图片 pos为0时为最近的一张图，取值范围0-（picnumber-1）
}
LinesSlice* DataPacket::GetLinesSliceReverse(int pos)
{
	return LinesSliceArray[(CurrentLinesSlicePos - pos - 1 + LinesSliceArraySize) % LinesSliceArraySize];

}
void DataPacket::LineGrowUp()
{
//	return;
	if (LinesSliceIDcount < 10)
		return;

	int i, j;

	Line* pLine = 0;
	i = CompleteLinesSliceArrayPos;
	while (1)
	{
		i = (i + 1) % LinesSliceArraySize;
		if (LinesSliceArray[i] && LinesSliceArray[i]->Flag == 1 &&
			LinesSliceArray[i]->ID > LinesSliceArray[CompleteLinesSliceArrayPos]->ID)
		{
			CompleteLinesSliceArrayPos = i;
			LinesSliceArray[i]->Flag = 2;
			LineSliceMatchLines(LinesSliceArray[i]);

		}
		else
			break;
	}

}
void DataPacket::LineSliceMatchLines(LinesSlice* pLineSlice)
{
	int i;
	Line* pLine;
	if (LineArray.GetSize() == 0)
	{
		for (i = 0; i < pLineSlice->SliceArray.GetSize(); ++i)
		{
			pLine = new Line(pLineSlice->SliceArray[i], pmyMath);
			pLine->ID = LinesIDcount++;
			LineArray.Add(pLine);
			//DebugPrintf("new Line:size %d	%d   %f\n", LineArray.GetSize(), 0, pLineSlice->SliceArray[i]->CenterlineLocation);
		}
		return;
	}
	for (i = 0; i < pLineSlice->SliceArray.GetSize(); ++i)
	{//线添加到slice的预配准Array中
		LineAddtoSliceApply(pLineSlice->SliceArray[i]);
		AverageSliceWidthAnalysis(pLineSlice->SliceArray[i]);
	}

	//	CArray<Slice*> tempSliceArray;
	Slice* pLineSlice0;
	int pos = 0;
	for (i = 0; i < pLineSlice->SliceArray.GetSize(); ++i)
	{//处理预配准Array
		if (pLineSlice->SliceArray[i]->LineApplyArray.GetSize() == 0)
		{//添加一条新线
			pLine = new Line(pLineSlice->SliceArray[i], pmyMath);
			pLine->ID = LinesIDcount++;
			for (pos = 0; pos < LineArray.GetSize(); ++pos)
			{
				pLineSlice0 = LineArray[pos]->GetSliceReverse(0);
				if (pLineSlice->SliceArray[i]->CenterlineLocation < pLineSlice0->CenterlineLocation)
					break;
			}
			LineArray.InsertAt(pos, pLine);
			//DebugPrintf("new (LineApplyArray) Line:size %d	%d   %f\n", LineArray.GetSize(), pos, pLineSlice->SliceArray[i]->CenterlineLocation);

			continue;
		}
		//===决策Slice选择哪条线,或者slice分配问题
		pLineSlice->SliceArray[i]->Allocate2Line();
		//======================
	}
	CompleteSliceID = pLineSlice->ID;
	///==线整理：删除不活跃的线。如果是比较长的线，且处于中间区域，
	/// 则报警
	DeleteInactivelines();//并报警发送
//	LineWidthAnalysis();
}
void DataPacket::LineWidthAnalysis()
{
	int i, j;
	Line* pLine;
	//线排序=冒泡法=========
	int flag = 0;
	int count = 0;
	for (j = LineArray_AfterFiltering.GetSize() - 1; j > 0; --j)
	{
		flag = 0;
		for (i = 1; i < j; ++i)
		{
			if (LineArray_AfterFiltering[i - 1]->Center > LineArray_AfterFiltering[i]->Center)
			{
				pLine = LineArray_AfterFiltering[i - 1];
				LineArray_AfterFiltering[i - 1] = LineArray_AfterFiltering[i];
				LineArray_AfterFiltering[i] = pLine;
				flag = 1; count++;
				//		DebugPrintf("\n\n\n%d 【%f %d】【 %f  %d】jiaohuan\n\n\n",i, LineArray[i - 1]->Width, LineArray[i-1]->Length, LineArray[i]->Width, LineArray[i]->Length);
			}
		}
		if (flag == 0) break;
	}

	if (count)
	{
		for (i = 0; i < LineArray_AfterFiltering.GetSize(); ++i)
		{
			DebugPrintf("%d %d  %f %f %d】\n", i, LineArray_AfterFiltering[i]->ID, LineArray_AfterFiltering[i]->Center, LineArray_AfterFiltering[i]->Width, LineArray_AfterFiltering[i]->Length);
		}
		//	DebugPrintMsgBox("");
	}

	//===================
}
void DataPacket::DeleteInactivelines()
{
	Slice* pSlice;
	int i;
	for (i = 0; i < LineArray.GetSize(); ++i)
	{
		pSlice = LineArray[i]->GetSliceReverse(0);
		if (CompleteSliceID - pSlice->LinsSliceID > LineVitality)//失去活性
		{
			//DebugPrintf("DeleteInactivelines  pos: %d size:%d\n\n",i, LineArray[i]->Length);
			if (LineArray[i]->Length > 200)
			{
				//	MSG msg;
				//	msg.message = defectType_BrokenLongLine;
				//	msg.wParam = (WPARAM)LineArray[i];
				//	CentralizedTreatmentofDefects(&msg);//进入报警入口
				DefectiveJudgment_BrokenLongLine(LineArray[i]);
			}
			delete LineArray[i];
			LineArray.RemoveAt(i);
			--i;
		}
	}
	LineArray_AfterFiltering.RemoveAll();
	LineArray_AfterFiltering.Append(LineArray);
	for (i = 0; i < LineArray_AfterFiltering.GetSize(); ++i)
	{
		if (LineArray_AfterFiltering[i]->Length < 5)
		{
			LineArray_AfterFiltering.RemoveAt(i);
			--i;
		}
	}
}
void DataPacket::AverageSliceWidthAnalysis(Slice* pSlice)
{
	//Slice宽度分析==

	if (AverageSliceWidth < 0.001)
	{
		AverageSliceWidth = pSlice->Width;
		AverageSliceWidth_half = AverageSliceWidth / 2;
	}
	double avergeerror;
	double t = (AverageSliceWidth + AverageSliceWidth_half) / 2;
	if (pSlice->Width > t)
	{
		AverageSliceWidth = AverageSliceWidth * 0.999 + pSlice->Width * 0.001;
		AverageSliceWidth_half = AverageSliceWidth_half * 0.999 + pSlice->Width * 0.0005;
		avergeerror = abs(pSlice->Width - AverageSliceWidth);
	}
	else
	{
		AverageSliceWidth_half = AverageSliceWidth_half * 0.999 + pSlice->Width * 0.001;
		avergeerror = abs(pSlice->Width - AverageSliceWidth_half);
	}
	SliceWidthAverageError = SliceWidthAverageError * 0.999 + avergeerror * 0.001;
	TripleStandardDeviation = SliceWidthAverageError * 4;//3.76;
	if (avergeerror > TripleStandardDeviation)
	{
		pSlice->GapMarking = 1;
	}
}
int DataPacket::LineAddtoSliceApply(Slice* pSlice)
{//判断slice属于哪条线
//	AddSlice();
	int i;
	Line* pLine;
	for (i = 0; i < LineArray.GetSize(); ++i)
	{
		pSlice->ApplyLineAndSlice(LineArray[i]);
	}
	return 1;
}

void DataPacket::CentralizedTreatmentofDefects(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case defectType_BrokenLongLine:
		{
			DefectiveJudgment_BrokenLongLine((Line*)pMsg->wParam);
			break;
		}
	}
}
/// <summary>
/// 该线还在linearray中，在则返回下标，否则返回-1
/// </summary>
/// <param name="pLine"></param>
/// <returns></returns>
int DataPacket::isLineLiving(Line* pLine)
{//
	int i;
	for (i = 0; i < LineArray.GetSize(); ++i)
	{
		if (LineArray[i] == pLine)
			return i;
	}
	return -1;
}
void DataPacket::EvaluateOverlap(CPoint p1, CPoint p2, CPoint* pret)
{
	pret->x = (p1.x > p2.x) ? p1.x : p2.x;
	pret->y = (p1.y < p2.y) ? p1.y : p2.y;
	if (pret->x >= pret->y)
		pret->x = pret->y = 0;
}
void DataPacket::EvaluateUnion(CPoint p1, CPoint p2, CPoint* pret)
{
	pret->x = (p1.x < p2.x) ? p1.x : p2.x;
	pret->y = (p1.y > p2.y) ? p1.y : p2.y;
	if (pret->x >= pret->y)
		pret->x = pret->y = 0;
}
void DataPacket::DefectiveJudgment_BrokenLongLine(Line* pLine)
{//调查该线的死因
	Line* pEnemyLine;
	Slice* pEnemyLineSlice0 = 0, * pEnemyLineSlice40 = 0;
	Slice* pCurrentLineSlice = 0, * pCurrentLineSlice40 = 0;
	CArray<Line*>tLineArray;
	CArray<double>tOverlapArray;
	double tOverlap;
	int i;
	for (i = 0; i < LineArray.GetSize(); ++i)
	{
		pEnemyLine = LineArray[i];
		if (pEnemyLine == pLine)continue;
		if (pEnemyLine->Length < 100)
		{
			continue;
		}

		pCurrentLineSlice = pLine->GetSliceReverse(LineVitality);
		pEnemyLineSlice0 = pEnemyLine->GetSliceReverse(0);
		pEnemyLineSlice40 = pEnemyLine->GetSliceReverse(LineVitality * 3);
		if (pEnemyLineSlice40 == 0)
		{
			continue;
		}
		CPoint p1, p2, ret;
		p1 = CPoint(pEnemyLineSlice0->LeftLocation, pEnemyLineSlice0->RightLocation);
		p2 = CPoint(pEnemyLineSlice40->LeftLocation, pEnemyLineSlice40->RightLocation);

		EvaluateUnion(p1, p2, &ret);
		//	DebugPrintf("p1[%d,%d], p2[%d,%d], &ret[%d,%d]\n",p1.x,p1.y,p2.x,p2.y,ret.x,ret.y);
		p1 = ret;
		p2 = CPoint(pCurrentLineSlice->LeftLocation, pCurrentLineSlice->RightLocation);
		EvaluateOverlap(p1, p2, &ret);

		//	if (ret != p2)continue;// else DebugPrintMsgBox("测试找到敌人");
		tOverlap = float(ret.y - ret.x) / (p2.y - p2.x);
		if (tOverlap < 0.3)continue;
		tLineArray.Add(pEnemyLine);
		tOverlapArray.Add(tOverlap);
	}
	if (tLineArray.GetSize() == 0)return;

	//=================================
	//寻找敌人位置pos
	int pos = -1;
	double tOverlapMax = 0;
	for (i = 0; i < tLineArray.GetSize(); ++i)
	{
		if (tOverlapArray[i] > tOverlapMax)
		{
			pos = i;
			tOverlapMax = tOverlapArray[i];
		}
	}
	if (pos == -1)return;
	//========================================
	//已经确定敌人位置为pos
	

	pEnemyLine = tLineArray[pos];
	///////////////////////////////////////////////////
	if (pEnemyLine->Length < 100)
	{
		if(pEnemyLine->SliceArray[0]->LinsSliceID>pLine->GetSliceReverse(0)->LinsSliceID)
		return;
	}
	pCurrentLineSlice = pLine->GetSliceReverse(LineVitality);
	pEnemyLineSlice0 = pEnemyLine->GetSliceReverse(0);
	pEnemyLineSlice40 = pEnemyLine->GetSliceReverse(LineVitality * 3);

	//===============================
	//******************************
	//如果能执行到此处，证明已经被盖帽
	//********************************
	//==准备报警信息=========
	//*********************************
	pCurrentLineSlice40 = pLine->GetSliceReverse(40);

	AlarmingData* pAlarmingData = new AlarmingData;
	pAlarmingData->AlarmingType = 3;
	pEnemyLine->AlarmingType = 3;

	pAlarmingData->SlidePosition = pCurrentLineSlice->SlidePosition;
	pAlarmingData->SpindlePosition = pCurrentLineSlice->SpindlePosition;
	pAlarmingData->SlideSpeed = (pEnemyLineSlice0->SlidePositionbase - pEnemyLineSlice40->SlidePositionbase) /
		(pEnemyLineSlice0->Time - pEnemyLineSlice40->Time) * 1000;
	pAlarmingData->SpindleSpeed = (pEnemyLineSlice0->SpindlePosition - pEnemyLineSlice40->SpindlePosition) /
		(pEnemyLineSlice0->Time - pEnemyLineSlice40->Time) * 1000;

	if (abs(pAlarmingData->SlideSpeed) < 0.05)
	{
		pAlarmingData->AlarmingType = 2;
		pEnemyLine->AlarmingType = 2;
	}

	if (pCurrentLineSlice->CenterlineLocation < pEnemyLineSlice40->CenterlineLocation)
	{//死线在左侧
		pEnemyLine->KillLeft++;
		if (SlideDirection == -1)
		{//报警回跌，怀疑，后续通过历史信息验证
		//	if(pEnemyLine->KillLeft+ pEnemyLine->KillRight>5)
			pAlarmingData->AlarmingType = 1;
			pEnemyLine->AlarmingType = 1;
		}
		//----加入左侧-----

		if(pAlarmingData->AlarmingType==2)
		pEnemyLine->AddDeadLinetoKillLeftLine(pLine,SliceWidthAverageError);

		//-------------------
	}
	else
	{ //死线在右侧
		pEnemyLine->KillRight++;
		if (SlideDirection == 1)
		{//报警回跌，怀疑，后续通过历史信息验证
			pAlarmingData->AlarmingType = 1;
			pEnemyLine->AlarmingType = 1;
		}
		//--加入右侧---

		if (pAlarmingData->AlarmingType == 2)
		{

			pEnemyLine->AddDeadLinetoKillRightLine(pLine, SliceWidthAverageError);

		}

		//-------------
	}

	//DebugPrintf("deadlineID %d enemyID %d EnemyLeft%d EnemyRight %d SlideDir %d AlarmType %d pEnemySlice0 %0.3lf pCurrentSlice %0.3lf SlideSpeed %0.3lf\n",
		//pLine->ID, pEnemyLine->ID, pEnemyLine->KillLeft, pEnemyLine->KillRight, SlideDirection, pAlarmingData->AlarmingType, pEnemyLineSlice0->CenterlineLocation, pCurrentLineSlice->CenterlineLocation, pAlarmingData->SlideSpeed);

	CWinThread* pth = (CWinThread*)pResourceArray->GetAt(Pos_DefectManagementWorker);
	pth->PostThreadMessage(MSG_AlarmingData, (WPARAM)pAlarmingData, 0);

	//***************************************
	//===========================================
}
