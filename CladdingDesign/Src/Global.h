//===================================================================================================
// Summary:
//		�����ڲ�ʹ�õĹ�������
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

// ��ȡ�ļ������ͣ�����ֵ�ο�READERFILETYPEö��ֵ
int GetFileOpenType(const CString& strFileName);
// ��ȡ�ļ���չ��
BOOL GetFileExtName(const CString& strFilePathName, CString& strRetExt);
// ����Ƿ����ָ����Ŀ¼
BOOL IsFolderExist(LPCTSTR pszFolderName);
// �ж��ļ��Ƿ���ڡ�
BOOL IsFileExist(LPCTSTR pszFileName);
// �ж��ļ��Ƿ񱻴򿪡�
BOOL IsFileOpened(LPCTSTR pszFileName);
// ��ȫɾ���ļ���
void SafeDeleteFile(LPCTSTR pszFileName);
// ɾ��ָ��Ŀ¼�µ��ļ����ļ���
BOOL DeleteDirectoryFiles(LPCTSTR pszFolderName);
// ��ȡһ����ʱ�ļ�����
BOOL GetTempFile(CString& strTempFileName, const CString &strExt = _T("tmp"));
// �õ��ļ��Ķ���������
BOOL GetFileData(const CString &strFilePath, BYTE *&pFileData, DWORD &nLength);
// �������Ƕ�׵�Ŀ¼��
BOOL RecursiveCreateDirectory(LPCTSTR szPath);
// ��ȡ����������·����
void GetMainModulePath(HMODULE hModule, CString& strPath);
// ��ȡ������İ汾��Ϣ��
BOOL GetApabiVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// �ж��ַ��Ƿ���ĸ
inline BOOL IsLetter(TCHAR ch) { return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z'); }
// �ж��ַ��Ƿ�����
inline BOOL IsNumber(TCHAR ch) { return (ch >= L'0' && ch <= L'9'); }

void CStringToConvertByte(const CString &strSrcText, BYTE *pDestText, int &nDestLen);
void ConvertByteToCString(const BYTE *pSrcText, CString &strDesText);
void BSTRToCString(BSTR pBFName, wchar_t *szNodeName, int &nLen);

BOOL GetSdVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
// ��Logͼ����ʾ�ڲ��汾��
void DisplayBuildVersionText(CDC* pDC, CPoint ptPos);
// ��ȡ��ĻDPI
double GetScreenDPI();
// ��ȡ��ģ̬�Ի�������Ͻ�λ��
CRect GetSceenCenterRect(int nOffsetX, int nOffsetY, const CRect &rcRect);
// �����ַ��������������ַ�����λ��
int FindInCStringArray(const CStringArray &arrString, const CString &strIn);
// ���ո���ַ������
BOOL CStringToCStringArray(const CString &str, CStringArray &arr, const CString &strKeyword = L" ");
// ���ո���ַ������
BOOL CStringArrayToCString(const CStringArray &arr, CString &str, const CString &strKeyword = L" ");
// �ж��ַ����Ƿ�Ϊ����
BOOL IsNumber(const CString &str);
// �ж��ַ����Ƿ�Ϊ����
BOOL IsPoistive(const CString &str);
// ��ȡ�ļ���,��������չ��
CString GetFileNameNoExtByPath(const CString& strFullPath);
// ��ȡת������ʱ�ļ���
CString GetFileTempPath();
// �õ���ֵ���ַ���
void GetValueString(double dValue, CString &strValue, BOOL bWithFlag = FALSE);
// ɾ���ַ���ǰ�����Ŀո�
void DelSpace(CString &strText);
// �жϲ������Ƿ����
BOOL IsParameterNameValid(const CString &strParaName);
// ��ȡ�ҵ��ĵ��ļ���
CString GetMyDocuments();

//===================================================================================================

// �û���
extern CString g_strUserName;
// ��Ȩ��Ϣ
extern CString g_strCopyRight;

//===================================================================================================

#endif