#include "pch.h"
#include "VisualArea.h"


VisualArea::VisualArea()
{
	BinaryzationWindowRadius = 30;//求包络线的窗口大小
	AverageFilterEnvelopeWindowRadius = 30;//包络线移动平均窗口大小
	pOpticalNeuralNetwork = 0;
	OpticalNeuralNetwork_Columns = 2048;
}


VisualArea::~VisualArea()
{
	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	//	delete OpticCellBlockArray[i];
	//OpticCellBlockArray.RemoveAll();
	//DestroyOpticalNeuralNetwork();
}

void VisualArea::Clear()
{
	for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
		delete OpticCellBlockArray[i];
	OpticCellBlockArray.RemoveAll();

	for (int i = 0; i < TempOpticCellArray.GetSize(); ++i)
		delete TempOpticCellArray[i];
	TempOpticCellArray.RemoveAll();
}

void VisualArea::CreateOpticalNeuralNetwork(int columns, BYTE* pLinedata)
{
	pOpticalNeuralNetwork = new CArray<OpticCell*>;
	pOpticalNeuralNetwork->SetSize(columns);
	OpticalNeuralNetwork_Columns = columns;
	OpticCell* pOpticCell;
	for (int i = 0; i < columns; i++)
	{
		pOpticCell = new OpticCell(this, i, columns, pLinedata[i]);
		pOpticalNeuralNetwork->SetAt(i, pOpticCell);
	}
}

void VisualArea::CreateOpticalNeuralNetwork(int columns)
{
	pOpticalNeuralNetwork = new CArray<OpticCell*>;
	pOpticalNeuralNetwork->SetSize(columns);
	OpticalNeuralNetwork_Columns = columns;
	OpticCell* pOpticCell;
	for (int i = 0; i < columns; i++)
	{
		pOpticCell = new OpticCell(this, i, columns, 0);

		pOpticalNeuralNetwork->SetAt(i, pOpticCell);
	}
}

void VisualArea::DestroyOpticalNeuralNetwork()
{
	if (!pOpticalNeuralNetwork)
		return;
	int i, n = (int)pOpticalNeuralNetwork->GetSize();
	for (i = 0; i < n; i++)
		delete pOpticalNeuralNetwork->GetAt(i);
	OpticalNeuralNetwork_Columns = 0;
	pOpticalNeuralNetwork->RemoveAll();
	delete pOpticalNeuralNetwork;
}

int  VisualArea::CalThreshold()
{//计算波峰波谷阈值
	if (!pOpticalNeuralNetwork)
	{
		AfxMessageBox("CalThreshold Error!");
		return -1;
	}

	//DebugPrintMsgBox("%d", pOpticalNeuralNetwork->GetSize());
	//for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); i++)
	//{
	//	if (i == 0)
	//	{
	//		double total = 0;
	//		for (int n = 0; n <= 2 * BinaryzationWindowRadius; n++)
	//			total += pOpticalNeuralNetwork->GetAt(n)->GrayValue;
	//		pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue = total / (2 * BinaryzationWindowRadius + 1);
	//		pOpticalNeuralNetwork->GetAt(i)->UpOrDown = (pOpticalNeuralNetwork->GetAt(i)->GrayValue > pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue) ? 1 : 0;

	//	}
	//	else if (i > 0 && i <= BinaryzationWindowRadius)
	//	{
	//		pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue = pOpticalNeuralNetwork->GetAt(0)->ThreSholdValue;
	//		pOpticalNeuralNetwork->GetAt(i)->UpOrDown = (pOpticalNeuralNetwork->GetAt(i)->GrayValue > pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue) ? 1 : 0;
	//	}

	//	else if (i > BinaryzationWindowRadius && i <= pOpticalNeuralNetwork->GetSize() - 1 - BinaryzationWindowRadius)
	//	{
	//		double PreviousTotal = pOpticalNeuralNetwork->GetAt(i - 1)->ThreSholdValue * (2 * BinaryzationWindowRadius + 1);
	//		double CurrentTotal = PreviousTotal - (pOpticalNeuralNetwork->GetAt(i - 1 - BinaryzationWindowRadius)->GrayValue) + (pOpticalNeuralNetwork->GetAt(i + BinaryzationWindowRadius)->GrayValue);
	//		pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue = CurrentTotal / (2 * BinaryzationWindowRadius + 1);
	//		pOpticalNeuralNetwork->GetAt(i)->UpOrDown = (pOpticalNeuralNetwork->GetAt(i)->GrayValue > pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue) ? 1 : 0;
	//	}
	//	else
	//	{
	//		pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue = pOpticalNeuralNetwork->GetAt(pOpticalNeuralNetwork->GetSize() - 1 - BinaryzationWindowRadius)->ThreSholdValue;
	//		pOpticalNeuralNetwork->GetAt(i)->UpOrDown = (pOpticalNeuralNetwork->GetAt(i)->GrayValue > pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue) ? 1 : 0;
	//	}
	//}
	CalEnvelope();
	MovingAverageEnvelope();
	ThresholdOpticCell();
	return 0;
}


int VisualArea::ConvergeForPeaksRoughs()
{//聚合波峰波谷
	//生成OpticCellBlockArray;
	if (!pOpticalNeuralNetwork)
	{
		AfxMessageBox("ConvergeForPeaksRoughs Error!");
		return -1;
	}
	//int FilterThreshold = 4;//滤波的阈值，block少于阈值的
	//int PeaksRoughsCount = 0;
	//
	//int i = 0;
	//while (i < OpticalNeuralNetwork_Columns)
	//{
	//	int FlagL = pOpticalNeuralNetwork->GetAt(i)->UpOrDown;
	//	OpticCellBlock* pOpticCellBlock = new OpticCellBlock(FlagL);
	//	while ((i < OpticalNeuralNetwork_Columns) && (FlagL == pOpticalNeuralNetwork->GetAt(i)->UpOrDown))
	//	{
	//		pOpticCellBlock->OpticCellArray.Add(pOpticalNeuralNetwork->GetAt(i));
	//		++i;
	//	}
	//	if (pOpticCellBlock->OpticCellArray.GetSize() >= FilterThreshold)
	//	{
	//		/*	pOpticCellBlock->FittingInitial();
	//			pOpticCellBlock->LeastSquareFittingPolynomial();*/
	//		pOpticCellBlock->Left = pOpticCellBlock->OpticCellArray[0]->col;
	//		pOpticCellBlock->Right = pOpticCellBlock->OpticCellArray[pOpticCellBlock->OpticCellArray.GetSize() - 1]->col;
	//		OpticCellBlockArray.Add(pOpticCellBlock);
	//		++PeaksRoughsCount;
	//	}
	//	else
	//	{
	//		delete pOpticCellBlock;
	//	}
	//}
	//for (int i = 0; i < OpticCellBlockArray.GetSize() - 1; ++i)
	//{
	//	if (OpticCellBlockArray[i]->PeaksOrRoughs == OpticCellBlockArray[i + 1]->PeaksOrRoughs)
	//	{
	//		int BeginCol = OpticCellBlockArray[i]->OpticCellArray[OpticCellBlockArray[i]->OpticCellArray.GetSize() - 1]->col;
	//		int BeginGrayValue = OpticCellBlockArray[i]->OpticCellArray[OpticCellBlockArray[i]->OpticCellArray.GetSize() - 1]->GrayValue;
	//		int EndCol = OpticCellBlockArray[i + 1]->OpticCellArray[0]->col;
	//		int EndGrayValue = OpticCellBlockArray[i + 1]->OpticCellArray[0]->GrayValue;
	//		int A = (EndGrayValue - BeginGrayValue) / (EndCol - BeginCol);
	//		int B = BeginGrayValue - A * BeginCol;
	//		for (int k = BeginCol + 1; k < EndCol; ++k)
	//		{
	//			OpticCell* pOpticCell = new OpticCell(NULL, k, OpticalNeuralNetwork_Columns, (A * k + B));
	//			OpticCellBlockArray[i]->OpticCellArray.Add(pOpticCell);
	//			TempOpticCellArray.Add(pOpticCell);
	//		}
	//		OpticCellBlockArray[i]->OpticCellArray.Append(OpticCellBlockArray[i + 1]->OpticCellArray);
	//		OpticCellBlockArray[i]->Left = OpticCellBlockArray[i]->OpticCellArray[0]->col;
	//		OpticCellBlockArray[i]->Right = OpticCellBlockArray[i]->OpticCellArray[OpticCellBlockArray[i]->OpticCellArray.GetSize() - 1]->col;
	//		delete OpticCellBlockArray[i + 1];
	//		OpticCellBlockArray.RemoveAt(i + 1);
	//		i--;
	//	}
	//}
	//DebugPrintMsgBox("%d", PeaksRoughsCount);
	int PeaksRoughsCount = 0;
	int i = 0;
	while (i < OpticalNeuralNetwork_Columns)
	{
		while (i < OpticalNeuralNetwork_Columns && pOpticalNeuralNetwork->GetAt(i)->UpOrDown != 0 )
		{
			i++;			
		}

		if (i >= OpticalNeuralNetwork_Columns)
			break;

			int FlagL = pOpticalNeuralNetwork->GetAt(i)->UpOrDown;
			if (FlagL == 0)
			{
				OpticCellBlock* pOpticCellBlock = new OpticCellBlock(FlagL);
				while ((i < OpticalNeuralNetwork_Columns) && (pOpticalNeuralNetwork->GetAt(i)->UpOrDown==0))
				{
					pOpticCellBlock->OpticCellArray.Add(pOpticalNeuralNetwork->GetAt(i));
					++i;
				}
				pOpticCellBlock->GetBlockInformation();
				//DebugPrintf("Block:%d %d %d\n", pOpticCellBlock->Left, pOpticCellBlock->Right, pOpticCellBlock->Width);
				OpticCellBlockArray.Add(pOpticCellBlock);
				PeaksRoughsCount++;
			}
		
	}
	int space=8;//可调参数,判断两离散block是否为同一block的间隔
	ReConverge(space);

	return PeaksRoughsCount;
}

void VisualArea::ReConverge(int space)
{
	if (OpticCellBlockArray.GetSize() < 2)
		return;
	for (int i = 0; i < OpticCellBlockArray.GetSize() - 1; ++i)
	{
		if (OpticCellBlockArray[i + 1]->Left - OpticCellBlockArray[i]->Right < space)
		{
			//DebugPrintf("400:%d\n", OpticCellBlockArray[i + 1]->OpticCellArray.GetSize());
			OpticCellBlockArray[i]->OpticCellArray.Append(OpticCellBlockArray[i + 1]->OpticCellArray);
			delete OpticCellBlockArray[i + 1];
			OpticCellBlockArray.RemoveAt(i + 1);
			OpticCellBlockArray[i]->GetBlockInformation();
			i--;
		}
	}
}

void VisualArea::FitAccuratePeaksRoughsPos()
{
	//int n = 0;
	//for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	//{
	//	if (OpticCellBlockArray[i]->PeaksOrRoughs > 0)
	//	{
	//		
	//		OpticCellBlockArray[i]->FittingInitial();
	//		if(OpticCellBlockArray[i]->LeastSquareFittingPolynomial()==0)
	//			DebugPrintMsgBox("%d根 位置%f", n,OpticCellBlockArray[i]->PeaksRoughsCol);
	//		++n;
	//	}
	//}

	for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	{
		if (OpticCellBlockArray[i]->PeaksOrRoughs > 0)
		{
			OpticCellBlockArray[i]->pSliceMath = pSliceMath;
			OpticCellBlockArray[i]->FittingInitial();
			OpticCellBlockArray[i]->LeastSquareFittingPolynomial();
		}
	}
}

void VisualArea::ExtendCellBlock()
{
	double Percentage = 0;

	for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
		OpticCellBlockArray[i]->FindHighestOrLowestPos();

	for (int i = 1; i < OpticCellBlockArray.GetSize() - 1; ++i)
	{
		if (i == 0)
		{
			if (OpticCellBlockArray[i]->PeaksOrRoughs)
			{
				double RightExtendThreshold = Percentage * (OpticCellBlockArray[i]->MaxOrMinValue - OpticCellBlockArray[i + 1]->MaxOrMinValue) + OpticCellBlockArray[i + 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;

				for (int n = OpticCellBlockArray[i]->Right + 1; n <= OpticCellBlockArray[i + 1]->MaxOrMinPos; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue > RightExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.Append(*pOpticCellArray);

				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;



			}
			else
			{
				double RightExtendThreshold = -Percentage * (OpticCellBlockArray[i + 1]->MaxOrMinValue - OpticCellBlockArray[i]->MaxOrMinValue) + OpticCellBlockArray[i + 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;

				for (int n = OpticCellBlockArray[i]->Right + 1; n <= OpticCellBlockArray[i + 1]->MaxOrMinPos; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue < RightExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.Append(*pOpticCellArray);

				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;


			}


		}
		else if (i == OpticCellBlockArray.GetSize() - 1)
		{
			if (OpticCellBlockArray[i]->PeaksOrRoughs)
			{
				double LeftExtendThreshold = Percentage * (OpticCellBlockArray[i]->MaxOrMinValue - OpticCellBlockArray[i - 1]->MaxOrMinValue) + OpticCellBlockArray[i - 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i - 1]->MaxOrMinPos; n < OpticCellBlockArray[i]->Left; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue > LeftExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.InsertAt(0, pOpticCellArray);
				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;
			}
			else
			{
				double LeftExtendThreshold = -Percentage * (OpticCellBlockArray[i - 1]->MaxOrMinValue - OpticCellBlockArray[i]->MaxOrMinValue) + OpticCellBlockArray[i - 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i - 1]->MaxOrMinPos; n < OpticCellBlockArray[i]->Left; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue < LeftExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.InsertAt(0, pOpticCellArray);
				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;
			}
		}
		else
		{
			if (OpticCellBlockArray[i]->PeaksOrRoughs)
			{
				double LeftExtendThreshold = Percentage * (OpticCellBlockArray[i]->MaxOrMinValue - OpticCellBlockArray[i - 1]->MaxOrMinValue) + OpticCellBlockArray[i - 1]->MaxOrMinValue;
				double RightExtendThreshold = Percentage * (OpticCellBlockArray[i]->MaxOrMinValue - OpticCellBlockArray[i + 1]->MaxOrMinValue) + OpticCellBlockArray[i + 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i - 1]->MaxOrMinPos; n < OpticCellBlockArray[i]->Left; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue > LeftExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.InsertAt(0, pOpticCellArray);
				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;

				pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i]->Right + 1; n <= OpticCellBlockArray[i + 1]->MaxOrMinPos; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue > RightExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.Append(*pOpticCellArray);

				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;



			}
			else
			{
				double LeftExtendThreshold = -Percentage * (OpticCellBlockArray[i - 1]->MaxOrMinValue - OpticCellBlockArray[i]->MaxOrMinValue) + OpticCellBlockArray[i - 1]->MaxOrMinValue;
				double RightExtendThreshold = -Percentage * (OpticCellBlockArray[i + 1]->MaxOrMinValue - OpticCellBlockArray[i]->MaxOrMinValue) + OpticCellBlockArray[i + 1]->MaxOrMinValue;
				CArray<OpticCell*>* pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i - 1]->MaxOrMinPos; n < OpticCellBlockArray[i]->Left; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue < LeftExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.InsertAt(0, pOpticCellArray);
				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;

				pOpticCellArray = new CArray<OpticCell*>;
				for (int n = OpticCellBlockArray[i]->Right + 1; n <= OpticCellBlockArray[i + 1]->MaxOrMinPos; ++n)
				{
					if (pOpticalNeuralNetwork->GetAt(n)->GrayValue < RightExtendThreshold)
					{
						pOpticCellArray->Add(pOpticalNeuralNetwork->GetAt(n));

					}

				}
				OpticCellBlockArray[i]->OpticCellArray.Append(*pOpticCellArray);

				pOpticCellArray->RemoveAll();
				delete pOpticCellArray;
			}
		}
	}
	//DebugPrintMsgBox("OpticCellBlockArray.GetSize() %d", OpticCellBlockArray.GetSize());
	if (OpticCellBlockArray.GetSize() > 2)
	{
		delete OpticCellBlockArray[0];
		OpticCellBlockArray.RemoveAt(0);
		delete OpticCellBlockArray[OpticCellBlockArray.GetSize() - 1];
		OpticCellBlockArray.RemoveAt(OpticCellBlockArray.GetSize() - 1);
	}
	else return;

	for (int i = 0; i < OpticCellBlockArray.GetSize(); ++i)
	{
		OpticCellBlockArray[i]->Left = OpticCellBlockArray[i]->OpticCellArray.GetAt(0)->col;
		OpticCellBlockArray[i]->Right = OpticCellBlockArray[i]->OpticCellArray.GetAt(OpticCellBlockArray[i]->OpticCellArray.GetSize() - 1)->col;
	}
}
 
void VisualArea::CalEnvelope()
{
	CalMaxMinWithinBinaryzationWindowRadius(BinaryzationWindowRadius);
	int i;
	for (i=BinaryzationWindowRadius; i > 0; --i)
	{
		pOpticalNeuralNetwork->GetAt(i-1)->Pos_Envelope_Max = pOpticalNeuralNetwork->GetAt(i)->Pos_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i - 1)->Value_Envelope_Max = pOpticalNeuralNetwork->GetAt(i )->Value_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i - 1)->Pos_Envelope_Min = pOpticalNeuralNetwork->GetAt(i )->Pos_Envelope_Min;
		pOpticalNeuralNetwork->GetAt(i - 1)->Value_Envelope_Min = pOpticalNeuralNetwork->GetAt(i )->Value_Envelope_Min;
	}	
	for ( i= BinaryzationWindowRadius; i < pOpticalNeuralNetwork->GetSize()- BinaryzationWindowRadius; ++i)
	{				
			CalMaxMinWithinBinaryzationWindowRadius(i);
		
	}
	for (; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{
		pOpticalNeuralNetwork->GetAt(i)->Pos_Envelope_Max = pOpticalNeuralNetwork->GetAt(i - 1)->Pos_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i)->Value_Envelope_Max = pOpticalNeuralNetwork->GetAt(i - 1)->Value_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i)->Pos_Envelope_Min = pOpticalNeuralNetwork->GetAt(i - 1)->Pos_Envelope_Min;
		pOpticalNeuralNetwork->GetAt(i)->Value_Envelope_Min = pOpticalNeuralNetwork->GetAt(i - 1)->Value_Envelope_Min;
	}
}

void VisualArea::ThresholdOpticCell()
{
	double P = 0.7;//可调参数,调节分界阈值
	for (int i = 0; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{
		pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue = (1-P) * pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Max + pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Min * P;
		pOpticalNeuralNetwork->GetAt(i)->UpOrDown = (pOpticalNeuralNetwork->GetAt(i)->GrayValue > pOpticalNeuralNetwork->GetAt(i)->ThreSholdValue) ? 1 : 0;
	}
}

void VisualArea::CalMaxMinWithinBinaryzationWindowRadius(int CellPosition)
{
	int MaxPosition, MinPosition, MaxValue, MinValue;
	MaxPosition = MinPosition = CellPosition;
	MaxValue = MinValue = pOpticalNeuralNetwork->GetAt(CellPosition)->GrayValue;

	for (int i = CellPosition - BinaryzationWindowRadius; i <= CellPosition + BinaryzationWindowRadius;++i)
	{
		if (pOpticalNeuralNetwork->GetAt(i)->GrayValue > MaxValue)
		{
			MaxPosition = i;
			MaxValue = pOpticalNeuralNetwork->GetAt(i)->GrayValue;
		}
		if (pOpticalNeuralNetwork->GetAt(i)->GrayValue < MinValue)
		{
			MinPosition = i;
			MinValue= pOpticalNeuralNetwork->GetAt(i)->GrayValue;
		}
	}
	pOpticalNeuralNetwork->GetAt(CellPosition)->Pos_Envelope_Max = MaxPosition;
	pOpticalNeuralNetwork->GetAt(CellPosition)->Value_Envelope_Max = MaxValue;
	pOpticalNeuralNetwork->GetAt(CellPosition)->Pos_Envelope_Min = MinPosition;
	pOpticalNeuralNetwork->GetAt(CellPosition)->Value_Envelope_Min = MinValue;
}

void VisualArea::MovingAverageEnvelope()
{
	double Maxtotal, Mintotal;
	Maxtotal = Mintotal = 0;
	
	for (int i = 0; i < 2 * AverageFilterEnvelopeWindowRadius; ++i)
	{
		Maxtotal += pOpticalNeuralNetwork->GetAt(i)->Value_Envelope_Max;
		Mintotal+= pOpticalNeuralNetwork->GetAt(i)->Value_Envelope_Min;
	}
	pOpticalNeuralNetwork->GetAt(AverageFilterEnvelopeWindowRadius)->MovingAverage_Envelope_Max = Maxtotal / (AverageFilterEnvelopeWindowRadius * 2 + 1);
	pOpticalNeuralNetwork->GetAt(AverageFilterEnvelopeWindowRadius)->MovingAverage_Envelope_Min = Mintotal / (AverageFilterEnvelopeWindowRadius * 2 + 1);
	//==================================================================================
	for (int i = AverageFilterEnvelopeWindowRadius; i >0; --i)
	{
		pOpticalNeuralNetwork->GetAt(i-1)->MovingAverage_Envelope_Max = pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i-1)->MovingAverage_Envelope_Min = pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Min;

	}
	for (int i = AverageFilterEnvelopeWindowRadius + 1; i < pOpticalNeuralNetwork->GetSize() - AverageFilterEnvelopeWindowRadius;++i)
	{
		Maxtotal = Maxtotal - pOpticalNeuralNetwork->GetAt(i - 1 - AverageFilterEnvelopeWindowRadius)->Value_Envelope_Max+ pOpticalNeuralNetwork->GetAt(i + AverageFilterEnvelopeWindowRadius)->Value_Envelope_Max;
		Mintotal = Mintotal - pOpticalNeuralNetwork->GetAt(i - 1 - AverageFilterEnvelopeWindowRadius)->Value_Envelope_Min + pOpticalNeuralNetwork->GetAt(i + AverageFilterEnvelopeWindowRadius)->Value_Envelope_Min;
		pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Max= Maxtotal / (AverageFilterEnvelopeWindowRadius * 2 + 1);
		pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Min = Mintotal / (AverageFilterEnvelopeWindowRadius * 2 + 1);

	}
	for (int i = pOpticalNeuralNetwork->GetSize() - AverageFilterEnvelopeWindowRadius; i < pOpticalNeuralNetwork->GetSize(); ++i)
	{
		pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Max = pOpticalNeuralNetwork->GetAt(i-1)->MovingAverage_Envelope_Max;
		pOpticalNeuralNetwork->GetAt(i)->MovingAverage_Envelope_Min = pOpticalNeuralNetwork->GetAt(i-1)->MovingAverage_Envelope_Min;

	}

}

int VisualArea::FindMostUsusalLineWidth()
{
	if (OpticCellBlockArray.GetSize() < 2)
		return -1;
	int MaxLineWidth = FindMaxLineWidth();
	int MinLineWidth = FindMinLineWidth();
	CArray<int>Historgram;
	Historgram.SetSize(MaxLineWidth - MinLineWidth + 1);
	int width= OpticCellBlockArray[ 1]->MaxOrMinPos - OpticCellBlockArray[0]->MaxOrMinPos+1;
	for (int i = 1; i < OpticCellBlockArray.GetSize()-1; ++i)
	{
		width = OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos- MinLineWidth+1;
		Historgram[width]++;
	}
	int value = Historgram[0];
	int pos = 0;
	for (int i = 1; i < Historgram.GetSize(); ++i)
	{
		if (Historgram[i] > value)
		{
			value = Historgram[i];
			pos = i;
		}

	}
	UsusalLineWidth = pos + MinLineWidth;
	//DebugPrintf("min:%d max:%d usual:%d value:%d\n", MinLineWidth, MaxLineWidth, UsusalLineWidth,Historgram[pos]);
	return UsusalLineWidth;

}

int VisualArea::FindMaxLineWidth()
{
	if (OpticCellBlockArray.IsEmpty())
	{
		AfxMessageBox("OpticCellBlockArray Empty!");
		return -1;
	}
	//int n;
	//for (n = 0; n < OpticCellBlockArray.GetSize(); ++n)
	//{
	//	if (OpticCellBlockArray[n]->PeaksOrRoughs == 1)
	//		break;
	//}
	//int MaxBlockWidth = OpticCellBlockArray[n]->Width;
	//for (int i = n+1; i < OpticCellBlockArray.GetSize(); ++i)
	//{
	//	if (OpticCellBlockArray[i]->PeaksOrRoughs == 1&&OpticCellBlockArray[i]->Width > MaxBlockWidth)
	//		MaxBlockWidth = OpticCellBlockArray[i]->Width;
	//}
	int MaxBlockWidth = OpticCellBlockArray[1]->MaxOrMinPos - OpticCellBlockArray[0]->MaxOrMinPos+1;
	for (int i = 1; i < OpticCellBlockArray.GetSize() - 1; ++i)
	{
		if (OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos+1 > MaxBlockWidth)
			MaxBlockWidth = OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos+1;
	}
	return MaxBlockWidth;
}

int VisualArea::FindMinLineWidth()
{
	if (OpticCellBlockArray.IsEmpty())
	{
		AfxMessageBox("OpticCellBlockArray Empty!");
		return -1;
	}
	/*int n;
	for (n = 0; n < OpticCellBlockArray.GetSize(); ++n)
	{
		if (OpticCellBlockArray[n]->PeaksOrRoughs == 1)
			break;
	}
	int MinBlockWidth = OpticCellBlockArray[n]->Width;
	for (int i = n+1; i < OpticCellBlockArray.GetSize(); ++i)
	{
		if (OpticCellBlockArray[i]->PeaksOrRoughs == 1 &&OpticCellBlockArray[i]->Width<MinBlockWidth)
			MinBlockWidth = OpticCellBlockArray[i]->Width;
	}
	*/
	int MinBlockWidth = OpticCellBlockArray[1]->MaxOrMinPos - OpticCellBlockArray[0]->MaxOrMinPos+1;
	for (int i = 1; i < OpticCellBlockArray.GetSize() - 1; ++i)
	{
		if (OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos+1 < MinBlockWidth)
			MinBlockWidth = OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos+1;
	}
	return MinBlockWidth;
}

void VisualArea::CutBlockIntoSlice(int numleft, int numright)
{
	int Left = OpticCellBlockArray[numleft]->MaxOrMinPos;
	int Right = OpticCellBlockArray[ numright]->MaxOrMinPos;
	//========================可调参数，左右搜索区间===================================
	int SearchSpace = 0.2*UsusalLineWidth;//搜素区间的大小
	int LeftSearchDistance = UsusalLineWidth;//左侧搜索区间矩左侧的距离
	int RightSearchDistance = UsusalLineWidth;//右侧搜索区间矩右侧的距离
	//================================================================
	int Leftfirst = Left + LeftSearchDistance - SearchSpace / 2;//左侧的搜索区间
	int Leftlast = Left + LeftSearchDistance + SearchSpace / 2;
	int Rightfirst = Right - RightSearchDistance - SearchSpace / 2;//右侧的搜索区间
	int Rightlast = Right - RightSearchDistance +SearchSpace / 2;
	//============================================================
	int valueLeft = pOpticalNeuralNetwork->GetAt(Leftfirst)->GrayValue;
	int posLeft = pOpticalNeuralNetwork->GetAt(Leftfirst)->col;
	int valueRight= pOpticalNeuralNetwork->GetAt(Rightfirst)->GrayValue;
	int posRight= pOpticalNeuralNetwork->GetAt(Rightfirst)->col;
	//=========================================================
	for (int i = Leftfirst+1; i <= Leftlast; ++i)
	{
		if (pOpticalNeuralNetwork->GetAt(i)->GrayValue < valueLeft)
		{
			valueLeft = pOpticalNeuralNetwork->GetAt(i)->GrayValue;
			posLeft = pOpticalNeuralNetwork->GetAt(i)->col;
		}
	}

	for (int i = Rightfirst + 1; i <= Rightlast; ++i)
	{
		if (pOpticalNeuralNetwork->GetAt(i)->GrayValue < valueRight)
		{
			valueRight = pOpticalNeuralNetwork->GetAt(i)->GrayValue;
			posRight = pOpticalNeuralNetwork->GetAt(i)->col;
		}
	}
	int value;
	int pos;
	if (valueLeft < valueRight)
	{
		value = valueLeft;
		pos = posLeft;
	}
	else
	{
		value = valueRight;
		pos = posRight;
	}
	OpticCellBlock* pOpticCellBlock;
	pOpticCellBlock = new OpticCellBlock(0);
	pOpticalNeuralNetwork->GetAt(pos)->UpOrDown = 0;
	pOpticCellBlock->OpticCellArray.Add(pOpticalNeuralNetwork->GetAt(pos));
	pOpticCellBlock->GetBlockInformation();
	OpticCellBlockArray.InsertAt(numright, pOpticCellBlock);

}

void VisualArea::RecutBlock()
{
	if (OpticCellBlockArray.GetSize() < 2)
		return;
	double P = 1.1;//可调参数，调节判断粘连block的线宽界限
	for (int i = 0; i < OpticCellBlockArray.GetSize()-1; ++i)
	{
		int distance = OpticCellBlockArray[i + 1]->MaxOrMinPos - OpticCellBlockArray[i]->MaxOrMinPos;
		if (distance > P * UsusalLineWidth)
		{
			CutBlockIntoSlice(i, i + 1);
			--i;
		}
	}
}






