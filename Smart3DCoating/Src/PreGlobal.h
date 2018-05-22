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
#include <ProDtmCsys.h>

//===================================================================================================
// （通用基础函数）

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

// 设置对象的颜色
void SetSurfColor(/*input*/const ProModelitem& item, 
				  /*input*/double dRed, /*input*/double dGreen, /*input*/double dBlue);

// 测量两个几何对象之间的距离
BOOL MeasureDistance(ProGeomitem& item1, ProGeomitem& item2,
					 double& dDistance, Pro3dPnt& pnt1, Pro3dPnt& pnt2);

// 输出特征的xml
void ExportElemTreeXML();

//===================================================================================================
// （数学函数）

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
// （数据获取和过滤函数）

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

// 获取复合曲线的子项
ProError CurveCompGetAction(ProCurve pCurve, ProCurve pComp, int Index, ProBoolean flip, ProError status, ProAppData data);

//===================================================================================================
// （主体功能函数）

// 创建特征
BOOL CreateFeature(/*input*/ProMdl pMdl, /*input*/ProElement elemRoot, /*output*/ProFeature& feat,
				   ProFeatureCreateOptions optCreate = PRO_FEAT_CR_NO_OPTS);

// 创建特征组
int CreateFeatGroup(ProMdl pMdl, vector<int>& arrFeatID, const CString& strGroupName);

// 创建基准平面
int CreateDatumPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selSrf, /*output*/ProModelitem& itemPlane);

// 创建实体化特征
int CreateSolidify(/*input*/ProMdl pMdl, /*input*/ProSelection selQuiltOrPlane, /*input*/int nSide = PRO_SOLIDIFY_SIDE_ONE);

// 创建面
int CreateSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemQlt);

// 通过复制方式创建面
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemSrf);

// 通过复制方式创建面（排除1个内环）
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/ProSelection selInputEdge, 
						/*output*/ProModelitem& itemSrf);

// 通过填充平面方式来创建面
int CreateSurfByFillPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*output*/ProModelitem& itemQlt);

// 通过填充草绘方式创建面
int CreateSurfByFillSketch(/*input*/ProMdl pMdl, /*input*/ProSelection selSketchFeat, /*output*/ProModelitem& itemQlt);

// 通过两条边创建面（边界混合）
int CreateSurfByTwoEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2,
					/*output*/ProModelitem& itemQlt);

// 通过两条曲线创建面（边界混合）
int CreateSurfByTwoCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selCurve1, /*input*/ProSelection selCurve2,
						   /*output*/ProModelitem& itemQlt);

// 通过一组边修剪面
int TrimQuiltByEdgeGroup(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
						   /*input*/vector<ProSelection> arrSelEdges, /*input*/int nTrimDir);

// 通过曲线修剪面
int TrimQuiltByCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
					   /*input*/ProSelection selCurve, /*input*/int nTrimDir);

// 通过曲线修剪面（将曲线延展至与面的外轮廓相交）
BOOL TrimQuiltByExtendCurveToContour(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
									 /*input*/ProSelection selCurve, /*input*/ProSelection selContourCurve, 
									 /*input*/int nTrimDir,
									 ProFeature& feat);

// 通过曲线修剪面（将曲线延展至与面的外轮廓相交）
BOOL TrimQuiltByExtendCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
							/*input*/ProSelection selCurve,  /*input*/ProSelection selContourCurve, 
							/*input*/int nTrimDir,
							ProFeature& feat);

// 通过面组或基准面修剪面
int TrimSurfaceBySurf(/*input*/ProMdl pMdl, /*input*/ProSelection selCutQlt, 
					  /*input*/ProSelection selRefSrf, int nTrimDir);

// 修改裁剪方向
int ReverseTrimDirection(/*input*/ProFeature feat);

// 创建合并特征
int MergeQuilts(/*input*/ProMdl pMdl, /*input*/const vector<ProSelection> &arrSelQuilts, 
			   /*input*/ProSrfMrgType mrgType, ProFeature& feat);

// 修改合并方向
BOOL ChangeMergeDir(ProFeature featMerge, int nIndex);

// 偏移面组
int OffsetQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// 偏移面组（生成侧面）
int OffsetSurfaceWithSide(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset);

// 偏移曲面
int OffsetSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// 通过投影方式创建曲线
int CreateCurveByProject(ProMdl pMdl, ProSelection selQuilt, ProSelection selEdge);

// 将面导出至stl文件
BOOL ExportStl(const vector<ProGeomitem> &arrSurfs, const CString& strStlPath, 
			   double dAngleCntrol, double dChordHeight);

// 移除面组中的面
int RemoveSurfInQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selQltSrf);

// 延展面的一条边
int ExtendQuiltByEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge, /*input*/double dExtendDist);

// 延展面的整个外轮廓
int ExtendQuiltByContour(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/double dExtendDist);

// 延展面组至参考面
int ExtendQuiltToSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/ProSelection selSrf);

// 通过起始边和终止边创建曲线
int CreateFromToCurveByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, 
							/*output*/ProModelitem& itemCurve);

// 通过起始边和终止边创建目的链
int CreateFromToChain(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2);

// 加厚面
int ThickQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/double dThick);

// 创建发布几何
int CreatePublishGeom(ProMdl pMdl, ProSelection selQuilt);

// 创建复制几何
int CreateCopyGeom(ProMdl pMdl, ProSelection selRefMdl, ProSelection selPubFeat, ProModelitem& itemQuilt);

// 通过面面相交创建曲线
int CreateCurveByIntersect(ProMdl pMdl, ProSelection selQuilt1, ProSelection selQuilt2, ProModelitem& itemCurve);

// 通过复制面的外环创建曲线
int CreateCurveByCopyContour(ProMdl pMdl, ProSelection selQuilt, ProModelitem& itemCompCurve);

// 通过延展使两个面相交
BOOL ExtendToInterSect(ProMdl pMdl, ProModelitem& itemQuilt1, ProModelitem& itemQuilt2, int& nFeatID1, int& nFeatID2);

//===================================================================================================
// （辅助函数）

// 获取边所在的环（内环）
BOOL GetContourByEdge(/*input*/ProSelection selEdge, /*output*/vector<ProSelection> &arrSelEdge, /*output*/int& nDir);

// 通过面上某条边的邻面
BOOL GetNeighborSurfByEdge(ProSurface pSurface, ProEdge pPublicEdge,
						   ProSurface &pNeighborSurf);

// 检查两个面组是否相邻
BOOL CheckTwoQuiltNeighbor(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2);

// 检查两个面组是否相交
BOOL CheckTwoQuiltInstersect(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2);

// 判断两条边是否共面
BOOL IsEdgeInSameSurf(/*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, /*output*/ProSelection& selSurf);

// 获取指定面在面组中的连接面
void GetNeighborSurfInQuilt(ProMdl pMdl, ProSurface surf, ProQuilt quilt, vector<int>&arrNeighborID);

// 含单个面的面组转换为曲面
BOOL QuiltToSurf(ProModelitem itemQuilt, ProModelitem& itemSurf);

// 获取曲面上的一个点
void GetPointOnSurface(ProMdl pMdl, ProSurface surf, Pro3dPnt& pntOnSurf);

// 判断点是否在曲面内
BOOL IsPntInsideSurf(ProMdl pMdl, ProSurface surf, Pro3dPnt pntOnSurf);

// 获取模型的最大尺寸（包络的对角线长度）
double GetMaxSize(ProMdl pMdl);

// 获取曲面的默认偏离方向
int GetDefaultOffsetDirection(ProMdl pMdl, ProSurface surf);

// 获取面组的默认偏离方向
int GetDefaultOffsetDirection(ProQuilt quilt);

// 获取面组的面积
double GetQuiltArea(ProQuilt);

#endif
