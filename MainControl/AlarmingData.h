#pragma once
class AlarmingData
{
public:
	///��������:
	/// �ص�:			1 ����
	/// ��������:		2 ����Ҫ����
	/// ǰ����ѹ��ʷ:	3 ����Ҫ����
	/// ��϶:			4 ����
	/// ����������:		5 ����
	int AlarmingType;

	
//	int ActionType;//��������

	double Time;//msΪ��λ
	double SpindlePosition;
	double SlidePosition;
	double SpindleSpeed;
	double SlideSpeed;
};

