#include "pch.h"
#include "VisualArea.h"


VisualArea::VisualArea()
{
//	v3(3)=1.0;
}


VisualArea::~VisualArea()
{
	DestroyOpticalNeuralNetwork();
}


void VisualArea::CreateOpticalNeuralNetwork(int column)
{
	pOpticalNeuralNetwork = new CArray<OpticCell*>;
	OpticalNeuralNetwork_Column = column, OpticalNeuralNetwork_Row = row;
	int i, n = column * row;
	OpticCell * pOpticCell;
	for (i = 0; i < n; i++)
	{
		pOpticCell = new OpticCell(pOpticalNeuralNetwork, i%column, i / column, 640, 480);
		pOpticalNeuralNetwork->Add(pOpticCell);
	}

}

void VisualArea::DestroyOpticalNeuralNetwork()
{
	int i, n = (int)pOpticalNeuralNetwork->GetSize();
	for (i = 0; i < n; i++)
		delete pOpticalNeuralNetwork->GetAt(i);

	OpticalNeuralNetwork_Column = 0, OpticalNeuralNetwork_Row = 0;
	pOpticalNeuralNetwork->RemoveAll();
	delete pOpticalNeuralNetwork;
}
