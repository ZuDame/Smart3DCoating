//===================================================================================================
// Summary:
//		Smart3DCoating�ڲ�ʹ�ó����ͽṹ��
// Date:
//		2017-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __ConstantValue_h__
#define __ConstantValue_h__

//===================================================================================================
// ����Ϊ��������
//===================================================================================================





//===================================================================================================
// ����Ϊö��ֵ����
//===================================================================================================

// ��������
typedef enum __tagErrorResult
{
	ER_CREATE_FAIL	=		-1,		// 
	ER_BAD_INPUT	=		-2,		// 
	ER_INVALID_UDF		=	-3,		// 
}ErrorResult;


//// �Զ�����ѡ��
//typedef struct __tagAutoSaveOption
//{
//	BOOL bAutoSaveMdl;		// �Զ�����ģ��
//	double dTimeStep;		// ʱ����
//	int nProcAfterSave;		// �������
//	int nKeepFileNum;		// �����ļ�����
//
//	__tagAutoSaveOption()
//	{
//		bAutoSaveMdl = FALSE;
//		dTimeStep = -1.;
//		nProcAfterSave = PAS_KEEPALLOLDFILE;
//		nKeepFileNum = -1;
//	}
//	__tagAutoSaveOption& operator=(const __tagAutoSaveOption &asp)
//	{
//		if (&asp == this)
//			return *this;
//		bAutoSaveMdl = asp.bAutoSaveMdl;
//		dTimeStep = asp.dTimeStep;
//		nProcAfterSave = asp.nProcAfterSave;
//		nKeepFileNum = asp.nKeepFileNum;
//		return *this;
//	}
//} AutoSaveOption;

//===================================================================================================
// ����Ϊ�ṹ�嶨��

typedef CArray<int> IntArray;
typedef CArray<double> DoubleArray;

// ������Ϣ
typedef struct __tagParaInfo
{
	CString strParaName;			// ������
	CString strParaType;			// ��������
	union {
		BOOL b;
		int i;
		double d;
		wchar_t s[256];
	}paraValue;
	CString strDescription;			// ����˵��

	__tagParaInfo()
	{
	}
	__tagParaInfo& operator=(const __tagParaInfo &pi)
	{
		if (&pi == this)
			return *this;
		strParaName = pi.strParaName;
		strParaType = pi.strParaType;
		if (strParaType.CompareNoCase(L"����") == 0)
			paraValue.i = pi.paraValue.i;
		else if (strParaType.CompareNoCase(L"ʵ��") == 0)
			paraValue.d = pi.paraValue.d;
		else if (strParaType.CompareNoCase(L"�ַ���") == 0)
			wcsncpy_s(paraValue.s, 256, pi.paraValue.s, _TRUNCATE);
		else if (strParaType.CompareNoCase(L"��/��") == 0)
			paraValue.b = pi.paraValue.b;
		strDescription = pi.strDescription;
		return *this;
	}
} ParaInfo;

typedef CArray<ParaInfo> ParaInfoArray;

//===================================================================================================


#endif

