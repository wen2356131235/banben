#pragma once
class AlarmingData
{
public:
	///报警类型:
	/// 回叠:			1 报警
	/// 正常跨匝:		2 不需要报警
	/// 前进碾压历史:	3 不需要报警
	/// 缝隙:			4 报警
	/// 不正常跨匝:		5 报警
	int AlarmingType;

	
//	int ActionType;//报警动作

	double Time;//ms为单位
	double SpindlePosition;
	double SlidePosition;
	double SpindleSpeed;
	double SlideSpeed;
};

