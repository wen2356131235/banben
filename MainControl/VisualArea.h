#pragma once
#include "OpticCell.h"
#include "OpticCellBlock.h"
class VisualArea
{
public:
	VisualArea();
	~VisualArea();

	void Clear();

	CArray<OpticCell*>*  pOpticalNeuralNetwork;

	CArray<OpticCellBlock*> OpticCellBlockArray;
	//void ResetOpticalNeuralNetwork(CArray<OpticCell*>* popticalneuralnetwork);
	int	BinaryzationWindowRadius;//������ߵĴ���
	int OpticalNeuralNetwork_Columns;
	int AverageFilterEnvelopeWindowRadius;//�Ӱ�����ƽ���Ĵ���
	int UsusalLineWidth;//��ͨ�߿�
	myMath* pSliceMath;
	void CreateOpticalNeuralNetwork(int columns, BYTE* pLinedata);
	void CreateOpticalNeuralNetwork(int columns);
	void DestroyOpticalNeuralNetwork();
	//=============�Բ������¼���======================
	int  CalThreshold();//������ֵ
	void CalEnvelope();//���������
	void CalMaxMinWithinBinaryzationWindowRadius(int CellPosition);//�Ҵ��������ֵ����Сֵλ�ã���ʼ�����ߣ�
	void MovingAverageEnvelope();//ƽ��������
	void ThresholdOpticCell();//����ϸ����ֵ������
	//====================================================
	int ConvergeForPeaksRoughs();//�ۺϲ���
	void ReConverge(int space);//����block�������¾ۺ�
	//========================================================
	int  FindMostUsusalLineWidth();//����ͨ�߿�
	int FindMaxLineWidth();//������߿�
	int FindMinLineWidth();//����С�߿�
	void CutBlockIntoSlice(int numleft, int numright);//��ճ��block�з֣�numleft��numrightΪճ������block��blockarray�����
	void RecutBlock();//��CutBlock������ճ��block�����з�
	
	




//====================�����������================
	void FitAccuratePeaksRoughsPos();
	void ExtendCellBlock();
	CArray<OpticCell*> TempOpticCellArray;
//	Eigen::Vector3f v3;

};

