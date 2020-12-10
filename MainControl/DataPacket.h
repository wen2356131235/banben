#pragma once
#include "Picture.h"
#include "Line.h"
#include "LinesSlice.h"
class DataPacket
{
public:
	DataPacket();
	~DataPacket();

	CPtrArray* pResourceArray;
	int LineVitality;
	int SlideDirection;//��̨����1���ң�-1����0ֹͣ
	CArray<Line*> LineArray;
	CArray<Line*> LineArray_AfterFiltering;
//protected:
	int picIDcount;
	CArray<Picture*> PictureArray;
	int CurrentPicPos;
	int PictureArraySize;

	double AverageSliceWidth,
		AverageSliceWidth_half;
	double TripleStandardDeviation;
	double SliceWidthAverageError;

	CArray<LinesSlice*> LinesSliceArray;
	int CurrentLinesSlicePos;
	int LinesSliceArraySize;
	int LinesIDcount;
	int LinesSliceIDcount;
	int CompleteLinesSliceArrayPos;
	int CompleteSliceID;


public:
	myMath* pmyMath;
	int myMathindex5_2;

//	void AddReLinesSlice(LinesSlice* plinesslice);
	void AddLinesSlice(LinesSlice* plinesslice);
	int AddPicture(Picture* pPic);
	Picture* GetPictureReverse(int pos);
	LinesSlice* GetLinesSliceReverse(int pos);
	void LineGrowUp();
	void LineSliceMatchLines(LinesSlice* pLineSlice);
	void LineWidthAnalysis();
	void DeleteInactivelines();
	void AverageSliceWidthAnalysis(Slice* pSlice);
	int LineAddtoSliceApply(Slice* pSlice);
	void CentralizedTreatmentofDefects(MSG* pMsg);
	int isLineLiving(Line* pLine);
	void EvaluateOverlap(CPoint p1, CPoint p2, CPoint* pret);
	void EvaluateUnion(CPoint p1, CPoint p2, CPoint* pret);
	void DefectiveJudgment_BrokenLongLine(Line* pLine);
};

