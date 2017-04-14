//===================================================================================================
// Summary:
//		CladdingDesign�Ĳ˵��Լ�������Ӧ����
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

// ��������
void OnCleanActFn();

// ���ٴ���
void OnRapidCreatActFn();

// ͨ���߽紴��
void OnCreateByEdgeActFn();

// �ü�
void OnTrimActFn();

// ƴ��
void OnMergeActFn();

// �������
void OnOffsetGeneralActFn();

// �������
void OnOffsetSingleActFn();

// �����г�
void OnBottomCutActFn();

// ��������
void OnBottomExpandActFn();

// ����
void OnExportActFn();

// ��ʾ��ݲ˵�
void ShowShortCutMenu();

// �˳���ݲ˵�
void OnMenuQuit();

//===================================================================================================

#endif