#pragma once
class Line;
class LinesSlice;
class Slice
{
public:

	Slice(float Center, float Left, float Right, int ID, double time, double spindleposition, double slideposition);
	//������
	Slice(float Center, float Left, float Right, int ID, int opticcellnumber, double* pcoefficients, double* pgrayvalues, double* pcols);

	~Slice();

	byte GapMarking;
	byte R, G, B;
	Line* pRelyOnLine;
	CArray<Line*> LineApplyArray;
	int LinsSliceID;
	float CenterlineLocation;// CenterlineLocation100;
	float LeftLocation;
	float RightLocation;
	int DefectFlag;//��Silce�Ƿ�Ϊȱ�� ȱ��Ϊ1������Ϊ0
	int Width;//�߿�
	void EvaluateOverlap(Line* pLine, CPoint* pret);
	//	LinesSlice* pLinesSlice;
	void  EvaluateOverlap(Slice* pSlice, CPoint* pret);
	void ApplyLineAndSlice(Line* pline);
	void Allocate2Line();
	double Time;//msΪ��λ
	double SpindlePosition;
	double SlidePosition;
	double SlidePositionbase;
	int SlideDirection;
	//===================================================================
	//������
	int OpticCellNumber;
	double* pCoefficients;
	double* pGrayValues;
	double* pCols;
};

