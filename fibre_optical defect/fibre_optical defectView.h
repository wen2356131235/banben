
// fibre_optical defectView.h: CfibreopticaldefectView 类的接口
//

#pragma once
#include"..//MainControl//MainCtrl.h"
///x86///////////////////
#ifdef _M_X64
#ifdef _DEBUG
#pragma comment(lib,"..//x64//Debug//MainControl.lib")
#else
#pragma comment(lib,"..//x64//Release//MainControl.lib")

#endif
////x64////////////
#else
#ifdef _DEBUG
#pragma comment(lib,"..//Debug//MainControl.lib")
#else
#pragma comment(lib,"..//Release//MainControl.lib")
#endif

#endif // COMPILE
#include"Dlg_CameraParameter.h"



class CfibreopticaldefectView : public CFormView
{
protected: // 仅从序列化创建
	CfibreopticaldefectView() noexcept;
	DECLARE_DYNCREATE(CfibreopticaldefectView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FIBRE_OPTICALDEFECT_FORM };
#endif

// 特性
public:
	CfibreopticaldefectDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CfibreopticaldefectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	MainCtrl* pMainCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCameraparameter();
	Dlg_CameraParameter* m_pDlg;
};

#ifndef _DEBUG  // fibre_optical defectView.cpp 中的调试版本
inline CfibreopticaldefectDoc* CfibreopticaldefectView::GetDocument() const
   { return reinterpret_cast<CfibreopticaldefectDoc*>(m_pDocument); }
#endif

