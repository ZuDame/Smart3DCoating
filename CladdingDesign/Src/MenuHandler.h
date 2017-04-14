//===================================================================================================
// Summary:
//		CladdingDesign的菜单以及命令响应函数
// Date:
//		2016-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __MenuHandler_h__
#define __MenuHandler_h__

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

// 表面清理
void OnCleanActFn();

// 快速创建
void OnRapidCreatActFn();

// 通过边界创建
void OnCreateByEdgeActFn();

// 裁剪
void OnTrimActFn();

// 拼接
void OnMergeActFn();

// 整体放量
void OnOffsetGeneralActFn();

// 单侧放量
void OnOffsetSingleActFn();

// 底面切除
void OnBottomCutActFn();

// 底面延伸
void OnBottomExpandActFn();

// 导出
void OnExportActFn();

// 显示快捷菜单
void ShowShortCutMenu();

// 退出快捷菜单
void OnMenuQuit();

//===================================================================================================

#endif