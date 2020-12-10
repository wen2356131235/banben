// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H


extern CPtrArray* pGlobalResourceArray;

class ImageDataSheet
{
public:
	SYSTEMTIME time;
	float SpindlePosition;//主轴位置
	float SlidePosition;//滑台位置
};



#include "WDebug.h"
#include "myMath.h"
#include"WLightCtrl.h"

#ifdef _M_X64
#pragma comment(lib,"WDebug.lib")
#pragma comment(lib,"WMath.lib")
#pragma comment(lib,"LightCtrl.lib")
#else
#pragma comment(lib,".//win32//WMath.lib")
#pragma comment(lib,".//win32//WDebug.lib")
#pragma comment(lib,".//win32//LightCtrl.lib")
#include "..\\..\\PLCControl\\PLCControl\\PlcCtrl.h"//64位注释掉
#pragma comment(lib,"..\\..\\PLCControl\\Release\\PLCControl.lib")
#endif // 

#include<atlimage.h>
#define Const_PictureArraySize		60
#define Const_linesPerPicture		100
#define Const_LineSliceArraySize	20000//200,000占用内存1448.1M
#define AnalysisArea				2048

#define ResourceArraySize		7

#define Pos_MainView						0
#define Pos_DataPacket						1
#define Pos_MainCtrlWorker					2
#define Pos_LineCameraWorker				3
#define Pos_DataAnalysisWorker				4
#define Pos_DefectManagementWorker			5
#define Pos_PLC								6

#define	  MSG_drawScreen							WM_USER+0
#define   MSG_SetMainCtrlWorker						WM_USER+1
#define   MSG_SetLineCameraWorker					WM_USER+2
#define   MSG_AskImage								WM_USER+4
#define   MSG_CarryingImage							WM_USER+5
#define   MSG_LineCameraSwitch						WM_USER+6
#define   MSG_SetDataAnalysisWorker					WM_USER+7
#define   MSG_ComputingService_LinesSlice			WM_USER+8
#define   MSG_ComputingService_complete 			WM_USER+9
#define	  MSG_SavePictureData						WM_USER+10
#define	  MSG_AlarmingData							WM_USER+11
#define   MSG_LightAdjust							WM_USER+12
#define   MSG_SetInitialLight						WM_USER+13
#define   MSG_SetDefectManagementWorker				WM_USER+14

//缺陷类型定义
#define defectType_BrokenLongLine				0