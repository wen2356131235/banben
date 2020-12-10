#pragma once
//version number:20180202

//#include "mkl.h"

//#define EIGEN_USE_MKL_ALL
//#define EIGEN_VECTORIZE_SSE4_2
//#include <Eigen\Core>
//#include <Eigen\Dense>
//using namespace Eigen;
//using namespace std;

class _declspec(dllexport) myMath
{
public:
	myMath(void);
	~myMath(void);
//	void printMatrixXd(MatrixXd &A);
//	void printMatrixXf(MatrixXf &A);

	double std(double * pd,int n);
	__int64 VersionNumber;
	__int64 FirstNumberLargerThanThreshold(double * p, int n, double Threshold);
	__int64 maxByArray_index(double * p,int n);
	__int64 minByArray_index(double * p, int n);
	float CalStringSimilarity(CString s0,CString s1);//计算两个字符串相似程度
	float average(float * p, int m);
	float Correlation(float * p, int m, float * q, int n);
	float Correlation(float * p, float * q, int n);
	double Correlation(double * p, double * q, int n);
	void LinearInterpolation(float* px, float *py, int m, float* qx, float* qy, int n);
	void FillingMissingValue(int* px, float *py, int m, int* qx, float* qy, int& n);

	int calculate_fx0(double from, double to, double *coefficientlist, long maxH, float step, double *output, long &n);
	double calculate_y(double* coefficientlist, long List_Length, double x);
	
	int AddFittingLineInput(double* px, int size, int FittingTimes);//FittingTimes 返回系数个数
	int DeleteFittingLineInput(int id);
	int FittingLineByIndex(int coeID,double* py,double* preturn);
	int FittingLineByIndex(double* py, int size, double* preturn);
	int FittingLine(double* px, double* py, int m, double* preturn, int FittingTimes, int info = 0); //FittingTimes 返回系数个数
	int ExtremeValue(double* px, double* py, int m, double *outputx, double *outputy, char* pdirection, long &n);//int retbufMAX, 
	void ExtremeValue(double from, double to, double *coefficientlist, long coefListSize, double *output, char* pdirection, long &n);
	int isNumber_str(CString s);
	double CalIncludedAngle(double v0[2], double v1[2]);
	/*
	对数据序列进行窗口数据拟合，得到一系列不同频率数据集，
	对各频率系数进行拟合，以此对未来系数进行预测
	*/
//protected:
	CPtrArray FittingIntermediateResults;
};

