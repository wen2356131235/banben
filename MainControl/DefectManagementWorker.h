#pragma once



// DefectManagementWorker
class AlarmingData;
class DefectManagementWorker : public CWinThread
{
	DECLARE_DYNCREATE(DefectManagementWorker)

protected:
	DefectManagementWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~DefectManagementWorker();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	struct AlarmSlideSubsection
	{
		int SlidePos;
		CArray<AlarmingData*> AlarmingDataArray;
	};
	void init();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//pAlarmingDataSet步长为0.1mm
	double AlarmingDataSetStep;
	int FistSlidePos;
	CArray<AlarmSlideSubsection*> pAlarmingDataSet;
	CArray<AlarmingData*> AlarmingDataArray;
	void DeelAlarmingData(AlarmingData* pAlarmingData);
	AlarmingData* FindSimilarAlarmingData(AlarmingData* pAlarmingData);
	int AddAlarmingData(AlarmingData* pAlarmingData);
	void AdjustBrightness(int MAX);
	void Timer(UINT_PTR nEvent);
	void PLCTimer();
	void MakeValueInRange();
	int LightFlag;
	int LightValue;
	int Step;
	int PixelMax;
	WLightCtrl* pWLightCtrl;
	UINT_PTR Timer100;
	CClientDC* pViewDC;
#ifdef _M_X64
#else
	PlcCtrl* pPlcCtrl;//64位注释掉
#endif 
};


