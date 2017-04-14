//===================================================================================================
// Summary:
//		包覆功能实现函数
// Date:
//		2017-04-09
// Author:
//		
//===================================================================================================

#ifndef __PreGlobal_h__
#define __PreGlobal_h__

//===================================================================================================

// 基础
#include <ProToolkit.h>
#include <ProUtil.h>
#include <ProMessage.h>
#include <ProArray.h>
#include <ProMdl.h>
#include <ProModelitem.h>
#include <ProWindows.h>
#include <ProSolid.h>
#include <prodev_light.h>
#include <ProDisplist.h>

// 菜单
#include <ProMenu.h>
#include <ProMenuBar.h>
#include <ProPopupmenu.h>

// 选择
#include <ProSrfcollection.h>
#include <ProCrvcollection.h>
#include <ProSelection.h>
#include <ProSelbuffer.h>

// 几何、基准
#include <ProPoint.h>
#include <ProCurve.h>
#include <ProEdge.h>
#include <ProSurface.h>
#include <ProQuilt.h>
#include <ProAxis.h>
#include <ProCsys.h>

// 外观
#include <ProColor.h>
#include <prodev_dgm.h>

// 特征
#include <ProFeature.h>
#include <ProFeatType.h>
#include <ProSolidify.h>
#include <proUdf.h>
#include <ProMerge.h>
#include <ProTrim.h>
#include <ProRemoveSurf.h>
#include <ProStdSection.h>
#include <ProDtmPln.h>
#include <ProLayer.h>
#include <ProAsmcomp.h>

//===================================================================================================

// 获得当前模型  
ProMdl GetCurrentMdl();

// 刷新当前窗口
void InvalidateDrawing();

// 执行映射键
void ExecuteMapKey(const CString &strMacro, ProModelitem *pMdlItem = NULL);

// 执行映射键
void ExecuteMapKey(const CString &strMacro, ProSelection pSel);

// 加载映射键（在函数结束后执行）
void LoadMapKey(const CString &strMacro, ProModelitem *pMdlItem = NULL);

// 加载映射键（在函数结束后执行）
void LoadMapKey(const CString &strMacro, ProSelection pSel);

// 显示提示信息
void ShowMessageTip(const CString& strPrompt);

// 获取选择对象的类型
ProType GetSelectionType(ProSelection pSel);

// 选择对象
BOOL SelectObject(/*output*/vector<ProSelection> &arrSelObj, 
				  /*input*/char *pSelOpt, /*input*/int nCount = 1, /*input*/ProSelection* pArrSelInit = NULL);

//===================================================================================================

// 判断向量平行
BOOL CheckVectorParallel(const ProVector pc1, const ProVector pv2);

// 获取向量夹角的余弦值
double GetVectorCosAngle(const ProVector v1, const ProVector v2);

// 复制向量
void VectorCopy(const ProPoint3d from, ProPoint3d to);

// 获取误差修正后的Sin或Cos数值
double GetCorrectSinCosValue(double dCosValue);

// 计算反余弦值
double CalArcCos(double dValue);

// 计算反正弦值
double CalArcSin(double dValue);

//===================================================================================================

// 获取特征中的所有几何项
ProError FeatureGeomsGetAction(ProGeomitem* pDatum, ProError status, ProAppData pData);

// 获取面组中的所有面
ProError QuiltSurfacesGetAction(ProSurface surface, ProError status, ProAppData pData);

// 获取面的内环
ProError SurfaceInnerContoursGetAction(ProContour contour, ProError status, ProAppData pData);

// 获取面的外环
ProError SurfaceExtContoursGetAction(ProContour contour, ProError status, ProAppData pData);

// 获取环的所有边
ProError ContourEdgesGetAction(ProEdge Edge, ProError status, ProAppData data);

// 获取模型的一个面
ProError OneSurfaceGetAction(ProSurface surface, ProError status, ProAppData pData);

// 获取模型的所有面组
ProError SolidQuiltsGetAction(ProQuilt quilt, ProError status, ProAppData pData);

//===================================================================================================

// 创建特征
BOOL CreateFeature(/*input*/ProMdl pMdl, /*input*/ProElement elemRoot, /*output*/ProFeature& feat);

// 创建基准平面
int CreateDatumPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selSrf, /*output*/ProModelitem& itemPlane);

// 创建实体化特征
int CreateSolidify(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*input*/int nSide = PRO_SOLIDIFY_SIDE_ONE);

// 创建包覆面
int CreateSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemQlt);

// 创建复制特征
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemSrf);

// 创建复制特征（排除孔）
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/ProSelection selInputEdge, 
						/*output*/ProModelitem& itemSrf);

// 获取边所在的环（内环）
BOOL GetContourByEdge(/*input*/ProSelection selEdge, /*output*/vector<ProSelection> &arrSelEdge, /*output*/int& nDir);

// 通过边或曲线修剪面
int TrimSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
				/*input*/vector<ProSelection> arrSelEdges, /*input*/int nTrimDir);

// 通过面组或基准面修剪面
int TrimSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selCutQlt, 
				/*input*/ProSelection selRefSrf, int nTrimDir);

// 修改裁剪方向
int ReverseTrimDirection(/*input*/ProFeature feat);

// 设置对象的颜色
void SetSurfColor(/*input*/const ProModelitem& item, 
				  /*input*/double dRed, /*input*/double dGreen, /*input*/double dBlue);

// 创建填充面
int CreateFillSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*output*/ProModelitem& itemQlt);

// 创建边界混合特征
int CreateBlendSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2,
					/*output*/ProModelitem& itemQlt);

// 创建合并特征
int MergeSurfs(/*input*/ProMdl pMdl, /*input*/const vector<ProSelection> &arrSelQuilts, /*input*/BOOL bCrossed);

// 偏移面
int OffsetSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// 创建投影特征
int CreateProjectCurve(ProMdl pMdl, ProSelection selQuilt, ProSelection selEdge);

// 将一组曲面输出到stl文件中
BOOL ExportStl(const vector<ProGeomitem> &arrSurfs, const CString& strStlPath, 
			   double dAngleCntrol, double dChordHeight);

// 单面偏移
int OffsetSingleSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset);

// 移除面
int RemoveQuiltSrf(/*input*/ProMdl pMdl, /*input*/ProSelection selQltSrf);

// 延展面
int ExtendSrfByEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge, /*input*/double dExtendDist);

// 延展到面
int ExtendQuiltToSrf(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/ProSelection selSrf);

// 输出特征的xml
void ExportElemTreeXML();



//===================================================================================================

#endif
