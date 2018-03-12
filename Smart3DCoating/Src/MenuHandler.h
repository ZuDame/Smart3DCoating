//===================================================================================================
// Summary:
//		Smart3DCoating的菜单以及命令响应函数
// Date:
//		2016-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __MenuHandler_h__
#define __MenuHandler_h__

#include "DlgSetThick.h"

//===================================================================================================

typedef void (*actionFunc)();
typedef uiCmdAccessState (*accessFunc)(uiCmdAccessMode mode);

// 添加菜单项
void AddMenuItem(char *menuName, char *itemName, actionFunc menuitemAction, accessFunc menuitemAccess, 
				 ProFileName msgFile, char *iconName = NULL);

// 释出所有对话框
void DestroyAllDialog();

// 取消当前操作
void CancelOperator();

// 取消选择状态
void CancelSelectDialog();

// 删除无效操作指针
void DeleteInvalidOperator();

// 释放操作对象
void DeleteOperator();

//===================================================================================================

class CMenuHandler
{
public:
	CMenuHandler() {}
	~CMenuHandler() { }

public:
	BOOL Init();
	void UnInit();
};

//===================================================================================================
// 定义菜单响应函数

// 加载待包覆对象
void OnLoadSolidActFn();

// 表面清理
void OnCleanActFn();

// 快速创建
void OnRapidCreatActFn();

// 从边界创建
void OnCreateByEdgeActFn();

// 从边界(链)创建
void OnCreateByChainActFn();

// 从指定区域创建
void OnCreateBySketchActFn();

// 环边裁剪
void OnTrimByEdgeActFn();

// 草绘裁剪
void OnTrimBySketchActFn();

// 手动拼接
void OnMergeManualActFn();

// 自动拼接
void OnMergeAutoActFn();

// 整体放量
void OnOffsetGeneralActFn();

// 单侧放量
void OnOffsetSingleActFn();

// 底面切除
void OnBottomCutActFn();

// 底面延伸
void OnBottomExpandActFn();

// 生成包覆体
void OnCreateBodyActFn();

// 导出至STL
void OnExportSTLActFn();

// 导出至STP
void OnExportSTPActFn();

// 显示快捷菜单
void ShowShortCutMenu();

// 退出快捷菜单
void OnMenuQuit();

// 网络连接的响应函数
void ConServerActFn();
// 网络断开的响应函数
void DisConServerActFn();
// 帮助
void OnHelp();
// 关于...
void OnAbout();

//===================================================================================================

extern CDlgSetThick* g_pDlgThick;

#endif