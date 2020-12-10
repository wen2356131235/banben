#include "pch.h"
#include "LinesSlice.h"
//#include"DataPacket.h"
#include "Line.h"

LinesSlice::LinesSlice(byte* pbuffer, int n, int id, double time, double spindlepostion, double slidepostion)
{
	BufferSize = n;
	pBuffer = new byte[n];
	memcpy(pBuffer, pbuffer, n);
	Flag = 0;
	ID = id;
	SpindlePosition = spindlepostion;
	SlidePosition = slidepostion;
	Time = time;
	ShowMode = 2;


	//UsualSliceWidth = 0;

}
LinesSlice::LinesSlice(byte* pbuffer, int n, int id, double time, double spindlepostion, double slidepostion,CClientDC* pdc)
{
	BufferSize = n;
	pBuffer = new byte[n];
	memcpy(pBuffer, pbuffer, n);
	Flag = 0;
	ID = id;
	SpindlePosition = spindlepostion;
	SlidePosition = slidepostion;
	Time = time;
	ShowMode = 2;
	pClient = pdc;

	//UsualSliceWidth = 0;

}

LinesSlice::~LinesSlice()
{
	for (int i = 0; i < SliceArray.GetSize(); ++i)
		delete SliceArray[i];
	SliceArray.RemoveAll();
	delete[] pBuffer;
}
void LinesSlice::Smooth(BYTE* psmoothbuf)
{
	if (ID > 0)
	{
		//byte* pbuf = pdatapacket->LinesSliceArray[pdatapacket->LinesSliceArray.GetSize() - 1]->pBuffer;
		for (int i = 0; i < BufferSize; ++i)pBuffer[i] = pBuffer[i] * 0.3 + psmoothbuf[i] * 0.7;
		memcpy(psmoothbuf, pBuffer, 2048);
	}
	else
	{
		memcpy(psmoothbuf, pBuffer, 2048);
	}
}
void LinesSlice::draw(byte* pScrBuf24Color)
{
	/*if (pClient)
	{
		CString s;
		s.Format("%d", ID);
		pClient->TextOutA(1000, 600, s);
	}*/
	int i, j;

	for (i = 0; i < BufferSize; ++i)
	{
		pScrBuf24Color[3 * i] = pScrBuf24Color[3 * i + 1] = pScrBuf24Color[3 * i + 2] = pBuffer[i];
	}
	if (ShowMode == 2)
		return;
	int n = SliceArray.GetSize(), pos;
	if (Flag == 2)
		for (i = 0; i < n; ++i)
		{

			//pos = SliceArray[i]->CenterlineLocation;
			//pScrBuf24Color[3 * pos] = SliceArray[i]->R;
			//pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
			//pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
			
			pos = SliceArray[i]->LeftLocation + 1;
			IsIn2048(pos);
			pScrBuf24Color[3 * pos] = SliceArray[i]->R;
			pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
			pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
			if (SliceArray[i]->R == 0 && SliceArray[i]->G == 0 && SliceArray[i]->B == 0)
			{
				int tii;
				for (tii = SliceArray[i]->LeftLocation; tii < SliceArray[i]->RightLocation; ++tii)
				{
					pos = tii;
					IsIn2048(pos);
					pScrBuf24Color[3 * pos] *= 0.5;
					pScrBuf24Color[3 * pos + 1] *= 0.5;
					pScrBuf24Color[3 * pos + 2] *= 0.5;
				}
			}
			if (SliceArray[i]->LinsSliceID%10==0&&SliceArray[i]->pRelyOnLine && SliceArray[i]->pRelyOnLine->KillLeft > 0)
			{
				int tii;
				for (tii = SliceArray[i]->LeftLocation; tii < SliceArray[i]->CenterlineLocation; ++tii)
				{
					pos = tii;
					IsIn2048(pos);
					pScrBuf24Color[3 * pos] = SliceArray[i]->R;
					pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
					pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
				}
			}
			pos = SliceArray[i]->RightLocation - 1;
			IsIn2048(pos);
			pScrBuf24Color[3 * pos] = SliceArray[i]->R;
			pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
			pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
			if (SliceArray[i]->LinsSliceID % 10 == 0 && SliceArray[i]->pRelyOnLine && SliceArray[i]->pRelyOnLine->KillRight > 0)
			{
				int tii;
				for (tii = SliceArray[i]->CenterlineLocation; tii < SliceArray[i]->RightLocation; ++tii)
				{
					pos = tii;
					IsIn2048(pos);
					pScrBuf24Color[3 * pos] = SliceArray[i]->R;
					pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
					pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
				}
			}
		//	if(0)
			if (SliceArray[i]->GapMarking==2)
			{
				int tii;
				for (tii = SliceArray[i]->LeftLocation; tii < SliceArray[i]->RightLocation; tii+=2)
				{
					pos = tii;
					IsIn2048(pos);
					pScrBuf24Color[3 * pos] = SliceArray[i]->R;
					pScrBuf24Color[3 * pos + 1] = SliceArray[i]->G;
					pScrBuf24Color[3 * pos + 2] = SliceArray[i]->B;
				}
			}
	
			/*	pos = SliceArray[i]->CenterlineLocation100;
				pScrBuf24Color[3 * pos] = 0; SliceArray[i]->R;
				pScrBuf24Color[3 * pos + 1]= 0;  SliceArray[i]->G;
				pScrBuf24Color[3 * pos + 2]= 0;  SliceArray[i]->B;*/

		}
}
void LinesSlice::ResetOpticalNeuralNetwork(CArray<OpticCell*>* popticalneuralnetwork)
{
	pOpticalNeuralNetwork = popticalneuralnetwork;

	for (int i = 0; i < OpticalNeuralNetwork_Columns; i++)
	{
		pOpticalNeuralNetwork->GetAt(i)->GrayValue = pBuffer[i];
	}


}

void LinesSlice::SetShowMode(int showmode)
{
	ShowMode = showmode;
}

int LinesSlice::IsIn2048(int i)
{
	if (i < 2048 && i >= 0)
		return 1;
	else
	{
		DebugPrintMsgBox("400:%d", i);
		return 0;
	}
}

void LinesSlice::deel(CArray<OpticCell*>* popticcellarray)
{
	//CreateOpticalNeuralNetwork(BufferSize, pBuffer);
	ResetOpticalNeuralNetwork(popticcellarray);
	CalThreshold();
	ConvergeForPeaksRoughs();
	FindMostUsusalLineWidth();
	RecutBlock();
	CreateSlices();
	Clear();

	//ConvergeForPeaksRoughs();
	//ExtendCellBlock();
	//FitAccuratePeaksRoughsPos();//
	//CreateSlices();
	//Clear();
	//FindMostUsualSliceWidth();
	//DiagnoseEverySlice();
}

int LinesSlice::CreateSlices()
{
	if (OpticCellBlockArray.GetSize() == 0)
	{
		//	AfxMessageBox("CreateSlices Error!");
		return -1;
	}

	int SliceCount = 0;
	for (int i = 0; i < OpticCellBlockArray.GetSize() - 1; ++i)
	{
		Slice* pSlice;
		pSlice = new Slice((OpticCellBlockArray[i]->MaxOrMinPos + OpticCellBlockArray[i + 1]->MaxOrMinPos) / 2, OpticCellBlockArray[i]->MaxOrMinPos, OpticCellBlockArray[i + 1]->MaxOrMinPos, ID, Time, SpindlePosition, SlidePosition);
		SliceArray.Add(pSlice);
		//  DebugPrintf("%4d time:%f Spindle:%f Slide:%f\n", i, pSlice->Time, pSlice->SpindlePosition, pSlice->SlidePosition);

	}

	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	//{
	//	if (OpticCellBlockArray.GetAt(i)->PeaksOrRoughs == 1)
	//	{
	//		//Slice* pSlice = new Slice(OpticCellBlockArray.GetAt(i)->PeaksRoughsCol, OpticCellBlockArray.GetAt(i)->Left, OpticCellBlockArray.GetAt(i)->Right, ID);
	//		//调试用
	//		Slice* pSlice = new Slice(
	//			OpticCellBlockArray.GetAt(i)->PeaksRoughsCol,
	//			OpticCellBlockArray.GetAt(i)->Left,
	//			OpticCellBlockArray.GetAt(i)->Right,
	//			ID,
	//			OpticCellBlockArray.GetAt(i)->OpticCellNumber,
	//			OpticCellBlockArray.GetAt(i)->pCoefficients,
	//			OpticCellBlockArray.GetAt(i)->pGrayValues,
	//			OpticCellBlockArray.GetAt(i)->pCols);

	//		if (i == 0)
	//			MaxSliceWidth = MinSliceWidth = pSlice->Width;

	//		if (pSlice->Width > MaxSliceWidth)
	//			MaxSliceWidth = pSlice->Width;
	//		if (pSlice->Width < MinSliceWidth)
	//			MinSliceWidth = pSlice->Width;
	//		SliceArray.Add(pSlice);
	//		++SliceCount;

	//	}
	//}
//	Flag = 1;

	return SliceCount;
}

//int LinesSlice::FindMostUsualSliceWidth()
//{
//	if (OpticCellBlockArray.GetSize() == 0)
//	{
//		AfxMessageBox("FindMostUsualSiceWidth Error!");
//		return -1;
//	}
//
//	CArray<int>Histogram;
//	Histogram.SetSize(MaxSliceWidth - MinSliceWidth + 1);
//	int k;
//	for (int i = 0; i < SliceArray.GetSize(); ++i)
//	{
//		k = (SliceArray.GetAt(i)->Width) - MinSliceWidth;
//		++Histogram[k];
//	}
//	int n = 0;
//	int value = 0;
//	for (int i = 0; i < Histogram.GetSize(); ++i)
//	{
//		if (Histogram[i] > value)
//		{
//			n = i;
//			value = Histogram[i];
//		}
//	}
//	UsualSliceWidth = n + MinSliceWidth;
//
//
//
//	//CArray
//	return UsualSliceWidth;
//}

//void LinesSlice::DiagnoseEverySlice()
//{
//	if (UsualSliceWidth == -1 && SliceArray.GetSize() == 0)
//	{
//		AfxMessageBox("DiagnoseSlice Error!");
//		return;
//	}
//	int OriginalDefectThreshold = 6;
//	for (int i = 0; i < SliceArray.GetSize(); ++i)
//	{
//		if (abs(SliceArray.GetAt(i)->Width - UsualSliceWidth) > OriginalDefectThreshold)
//			SliceArray.GetAt(i)->DefectFlag = 1;
//
//	}
//
//	return;
//
//}

void LinesSlice::LinesSliceVisualization(CClientDC* pClient, TestDlg* pTestBoard)
{

	//画Cell
	pTestBoard->Invalidate();
	pTestBoard->UpdateWindow();
	pClient->SetViewportOrg(0, 0);


	pClient->Rectangle(0, 0, 2048, 300);
	pClient->Rectangle(0, 350, 2048, 300);
	pClient->SetViewportOrg(0, 300);

	CPen* pOldPen;
	CPen Up, Down, Threshold;
	//左右界限
	CPen Left, Right;
	//中轴线
	CPen Center;

	CPen Envelope;
	//包络线
	//============================================================
	Up.CreatePen(PS_SOLID, 4, RGB(255, 0, 0));//红色
	Down.CreatePen(PS_SOLID, 4, RGB(0, 255, 0));//绿色
	Threshold.CreatePen(PS_SOLID, 4, RGB(0, 0, 255));//蓝色
	//================================================================
	Left.CreatePen(PS_DASH, 1, RGB(0, 0, 0));//黑色
	Right.CreatePen(PS_DASH, 1, RGB(255, 0, 255));//洋红

	//==================================================================
	Center.CreatePen(PS_SOLID, 1, RGB(255, 69, 0));

	Envelope.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	//=====================================================
	//画Cell
	for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); ++i)//画波峰波谷
	{
		int flag = pOpticalNeuralNetwork->GetAt(i)->UpOrDown;
		if (flag > 0)
			pOldPen = pClient->SelectObject(&Up);
		else
			pOldPen = pClient->SelectObject(&Down);

		pClient->MoveTo(pOpticalNeuralNetwork->GetAt(i)->col, -pOpticalNeuralNetwork->GetAt(i)->GrayValue);
		pClient->LineTo(pOpticalNeuralNetwork->GetAt(i)->col, -pOpticalNeuralNetwork->GetAt(i)->GrayValue);
		pClient->SelectObject(pOldPen);
	}
	//画包络线
	pOldPen = pClient->SelectObject(&Envelope);
	pClient->MoveTo(pOpticalNeuralNetwork->GetAt(0)->col, -pOpticalNeuralNetwork->GetAt(0)->MovingAverage_Envelope_Max);
	for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{

		pClient->LineTo(pOpticalNeuralNetwork->GetAt(i)->col, -pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Max);


	}
	pClient->MoveTo(pOpticalNeuralNetwork->GetAt(0)->col, -pOpticalNeuralNetwork->GetAt(0)->MovingAverage_Envelope_Min);
	for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{

		pClient->LineTo(pOpticalNeuralNetwork->GetAt(i)->col, -pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Min);


	}
	pClient->SelectObject(pOldPen);
	//画阈值
	pOldPen = pClient->SelectObject(&Threshold);
	/*for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{
		if (i == 0)
			pClient->MoveTo(pOpticalNeuralNetwork->GetAt(i)->col,  - pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue);
		pClient->LineTo(pOpticalNeuralNetwork->GetAt(i)->col,  - pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue);
	}*/
	//============================================================================
	//画cellblock左右界限

	/*for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	{
		int size = OpticCellBlockArray[i]->OpticCellArray.GetSize();
		pClient->SelectObject(&Left);
		pClient->MoveTo(OpticCellBlockArray[i]->OpticCellArray.GetAt(0)->col, 300);
		pClient->LineTo(OpticCellBlockArray[i]->OpticCellArray.GetAt(0)->col, 0);

		pClient->SelectObject(&Right);
		pClient->MoveTo(OpticCellBlockArray[i]->OpticCellArray.GetAt(size - 1)->col, 300);
		pClient->LineTo(OpticCellBlockArray[i]->OpticCellArray.GetAt(size - 1)->col, 0);


	}
	pClient->SelectObject(pOldPen);*/
	//=======================================================================================
	//=======================================================================================
	//画OpticCellBlock
	pClient->SetViewportOrg(0, 700);
	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)//画波峰波谷
	//{
	//	if (OpticCellBlockArray.GetAt(i)->PeaksOrRoughs > 0)
	//	{
	//		pClient->SelectObject(&Up);
	//		/*else
	//			pClient->SelectObject(&Down);*/
	//		for (int k = 0; k < OpticCellBlockArray.GetAt(i)->OpticCellArray.GetSize(); ++k)
	//		{
	//			pClient->MoveTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col/2, - OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->GrayValue);
	//			pClient->LineTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col/2, - OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->GrayValue);
	//		}
	//	}
	//}

	//==================================================================================

	CFont NewFont;
	NewFont.CreatePointFont(70, "华文行楷", NULL);
	CFont* pOldFont = pClient->SelectObject(&NewFont);
	pClient->SelectObject(&Center);

	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	//{
	//	CString S;
	//	S.Format("%d", i);
	//	pClient->TextOut(OpticCellBlockArray[i]->MaxOrMinPos, -100, S);
	//	pClient->MoveTo(OpticCellBlockArray[i]->MaxOrMinPos, -700);
	//	pClient->LineTo(OpticCellBlockArray[i]->MaxOrMinPos, 0);
	//}
		//画Slice中心线
	//for (int i = 0; i < SliceArray.GetSize(); ++i)
	//{
	//	CString S;
	//	S.Format("%d", i);
	//	/*	if (SliceArray[i]->CenterlineLocation >= BufferSize || SliceArray[i]->CenterlineLocation < 0)
	//			DebugPrintMsgBox("%d行 %d根 中心为%f", ID, i, SliceArray[i]->CenterlineLocation);*/
	//	pClient->TextOut(SliceArray[i]->CenterlineLocation, -100, S);
	//	pClient->MoveTo(SliceArray[i]->CenterlineLocation, -700);
	//	pClient->LineTo(SliceArray[i]->CenterlineLocation, 0);


	//}
	for (int i = 0; i < SliceArray.GetSize(); ++i)
	{
		CString S;
		S.Format("%d", i);
		pClient->MoveTo(SliceArray[i]->LeftLocation, -700);
		pClient->LineTo(SliceArray[i]->LeftLocation, 0);
		pClient->TextOut(SliceArray[i]->CenterlineLocation, -100, S);
		pClient->MoveTo(SliceArray[i]->RightLocation, -700);
		pClient->LineTo(SliceArray[i]->RightLocation, 0);
	}
	//for (int i = 0; i < SliceArray.GetSize(); ++i)
	//{
	//	CString S;
	//	S.Format("%d", i);
	//	/*	if (SliceArray[i]->CenterlineLocation >= BufferSize || SliceArray[i]->CenterlineLocation < 0)
	//			DebugPrintMsgBox("%d行 %d根 中心为%f", ID, i, SliceArray[i]->CenterlineLocation);*/
	//	pClient->TextOut(SliceArray[i]->LeftLocation, -100, S);
	//	pClient->MoveTo(SliceArray[i]->LeftLocation, -700);
	//	pClient->LineTo(SliceArray[i]->LeftLocation, 0);
	//	pClient->TextOutA(SliceArray[i]->RightLocation, -100, S);
	//	pClient->MoveTo(SliceArray[i]->RightLocation, -700);
	//	pClient->LineTo(SliceArray[i]->RightLocation, 0);
	//} 
	pClient->SelectObject(pOldPen);

	//==============================================
	//测试
	pClient->SelectObject(pOldFont);
	//if(ID>100)
	int mmm = 10;
	for (int i = 0; i < SliceArray.GetSize(); ++i)
	{
		if (!(SliceArray[i]->CenterlineLocation >= SliceArray[i]->LeftLocation && SliceArray[i]->CenterlineLocation <= SliceArray[i]->RightLocation))
		{
			CString s;
			s.Format("%d行 %d根 中心线为%f 拟合数据点个数%d", ID, i, SliceArray[i]->CenterlineLocation, SliceArray[i]->OpticCellNumber);

			pClient->SetViewportOrg(0, 0);
			pClient->TextOutA(1100, mmm, s);
			mmm = mmm + 50;

		}
	}
	DebugPrintMsgBox("%d", ID);
	return;
	//==================================================
	//画Cellblock
	//pTestBoard->Invalidate();
	//pTestBoard->UpdateWindow();
	//
	//
	//pClient->Rectangle(0, 0,2048,300);
	//CPen* pOldPen;
	//CPen Up, Down, Threshold;
	//Up.CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
	//Down.CreatePen(PS_SOLID,4,RGB(0,255,0));
	//Threshold.CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)//画波峰波谷
	//{
	//	if (OpticCellBlockArray.GetAt(i)->PeaksOrRoughs > 1)
	//		pOldPen=pClient->SelectObject(&Up);
	//	else
	//		pOldPen = pClient->SelectObject(&Down);
	//	for (int k = 0; k < OpticCellBlockArray.GetAt(i)->OpticCellArray.GetSize(); ++k)
	//	{
	//		pClient->MoveTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col,300- OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->GrayValue);
	//		pClient->LineTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col, 300-OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->GrayValue);
	//	}
	//	pClient->SelectObject(pOldPen);
	//}
	//pOldPen = pClient->SelectObject(&Threshold);

	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)//画阈值曲线
	//{
	//	for (int k = 0; k < OpticCellBlockArray.GetAt(i)->OpticCellArray.GetSize(); ++k)
	//	{
	//		pClient->MoveTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col, 300-OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->ThreSholdValue);
	//		pClient->LineTo(OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->col, 300-OpticCellBlockArray.GetAt(i)->OpticCellArray.GetAt(k)->ThreSholdValue);
	//	}
	//}
	//pClient->SelectObject(pOldPen);
	//return;
}
