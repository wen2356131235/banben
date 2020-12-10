
// fibre_optical defectView.cpp: CfibreopticaldefectView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "fibre_optical defect.h"
#endif

#include "fibre_optical defectDoc.h"
#include "fibre_optical defectView.h"
#include"..//MainControl//MainCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfibreopticaldefectView

IMPLEMENT_DYNCREATE(CfibreopticaldefectView, CFormView)

BEGIN_MESSAGE_MAP(CfibreopticaldefectView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_COMMAND(ID_CameraParameter, &CfibreopticaldefectView::OnCameraparameter)
END_MESSAGE_MAP()

// CfibreopticaldefectView 构造/析构

CfibreopticaldefectView::CfibreopticaldefectView() noexcept
	: CFormView(IDD_FIBRE_OPTICALDEFECT_FORM)
{
	// TODO: 在此处添加构造代码
	pMainCtrl = 0;
}

CfibreopticaldefectView::~CfibreopticaldefectView()
{
}

void CfibreopticaldefectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CfibreopticaldefectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CfibreopticaldefectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	SetTimer(0, 100, 0);
	//pMainControl = AfxBeginThread(RUNTIME_CLASS(MainCtrl));
	//MainControl->PostThreadMessageW(message_draw, WPARAM(this), 0);


}


// CfibreopticaldefectView 打印

BOOL CfibreopticaldefectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CfibreopticaldefectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CfibreopticaldefectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CfibreopticaldefectView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此处添加自定义打印代码
}


// CfibreopticaldefectView 诊断

#ifdef _DEBUG
void CfibreopticaldefectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CfibreopticaldefectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CfibreopticaldefectDoc* CfibreopticaldefectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfibreopticaldefectDoc)));
	return (CfibreopticaldefectDoc*)m_pDocument;
}
#endif //_DEBUG


// CfibreopticaldefectView 消息处理程序


void CfibreopticaldefectView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case 0:
		{			
			KillTimer(0);		
			pMainCtrl = new MainCtrl(this);
			SetTimer(1, 50, 0);
			SetTimer(2, 50, 0);
			break;
		}
		case 1:
		{
			MSG msg;
			msg.message = WM_KEYDOWN;
			UCHAR* p = (UCHAR*)&msg.lParam;
			p[2] = 47;
			if (pMainCtrl)pMainCtrl->TranslateMessage(&msg);
			break;
		}
		case 2:
		{
			MSG msg;
			msg.message = WM_USER + 0;//MSG_drawScreen;
			if (pMainCtrl)pMainCtrl->TranslateMessage(&msg);
			break;
		}
		default:
			break;
	}
	CFormView::OnTimer(nIDEvent);
}


BOOL CfibreopticaldefectView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		case WM_MOUSEMOVE:
			pMsg->hwnd = this->GetSafeHwnd();
			break;
	}
	if(pMainCtrl)pMainCtrl->TranslateMessage(pMsg);
	return CFormView::PreTranslateMessage(pMsg);
}


void CfibreopticaldefectView::OnCameraparameter()
{
	Dlg_CameraParameter* pDlg = new Dlg_CameraParameter;
	pDlg->Create(IDD_CameraParameter);
	pDlg->ShowWindow(SW_SHOW);
	// TODO: 在此添加命令处理程序代码
}
