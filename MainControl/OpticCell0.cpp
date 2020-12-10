#include "pch.h"
#include "OpticCell.h"


OpticCell::OpticCell(CArray<OpticCell*>* popticalneuralnetwork, int column, int row, int opticalneuralnetwork_column, int opticalneuralnetwork_row)
{
	pOpticalNeuralNetwork = popticalneuralnetwork;
	R = G = B = -10000;
	x = y = z = -10000.0;
	Column = column, Row = row;
	flag = 0, lastflag = 0, flag3 = 0, flag4 = 0;
	OpticalNeuralNetwork_Column = opticalneuralnetwork_column;
	OpticalNeuralNetwork_Row = opticalneuralnetwork_row;
}


OpticCell::~OpticCell()
{
}


void OpticCell::WorkpieceUnit(CArray<OpticCell *> *pseedpointarr, CArray<OpticCell *> *pobjectpointarr, CArray<OpticCell *> *pedgepointarr)
{
	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - 1;
	ccolumn0 = Column - 1;
	rrow1 = Row + 2;
	ccolumn1 = Column + 2;

	OpticCell *pOpticCell;
	int i, j;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column)
			{
				ModifyFlag(2);
				continue;
			}

			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);

			if (pOpticCell->flag == -1)
			{
				ModifyFlag(2);
				continue;
			}

			double d = 1000;
			if (pOpticCell->flag == 0) {
				d = sqrt((x - pOpticCell->x)*(x - pOpticCell->x)
					+ (y - pOpticCell->y)*(y - pOpticCell->y)
					+ (z - pOpticCell->z)*(z - pOpticCell->z));
				if (d <= 25) {
					pOpticCell->ModifyFlag(1);
					pseedpointarr->Add(pOpticCell);
				}
				else {
					pOpticCell->ModifyFlag(-1);
					ModifyFlag(2);
				}
			}
		}
	}

	if (flag == 2)
		pedgepointarr->Add(pseedpointarr->GetAt(0));

	pobjectpointarr->Add(pseedpointarr->GetAt(0));
	pseedpointarr->RemoveAt(0);

}


void OpticCell::DilationUnit(CArray<OpticCell *> *pobjectpointarr, int r)
{
	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - r;
	ccolumn0 = Column - r;
	rrow1 = Row + r + 1;
	ccolumn1 = Column + r + 1;

	OpticCell *pOpticCell;
	int i, j;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column) continue;
			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);

			if (pOpticCell->flag == 0 || pOpticCell->flag == -1 || pOpticCell->flag == -3) {
				pOpticCell->ModifyFlag(3);
				pobjectpointarr->Add(pOpticCell);
				//DebugPrintf("%d\n", flag);
			}
		}
	}

}


void OpticCell::ErosionUnit(CArray<OpticCell *> *pobjectpointarr, int r)
{
	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - r;
	ccolumn0 = Column - r;
	rrow1 = Row + r + 1;
	ccolumn1 = Column + r + 1;

	OpticCell *pOpticCell;
	int i, j;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column) continue;
			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);
			pOpticCell->ModifyFlag(-3);
		}
	}

}


void OpticCell::EdgePointUnit(CArray<OpticCell *> *pedgepointarr, int fflag)
{

	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - 1;
	ccolumn0 = Column - 1;
	rrow1 = Row + 2;
	ccolumn1 = Column + 2;

	OpticCell *pOpticCell;
	int i, j;
	int k = 0;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column) continue;
			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);

			if (pOpticCell->flag != 0 && pOpticCell->flag != -1 && pOpticCell->flag != -3) k++;
		}
	}
	if (k != 9) {
		ModifyFlag(fflag);
		pedgepointarr->Add(pOpticalNeuralNetwork->GetAt(Row * OpticalNeuralNetwork_Column + Column));
	}
}


void OpticCell::EdgePointUnit(CArray<OpticCell *> *pedgepointarr)
{

	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - 1;
	ccolumn0 = Column - 1;
	rrow1 = Row + 2;
	ccolumn1 = Column + 2;

	OpticCell *pOpticCell;
	int i, j;
	int k = 0;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column) continue;
			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);

			if (pOpticCell->flag != 0 && pOpticCell->flag != -1 && pOpticCell->flag != -3) k++;
		}
	}
	if (k != 9) {
		pedgepointarr->Add(pOpticalNeuralNetwork->GetAt(Row * OpticalNeuralNetwork_Column + Column));
	}
}


void OpticCell::SetNewZ(int r) {

	if (flag3 != 3) return;

	int rrow0, rrow1, ccolumn0, ccolumn1;
	rrow0 = Row - r;
	ccolumn0 = Column - r;
	rrow1 = Row + r + 1;
	ccolumn1 = Column + r + 1;

	int k = 0;
	double zz = 0;

	OpticCell *pOpticCell;
	int i, j;
	for (i = rrow0; i < rrow1; i++)
	{
		for (j = ccolumn0; j < ccolumn1; j++)
		{
			if (i < 0 || i >= OpticalNeuralNetwork_Row || j < 0 || j >= OpticalNeuralNetwork_Column) continue;
			pOpticCell = pOpticalNeuralNetwork->GetAt(i * OpticalNeuralNetwork_Column + j);
			if (pOpticCell->flag == 1 || pOpticCell->flag == 2 || pOpticCell->flag == 4 || pOpticCell->flag == 5) {
				zz += pOpticCell->z;
				k++;
			}
		}
	}
	//DebugPrintf("%d\n", k);
	z = zz / k;

}


void OpticCell::ModifyFlag(int fflag)
{
	lastflag = flag;
	flag = fflag;
	if (flag == 3)
	{
		flag3 = 3;
	}

	if (flag == 4)
	{
		flag4 = 4;
	}
}