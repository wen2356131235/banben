#pragma once


// Dlg_CameraParameter 对话框

class Dlg_CameraParameter : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_CameraParameter)

public:
	Dlg_CameraParameter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dlg_CameraParameter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CameraParameter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ServerName;
	CEdit CompanyName;
	CEdit AcquisitionLineRate;
	CEdit ExposureTime;
	CEdit Gain;
	virtual BOOL OnInitDialog();
	void InitCameraParameter();
	afx_msg void OnBnClickedApplication();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
