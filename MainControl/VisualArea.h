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
	int	BinaryzationWindowRadius;//求包络线的窗口
	int OpticalNeuralNetwork_Columns;
	int AverageFilterEnvelopeWindowRadius;//队包络线平滑的窗口
	int UsusalLineWidth;//普通线宽
	myMath* pSliceMath;
	void CreateOpticalNeuralNetwork(int columns, BYTE* pLinedata);
	void CreateOpticalNeuralNetwork(int columns);
	void DestroyOpticalNeuralNetwork();
	//=============以波谷重新计算======================
	int  CalThreshold();//计算阈值
	void CalEnvelope();//计算包络线
	void CalMaxMinWithinBinaryzationWindowRadius(int CellPosition);//找窗口内最大值和最小值位置（初始包络线）
	void MovingAverageEnvelope();//平滑包络线
	void ThresholdOpticCell();//对视细胞二值化处理
	//====================================================
	int ConvergeForPeaksRoughs();//聚合波谷
	void ReConverge(int space);//相邻block进行重新聚合
	//========================================================
	int  FindMostUsusalLineWidth();//找普通线宽
	int FindMaxLineWidth();//找最大线宽
	int FindMinLineWidth();//找最小线宽
	void CutBlockIntoSlice(int numleft, int numright);//对粘连block切分，numleft、numright为粘连的两block在blockarray的序号
	void RecutBlock();//用CutBlock函数对粘连block进行切分
	
	




//====================舍弃波峰拟合================
	void FitAccuratePeaksRoughsPos();
	void ExtendCellBlock();
	CArray<OpticCell*> TempOpticCellArray;
//	Eigen::Vector3f v3;

};

