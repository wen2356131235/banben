#pragma once
#include "OpticCell.h"
class OpticCellBlock
{
public:
	OpticCellBlock(int peaksorrough);//peaksorrough ���岨�ȵı�ʶ ����Ϊ1 ����Ϊ0
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
	int FindHighestOrLowestPos();//��ȡ��Сֵ�����ȣ����߼���ֵ(����)
	void GetBlockInformation();//��ȡBlocl���� �� �Լ���Сֵ
	int GetLeft();
	int GetRight();
	int GetWidth();
	/*int  FindHighestPos();
	int  FindLowestPos();*/
};

