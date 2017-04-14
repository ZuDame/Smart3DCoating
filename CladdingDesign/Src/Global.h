//===================================================================================================
// Summary:
//		程序内部使用的公共函数
// Date:
//		2010-12-08
// Author:
//		YangXuefeng(yxf4640084@sina.com.cn)
//===================================================================================================

#ifndef __Global_h__
#define __Global_h__

//===================================================================================================

#include "BaseMacro.h"

//===================================================================================================

// 获取文件打开类型，返回值参考READERFILETYPE枚举值
int GetFileOpenType(const CString& strFileName);
// 获取文件扩展名
BOOL GetFileExtName(const CString& strFilePathName, CString& strRetExt);
// 检查是否存在指定的目录
BOOL IsFolderExist(LPCTSTR pszFolderName);
// 判断文件是否存在。
BOOL IsFileExist(LPCTSTR pszFileName);
// 判断文件是否被打开。
BOOL IsFileOpened(LPCTSTR pszFileName);
// 安全删除文件。
void SafeDeleteFile(LPCTSTR pszFileName);
// 删除指定目录下的文件和文件夹
BOOL DeleteDirectoryFiles(LPCTSTR pszFolderName);
// 获取一个临时文件名。
BOOL GetTempFile(CString& strTempFileName, const CString &strExt = _T("tmp"));
// 得到文件的二进制数据
BOOL GetFileData(const CString &strFilePath, BYTE *&pFileData, DWORD &nLength);
// 创建多层嵌套的目录。
BOOL RecursiveCreateDirectory(LPCTSTR szPath);
// 获取主程序所在路径。
void GetMainModulePath(HMODULE hModule, CString& strPath);
// 获取主程序的版本信息。
BOOL GetApabiVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// 判断字符是否字母
inline BOOL IsLetter(TCHAR ch) { return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z'); }
// 判断字符是否数字
inline BOOL IsNumber(TCHAR ch) { return (ch >= L'0' && ch <= L'9'); }

void CStringToConvertByte(const CString &strSrcText, BYTE *pDestText, int &nDestLen);
void ConvertByteToCString(const BYTE *pSrcText, CString &strDesText);
void BSTRToCString(BSTR pBFName, wchar_t *szNodeName, int &nLen);

BOOL GetSdVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// 在Log图上显示内部版本号
void DisplayBuildVersionText(CDC* pDC, CPoint ptPos);
// 获取屏幕DPI
double GetScreenDPI();
// 获取非模态对话框的左上角位置
CRect GetSceenCenterRect(int nOffsetX, int nOffsetY, const CRect &rcRect);
// 查找字符串数组中输入字符串的位置
int FindInCStringArray(const CStringArray &arrString, const CString &strIn);
// 按空格把字符串拆分
BOOL CStringToCStringArray(const CString &str, CStringArray &arr, const CString &strKeyword = L" ");
// 按空格把字符串组合
BOOL CStringArrayToCString(const CStringArray &arr, CString &str, const CString &strKeyword = L" ");
// 判断字符串是否为数字
BOOL IsNumber(const CString &str);
// 判断字符串是否为正数
BOOL IsPoistive(const CString &str);
// 获取文件名,不包含扩展名
CString GetFileNameNoExtByPath(const CString& strFullPath);
// 获取转换的临时文件夹
CString GetFileTempPath();
// 得到数值的字符串
void GetValueString(double dValue, CString &strValue, BOOL bWithFlag = FALSE);
// 删除字符串前后多余的空格
void DelSpace(CString &strText);
// 判断参数名是否合理
BOOL IsParameterNameValid(const CString &strParaName);
// 获取我的文档文件夹
CString GetMyDocuments();

//===================================================================================================

// 用户名
extern CString g_strUserName;
// 版权信息
extern CString g_strCopyRight;

//===================================================================================================

#endif