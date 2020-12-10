#pragma once
#include "OpticCell.h"
class OpticCellBlock
{
public:
	OpticCellBlock(int peaksorrough);//peaksorrough 波峰波谷的标识 波峰为1 波谷为0
	~OpticCellBlock();

	CArray<OpticCell*> OpticCellArray;
	int OpticCellNumber;
	double* pCoefficients;
	double* pGrayValues;
	double* pCols;
	double DeltaCol;
	float PeaksRoughsCol;
	int PeaksOrRoughs;//Peak==1 Rough==0
	int MaxOrMinPos;
	int MaxOrMinValue;
	int Left;
	int Right;
	int Width;
	myMath* pSliceMath;
	/*int MaxPos;
	int MaxValue;
	int MinPos;
	int MinValue;*/

	void FittingInitial();
	int  LeastSquareFittingPolynomial();
	int FindHighestOrLowestPos();//获取极小值（波谷）或者极大值(波峰)
	void GetBlockInformation();//获取Blocl的左 右 以及极小值
	int GetLeft();
	int GetRight();
	int GetWidth();
	/*int  FindHighestPos();
	int  FindLowestPos();*/
};

