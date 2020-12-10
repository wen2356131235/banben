// Dlg_CameraParameter.cpp: 实现文件
//

#include "pch.h"
#include "fibre_optical defect.h"
#include "Dlg_CameraParameter.h"
#include "afxdialogex.h"


// Dlg_CameraParameter 对话框

IMPLEMENT_DYNAMIC(Dlg_CameraParameter, CDialogEx)

Dlg_CameraParameter::Dlg_CameraParameter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CameraParameter, pParent)
{

}

Dlg_CameraParameter::~Dlg_CameraParameter()
{
}

void Dlg_CameraParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServerName, ServerName);
	DDX_Control(pDX, IDC_CompanyName, CompanyName);
	DDX_Control(pDX, IDC_AcquisitionLineRate, AcquisitionLineRate);
	DDX_Control(pDX, IDC_ExposureTime, ExposureTime);
	DDX_Control(pDX, IDC_Gain, Gain);
	InitCameraParameter();
}


BEGIN_MESSAGE_MAP(Dlg_CameraParameter, CDialogEx)
	ON_BN_CLICKED(IDC_APPLICATION, &Dlg_CameraParameter::OnBnClickedApplication)
END_MESSAGE_MAP()


// Dlg_CameraParameter 消息处理程序


BOOL Dlg_CameraParameter::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	
	this->SetWindowText("CameraParameter");

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void Dlg_CameraParameter::InitCameraParameter()
{
	CFile file;
	CString s;
	if (file.Open("T_Linea_M2048-7um_Default_Default.ccf", CFile::modeReadWrite))
	{
		ULONGLONG Len = file.GetLength();
		file.Read(s.GetBuffer(int(Len)), (UINT)Len);
		s.ReleaseBuffer();
		//=====================ServerName==========================	
		int num = s.Find("Server Name");
		int num1 = s.Find("=", num);
		int num2 = s.Find('\n', num);
		if (s.Find("\r", num) >= 0 && s.Find("\r", num) < num2)
			num2 = s.Find("\r", num);
		CString Sservername = s.Mid(num1 + 1, num2 - num1 - 1);
		ServerName.SetWindowTextA(Sservername);
		//=========================CompanyName====================================
		num = s.Find("Company Name");
		num1 = s.Find("=", num);
		num2 = s.Find('\n', num);
		if (s.Find("\r", num) >= 0 && s.Find("\r", num) < num2)
			num2 = s.Find("\r", num);
		CString SCompanyName = s.Mid(num1 + 1, num2 - num1 - 1);
		CompanyName.SetWindowTextA(SCompanyName);
		//==========================AcquisitionLineRate	===========================
		num = s.Find("AcquisitionLineRate");
		num2 = s.Find('\n', num);
		if (s.Find("\r", num) >= 0 && s.Find("\r", num) < num2)
			num2 = s.Find("\r", num);
		int i;
		for (i = num; i < num2; ++i)
		{
			if ('0' <= s[i] && '9' >= s[i])
				break;
		}
		CString SAcquisitionLineRate = s.Mid(i, num2 - i);
		AcquisitionLineRate.SetWindowTextA(SAcquisitionLineRate);
		//=========================ExposureTime====================================
		num = s.Find("ExposureTime");
		num2 = s.Find('\n', num);
		if (s.Find("\r", num) >= 0 && s.Find("\r", num) < num2)
			num2 = s.Find("\r", num);
		for (i = num; i < num2; ++i)
		{
			if ('0' <= s[i] && '9' >= s[i])
				break;
		}
		CString SExposureTime = s.Mid(i, num2 - i);
		ExposureTime.SetWindowTextA(SExposureTime);
		//==========================Gain======================================		
		num = s.Find("Gain	");
		num2 = s.Find('\n', num);
		if (s.Find("\r", num) >= 0 && s.Find("\r", num) < num2)
			num2 = s.Find("\r", num);

		for (i = num; i < num2; ++i)
		{
			if ('0' <= s[i] && '9' >= s[i])
			{
				break;
			}
		}

		CString SGain = s.Mid(i, num2 - i);
		Gain.SetWindowTextA(SGain);
	}
	else
	{
		AfxMessageBox("Error!");
		return;
	}
	file.Close();
}


void Dlg_CameraParameter::OnBnClickedApplication()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox("确定要更改相机参数？", "Warning", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
		return;
	CFile file;
	CString s;
	if (file.Open("T_Linea_M2048-7um_Default_Default.ccf", CFile::modeReadWrite))
	{
		ULONGLONG Len = file.GetLength();
		file.Read(s.GetBuffer(int(Len)), (UINT)Len);
		s.ReleaseBuffer();
		/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////AcquisitionLineRate////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////
		int num = s.Find("AcquisitionLineRate	");
		int num1 = s.Find("\n", num);
		CString Left = s.Left(num);
		CString Right = s.Right(int(Len) - (num1 + 1));
		CString Middle = s.Mid(num, num1 - num + 1);
		int i;
		for (i = 0; i < Middle.GetLength(); ++i)
		{
			if ('0' <= Middle[i] && '9' >= Middle[i])
				break;
		}
		CString MiddleLeft = Middle.Left(i);
		///==============================================================================
		CString MiddleRight = Middle.Right(Middle.GetLength() - i);
		for (i = 0; i < MiddleRight.GetLength(); ++i)
		{
			if (!('0' <= MiddleRight[i] && '9' >= MiddleRight[i]))
				break;
		}
		CString MiddleRightLeft = MiddleRight.Left(i);
		CString MiddleRightRight = MiddleRight.Right(MiddleRight.GetLength() - i);
		//===========================================================================
		CString Text;
		AcquisitionLineRate.GetWindowTextA(Text);
		for (int i = 0; i < Text.GetLength(); ++i)
		{
			if (!(isdigit(Text[i])))
			{
				AfxMessageBox("AcquisitionLineRate请输入整数！");
				return;
			}
		}
		int LineRate = _tstoi(Text);
		MiddleRightLeft.Format("%d", LineRate);
		MiddleRight = MiddleRightLeft + MiddleRightRight;
		Middle = MiddleLeft + MiddleRight;
		s = Left + Middle + Right;

		//////////////////////////////////////////////////////////////////////////////
		///////////////////////////////ExposureTime////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		num = s.Find("ExposureTime");
		num1 = s.Find("\n", num);
		Left = s.Left(num);
		Right = s.Right(int(Len) - (num1 + 1));
		Middle = s.Mid(num, num1 - num + 1);

		for (i = 0; i < Middle.GetLength(); ++i)
		{
			if ('0' <= Middle[i] && '9' >= Middle[i])
				break;
		}
		MiddleLeft = Middle.Left(i);
		///==============================================================================
		MiddleRight = Middle.Right(Middle.GetLength() - i);
		for (i = 0; i < MiddleRight.GetLength(); ++i)
		{
			if (!(('0' <= MiddleRight[i] && '9' >= MiddleRight[i]) || MiddleRight[i] == '.'))
				break;
		}
		MiddleRightLeft = MiddleRight.Left(i);
		MiddleRightRight = MiddleRight.Right(MiddleRight.GetLength() - i);
		//===========================================================================

		ExposureTime.GetWindowTextA(Text);
		for (int i = 0; i < Text.GetLength(); ++i)
		{
			if (!(isdigit(Text[i])))
			{
				AfxMessageBox("ExposureTime请输入整数！");
				return;
			}
		}
		int ExpTime = _tstoi(Text);
		MiddleRightLeft.Format("%d", ExpTime);
		MiddleRight = MiddleRightLeft + MiddleRightRight;
		Middle = MiddleLeft + MiddleRight;
		s = Left + Middle + Right;
		//////////////////////////////////////////////////////////////////////////////
		///////////////////////////////Gain////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		num = s.Find("Gain	");
		num1 = s.Find("\n", num);
		Left = s.Left(num);
		Right = s.Right(int(Len) - (num1 + 1));
		Middle = s.Mid(num, num1 - num + 1);

		for (i = 0; i < Middle.GetLength(); ++i)
		{
			if ('0' <= Middle[i] && '9' >= Middle[i])
				break;
		}
		MiddleLeft = Middle.Left(i);
		///==============================================================================
		MiddleRight = Middle.Right(Middle.GetLength() - i);
		for (i = 0; i < MiddleRight.GetLength(); ++i)
		{
			if (!(('0' <= MiddleRight[i] && '9' >= MiddleRight[i]) || MiddleRight[i] == '.'))
				break;
		}
		MiddleRightLeft = MiddleRight.Left(i);
		MiddleRightRight = MiddleRight.Right(MiddleRight.GetLength() - i);
		//===========================================================================

		Gain.GetWindowTextA(Text);
		for (int i = 0; i < Text.GetLength(); ++i)
		{
			if (!(isdigit(Text[i]) || Text[i] == '.'))
			{
				AfxMessageBox("Gain请输入数字！");
				return;
			}
		}
		float FGain = _tstof(Text);
		MiddleRightLeft.Format("%.3f", FGain);
		MiddleRight = MiddleRightLeft + MiddleRightRight;
		Middle = MiddleLeft + MiddleRight;
		s = Left + Middle + Right;
		//=================================================
		file.SeekToBegin();
		file.Write(s, s.GetLength());
	}
	else
	{
		AfxMessageBox("Error");
		return;
	}
	file.Close();
}


void Dlg_CameraParameter::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CDialogEx::OnOK();
}


void Dlg_CameraParameter::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void Dlg_CameraParameter::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}
