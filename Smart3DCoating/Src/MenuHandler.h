//===================================================================================================
// Summary:
//		Smart3DCoating�Ĳ˵��Լ�������Ӧ����
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

// ��Ӳ˵���
void AddMenuItem(char *menuName, char *itemName, actionFunc menuitemAction, accessFunc menuitemAccess, 
				 ProFileName msgFile, char *iconName = NULL);

// �ͳ����жԻ���
void DestroyAllDialog();

// ȡ����ǰ����
void CancelOperator();

// ȡ��ѡ��״̬
void CancelSelectDialog();

// ɾ����Ч����ָ��
void DeleteInvalidOperator();

// �ͷŲ�������
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
// ����˵���Ӧ����

// ���ش���������
void OnLoadSolidActFn();

// ��������
void OnCleanActFn();

// ���ٴ���
void OnRapidCreatActFn();

// �ӱ߽紴��
void OnCreateByEdgeActFn();

// �ӱ߽�(��)����
void OnCreateByChainActFn();

// ��ָ�����򴴽�
void OnCreateBySketchActFn();

// ���߲ü�
void OnTrimByEdgeActFn();

// �ݻ�ü�
void OnTrimBySketchActFn();

// �ֶ�ƴ��
void OnMergeManualActFn();

// �Զ�ƴ��
void OnMergeAutoActFn();

// �������
void OnOffsetGeneralActFn();

// �������
void OnOffsetSingleActFn();

// �����г�
void OnBottomCutActFn();

// ��������
void OnBottomExpandActFn();

// ���ɰ�����
void OnCreateBodyActFn();

// ������STL
void OnExportSTLActFn();

// ������STP
void OnExportSTPActFn();

// ��ʾ��ݲ˵�
void ShowShortCutMenu();

// �˳���ݲ˵�
void OnMenuQuit();

// �������ӵ���Ӧ����
void ConServerActFn();
// ����Ͽ�����Ӧ����
void DisConServerActFn();
// ����
void OnHelp();
// ����...
void OnAbout();

//===================================================================================================

extern CDlgSetThick* g_pDlgThick;

#endif