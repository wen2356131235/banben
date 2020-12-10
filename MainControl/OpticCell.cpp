#include "pch.h"
#include "OpticCell.h"
#include"VisualArea.h"


OpticCell::OpticCell(VisualArea* pInputVisualArea, int column,int opticalneuralnetwork_column, int grayvalue)
{
	ThreSholdValue = 0;
	UpOrDown = -1;
	pVisualArea = pInputVisualArea;
	col = column;
	OpticalNeuralNetwork_Column = opticalneuralnetwork_column;
	GrayValue = grayvalue;
}

OpticCell::~OpticCell()
{

}




