#pragma once
class VisualArea;
class OpticCell
{
public:
	OpticCell(VisualArea*, int column, int opticalneuralnetwork_column, int grayvalue);
	~OpticCell();

	int col;
	float ThreSholdValue;
	int UpOrDown;
	int OpticalNeuralNetwork_Column;
	int GrayValue;

	VisualArea* pVisualArea;

	//============°üÂçÏß===============
	int Pos_Envelope_Max;
	int Value_Envelope_Max;

	int Pos_Envelope_Min;
	int Value_Envelope_Min;

	float MovingAverage_Envelope_Max;
	float MovingAverage_Envelope_Min;



};

