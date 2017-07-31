//===================================================================================================
// Summary:
//		Smart3DCoating内部使用常量和结构体
// Date:
//		2017-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __ConstantValue_h__
#define __ConstantValue_h__

//===================================================================================================
// 以下为常量定义
//===================================================================================================





//===================================================================================================
// 以下为枚举值定义
//===================================================================================================

// 参数类型
typedef enum __tagErrorResult
{
	ER_CREATE_FAIL	=		-1,		// 
	ER_BAD_INPUT	=		-2,		// 
	ER_INVALID_UDF		=	-3,		// 
}ErrorResult;


//// 自动保存选项
//typedef struct __tagAutoSaveOption
//{
//	BOOL bAutoSaveMdl;		// 自动保存模型
//	double dTimeStep;		// 时间间隔
//	int nProcAfterSave;		// 保存后处理
//	int nKeepFileNum;		// 保留文件个数
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
// 以下为结构体定义

typedef CArray<int> IntArray;
typedef CArray<double> DoubleArray;

// 参数信息
typedef struct __tagParaInfo
{
	CString strParaName;			// 参数名
	CString strParaType;			// 参数类型
	union {
		BOOL b;
		int i;
		double d;
		wchar_t s[256];
	}paraValue;
	CString strDescription;			// 参数说明

	__tagParaInfo()
	{
	}
	__tagParaInfo& operator=(const __tagParaInfo &pi)
	{
		if (&pi == this)
			return *this;
		strParaName = pi.strParaName;
		strParaType = pi.strParaType;
		if (strParaType.CompareNoCase(L"整数") == 0)
			paraValue.i = pi.paraValue.i;
		else if (strParaType.CompareNoCase(L"实数") == 0)
			paraValue.d = pi.paraValue.d;
		else if (strParaType.CompareNoCase(L"字符串") == 0)
			wcsncpy_s(paraValue.s, 256, pi.paraValue.s, _TRUNCATE);
		else if (strParaType.CompareNoCase(L"是/否") == 0)
			paraValue.b = pi.paraValue.b;
		strDescription = pi.strDescription;
		return *this;
	}
} ParaInfo;

typedef CArray<ParaInfo> ParaInfoArray;

//===================================================================================================


#endif

