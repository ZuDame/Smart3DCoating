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

#include <ProIntfimport.h >

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
#include <ProDtmCrv.h>
#include <ProThicken.h>
#include <pd_prototype.h>
#include <ProDataShareFeat.h>

//===================================================================================================

// 激活当前窗口
ProError ActivateCurrentPreWindow();

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

// 加载STEP模型
BOOL LoadStep(const CString& strFilePath, ProMdl& pMdl);

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

// 获取特征中的所有尺寸
ProError FeatureDimensionGetAction(ProDimension* dimension, ProError status, ProAppData pData);

//===================================================================================================

// 创建特征
BOOL CreateFeature(/*input*/ProMdl pMdl, /*input*/ProElement elemRoot, /*output*/ProFeature& feat);

// 创建基准平面
int CreateDatumPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selSrf, /*output*/ProModelitem& itemPlane);

// 创建实体化特征
int CreateSolidify(/*input*/ProMdl pMdl, /*input*/ProSelection selQuiltOrPlane, /*input*/int nSide = PRO_SOLIDIFY_SIDE_ONE);

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

// 通过一条封闭曲线修剪面
int TrimSurfaceByCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
				/*input*/ProSelection selCurve, /*input*/int nTrimDir);

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

// 通过草绘创建填充面
int CreateFillSurfBySketch(/*input*/ProMdl pMdl, /*input*/ProSelection selSketchFeat, /*output*/ProModelitem& itemQlt);

// 创建边界混合特征
int CreateBlendSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2,
					/*output*/ProModelitem& itemQlt);

int CreateBlendSurfByCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selCurve1, /*input*/ProSelection selCurve2,
						   /*output*/ProModelitem& itemQlt);

// 创建合并特征
int MergeSurfs(/*input*/ProMdl pMdl, /*input*/const vector<ProSelection> &arrSelQuilts, 
			   /*input*/ProSrfMrgType mrgType, ProFeature& feat);

// 修改合并方向
BOOL ChangeMergeDir(ProFeature featMerge, int nIndex);

// 偏移面组
int OffsetQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// 偏移曲面
int OffsetSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/double dOffset,
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

// 延展面
int ExtendSrfByContour(/*input*/ProMdl pMdl, /*input*/ProSelection selSurf, /*input*/double dExtendDist);

// 输出特征的xml
void ExportElemTreeXML();

// 通过起始边和终止边创建曲线
int CreateFromToCopyCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, /*output*/ProModelitem& itemCurve);

// 修改复制曲线的方向
int ModifyCopeCurveDirection(/*input*/ProGroup udf, /*output*/ProModelitem& itemCurve);

// 通过起始边和终止边创建目的链
int CreateFromToChain(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2);

// 判断两条边是否共面
BOOL IsEdgeInSameSurf(/*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, /*output*/ProSelection& selSurf);

// 创建特征组
int CreateFeatGroup(ProMdl pMdl, vector<int>& arrFeatID, const CString& strGroupName);

// 创建加厚特征
int ThickQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/double dThick);

// 创建发布几何
int CreatePublishGeom(ProMdl pMdl, ProSelection selQuilt);

// 创建复制几何
int CreateCopyGeom(ProMdl pMdl, ProSelection selRefMdl, ProSelection selPubFeat, ProModelitem& itemQuilt);

// 创建相交特征
int CreateIntersect(ProMdl pMdl, ProSelection selQuilt1, ProSelection selQuilt2, ProModelitem& itemCurve);

// 创建复制曲线（面的外环）
int CreateContourCurve(ProMdl pMdl, ProSelection selQuilt, ProModelitem& itemCompCurve);

// 裁剪面 by 曲线（UDF方式）
int TrimSurfWithCurveByUDF(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt,
					   /*input*/ProSelection selTrimCurve, /*input*/ProSelection selContourCurve, int nMode);

//===================================================================================================

BOOL GetNeighborSurfByEdge(ProSurface pSurface, ProEdge pPublicEdge,
						   ProSurface &pNeighborSurf);											// 根据公共边获取邻面

// 检查两个Quilt是否相邻
BOOL CheckTwoQuiltNeighbor(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2, ProEdge& edge1, ProEdge& edge2);

// 检查两个Quilt是否相交
BOOL CheckTwoQuiltInstersect(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2);

// 获取指定面在面组中的连接面
void GetNeighborSurfInQuilt(ProMdl pMdl, ProSurface surf, ProQuilt quilt, vector<int>&arrNeighborID);

// 测量两个几何对象之间的距离
BOOL MeasureDistance(ProGeomitem& item1, ProGeomitem& item2,
					 double& dDistance, Pro3dPnt& pnt1, Pro3dPnt& pnt2);

// 含单个面的面组转换为面
BOOL QuiltToSurf(ProModelitem itemQuilt, ProModelitem& itemSurf);

// 通过延展使两个面相交
BOOL ExtendToInterSect(ProMdl pMdl, ProModelitem& itemQuilt1, ProModelitem& itemQuilt2, int& nFeatID1, int& nFeatID2);

// 获取面上的一个点
void GetPointOnSurface(ProMdl pMdl, ProSurface surf, Pro3dPnt& pntOnSurf);

// 判断点是否在面内
BOOL IsPntInsideSurf(ProMdl pMdl, ProSurface surf, Pro3dPnt pntOnSurf);

#endif
