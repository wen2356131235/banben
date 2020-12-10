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
	int Flag;//0:δ����1��slice�Ѿ��������
	CArray<Slice*> SliceArray;
	double Time;//msΪ��λ
	double SpindlePosition;
	double SlidePosition;
	int ShowMode;

	

	TestDlg* pTestBoard;
	CClientDC* pClient;



	void deel(CArray<OpticCell*>*popticcellarray);
	int CreateSlices();
//	int FindMostUsualSliceWidth();//���������߿��С
//void DiagnoseEverySlice();//���ÿ��Slice���߿����ͨ�߿���ϴ�����DefectFlagΪ1
	void LinesSliceVisualization(CClientDC* pClient, TestDlg* pTestBoard);//���ӻ�
	void ResetOpticalNeuralNetwork(CArray<OpticCell*>* popticalneuralnetwork);
	void SetShowMode(int showmode);

	int IsIn2048(int i);

};

