#pragma once
#include "OpticCell.h"
class VisualArea
{
public:
	VisualArea();
	~VisualArea();

	CArray<OpticCell*>*  pOpticalNeuralNetwork;
	int OpticalNeuralNetwork_Column, OpticalNeuralNetwork_Row;
	void CreateOpticalNeuralNetwork(int column);
	void DestroyOpticalNeuralNetwork();


};

