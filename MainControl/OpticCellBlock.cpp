#include "pch.h"
#include "OpticCellBlock.h"

OpticCellBlock::OpticCellBlock(int peaksorrough)
{
	PeaksOrRoughs = peaksorrough;
	pCoefficients = 0;
	pGrayValues = 0;
	pCols = 0;
	Right = Left = -100;
	//MaxValue = MaxPos = MinValue = MinPos = 0;
	MaxOrMinPos = MaxOrMinValue = -100;

}

OpticCellBlock::~OpticCellBlock()
{
	if (pCoefficients)
		delete[]pCoefficients;
	if (pGrayValues)
		delete[]pGrayValues;
	if (pCols)
		delete[]pCols;
	/*	for (int i = 0; i < OpticCellArray.GetSize(); ++i)
		{
			if (OpticCellArray.GetAt(i)->pVisualArea == NULL)
				delete OpticCellArray.GetAt(i);
		}*/
	if(OpticCellArray.GetSize()!=0)
	OpticCellArray.RemoveAll();
}
void OpticCellBlock::FittingInitial()
{
	/*double x[100];
	for (int i = 0; i < 100; ++i)
	{
		x[i] = i;
	}
	double y[100];
	double ret[3];
	for (int i = 0; i < 100; ++i)
	{
		y[i] = 0.89 * x[i] * x[i] + 0.54 * x[i] + 1.65;
	}
	pSliceMath->FittingLineByIndex(0, y, ret);
	DebugPrintMsgBox("%lf %lf %lf", ret[2], ret[1], ret[0]);*/

	OpticCellNumber = OpticCellArray.GetSize();

	pCoefficients = new double[3];
	pGrayValues = new double[OpticCellNumber];
	pCols = new double[OpticCellNumber];
	DeltaCol = OpticCellArray.GetAt(0)->col;
	for (int i = 0; i < OpticCellNumber; i++)
	{
		pGrayValues[i] = OpticCellArray.GetAt(i)->GrayValue;
		pCols[i] = (OpticCellArray.GetAt(i)->col) - DeltaCol;
	}

}
int  OpticCellBlock::LeastSquareFittingPolynomial()
{
	pSliceMath->FittingLineByIndex(OpticCellNumber - 5, pGrayValues, pCoefficients);
//	pSliceMath->FittingLine(pCols, pGrayValues, OpticCellNumber, pCoefficients, 3);
	PeaksRoughsCol = -pCoefficients[1] / (2 * pCoefficients[2]) + DeltaCol;

	if (!(PeaksRoughsCol >= Left && PeaksRoughsCol <= Right))
	{
		PeaksRoughsCol = MaxOrMinPos;
		return 0;
	}
	else
		return 1;

}

int OpticCellBlock::FindHighestOrLowestPos()
{
	MaxOrMinPos = OpticCellArray[0]->col;
	MaxOrMinValue = OpticCellArray[0]->GrayValue;
	if (PeaksOrRoughs)
	{
		for (int i = 0; i < OpticCellArray.GetSize(); ++i)
		{
			if (OpticCellArray[i]->GrayValue > MaxOrMinValue)
			{
				MaxOrMinValue = OpticCellArray[i]->GrayValue;
				MaxOrMinPos = OpticCellArray[i]->col;
			}

		}

	}
	else
	{
		for (int i = 0; i < OpticCellArray.GetSize(); ++i)
		{
			if (OpticCellArray[i]->GrayValue < MaxOrMinValue)
			{
				MaxOrMinValue = OpticCellArray[i]->GrayValue;
				MaxOrMinPos = OpticCellArray[i]->col;
			}
		}
	}
	return MaxOrMinValue;
}

void OpticCellBlock::GetBlockInformation()
{
	if (OpticCellArray.IsEmpty())
	{
		AfxMessageBox("New Blcok Error");
		return;
	}
	Left = OpticCellArray[0]->col;
	Right = OpticCellArray[OpticCellArray.GetSize()-1]->col;
	Width = Right - Left + 1;
	FindHighestOrLowestPos();


}

int OpticCellBlock::GetLeft()
{
	return Left;
}

int OpticCellBlock::GetRight()
{
	return Right;
}

int OpticCellBlock::GetWidth()
{
	return Width;
}

//int OpticCellBlock::FindHighestPos()
//{
//	if (PeaksOrRoughs == 0)
//		AfxMessageBox("FindHighestPos Error!");
//	 MaxPos = OpticCellArray[0]->col;
//	 MaxValue = OpticCellArray[0]->GrayValue;
//
//	for (int i = 0; i < OpticCellArray.GetSize(); ++i)
//	{
//		if (OpticCellArray[i]->GrayValue > MaxValue)
//		{
//			MaxValue = OpticCellArray[i]->GrayValue;
//			MaxPos = OpticCellArray[i]->col;
//		}
//
//	}
//
//	return MaxValue;
//}

//int OpticCellBlock::FindLowestPos()
//{
//	if (PeaksOrRoughs == 1)
//		AfxMessageBox("FindLowestPos Error!");
//
//	MinPos= OpticCellArray[0]->col;
//	MinValue= OpticCellArray[0]->GrayValue;
//
//	for (int i = 0; i < OpticCellArray.GetSize(); ++i)
//	{
//		if (OpticCellArray[i]->GrayValue < MinValue)
//		{
//			MinValue = OpticCellArray[i]->GrayValue;
//			MinPos = OpticCellArray[i]->col;
//		}
//	}
//	return MinValue;
//}

