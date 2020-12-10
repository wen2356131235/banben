#pragma once
#include "Slice.h"
#include "VisualArea.h"

#include"TestDlg.h"
//class DataPacket;
class LinesSlice:public VisualArea
{
public:
	LinesSlice(byte* pbuffer,int n,int id,double time,double spindlepostion,double slidepostion);

	LinesSlice(byte* pbuffer, int n, int id, double time, double spindlepostion, double slidepostion, CClientDC* pdc);
	
	~LinesSlice();
	void Smooth(BYTE* psmoothbuf);
	void draw(byte* pScrBuf24Color);

	int ID;
	byte* pBuffer;
	int BufferSize;
	int Flag;//0:未处理，1，slice已经生成完毕
	CArray<Slice*> SliceArray;
	double Time;//ms为单位
	double SpindlePosition;
	double SlidePosition;
	int ShowMode;

	

	TestDlg* pTestBoard;
	CClientDC* pClient;



	void deel(CArray<OpticCell*>*popticcellarray);
	int CreateSlices();
//	int FindMostUsualSliceWidth();//查找正常线宽大小
//void DiagnoseEverySlice();//诊断每个Slice，线宽跟普通线宽差距较大设置DefectFlag为1
	void LinesSliceVisualization(CClientDC* pClient, TestDlg* pTestBoard);//可视化
	void ResetOpticalNeuralNetwork(CArray<OpticCell*>* popticalneuralnetwork);
	void SetShowMode(int showmode);

	int IsIn2048(int i);

};

