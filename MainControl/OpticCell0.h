#pragma once
class OpticCell
{
public:
	OpticCell(CArray<OpticCell*>* popticalneuralnetwork, int column, int row, int opticalneuralnetwork_column, int opticalneuralnetwork_row);
	~OpticCell();

	CArray<OpticCell *> *pOpticalNeuralNetwork;
	void WorkpieceUnit(CArray<OpticCell *> *pseedpointarr, CArray<OpticCell *> *pobjectpointarr, CArray<OpticCell *> *pedgepointarr);
	void DilationUnit(CArray<OpticCell *> *pobjectpointarr, int r);
	void ErosionUnit(CArray<OpticCell *> *pobjectpointarr, int r);
	void EdgePointUnit(CArray<OpticCell *> *pedgepointarr, int fflag);
	void EdgePointUnit(CArray<OpticCell *> *pedgepointarr);
	void SetNewZ(int r = 1);
	int R, G, B;
	double x, y, z;
	int Row, Column;
	int OpticalNeuralNetwork_Column, OpticalNeuralNetwork_Row;
	int flag, lastflag, flag3, flag4;
	void ModifyFlag(int fflag);
};

