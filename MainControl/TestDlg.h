#pragma once
#include"TestDlg.h"

// TestDlg 对话框

class TestDlg : public CDialog
{
	DECLARE_DYNAMIC(TestDlg)

public:
	TestDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TestDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
