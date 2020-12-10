#pragma once
class Line;
class LinesSlice;
class Slice
{
public:

	Slice(float Center, float Left, float Right, int ID, double time, double spindleposition, double slideposition);
	//调试用
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
	int DefectFlag;//该Silce是否为缺陷 缺陷为1，否则为0
	int Width;//线宽
	void EvaluateOverlap(Line* pLine, CPoint* pret);
	//	LinesSlice* pLinesSlice;
	void  EvaluateOverlap(Slice* pSlice, CPoint* pret);
	void ApplyLineAndSlice(Line* pline);
	void Allocate2Line();
	double Time;//ms为单位
	double SpindlePosition;
	double SlidePosition;
	double SlidePositionbase;
	int SlideDirection;
	//===================================================================
	//调试用
	int OpticCellNumber;
	double* pCoefficients;
	double* pGrayValues;
	double* pCols;
};

