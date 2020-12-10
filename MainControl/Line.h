#pragma once
#include "Slice.h"
class Line
{
public:
	Line(Slice* pSlice, myMath* pmymath);
	~Line();

	int AlarmingType;
	Line* pGapLine;
	CArray<Line*> EnemyArray;//记录强者的地址，当自己死亡的时候，告诉强者
	int KillLeft, KillRight;//自身为强者，记录由于自身而死亡者，处于自己的左边和右边的统计
	CArray<Slice*> KillLeftLine;
	CArray<Slice*> KillRightLine;
	int AddDeadLinetoKillLeftLine(Line* pLine, double threshold);
	int AddDeadLinetoKillRightLine(Line* pLine, double threshold);
	float Left, Right,Center;
	float Width;
	int Length;
	float CalDistancePoint2Line(Slice* pSlice);
	
	int ID;
	byte R,G,B;
	myMath* pmyMath;
	CArray<Slice*> SliceArray;
	int CurrentSliceArrayPos;
	int SliceArraySize;
	int AddSlice(Slice* pSlice);

	void CalSpiralAngle();


	void draw(HDC dDC, int ScrHight);
	Slice* GetSliceReverse(int pos);
};

