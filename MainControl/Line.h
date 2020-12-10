#pragma once
#include "Slice.h"
class Line
{
public:
	Line(Slice* pSlice, myMath* pmymath);
	~Line();

	int AlarmingType;
	Line* pGapLine;
	CArray<Line*> EnemyArray;//��¼ǿ�ߵĵ�ַ�����Լ�������ʱ�򣬸���ǿ��
	int KillLeft, KillRight;//����Ϊǿ�ߣ���¼��������������ߣ������Լ�����ߺ��ұߵ�ͳ��
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

