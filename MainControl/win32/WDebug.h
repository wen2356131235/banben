// WDebug.h: WDebug DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CWDebugApp
// 有关此类实现的信息，请参阅 WDebug.cpp
//

class CWDebugApp : public CWinApp
{
public:
	CWDebugApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
void __declspec(dllexport)DebugPrintMsgBox(const char* strOutputString, ...);
void __declspec(dllexport)DebugPrintf(const char* strOutputString, ...);
class _declspec(dllexport) WDebug
{
public:
	WDebug();
	WDebug(CView* pV);
	~WDebug();

	CView* pView;
	CClientDC* pDC;
	CString OutputPos;
	void setOutputPos(CString outputpos);
	void DebugPrintf(const char* strOutputString, ...);
	void fDebugPrintf(const char* strOutputString, ...);
	void fDebugPrintf(CString data);
	void fDebugClear();

	void DebugPrintMsgBox(const char* strOutputString, ...);
	//====================
	void ReadCommaSeparatedFiles_CString(CString inputpos, CArray<CArray<CString>*>* pret);
	void ReadCommaSeparatedFiles_CString_release(CArray<CArray<CString>*>* pret);
	void ReadCommaSeparatedFiles_double(CString inputpos, CArray<CArray<double>*>* pret);
	void ReadCommaSeparatedFiles_double_release(CArray<CArray<double>*>* pret);

};
