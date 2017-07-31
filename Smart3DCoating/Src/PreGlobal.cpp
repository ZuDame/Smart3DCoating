//===================================================================================================
// Summary:
//		包覆功能实现函数
// Date:
//		2017-04-09
// Author:
//		
//===================================================================================================

#include "stdafx.h"
#include "Smart3DCoating.h"
#include "PreGlobal.h"
#include "Math.h"

//===================================================================================================

#ifdef _DEBUGw
#define new DEBUG_NEW
#endif

//===================================================================================================

// 刷新当前窗口
void InvalidateDrawing()
{
	ProDisplistInvalidate(GetCurrentMdl());
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

// 获得当前模型
ProMdl GetCurrentMdl()
{
	ProMdl mdl = NULL;
	if (ProMdlCurrentGet(&mdl) == PRO_TK_NO_ERROR)
		return mdl;
	return NULL;
}

// 执行映射键
void ExecuteMapKey(const CString &strMacro, ProModelitem *pMdlItem/* = NULL*/)
{
	if (strMacro.IsEmpty())
		return;
	if (NULL == pMdlItem)
	{
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
		ProMacroExecute();
	}
	else
	{
		ProSelection pSel = NULL;
		ProError status = ProSelectionAlloc(NULL, pMdlItem, &pSel);
		if (status == PRO_TK_NO_ERROR)
		{
			ExecuteMapKey(strMacro, pSel);
			SAFE_DELETE_SELECTION(pSel);
		}
	}
}

// 执行映射键
void ExecuteMapKey(const CString &strMacro, ProSelection pSel)
{
	if (strMacro.IsEmpty())
		return;
	if (NULL == pSel)
	{
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
		ProMacroExecute();
	}
	else
	{
		ProSelbufferClear();
		ProSelbufferSelectionAdd(pSel);
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
		ProMacroExecute();
		ProSelbufferClear();
	}
}

// 加载映射键（在函数结束后执行）
void LoadMapKey(const CString &strMacro, ProModelitem *pMdlItem/* = NULL*/)
{
	if (strMacro.IsEmpty())
		return;
	if (NULL == pMdlItem)
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
	else
	{
		ProSelection pSel = NULL;
		ProError status = ProSelectionAlloc(NULL, pMdlItem, &pSel);
		if (status == PRO_TK_NO_ERROR)
		{
			LoadMapKey(strMacro, pSel);
			SAFE_DELETE_SELECTION(pSel);
		}
	}
}

// 加载映射键（在函数结束后执行）
void LoadMapKey(const CString &strMacro, ProSelection pSel)
{
	if (strMacro.IsEmpty())
		return;
	if (NULL == pSel)
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
	else
	{
		ProSelbufferClear();
		ProSelbufferSelectionAdd(pSel);
		ProMacroLoad((wchar_t*)(LPCTSTR)strMacro);
	}
}

// 显示提示信息
void ShowMessageTip(const CString& strPrompt)
{
	ProMessageDisplay(L"Msg.txt","Prompt %0w", strPrompt);
}

// 获取选择对象的类型
ProType GetSelectionType(ProSelection pSel)
{
	if (NULL == pSel)
		return PRO_TYPE_INVALID;

	ProModelitem mdlItem;
	ProError status = ProSelectionModelitemGet(pSel, &mdlItem);
	ProType type = PRO_TYPE_INVALID;
	if (status == PRO_TK_NO_ERROR)
		type = mdlItem.type;
	else
	{
		ProReference pRef = NULL;
		status = ProSelectionToReference(pSel, &pRef);
		if (status == PRO_TK_NO_ERROR && NULL != pRef)
		{
			status = ProReferenceTypeGet(pRef, &type);
			ProReferenceFree(pRef);
		}
	}

	return type;
}

// 选择对象
BOOL SelectObject(/*output*/vector<ProSelection> &arrSelObj, 
				  /*input*/char *pSelOpt, /*input*/int nCount /*= 1*/, /*input*/ProSelection* pArrSelInit /*= NULL*/)
{
	arrSelObj.clear();
	BOOL bRet = FALSE;
	ProError status = PRO_TK_GENERAL_ERROR;

	ProSelection *pTempSelEntity = NULL;
	int nSelCount = 0;
	status = ProSelect(pSelOpt, nCount, pArrSelInit, NULL, NULL, NULL, &pTempSelEntity, &nSelCount);
	if (status == PRO_TK_NO_ERROR && nSelCount > 0)
	{
		for (int i=0; i<nSelCount; i++)
		{
			arrSelObj.push_back(pTempSelEntity[i]);
			ProSelectionHighlight(pTempSelEntity[i], PRO_COLOR_HIGHLITE);
		}
		bRet = TRUE;
	}
	return bRet;
}

//===================================================================================================

double VectorDot(const ProVector pV1, const ProVector pV2)
{
	return pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2];
}

double VectorLength( const ProPoint3d pV )
{
	return sqrt(pV[0]*pV[0] + pV[1]*pV[1] + pV[2]*pV[2]);
}

void VectorCopy(const ProPoint3d from, ProPoint3d to)
{
	to[0] = from[0];to[1] = from[1];to[2] = from[2];
}

void VectorNormalize(ProPoint3d pOut, const ProPoint3d pV)
{
	double dLength = VectorLength(pV);
	VectorCopy(pV, pOut);
	pOut[0] /= dLength;
	pOut[1] /= dLength;
	pOut[2] /= dLength;

	return;
}

double DisPntPnt(const ProPoint3d p1, const ProPoint3d p2)
{
	return sqrt((p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2]));
}

// 将一个点投影到一条直线上
void ProjectPtToLine(const ProPoint3d vLinePt, const ProVector vLineDir, const ProPoint3d vSrcPt, ProPoint3d vPrjPt)
{
	ProVector vLineDirN;
	VectorNormalize(vLineDirN, vLineDir);
	ProPoint3d vPrjPosVec;
	for (int i=0; i<3; i++)
		vPrjPosVec[i] = vSrcPt[i] - vLinePt[i];
	double dDot = VectorDot(vPrjPosVec, vLineDirN);
	for (int i=0; i<3; i++)
		vPrjPt[i] = vLinePt[i] + vLineDirN[i] * dDot;
}

// 点是否在直线上
BOOL IsPointOnLine(const ProPoint3d vLinePt, const ProVector vLineDir, const ProPoint3d vPt)
{
	ProPoint3d vPrjPt;
	ProjectPtToLine(vLinePt, vLineDir, vPt, vPrjPt);

	if (ISNEAR(vPt[0], vPrjPt[0]) && ISNEAR(vPt[1], vPrjPt[1]) && ISNEAR(vPt[2], vPrjPt[2]))
		return TRUE;
	else
		return FALSE;
}

// 两向量是否同向
BOOL IsVecSameDir(const ProVector v1, const ProVector v2)
{
	ProVector vVec1, vVec2;
	VectorNormalize(vVec1, v1);
	VectorNormalize(vVec2, v2);

	if (ISNEAR(vVec1[0], vVec2[0]) && ISNEAR(vVec1[1], vVec2[1]) && ISNEAR(vVec1[2], vVec2[2]))
		return TRUE;
	else
		return FALSE;
}

// 两向量是否反向
BOOL IsVecReverseDir(const ProVector v1, const ProVector v2)
{
	ProVector vVec1, vVec2;
	VectorNormalize(vVec1, v1);
	VectorNormalize(vVec2, v2);
	for (int i=0; i<3; i++)
		vVec2[i] = -vVec2[i];

	if (ISNEAR(vVec1[0], vVec2[0]) && ISNEAR(vVec1[1], vVec2[1]) && ISNEAR(vVec1[2], vVec2[2]))
		return TRUE;
	else
		return FALSE;
}

// 两点是否重合
BOOL IsPntCoincide(const ProPoint3d p1, const ProPoint3d p2)
{
	if (ISNEAR(p1[0], p2[0]) && ISNEAR(p1[1], p2[1]) && ISNEAR(p1[2], p2[2]))
		return TRUE;
	else
		return FALSE;
}

// 判断向量平行
BOOL CheckVectorParallel(const ProVector pc1, const ProVector pv2)
{
	double dValues[3];
	dValues[0] = pc1[1]*pv2[2] - pc1[2]*pv2[1];
	dValues[1] = pc1[2]*pv2[0] - pc1[0]*pv2[2];
	dValues[2] = pc1[0]*pv2[1] - pc1[1]*pv2[0];

	if ((!DEQUAL(dValues[0], 0.0)) || (!DEQUAL(dValues[1], 0.0)) || (!DEQUAL(dValues[2], 0.0)))
	{
		return FALSE;
	}
	return TRUE;
}

// 获取向量夹角的余弦值
double GetVectorCosAngle(const ProVector v1, const ProVector v2)
{
	ProVector vVec1, vVec2;
	VectorNormalize(vVec1, v1);
	VectorNormalize(vVec2, v2);

	double dAngleCons = 0.0;
	for (int i=0; i<3; i++)
	{
		dAngleCons += vVec1[i]*vVec2[i];
	}
	return dAngleCons;
}

// 获取误差修正后的Sin或Cos数值
double GetCorrectSinCosValue(double dCosValue)
{
	if (dCosValue > 1.0+double(DFE_ZERO))
	{
		ASSERT(FALSE);
		dCosValue = 1.0;
	}
	else if (dCosValue > 1.0-double(DFE_ZERO))	// acos(1)与acos(1-1e-6)相差了0.1度!因此需要修正此误差(1e-6)
	{
		dCosValue = 1.0;
	}
	else if (dCosValue < -1.0-double(DFE_ZERO))
	{
		ASSERT(FALSE);
		dCosValue = -1.0;
	}
	else if (dCosValue < -1.0+double(DFE_ZERO))	// acos(-1)与acos(-1+1e-6)相差了0.1度!因此需要修正此误差(1e-6)
	{
		dCosValue = -1.0;
	}

	return dCosValue;
}

// 计算反余弦值
double CalArcCos(double dValue)
{
	dValue = GetCorrectSinCosValue(dValue);
	return acos(dValue);
}

// 计算反正弦值
double CalArcSin(double dValue)
{
	dValue = GetCorrectSinCosValue(dValue);
	return asin(dValue);
}

//===================================================================================================

// 获取特征中的所有几何项
ProError FeatureGeomsGetAction(ProGeomitem* pDatum, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	vector <ProModelitem> &visitData = *(vector <ProModelitem> *)pData;
	visitData.push_back(*pDatum);
	return PRO_TK_NO_ERROR;
}

// 获取面组中的所有面
ProError QuiltSurfacesGetAction(ProSurface surface, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	vector<ProSurface> &visitData = *(vector<ProSurface>*)pData;
	visitData.push_back(surface);
	return PRO_TK_NO_ERROR;
}

// 获取面的内环
ProError SurfaceInnerContoursGetAction(ProContour contour, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	ProContourTraversal tral;
	ProContourTraversalGet(contour, &tral);
	if (tral == PRO_CONTOUR_TRAV_INTERNAL)
	{
		vector<ProContour> &visitData = *(vector<ProContour>*)pData;
		visitData.push_back(contour);
	}
	return PRO_TK_NO_ERROR;
}

// 获取面的外环
ProError SurfaceExtContoursGetAction(ProContour contour, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	ProContourTraversal tral;
	ProContourTraversalGet(contour, &tral);
	if (tral == PRO_CONTOUR_TRAV_EXTERNAL)
	{
		vector<ProContour> &visitData = *(vector<ProContour>*)pData;
		visitData.push_back(contour);
	}
	return PRO_TK_NO_ERROR;
}

// 获取环的所有边
ProError ContourEdgesGetAction(ProEdge Edge, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	vector<ProEdge> &visitData = *(vector<ProEdge>*)pData;
	visitData.push_back(Edge);
	return PRO_TK_NO_ERROR;
}

// 获取模型的一个面
ProError OneSurfaceGetAction(ProSurface surface, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	ProSurface &visitData = *(ProSurface*)pData;
	visitData = surface;
	return PRO_TK_USER_ABORT;
}

// 获取模型的所有面组
ProError SolidQuiltsGetAction(ProQuilt quilt, ProError status, ProAppData pData)
{
	UNUSED_ALWAYS(status);
	vector<ProQuilt> &visitData = *(vector<ProQuilt>*)pData;
	visitData.push_back(quilt);
	return PRO_TK_NO_ERROR;
}

//===================================================================================================

// 创建特征
BOOL CreateFeature(/*input*/ProMdl pMdl, /*input*/ProElement elemRoot, /*output*/ProFeature& feat)
{
	BOOL bRet = FALSE;
	ProError status;

	if (pMdl != NULL && elemRoot != NULL)
	{
		ProModelitem itemMdl;
		status = ProMdlToModelitem(pMdl, &itemMdl);
		ProSelection selModel;
		status = ProSelectionAlloc(NULL, &itemMdl, &selModel);

		ProErrorlist errorsList;
		status = ProFeatureCreate(selModel, elemRoot, NULL, 0, &feat, &errorsList);
		if (status == PRO_TK_NO_ERROR)
			bRet = TRUE;

		SAFE_DELETE_SELECTION(selModel);
	}

	return bRet;
}

// 创建基准平面
int CreateDatumPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selSrf, /*output*/ProModelitem& itemPlane)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selSrf->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 构建Element Tree
	ProElement elemRoot = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemRoot);
	{
		// PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_DATUM;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		status = ProElementValueSet(elemType, valueType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemType);

		// PRO_E_DTMPLN_CONSTRAINTS
		ProElement elemConstrs;
		status = ProElementAlloc(PRO_E_DTMPLN_CONSTRAINTS, &elemConstrs);
		{
			// PRO_E_DTMPLN_CONSTRAINT
			ProElement elemConstr;
			status = ProElementAlloc(PRO_E_DTMPLN_CONSTRAINT, &elemConstr);
			{
				// PRO_E_DTMPLN_CONSTR_TYPE
				ProElement elemConstrType;
				status = ProElementAlloc(PRO_E_DTMPLN_CONSTR_TYPE, &elemConstrType);
				ProValueData valdataTypeConstr;
				valdataTypeConstr.type = PRO_VALUE_TYPE_INT;
				valdataTypeConstr.v.i = PRO_DTMPLN_THRU;
				ProValue valueConstrType = NULL;
				status = ProValueAlloc(&valueConstrType);
				status = ProValueDataSet(valueConstrType, &valdataTypeConstr);
				status = ProElementValueSet(elemConstrType, valueConstrType);
				status = ProElemtreeElementAdd(elemConstr, NULL, elemConstrType);

				// PRO_E_DTMPLN_CONSTR_REF
				ProElement elemConstrRef;
				status = ProElementAlloc(PRO_E_DTMPLN_CONSTR_REF, &elemConstrRef);
				ProValueData valdataConstrRef;
				valdataConstrRef.type = PRO_VALUE_TYPE_SELECTION;
				valdataConstrRef.v.r = selSrf;
				ProValue valueConstrRef = NULL;
				status = ProValueAlloc(&valueConstrRef);
				status = ProValueDataSet(valueConstrRef, &valdataConstrRef);
				status = ProElementValueSet(elemConstrRef, valueConstrRef);
				status = ProElemtreeElementAdd(elemConstr, NULL, elemConstrRef);
			}
			status = ProElemtreeElementAdd(elemConstrs, NULL, elemConstr);
		}
		status = ProElemtreeElementAdd(elemRoot, NULL, elemConstrs);
	}

	// 创建特征
	ProFeature feat;
	if (CreateFeature(pMdl, elemRoot, feat))
	{
		// 获取特征中的基准面
		vector<ProModelitem> arrDatum;
		status = ProFeatureGeomitemVisit(&feat, PRO_SURFACE, FeatureGeomsGetAction, NULL, &arrDatum);
		itemPlane.id = arrDatum[0].id;
		itemPlane.owner = arrDatum[0].owner;
		itemPlane.type = arrDatum[0].type;
		nRet = feat.id;
	}

	return nRet;
}

// 创建实体化特征
int CreateSolidify(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*input*/int nSide/* = PRO_SOLIDIFY_SIDE_ONE*/)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selPlane->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	ProElement elemRoot = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemRoot);
	{
		// 特征类型 PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_CUT;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		ProElementValueSet(elemType, valueType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemType);

		// PRO_E_FEATURE_FORM
		ProElement elemForm;
		status = ProElementAlloc(PRO_E_FEATURE_FORM, &elemForm);
		ProValueData valdataForm;
		valdataForm.type = PRO_VALUE_TYPE_INT;
		valdataForm.v.i = PRO_USE_SURFS;
		ProValue valueForm = NULL;
		status = ProValueAlloc(&valueForm);
		status = ProValueDataSet(valueForm, &valdataForm);
		ProElementValueSet(elemForm, valueForm);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemForm);

		// PRO_E_PATCH_QUILT
		ProElement elemQuilt;
		status = ProElementAlloc(PRO_E_PATCH_QUILT, &elemQuilt);
		ProValueData valdataQuilt;
		valdataQuilt.type = PRO_VALUE_TYPE_SELECTION;
		valdataQuilt.v.r = selPlane;
		ProValue valueQuilt = NULL;
		status = ProValueAlloc(&valueQuilt);
		status = ProValueDataSet(valueQuilt, &valdataQuilt);
		status = ProElementValueSet(elemQuilt, valueQuilt);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemQuilt);

		// PRO_E_PATCH_TYPE
		ProElement elemPatchType;
		status = ProElementAlloc(PRO_E_PATCH_TYPE, &elemPatchType);
		ProValueData valdataPatchType;
		valdataPatchType.type = PRO_VALUE_TYPE_INT;
		valdataPatchType.v.i = PRO_FEAT_CUT;
		ProValue valuePatchType = NULL;
		status = ProValueAlloc(&valuePatchType);
		status = ProValueDataSet(valuePatchType, &valdataPatchType);
		status = ProElementValueSet(elemPatchType, valuePatchType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemPatchType);

		// PRO_E_PATCH_MATERIAL_SIDE
		ProElement elemSide;
		status = ProElementAlloc(PRO_E_PATCH_MATERIAL_SIDE, &elemSide);
		ProValueData valdataSide;
		valdataSide.type = PRO_VALUE_TYPE_INT;
		valdataSide.v.i = nSide;
		ProValue valueSide = NULL;
		status = ProValueAlloc(&valueSide);
		status = ProValueDataSet(valueSide, &valdataSide);
		status = ProElementValueSet(elemSide, valueSide);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemSide);
	}

	ProFeature feat;
	if (CreateFeature(pMdl, elemRoot, feat))
	{
		nRet = feat.id;
	}
	return nRet;
}

// 创建复制特征
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemSrf)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selInputSrf->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 检查UDF文件
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\copysrf.gph";
	if (IsFileExist(strUDFPath))
	{
		ProUdfdata udfdata;
		status = ProUdfdataAlloc(&udfdata);
		// 设置UDF路径
		ProPath pathUDF;
		wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
		status = ProUdfdataPathSet(udfdata, pathUDF);
		// 获取UDF参照项
		ProUdfRequiredRef* parrRef = NULL;
		status = ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
		ProLine prompt;
		ProUdfrequiredrefPromptGet(parrRef[0], prompt);
		// 设置参照
		ProUdfreference refUDF;
		status = ProUdfreferenceAlloc(prompt, selInputSrf, PRO_B_TRUE, &refUDF);
		status = ProUdfdataReferenceAdd(udfdata, refUDF);

		// 创建UDF特征
		ProGroup udf;
		status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
		if (status == PRO_TK_NO_ERROR)
		{
			ProFeature* pFeatArray = NULL;
			status = ProGroupFeaturesCollect(&udf, &pFeatArray);
			// 获取特征中的面
			vector<ProModelitem> arrSrfs;
			status = ProFeatureGeomitemVisit(&pFeatArray[1], PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfs);
			itemSrf.id = arrSrfs[0].id;
			itemSrf.owner = arrSrfs[0].owner;
			itemSrf.type = arrSrfs[0].type;
			nRet = pFeatArray[1].id;
			status = ProGroupUngroup(&udf);
			status = ProArrayFree((ProArray*)&pFeatArray);
		}
		status = ProUdfdataFree(udfdata);
		status = ProUdfreferenceFree(refUDF);
	}
	else
		nRet = ER_INVALID_UDF;

	return nRet;
}

// 创建复制特征（排除1项）
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/ProSelection selInputEdge, 
						/*output*/ProModelitem& itemSrf)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selInputSrf->sel_type != SEL_3D_SRF && selInputEdge->sel_type != SEL_3D_EDG)
		return ER_BAD_INPUT;

	// 检查UDF文件
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\copysrf_fill.gph";
	if (IsFileExist(strUDFPath))
	{
		ProUdfdata udfdata;
		status = ProUdfdataAlloc(&udfdata);
		// 设置UDF路径
		ProPath pathUDF;
		wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
		status = ProUdfdataPathSet(udfdata, pathUDF);
		// 获取参照项
		ProUdfRequiredRef* parrRef = NULL;
		status = ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
		ProLine prompt[2];
		status = ProUdfrequiredrefPromptGet(parrRef[0], prompt[0]);
		status = ProUdfrequiredrefPromptGet(parrRef[1], prompt[1]);
		// 设置参照
		ProUdfreference refUDF[2];
		status = ProUdfreferenceAlloc(prompt[0], selInputSrf, PRO_B_TRUE, &refUDF[0]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[0]);
		status = ProUdfreferenceAlloc(prompt[1], selInputEdge, PRO_B_TRUE, &refUDF[1]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[1]);
		// 创建UDF特征
		ProGroup udf;
		status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
		if (status == PRO_TK_NO_ERROR)
		{
			ProFeature* pFeatArray = NULL;
			status = ProGroupFeaturesCollect(&udf, &pFeatArray);
			// 获取特征中的面
			vector<ProModelitem> arrSrfs;
			status = ProFeatureGeomitemVisit(&pFeatArray[1], PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfs);
			itemSrf.id = arrSrfs[0].id;
			itemSrf.owner = arrSrfs[0].owner;
			itemSrf.type = arrSrfs[0].type;
			nRet = pFeatArray[1].id;
			status = ProGroupUngroup(&udf);
			status = ProArrayFree((ProArray*)&pFeatArray);
		}
		ProUdfdataFree(udfdata);
		ProUdfreferenceFree(refUDF[0]);
		ProUdfreferenceFree(refUDF[1]);
	}
	else
		nRet = ER_INVALID_UDF;

	return nRet;
}

// 创建填充面
int CreateFillSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*output*/ProModelitem& itemQlt)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selPlane->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 解析面，非平面无法填充
	ProModelitem itemSurf;
	status = ProSelectionModelitemGet(selPlane, &itemSurf);
	ProSurface plane;
	status = ProGeomitemToSurface(&itemSurf, &plane);
	ProSrftype srftype;
	status = ProSurfaceTypeGet(plane, &srftype);
	if (srftype != PRO_SRF_PLANE)
		return ER_BAD_INPUT;

	ProAsmcomppath compath;
	status = ProSelectionAsmcomppathGet(selPlane, &compath);

	//SAFE_DELETE_SELECTION(refEdge);

	ProElement elemTree = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemTree);
	{
		// 特征类型PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		status = ProElemtreeElementAdd(elemTree, NULL, elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_DATUM_SURF;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		status = ProElementValueSet(elemType, valueType);

		// PRO_E_FEATURE_FORM
		ProElement elemForm;
		status = ProElementAlloc(PRO_E_FEATURE_FORM, &elemForm);
		status = ProElemtreeElementAdd(elemTree, NULL, elemForm);
		ProValueData valdataForm;
		valdataForm.type = PRO_VALUE_TYPE_INT;
		valdataForm.v.i = PRO_FLAT;
		ProValue valueForm = NULL;
		status = ProValueAlloc(&valueForm);
		status = ProValueDataSet(valueForm, &valdataForm);
		status = ProElementValueSet(elemForm, valueForm);

		// PRO_E_STD_SECTION
		ProElement elemSection = NULL;
		status = ProElementAlloc(PRO_E_STD_SECTION, &elemSection);
		status = ProElemtreeElementAdd(elemTree, NULL, elemSection);
		{
			// PRO_E_STD_SEC_SETUP_PLANE
			ProElement elemSetupPlane;
			status = ProElementAlloc(PRO_E_STD_SEC_SETUP_PLANE, &elemSetupPlane);
			status = ProElemtreeElementAdd(elemSection, NULL, elemSetupPlane);
			{
				// PRO_E_STD_SEC_PLANE
				ProElement elemPlane;
				status = ProElementAlloc(PRO_E_STD_SEC_PLANE, &elemPlane);
				status = ProElemtreeElementAdd(elemSetupPlane, NULL, elemPlane);
				ProValueData valdataPlane;
				valdataPlane.type = PRO_VALUE_TYPE_SELECTION;
				valdataPlane.v.r = selPlane;
				ProValue valuePlane = NULL;
				status = ProValueAlloc(&valuePlane);
				status = ProValueDataSet(valuePlane, &valdataPlane);
				status = ProElementValueSet(elemPlane, valuePlane);

				// PRO_E_STD_SEC_PLANE_VIEW_DIR
				ProElement elemViewDir;
				status = ProElementAlloc(PRO_E_STD_SEC_PLANE_VIEW_DIR, &elemViewDir);
				status = ProElemtreeElementAdd(elemSetupPlane, NULL, elemViewDir);
				ProValueData valdataViewDir;
				valdataViewDir.type = PRO_VALUE_TYPE_INT;
				valdataViewDir.v.i = PRO_SEC_VIEW_DIR_SIDE_ONE;
				ProValue valueViewDir = NULL;
				status = ProValueAlloc(&valueViewDir);
				status = ProValueDataSet(valueViewDir, &valdataViewDir);
				status = ProElementValueSet(elemViewDir, valueViewDir);

				// PRO_E_STD_SEC_PLANE_ORIENT_DIR
				ProElement elemOrientDir;
				status = ProElementAlloc(PRO_E_STD_SEC_PLANE_ORIENT_DIR, &elemOrientDir);
				status = ProElemtreeElementAdd(elemSetupPlane, NULL, elemOrientDir);
				ProValueData valdataOrientDir;
				valdataOrientDir.type = PRO_VALUE_TYPE_INT;
				valdataOrientDir.v.i = PRO_SEC_ORIENT_DIR_UP;
				ProValue valueOrientDir = NULL;
				status = ProValueAlloc(&valueOrientDir);
				status = ProValueDataSet(valueOrientDir, &valdataOrientDir);
				status = ProElementValueSet(elemOrientDir, valueOrientDir);

				// PRO_E_STD_SEC_PLANE_ORIENT_REF
				//ProElement elemOrientRef;
				//ProElementAlloc(PRO_E_STD_SEC_PLANE_ORIENT_REF, &elemOrientRef);
				//ProValueData valueOrientRef;
				//valueOrientRef.type = PRO_VALUE_TYPE_SELECTION;
				//valueOrientRef.v.r = refEdge;
				//ProValue pValueOrientRef = NULL;
				//ProValueAlloc(&pValueOrientRef);
				//ProValueDataSet(pValueOrientRef, &valueOrientRef);
				//ProElementValueSet(elemOrientRef, pValueOrientRef);
				//status = ProElemtreeElementAdd(elemSetupPlane, NULL, elemOrientRef);
			}
		}
	}
	// 创建不完全定义特征
	ProModelitem itemMdl;
	ProMdlToModelitem(pMdl, &itemMdl);
	ProSelection selModel;
	ProSelectionAlloc(NULL, &itemMdl, &selModel);

	ProErrorlist errorsList;
	ProFeatureCreateOptions opts[2] = {PRO_FEAT_CR_INCOMPLETE_FEAT};
	ProFeature feat;
	status = ProFeatureCreate(selModel, elemTree, opts, 1, &feat, &errorsList);
	if (status == PRO_TK_NO_ERROR)
	{
		ProElement elemTreeRedifine = NULL;
		ProFeatureElemtreeExtract(&feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elemTreeRedifine);
		// 获取草图
		ProElempathItem pathItem[2];
		pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[0].path_item.elem_id = PRO_E_STD_SECTION;
		pathItem[1].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[1].path_item.elem_id = PRO_E_SKETCHER;
		ProElempath elemPath;
		ProElempathAlloc(&elemPath);
		ProElempathDataSet(elemPath, pathItem, 2);

		ProElement elemSketch;
		status = ProElemtreeElementGet(elemTreeRedifine, elemPath, &elemSketch);
		if (status == PRO_TK_NO_ERROR)
		{
			ProValue val;
			status = ProElementValueGet(elemSketch, &val);
			ProValueData valdata;
			status = ProValueDataGet(val, &valdata);

			ProSection secSketch = NULL;
			secSketch = (ProSection)valdata.v.p;

			// 获取面的外轮廓
			vector<ProContour> arrExContours;
			status = ProSurfaceContourVisit(plane, SurfaceExtContoursGetAction, NULL, &arrExContours);
			vector<ProEdge> arrEdges;
			status = ProContourEdgeVisit(plane, arrExContours[0], ContourEdgesGetAction, NULL, &arrEdges);
			ProModelitem itemEdgeTemp;
			status = ProEdgeToGeomitem(ProMdlToSolid(itemSurf.owner), arrEdges[0], &itemEdgeTemp);
			ProSelection refEdgeTemp;
			status = ProSelectionAlloc(&compath, &itemEdgeTemp, &refEdgeTemp);

			int nIds;
			ProIntlist id_list;
			status = ProSectionEntityUseEdgeLoop(secSketch, selPlane, refEdgeTemp, &id_list, &nIds);
			//for (int i=0; i<arrEdges.size(); i++)
			//{
			//	ProModelitem itemEdgeTemp;
			//	status = ProEdgeToGeomitem(ProMdlToSolid(itemSurf.owner), arrEdges[i], &itemEdgeTemp);
			//	ProSelection refEdgeTemp;
			//	status = ProSelectionAlloc(&compath, &itemEdgeTemp, &refEdgeTemp);
			//	//status = ProSectionEntityUseEdge(secSketch, refEdgeTemp, &nIds);
			//	//SAFE_DELETE_SELECTION(refEdgeTemp);
			//	ProSelectionHighlight(refEdgeTemp, PRO_COLOR_HIGHLITE);
			//}

			if (status == PRO_TK_NO_ERROR)
			{
				status = ProSectionNameSet(secSketch, L"Section");
				ProWSecerror errors;
				status = ProSecerrorAlloc(&errors);
				status = ProSectionSolve(secSketch, &errors);
				if (status == PRO_TK_NO_ERROR)
				{
					// 重定义特征
					ProAsmcomppath compath;
					status = ProSelectionAsmcomppathGet(selModel, &compath);
					opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;
					opts[1] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;
					ProErrorlist errors;
					status = ProFeatureRedefine(&compath, &feat, elemTreeRedifine, opts, 2, &errors);
					if (status == PRO_TK_NO_ERROR)
					{
						// 获取特征中面
						vector<ProModelitem> arrSrfItem;
						status = ProFeatureGeomitemVisit(&feat, PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfItem);
						itemQlt.id = arrSrfItem[0].id;
						itemQlt.owner = arrSrfItem[0].owner;
						itemQlt.type = arrSrfItem[0].type;
						nRet = feat.id;
					}
				}
				ProSecerrorFree(&errors);
			}
		}
		ProElempathFree(&elemPath);
		ProFeatureElemtreeFree(&feat, elemTreeRedifine);
	}
	ProFeatureElemtreeFree(&feat, elemTree);
	return nRet;
}

// 创建面
int CreateSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemQlt)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	ProAsmcomppath compath;
	status = ProSelectionAsmcomppathGet(selInputSrf, &compath);

	// 检查输入
	if (selInputSrf->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 判断面的类型
	ProModelitem itemInputSrf;
	status = ProSelectionModelitemGet(selInputSrf, &itemInputSrf);
	ProSurface srfInput;
	status = ProGeomitemToSurface(&itemInputSrf, &srfInput);
	ProSrftype typeInputSrf;
	status = ProSurfaceTypeGet(srfInput, &typeInputSrf);
	if (typeInputSrf == PRO_SRF_PLANE)	// 如果是平面，采取填充的方式重建
	{
		nRet = CreateFillSurf(pMdl, selInputSrf, itemQlt);
	}
	else	// 如果是曲面，采取复制的方式创建
	{
		// 检查曲面是否包含内环
		vector<ProContour> arrContours;
		status = ProSurfaceContourVisit(srfInput, SurfaceInnerContoursGetAction, NULL, &arrContours);
		int nCount = (int)arrContours.size();
		if (nCount >= 1)	// 如果包含内环，复制面时按顺序逐一排除后再合并
		{
			int* pArrFeatIDs = NULL;
			status = ProArrayAlloc(0, sizeof(int), 1, (ProArray*)&pArrFeatIDs);
			vector<ProModelitem> arrQuilts;

			// 遍历面的所有内环
			vector<ProSelection> arrSelQuilts;
			BOOL bError = FALSE;
			for (int i=0; i<nCount; i++)
			{
				// 获取环的边界
				vector<ProEdge> arrEdges;
				status = ProContourEdgeVisit(srfInput, arrContours[i], ContourEdgesGetAction, NULL, &arrEdges);
				// 将环的第一条边作为输入，创建复制面特征
				ProModelitem edgeItem;
				status = ProEdgeToGeomitem(ProMdlToSolid(itemInputSrf.owner), arrEdges[0], &edgeItem);
				ProSelection selEdge;
				status = ProSelectionAlloc(&compath, &edgeItem, &selEdge);
				ProModelitem itemQuilt;
				int nCopyID = CreateSurfaceByCopy(pMdl, selInputSrf, selEdge, itemQuilt);
				SAFE_DELETE_SELECTION(selEdge);
				if (nCopyID > 0)
				{
					status = ProArrayObjectAdd((ProArray*)&pArrFeatIDs, 0, 1, &nCopyID);
					ProSelection selQuiltTemp;
					status = ProSelectionAlloc(&compath, &itemQuilt, &selQuiltTemp);
					arrSelQuilts.push_back(selQuiltTemp);

					// 将复制面进行合并
					if (i != 0)
					{
						int nMergeID = MergeSurfs(pMdl, arrSelQuilts, TRUE);
						SAFE_DELETE_SELECTION(arrSelQuilts[1]);
						arrQuilts.pop_back();
						if (nMergeID > 0)
						{
							status = ProArrayObjectAdd((ProArray*)&pArrFeatIDs, 0, 1, &nMergeID);
						}
						else
						{
							bError = TRUE;
							break;
						}
					}
					else
					{
						itemQlt.id = itemQuilt.id;
						itemQlt.type = itemQuilt.type;
						itemQlt.owner = itemQuilt.owner;
					}
				}
				else
				{
					bError = TRUE;
					break;
				}
			}
			// 创建组
			int nFeatsCount;
			status = ProArraySizeGet(pArrFeatIDs, &nFeatsCount);
			if (nFeatsCount>1)
			{
				ProGroup group;
				status = ProLocalGroupCreate(ProMdlToSolid(pMdl), pArrFeatIDs, nFeatsCount, L"Surface", &group);
				nRet = group.id;
			}
			else if (nFeatsCount == 1)
			{
				nRet = pArrFeatIDs[0];
			}
			if (bError)
				nRet = ER_CREATE_FAIL;

			ProArrayFree((ProArray*)&pArrFeatIDs);
		}
		else	// 如果曲面不包含内环，直接复制面
		{
			nRet = CreateSurfaceByCopy(pMdl, selInputSrf, itemQlt);
		}
	}
	return nRet;
}

// 获取边所在的完整环（内环）
BOOL GetContourByEdge(/*input*/ProSelection selEdge, /*output*/vector<ProSelection> &arrSelEdge, /*output*/int& nDir)
{
	ProError status;

	// 获取边的相邻面
	ProModelitem itemEdge;
	status = ProSelectionModelitemGet(selEdge, &itemEdge);
	ProEdge edge[3];
	status = ProGeomitemToEdge(&itemEdge, &edge[0]);
	ProSurface surfNeighbour[2];
	status = ProEdgeNeighborsGet(edge[0], &edge[1], &edge[2], &surfNeighbour[0], &surfNeighbour[1]);
	if (status != PRO_TK_NO_ERROR)
		return FALSE;

	// 检查面的环与边是否匹配
	vector<ProContour> arrContours;
	vector<ProEdge> arrEdges;
	for (int i=0; i<2; i++)
	{
		arrContours.clear();
		status = ProSurfaceContourVisit(surfNeighbour[i], SurfaceInnerContoursGetAction, NULL, &arrContours);
		for (int j=0; j<(int)arrContours.size(); j++)
		{
			arrEdges.clear();
			status = ProContourEdgeVisit(surfNeighbour[i], arrContours[j], ContourEdgesGetAction, NULL, &arrEdges);
			for (int k=0; k<(int)arrEdges.size(); k++)
			{
				// 如果匹配
				if (arrEdges[k] == edge[0])
				{
					arrSelEdge.clear();
					ProAsmcomppath compath;
					status = ProSelectionAsmcomppathGet(selEdge, &compath);
					ProModelitem itemEdgeTemp;
					for (int m=0; m<(int)arrEdges.size(); m++)
					{
						status = ProEdgeToGeomitem(ProMdlToSolid(itemEdge.owner), arrEdges[m], &itemEdgeTemp);
						ProSelection selEdgeTemp;
						status = ProSelectionAlloc(&compath, &itemEdgeTemp, &selEdgeTemp);
						arrSelEdge.push_back(selEdgeTemp);
					}
					// 计算环的方向
					ProEdgeDirGet(arrEdges[0], surfNeighbour[i], &nDir);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// 设置对象的颜色
void SetSurfColor(/*input*/const ProModelitem& item, /*input*/double dRed, /*input*/double dGreen, /*input*/double dBlue)
{
	Pro_surf_props surfProps;

	int nResult = -1;
	if (item.type == PRO_SURFACE)
	{
		nResult = prodb_get_surface_props((Prohandle)(item.owner), SEL_3D_SRF, item.id, 0, &surfProps);
		prodb_unset_surface_props((Prohandle)(item.owner), SEL_3D_SRF, item.id, 0);
		surfProps.color_rgb[0] = dRed/255.0;
		surfProps.color_rgb[1] = dGreen/255.0;
		surfProps.color_rgb[2] = dBlue/255.0;
		surfProps.highlight_color[0] = 1.0;
		surfProps.highlight_color[1] = 1.0;
		surfProps.highlight_color[2] = 1.0;
		surfProps.ambient = 0.5;
		surfProps.diffuse = 0.8;
		surfProps.highlite = 0.5;
		surfProps.shininess = 0.4;
		surfProps.reflection = 0.25;
		nResult = prodb_set_surface_props((Prohandle)(item.owner), SEL_3D_SRF, item.id, 0, &surfProps);
		nResult = prodb_set_surface_props((Prohandle)(item.owner), SEL_3D_SRF, item.id, 1, &surfProps);
	}
	else if (item.type == PRO_QUILT)
	{
		nResult = prodb_get_surface_props((Prohandle)(item.owner), SEL_3D_SRF_LIST, item.id, 0, &surfProps);
		prodb_unset_surface_props((Prohandle)(item.owner), SEL_3D_SRF_LIST, item.id, 0);
		surfProps.color_rgb[0] = dRed/255.0;
		surfProps.color_rgb[1] = dGreen/255.0;
		surfProps.color_rgb[2] = dBlue/255.0;
		surfProps.highlight_color[0] = 1.0;
		surfProps.highlight_color[1] = 1.0;
		surfProps.highlight_color[2] = 1.0;
		surfProps.ambient = 0.5;
		surfProps.diffuse = 0.8;
		surfProps.highlite = 0.5;
		surfProps.shininess = 0.4;
		surfProps.reflection = 0.25;
		nResult = prodb_set_surface_props((Prohandle)(item.owner), SEL_3D_SRF_LIST, item.id, 0, &surfProps);
		nResult = prodb_set_surface_props((Prohandle)(item.owner), SEL_3D_SRF_LIST, item.id, 1, &surfProps);
	}
	else if (item.type == PRO_ASSEMBLY || item.type == PRO_PART)
	{
		nResult = prodb_get_surface_props((Prohandle)(item.owner), SEL_3D_PART, -1, 0, &surfProps);
		prodb_unset_surface_props((Prohandle)(item.owner), SEL_3D_PART, item.id, 0);
		surfProps.color_rgb[0] = dRed/255.0;
		surfProps.color_rgb[1] = dGreen/255.0;
		surfProps.color_rgb[2] = dBlue/255.0;
		surfProps.highlight_color[0] = 1.0;
		surfProps.highlight_color[1] = 1.0;
		surfProps.highlight_color[2] = 1.0;
		surfProps.ambient = 0.5;
		surfProps.diffuse = 0.8;
		surfProps.highlite = 0.5;
		surfProps.shininess = 0.4;
		surfProps.reflection = 0.25;
		nResult = prodb_get_surface_props((Prohandle)(item.owner), SEL_3D_PART, -1, 0, &surfProps);
	}
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

// 通过边或曲线修剪面
int TrimSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
				/*input*/vector<ProSelection> arrSelEdges, int nTrimDir)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (GetSelectionType(selQuilt) != PRO_QUILT || arrSelEdges.size() < 1)
		return ER_BAD_INPUT;

	// 设置裁剪方向
	ProTrimMatlSideOpts matside;
	ProTrimPrimaryQltSide qltside;
	if (nTrimDir == PRO_EDGE_SAME_DIRECTION)
	{
		matside = PRO_TRIM_MATL_SIDE_TWO;
		qltside = PRO_TRIM_PRIM_QLT_SIDE_TWO;
	}
	else
	{
		matside = PRO_TRIM_MATL_SIDE_ONE;
		qltside = PRO_TRIM_PRIM_QLT_SIDE_ONE;
	}

	ProElement elemTree = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemTree);
	{
		// 特征类型 PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_CUT;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		status = ProElementValueSet(elemType, valueType);
		status = ProElemtreeElementAdd(elemTree, NULL, elemType);

		// 修剪类型 PRO_E_SRF_TRIM_TYPE
		ProElement elemTrimType;
		status = ProElementAlloc(PRO_E_SRF_TRIM_TYPE, &elemTrimType);
		ProValueData valdataTrimType;
		valdataTrimType.type = PRO_VALUE_TYPE_INT;
		valdataTrimType.v.i = PRO_SURF_TRIM_USE_CRV;
		ProValue valueTrimType = NULL;
		status = ProValueAlloc(&valueTrimType);
		status = ProValueDataSet(valueTrimType, &valdataTrimType);
		status = ProElementValueSet(elemTrimType, valueTrimType);
		status = ProElemtreeElementAdd(elemTree, NULL, elemTrimType);

		// 面类型 PRO_E_SURF_TRIM_TYPE
		ProElement elemSurfType;
		status = ProElementAlloc(PRO_E_SRF_TRIM_TYPE, &elemSurfType);
		ProValueData valdataSurfType;
		valdataSurfType.type = PRO_VALUE_TYPE_INT;
		valdataSurfType.v.i = PRO_SURF_TRIM_USE_CRV;
		ProValue valueSurfType = NULL;
		status = ProValueAlloc(&valueSurfType);
		status = ProValueDataSet(valueSurfType, &valdataSurfType);
		status = ProElementValueSet(elemSurfType, valueSurfType);
		status = ProElemtreeElementAdd(elemTree, NULL, elemSurfType);

		// 参考面组 PRO_E_TRIM_QUILT
		ProElement elemTrimQuilt;
		status = ProElementAlloc(PRO_E_TRIM_QUILT, &elemTrimQuilt);
		ProValueData valdataTrimQuilt;
		valdataTrimQuilt.type = PRO_VALUE_TYPE_SELECTION;
		valdataTrimQuilt.v.r = selQuilt;
		ProValue valueTrimQuilt = NULL;
		status = ProValueAlloc(&valueTrimQuilt);
		status = ProValueDataSet(valueTrimQuilt, &valdataTrimQuilt);
		status = ProElementValueSet(elemTrimQuilt, valueTrimQuilt);
		status = ProElemtreeElementAdd(elemTree, NULL, elemTrimQuilt);

		// 参考曲线 PRO_E_STD_CURVE_COLLECTION_APPL
		ProElement elemCurve;
		status = ProElementAlloc(PRO_E_STD_CURVE_COLLECTION_APPL, &elemCurve);
		ProCollection collect;
		status = ProCrvcollectionAlloc(&collect);
		ProCrvcollinstr instCurv;
		status = ProCrvcollinstrAlloc(PRO_CURVCOLL_ADD_ONE_INSTR, &instCurv);
		for (int i=0; i<(int)arrSelEdges.size(); i++)
		{
			ProReference refCurve;
			status = ProSelectionToReference(arrSelEdges[i], &refCurve);
			status = ProCrvcollinstrReferenceAdd(instCurv, refCurve);
		}
		status = ProCrvcollectionInstructionAdd(collect, instCurv);
		status = ProElementCollectionSet(elemCurve, collect);
		status = ProElemtreeElementAdd(elemTree, NULL, elemCurve);
		status = ProCollectionFree(&collect);
		status = ProCrvcollinstrFree(instCurv);

		// PRO_E_MATERIAL_SIDE
		ProElement elemMatSide;
		status = ProElementAlloc(PRO_E_MATERIAL_SIDE, &elemMatSide);
		ProValueData valdataMatSide;
		valdataMatSide.type = PRO_VALUE_TYPE_INT;
		valdataMatSide.v.i = matside;
		ProValue valueMatSide = NULL;
		status = ProValueAlloc(&valueMatSide);
		status = ProValueDataSet(valueMatSide, &valdataMatSide);
		status = ProElementValueSet(elemMatSide, valueMatSide);
		status = ProElemtreeElementAdd(elemTree, NULL, elemMatSide);

		// PRO_E_PRIMARY_QLTSIDE
		ProElement elemQltSide;
		status = ProElementAlloc(PRO_E_PRIMARY_QLTSIDE, &elemQltSide);
		ProValueData valdataQltSide;
		valdataQltSide.type = PRO_VALUE_TYPE_INT;
		valdataQltSide.v.i = qltside;
		ProValue valueQltSide = NULL;
		status = ProValueAlloc(&valueQltSide);
		status = ProValueDataSet(valueQltSide, &valdataQltSide);
		status = ProElementValueSet(elemQltSide, valueQltSide);
		status = ProElemtreeElementAdd(elemTree, NULL, elemQltSide);
	}

	ProFeature feat;
	if (CreateFeature(pMdl, elemTree, feat))
	{
		nRet = feat.id;
	}
	return nRet;
}

// 通过面组或基准面修剪面
int TrimSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selCutQlt, 
				/*input*/ProSelection selRefSrf, int nTrimDir)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selCutQlt->sel_type != SEL_3D_SRF_LIST || (selRefSrf->sel_type != SEL_3D_SRF_LIST && 
		selRefSrf->sel_type != SEL_3D_SRF))
		return ER_BAD_INPUT;

	// 设置裁剪方向
	ProTrimMatlSideOpts matside;
	ProTrimPrimaryQltSide qltside;
	if (nTrimDir == PRO_EDGE_SAME_DIRECTION)
	{
		matside = PRO_TRIM_MATL_SIDE_TWO;
		qltside = PRO_TRIM_PRIM_QLT_SIDE_TWO;
	}
	else
	{
		matside = PRO_TRIM_MATL_SIDE_ONE;
		qltside = PRO_TRIM_PRIM_QLT_SIDE_ONE;
	}

	ProElement elemRoot = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemRoot);
	{
		// 特征类型 PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_CUT;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		status = ProElementValueSet(elemType, valueType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemType);

		// PRO_E_FEATURE_FORM
		ProElement elemForm;
		status = ProElementAlloc(PRO_E_FEATURE_FORM, &elemForm);
		ProValueData valdataForm;
		valdataForm.type = PRO_VALUE_TYPE_INT;
		valdataForm.v.i = PRO_USE_SURFS;
		ProValue valueForm = NULL;
		status = ProValueAlloc(&valueForm);
		status = ProValueDataSet(valueForm, &valdataForm);
		status =ProElementValueSet(elemForm, valueForm);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemForm);

		// 修剪类型 PRO_E_SRF_TRIM_TYPE
		ProElement elemTrimType;
		status = ProElementAlloc(PRO_E_SRF_TRIM_TYPE, &elemTrimType);
		ProValueData valdataTrimType;
		valdataTrimType.type = PRO_VALUE_TYPE_INT;
		valdataTrimType.v.i = PRO_SURF_TRIM_USE_QLT;
		ProValue valueTrimType = NULL;
		status = ProValueAlloc(&valueTrimType);
		status = ProValueDataSet(valueTrimType, &valdataTrimType);
		status = ProElementValueSet(elemTrimType, valueTrimType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemTrimType);

		// 面类型 PRO_E_SURF_TRIM_TYPE
		ProElement elemSurfType;
		status = ProElementAlloc(PRO_E_SURF_TRIM_TYPE, &elemSurfType);
		ProValueData valdataSurfType;
		valdataSurfType.type = PRO_VALUE_TYPE_INT;
		valdataSurfType.v.i = PRO_SURF_TRIM_USE_QLT;
		ProValue valueSurfType = NULL;
		status = ProValueAlloc(&valueSurfType);
		status = ProValueDataSet(valueSurfType, &valdataSurfType);
		status = ProElementValueSet(elemSurfType, valueSurfType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemSurfType);

		// 参考面组 PRO_E_TRIM_QUILT
		ProElement elemTrimQuilt;
		status = ProElementAlloc(PRO_E_TRIM_QUILT, &elemTrimQuilt);
		ProValueData valdataTrimQuilt;
		valdataTrimQuilt.type = PRO_VALUE_TYPE_SELECTION;
		valdataTrimQuilt.v.r = selCutQlt;
		ProValue valueTrimQuilt = NULL;
		status = ProValueAlloc(&valueTrimQuilt);
		status = ProValueDataSet(valueTrimQuilt, &valdataTrimQuilt);
		status = ProElementValueSet(elemTrimQuilt, valueTrimQuilt);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemTrimQuilt);

		// 参考面 PRO_E_STD_USEQLT_QLT
		ProElement elemUseQuilt;
		status = ProElementAlloc(PRO_E_STD_USEQLT_QLT, &elemUseQuilt);
		ProValueData valdataUseQuilt;
		valdataUseQuilt.type = PRO_VALUE_TYPE_SELECTION;
		valdataUseQuilt.v.r = selRefSrf;
		ProValue valueUseQuilt = NULL;
		status = ProValueAlloc(&valueUseQuilt);
		status = ProValueDataSet(valueUseQuilt, &valdataUseQuilt);
		status = ProElementValueSet(elemUseQuilt, valueUseQuilt);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemUseQuilt);

		// PRO_E_MATERIAL_SIDE
		ProElement elemMatSide;
		status = ProElementAlloc(PRO_E_MATERIAL_SIDE, &elemMatSide);
		ProValueData valdataMatSide;
		valdataMatSide.type = PRO_VALUE_TYPE_INT;
		valdataMatSide.v.i = matside;
		ProValue valueMatSide = NULL;
		status = ProValueAlloc(&valueMatSide);
		status = ProValueDataSet(valueMatSide, &valdataMatSide);
		status = ProElementValueSet(elemMatSide, valueMatSide);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemMatSide);

		// PRO_E_PRIMARY_QLTSIDE
		ProElement elemQltSide;
		status = ProElementAlloc(PRO_E_PRIMARY_QLTSIDE, &elemQltSide);
		ProValueData valdataQltSide;
		valdataQltSide.type = PRO_VALUE_TYPE_INT;
		valdataQltSide.v.i = qltside;
		ProValue valueQltSide = NULL;
		status = ProValueAlloc(&valueQltSide);
		status = ProValueDataSet(valueQltSide, &valdataQltSide);
		status = ProElementValueSet(elemQltSide, valueQltSide);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemQltSide);

		// PRO_E_KEEP_TRIM_SURF_OPT
		ProElement elemTrimOpt;
		status = ProElementAlloc(PRO_E_KEEP_TRIM_SURF_OPT, &elemTrimOpt);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemTrimOpt);
		ProValueData valdataTrimOpt;
		valdataTrimOpt.type = PRO_VALUE_TYPE_INT;
		valdataTrimOpt.v.i = PRO_KEEP_TRIM_SURF_OPT_YES;
		ProValue valueTrimOpt = NULL;
		status = ProValueAlloc(&valueTrimOpt);
		status = ProValueDataSet(valueTrimOpt, &valdataTrimOpt);
		status = ProElementValueSet(elemTrimOpt, valueTrimOpt);
	}

	ProFeature feat;
	if (CreateFeature(pMdl, elemRoot, feat))
	{
		nRet = feat.id;
	}
	return nRet;
}

// 修改裁剪方向
int ReverseTrimDirection(ProFeature feat)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	ProFeattype feattype;
	status = ProFeatureTypeGet(&feat, &feattype);
	if (feattype != PRO_FEAT_CUT)
	{
		return ER_BAD_INPUT;
	}
	ProElement elemRoot = NULL;
	status = ProFeatureElemtreeExtract(&feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elemRoot);

	ProElempathItem pathItemMatSide[1];
	pathItemMatSide[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
	pathItemMatSide[0].path_item.elem_id = PRO_E_MATERIAL_SIDE;
	ProElempath elempathMatSide;
	status = ProElempathAlloc(&elempathMatSide);
	status = ProElempathDataSet(elempathMatSide, pathItemMatSide, 1);

	ProElement elemMatSide;
	status = ProElemtreeElementGet(elemRoot, elempathMatSide, &elemMatSide);
	if (status == PRO_TK_NO_ERROR)
	{
		ProValue valueMatSide;
		status = ProElementValueGet(elemMatSide, &valueMatSide);
		ProValueData valdataMatSide;
		status = ProValueDataGet(valueMatSide, &valdataMatSide);
		if (valdataMatSide.v.i == PRO_TRIM_MATL_SIDE_TWO)
			valdataMatSide.v.i = PRO_TRIM_MATL_SIDE_ONE;
		else
			valdataMatSide.v.i = PRO_TRIM_MATL_SIDE_TWO;

		status = ProValueDataSet(valueMatSide, &valdataMatSide);
		status = ProElementValueSet(elemMatSide, valueMatSide);

		// 重定义特征
		ProErrorlist errors;
		status = ProFeatureRedefine(NULL, &feat, elemRoot, NULL, 0, &errors);
		if (status ==  PRO_TK_NO_ERROR)
			nRet = 1;
	}
	status = ProElempathFree(&elempathMatSide);
	status = ProFeatureElemtreeFree(&feat, elemRoot);

	return nRet;
}

// 创建边界混合特征
int CreateBlendSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2,
					/*output*/ProModelitem& itemQlt)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selEdge1->sel_type != SEL_3D_EDG || selEdge2->sel_type != SEL_3D_EDG)
		return ER_BAD_INPUT;

	// 检查UDF文件
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\blendsrf.gph";
	if (IsFileExist(strUDFPath))
	{
		ProUdfdata udfdata;
		status = ProUdfdataAlloc(&udfdata);
		// 设置UDF路径
		ProPath pathUDF;
		wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
		status = ProUdfdataPathSet(udfdata, pathUDF);
		// 获取参照项
		ProUdfRequiredRef* parrRef = NULL;
		status = ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
		ProLine prompt[2];
		status = ProUdfrequiredrefPromptGet(parrRef[0], prompt[0]);
		status = ProUdfrequiredrefPromptGet(parrRef[1], prompt[1]);
		// 设置参照
		ProUdfreference refUDF[2];
		status = ProUdfreferenceAlloc(prompt[0], selEdge1, PRO_B_TRUE, &refUDF[0]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[0]);
		status = ProUdfreferenceAlloc(prompt[1], selEdge2, PRO_B_TRUE, &refUDF[1]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[1]);
		// 创建UDF特征
		ProGroup udf;
		status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
		if (status == PRO_TK_NO_ERROR)
		{
			ProFeature* pFeatArray = NULL;
			status = ProGroupFeaturesCollect(&udf, &pFeatArray);
			// 获取特征中的面
			vector<ProModelitem> arrSrfs;
			status = ProFeatureGeomitemVisit(&pFeatArray[1], PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfs);
			itemQlt.id = arrSrfs[0].id;
			itemQlt.owner = arrSrfs[0].owner;
			itemQlt.type = arrSrfs[0].type;
			nRet = pFeatArray[1].id;
			status = ProGroupUngroup(&udf);
			status = ProArrayFree((ProArray*)&pFeatArray);
		}
		status = ProUdfdataFree(udfdata);
		status = ProUdfreferenceFree(refUDF[0]);
		status = ProUdfreferenceFree(refUDF[1]);
	}
	else
		nRet = ER_INVALID_UDF;
	return nRet;
}

// 创建合并特征
int MergeSurfs(/*input*/ProMdl pMdl, /*input*/const vector<ProSelection> &arrSelQuilts, /*input*/BOOL bCrossed)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	ProSrfMrgType mrgType = PRO_SRF_MRG_INTSCT;
	int nCount = (int)arrSelQuilts.size();
	if (nCount < 2)
		return ER_BAD_INPUT;
	else if (nCount == 2 && bCrossed)
	{
		mrgType = PRO_SRF_MRG_JOIN;
	}

	ProElement elemRoot = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemRoot);
	{
		// 特征类型PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemType);
		ProValueData valdataType;
		valdataType.type = PRO_VALUE_TYPE_INT;
		valdataType.v.i = PRO_FEAT_DATUM_QUILT;
		ProValue valueType = NULL;
		status = ProValueAlloc(&valueType);
		status = ProValueDataSet(valueType, &valdataType);
		status = ProElementValueSet(elemType, valueType);

		// PRO_E_SRF_MRG_QUILT_ARR
		ProElement elemArr;
		status = ProElementAlloc(PRO_E_SRF_MRG_QUILT_ARR, &elemArr);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemArr);
		{
			for (int i=0; i<nCount; i++)
			{
				// PRO_E_SRF_MRG_QUILT_CMPD
				ProElement elemCmpd;
				status = ProElementAlloc(PRO_E_SRF_MRG_QUILT_CMPD, &elemCmpd);
				status = ProElemtreeElementAdd(elemArr, NULL, elemCmpd);
				{
					// PRO_E_SRF_MRG_QUILT_REF
					ProElement elemRef;
					status = ProElementAlloc(PRO_E_SRF_MRG_QUILT_REF, &elemRef);
					status = ProElemtreeElementAdd(elemCmpd, NULL, elemRef);
					ProValueData valdataRef;
					valdataRef.type = PRO_VALUE_TYPE_SELECTION;
					valdataRef.v.r = arrSelQuilts[i];
					ProValue valueRef = NULL;
					status = ProValueAlloc(&valueRef);
					status = ProValueDataSet(valueRef, &valdataRef);
					status = ProElementValueSet(elemRef, valueRef);
				}
			}
		}
		// PRO_E_SRF_MRG_TYPE
		ProElement elemMrgType;
		status = ProElementAlloc(PRO_E_SRF_MRG_TYPE, &elemMrgType);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemMrgType);
		ProValueData valdataMrgType;
		valdataMrgType.type = PRO_VALUE_TYPE_INT;
		valdataMrgType.v.i = mrgType;
		ProValue valueMrgType = NULL;
		status = ProValueAlloc(&valueMrgType);
		status = ProValueDataSet(valueMrgType, &valdataMrgType);
		status = ProElementValueSet(elemMrgType, valueMrgType);
	}
	ProFeature feat;
	if (CreateFeature(pMdl, elemRoot, feat))
	{
		nRet = feat.id;
	}
	return nRet;
}

// 偏移面
int OffsetSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selInputQlt->sel_type != SEL_3D_SRF_LIST)
		return ER_BAD_INPUT;

	// 检查UDF文件
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\offsetsrf.gph";
	if (IsFileExist(strUDFPath))
	{
		ProUdfdata udfdata;
		status = ProUdfdataAlloc(&udfdata);
		// 设置UDF路径
		ProPath pathUDF;
		wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
		status = ProUdfdataPathSet(udfdata, pathUDF);

		// 获取参照项
		ProUdfRequiredRef* parrRef = NULL;
		status = ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
		ProLine prompt;
		status = ProUdfrequiredrefPromptGet(parrRef[0], prompt);

		// 设置参照
		ProUdfreference refUDF;
		status = ProUdfreferenceAlloc(prompt, selInputQlt, PRO_B_TRUE, &refUDF);
		status = ProUdfdataReferenceAdd(udfdata, refUDF);

		// 设置偏移值
		ProUdfvardim vardim;
		status = ProUdfvardimAlloc(L"d0", dOffset, PROUDFVARTYPE_DIM, &vardim);
		status = ProUdfdataUdfvardimAdd(udfdata, vardim);

		// 创建UDF特征
		ProGroup udf;
		status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
		if (status == PRO_TK_NO_ERROR)
		{
			ProFeature* pFeatArray = NULL;
			status = ProGroupFeaturesCollect(&udf, &pFeatArray);
			// 获取特征中的面
			vector<ProModelitem> arrSrfs;
			status = ProFeatureGeomitemVisit(&pFeatArray[1], PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfs);
			itemQlt.id = arrSrfs[0].id;
			itemQlt.owner = arrSrfs[0].owner;
			itemQlt.type = arrSrfs[0].type;
			nRet = pFeatArray[1].id;
			status = ProGroupUngroup(&udf);
			status = ProArrayFree((ProArray*)&pFeatArray);
		}
		status = ProUdfdataFree(udfdata);
		status = ProUdfreferenceFree(refUDF);
	}
	else
		nRet = ER_INVALID_UDF;
	return nRet;
}

// 单面偏移
int OffsetSingleSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset)
{
	/*if (selQuilt->sel_type != SEL_3D_SRF_LIST)
	return -1;*/

	ProUdfdata udfdata;
	ProUdfdataAlloc(&udfdata);

	//// 获取面的环
	//ProModelitem itemSurfSeled;
	//ProSelectionModelitemGet(selSurf, &itemSurfSeled);
	//ProSurface surfSeled;
	//ProGeomitemToSurface(&itemSurfSeled, &surfSeled);
	//vector<ProContour> arrContours;
	//ProSurfaceContourVisit(surfSeled, SurfaceExtContoursGetAction, NULL, &arrContours);
	//vector<ProEdge> arrEdges;
	//ProContourEdgeVisit(surfSeled, arrContours[0], ContourEdgesGetAction, NULL, &arrEdges);
	//// 遍历环边，判断是否仅有一个相邻面
	//ProEdge edgeTemp1, edgeTemp2;
	//ProSurface surfTemp1, surfTemp2;
	//IntArray arrEdgesIDs;
	//for (int i=0; i<arrEdges.size(); i++)
	//{
	//	ProEdgeNeighborsGet(arrEdges[i], &edgeTemp1, &edgeTemp2, &surfTemp1, &surfTemp2);
	//	if (surfTemp1 == NULL || surfTemp2 == NULL)
	//	{
	//		arrEdgesIDs.Add(i);
	//	}
	//}

	ProError status;
	// 设置文件路径
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\offsetpack.gph";
	ProPath pathUDF;
	wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
	if (!IsFileExist(strUDFPath))
		return -3;
	ProUdfdataPathSet(udfdata, pathUDF);

	ProUdfRequiredRef* parrRef = NULL;
	ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
	ProLine prompt;
	ProUdfrequiredrefPromptGet(parrRef[0], prompt);

	// 设置参照
	ProUdfreference refUDF;
	status = ProUdfreferenceAlloc(prompt, selInputQlt, PRO_B_TRUE, &refUDF);
	status = ProUdfdataReferenceAdd(udfdata, refUDF);

	// 设置偏移值
	ProUdfvardim vardim;
	status = ProUdfvardimAlloc(L"d33", dOffset, PROUDFVARTYPE_DIM, &vardim);
	status = ProUdfdataUdfvardimAdd(udfdata, vardim);

	ProGroup udf;
	status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
	ProUdfdataFree(udfdata);
	ProUdfreferenceFree(refUDF);
	ProUdfvardimFree(vardim);
	if (status != PRO_TK_NO_ERROR)
	{
		return -2;
	}
	ProFeature* pFeatArray = NULL;
	ProGroupFeaturesCollect(&udf, &pFeatArray);
	int nID = pFeatArray[1].id;
	ProGroupUngroup(&udf);
	ProArrayFree((ProArray*)&pFeatArray);

	//ProFeature feat;
	//feat.id = nID;
	//feat.owner=pMdl;
	//feat.type=PRO_FEATURE;
	//vector<ProModelitem> arrQuilts;
	//ProFeatureGeomitemVisit(&feat, PRO_QUILT, FeatureGeomsGetAction, NULL, &arrQuilts);
	//vector<ProSurface> arrSurfaces;
	//ProSurface surfToHide;
	//for (int i=0; i<arrEdgesIDs.GetCount(); i++)
	//{
	//	surfTemp1 = NULL;
	//	surfTemp2 = NULL;
	//	ProEdgeNeighborsGet(arrEdges[arrEdgesIDs[i]], &edgeTemp1, &edgeTemp2, &surfTemp1, &surfTemp2);
	//	if (surfTemp1 == surfSeled && surfTemp2 != NULL)
	//		surfToHide = surfTemp2;
	//	else if (surfTemp2 == surfSeled && surfTemp1 != NULL)
	//		surfToHide = surfTemp1;
	//	else
	//		continue;

	//	BOOL bFind = FALSE;
	//	ProQuilt quilt;
	//	for (int j=0; j<arrQuilts.size(); j++)
	//	{
	//		ProGeomitemToQuilt(&arrQuilts[j], &quilt);
	//		arrSurfaces.clear();
	//		ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrSurfaces);
	//		
	//		for (int k=0; k<arrSurfaces.size(); k++)
	//		{
	//			if (surfToHide == arrSurfaces[k])
	//			{
	//				bFind = TRUE;
	//				break;
	//			}
	//		}
	//		if (bFind)
	//			break;
	//	}
	//	if (bFind)
	//	{
	//		ProAsmcomppath asmpath;
	//		ProSelectionAsmcomppathGet(selSurf, &asmpath);

	//		vector<ProContour> arrHideSurfContours;
	//		ProSurfaceContourVisit(surfToHide, SurfaceExtContoursGetAction, NULL, &arrHideSurfContours);
	//		vector<ProEdge> arrHideSurfEdges;
	//		ProContourEdgeVisit(surfToHide, arrHideSurfContours[0], ContourEdgesGetAction, NULL, &arrHideSurfEdges);
	//		vector<ProSelection> arrSelHideSurfEdges;
	//		int nEdgeDir;
	//		ProEdgeDirGet(arrHideSurfEdges[0], surfToHide, &nEdgeDir);
	//		BOOL bEdgeSameDir = TRUE;
	//		if (nEdgeDir == PRO_EDGE_SAME_DIRECTION)
	//		{
	//			bEdgeSameDir = FALSE;
	//		}
	//		for (int j=arrHideSurfEdges.size()-1; j>=0; j--)
	//		{
	//			ProModelitem itemEdge;
	//			ProEdgeToGeomitem(ProMdlToSolid(pMdl), arrHideSurfEdges[j], &itemEdge);
	//			ProSelection selEdge;
	//			ProSelectionAlloc(&asmpath, &itemEdge, &selEdge);
	//			arrSelHideSurfEdges.push_back(selEdge);
	//		}

	//		ProModelitem itemQuilt;
	//		ProQuiltToGeomitem(ProMdlToSolid(pMdl), quilt, &itemQuilt);
	//		ProSelection selQuilt;
	//		ProSelectionAlloc(&asmpath, &itemQuilt, &selQuilt);
	//		// 裁剪
	//		TrimSurface(pMdl, selQuilt, arrSelHideSurfEdges, bEdgeSameDir, bEdgeSameDir);
	//	}
	//	
	//}

	return nID;
}


// 创建投影特征（输入1个面和1个环）
int CreateProjectCurve(ProMdl pMdl, ProSelection selQuilt, ProSelection selEdge)
{
	if (selQuilt->sel_type != SEL_3D_SRF_LIST || selEdge->sel_type != SEL_3D_EDG)
		return -1;

	ProError status;

	ProUdfdata udfdata;
	ProUdfdataAlloc(&udfdata);

	// 设置文件路径
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\projectcurve.gph";
	ProPath pathUDF;
	wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
	if (!IsFileExist(strUDFPath))
		return -3;
	ProUdfdataPathSet(udfdata, pathUDF);

	// 设置参照
	ProUdfRequiredRef* parrRef = NULL;
	ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
	ProLine prompt1, prompt2;
	ProUdfrequiredrefPromptGet(parrRef[0], prompt1);
	ProUdfrequiredrefPromptGet(parrRef[1], prompt2);

	ProUdfreference refSurf, refEdge;
	status = ProUdfreferenceAlloc(prompt1, selQuilt, PRO_B_TRUE, &refSurf);
	status = ProUdfreferenceAlloc(prompt2, selEdge, PRO_B_TRUE, &refEdge);

	status = ProUdfdataReferenceAdd(udfdata, refSurf);
	status = ProUdfdataReferenceAdd(udfdata, refEdge);

	// 创建UDF特征
	ProGroup udf;
	status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
	ProUdfdataFree(udfdata);
	ProUdfreferenceFree(refSurf);
	ProUdfreferenceFree(refEdge);
	if (status != PRO_TK_NO_ERROR)
	{
		return -2;
	}
	ProFeature* pFeatArray = NULL;
	ProGroupFeaturesCollect(&udf, &pFeatArray);
	int nID = pFeatArray[1].id;
	ProGroupUngroup(&udf);
	ProArrayFree((ProArray*)&pFeatArray);
	return nID;
}

// 将一组曲面输出到stl文件中
BOOL ExportStl(const vector<ProGeomitem> &arrSurfs, const CString& strStlPath, 
			   double dAngleCntrol, double dChordHeight)
{
	BOOL bResult = TRUE;
	ProError status;

	int nSurfItems = (int)arrSurfs.size();
	if (nSurfItems < 1)
		return FALSE;

	CStdioFile stlFile(strStlPath, CFile::modeReadWrite|CFile::modeCreate);
	if (stlFile.m_hFile)
	{
		// 输出模型名
		ProName nameMdl;
		ProMdlNameGet(arrSurfs[0].owner, nameMdl);
		CString strName(nameMdl);
		CStringA strContent(strName);
		strContent = "solid " + strContent + "\n";
		stlFile.Write((LPCSTR)strContent, strContent.GetLength());

		// 设置输出的三角面片精度
		ProSurfaceTessellationInput input;
		ProSurfacetessellationinputAlloc(&input);
		ProSurfacetessellationinputAnglecontrolSet(input, dAngleCntrol);
		ProSurfacetessellationinputChordheightSet(input, dChordHeight);

		ProGeomitem geoSurf;
		for (int i=0; i<nSurfItems; i++)
		{
			geoSurf.id = arrSurfs[i].id;
			geoSurf.owner = arrSurfs[i].owner;
			geoSurf.type = arrSurfs[i].type;

			ProSurface pSruf = NULL;
			status = ProGeomitemToSurface(&geoSurf, &pSruf);
			if (status == PRO_TK_NO_ERROR)
			{
				// 获取三角面数据
				ProTessellation pTess;
				status = ProSurfaceTessellationGet(pSruf, input, &pTess);
				if (status == PRO_TK_NO_ERROR)
				{
					// 获取定点数据
					Pro3dPnt* pArrayVertices = NULL;
					ProTessellationVerticesGet(pTess, &pArrayVertices);

					ProVector* pArrayNormals = NULL;
					ProTessellationNormalsGet(pTess, &pArrayNormals);

					ProTriangle* pArrayFacets = NULL;
					ProTessellationFacetsGet(pTess, &pArrayFacets);

					// 按格式输出三角面片数据
					int nFacetsCount;
					ProArraySizeGet(pArrayFacets, &nFacetsCount);
					for (int j=0; j<nFacetsCount; j++)
					{
						strContent.Format("  facet normal %e %e %e\n", pArrayNormals[j][0], pArrayNormals[j][1], pArrayNormals[j][2]);
						stlFile.Write((LPCSTR)strContent, strContent.GetLength());
						strContent = "    outer loop\n";
						stlFile.Write((LPCSTR)strContent, strContent.GetLength());
						for (int k=0; k<3; k++)
						{
							strContent.Format("      vertex %e %e %e\n", pArrayVertices[pArrayFacets[j][k]][0], pArrayVertices[pArrayFacets[j][k]][1], pArrayVertices[pArrayFacets[j][k]][2]);
							stlFile.Write((LPCSTR)strContent, strContent.GetLength());
						}
						strContent = "    endloop\n  endfacet\n";
						stlFile.Write((LPCSTR)strContent, strContent.GetLength());
					}
					ProArrayFree((ProArray*)&pArrayVertices);
					ProArrayFree((ProArray*)&pArrayNormals);
				}
				else
					bResult = FALSE;

				// 释放
				ProTessellationFree(pTess);
			}
		}
		ProSurfacetessellationinputFree(input);
		// 结尾
		strContent = strName;
		strContent = "endsolid " + strContent + "\n";
		stlFile.Write((LPCSTR)strContent, strContent.GetLength());
		stlFile.Close();
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}

// 移除面（创建移除特征）
int RemoveQuiltSrf(/*input*/ProMdl pMdl, /*input*/ProSelection selQltSrf)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selQltSrf->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 设置裁剪方向
	ProElement elemRoot = NULL;
	status = ProElementAlloc(PRO_E_FEATURE_TREE, &elemRoot);
	{
		// 特征类型 PRO_E_FEATURE_TYPE
		ProElement elemType;
		status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
		status = ProElementIntegerSet(elemType, PRO_FEAT_RM_SURF);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemType);

		// PRO_E_RM_SURF_REF_TYPE
		ProElement elemRefType;
		status = ProElementAlloc(PRO_E_RM_SURF_REF_TYPE, &elemRefType);
		status = ProElementIntegerSet(elemRefType, PRO_RM_SURF_SRF_REF);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemRefType);

		// PRO_E_RM_SURF_ATTACH_TYPE
		ProElement elemAttachType;
		status = ProElementAlloc(PRO_E_RM_SURF_ATTACH_TYPE, &elemAttachType);
		status = ProElementIntegerSet(elemAttachType, FM_RM_SURF_ATTACH_SAME);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemAttachType);

		// PRO_E_RM_SURF_SRF_REFS
		ProElement elemSrfRefs;
		status = ProElementAlloc(PRO_E_RM_SURF_SRF_REFS, &elemSrfRefs);
		status = ProElemtreeElementAdd(elemRoot, NULL, elemSrfRefs);
		{
			// PRO_E_STD_SURF_COLLECTION_APPL
			ProElement elemSrfCollect;
			status = ProElementAlloc(PRO_E_STD_SURF_COLLECTION_APPL, &elemSrfCollect);
			ProCollection collect;
			status = ProSrfcollectionAlloc(&collect);
			ProSrfcollinstr instrSrf;
			status = ProSrfcollinstrAlloc(PRO_SURFCOLL_SINGLE_SURF, PRO_B_TRUE, &instrSrf);
			status = ProSrfcollinstrIncludeSet (instrSrf, PRO_B_TRUE);
			ProReference refSrf;
			status = ProSelectionToReference(selQltSrf, &refSrf);
			ProSrfcollref instrRefSrf;
			status = ProSrfcollrefAlloc(PRO_SURFCOLL_REF_SINGLE, refSrf, &instrRefSrf);
			status = ProSrfcollinstrReferenceAdd(instrSrf, instrRefSrf);
			status = ProSrfcollectionInstructionAdd(collect, instrSrf);
			status = ProElementCollectionSet(elemSrfCollect, collect);
			status = ProElemtreeElementAdd(elemSrfRefs, NULL, elemSrfCollect);

			// PRO_E_STD_EXCL_CNTRS
			ProElement elemExclCntrs;
			status = ProElementAlloc(PRO_E_STD_EXCL_CNTRS, &elemExclCntrs);
			status = ProElemtreeElementAdd(elemSrfRefs, NULL, elemExclCntrs); 

			//status = ProCollectionFree(&collect);
			//status = ProSrfcollinstrFree(instSrf);
			//status = ProSrfcollrefFree(collrefSrf);
		}
	}

	ProFeature feat;
	if (CreateFeature(pMdl, elemRoot, feat))
	{
		nRet = feat.id;
	}
	status = ProElementFree(&elemRoot);
	return nRet;
}

// 延展面
int ExtendSrfByEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge, /*input*/double dExtendDist)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selEdge->sel_type != SEL_3D_EDG)
		return ER_BAD_INPUT;

	// 获取边的顶点
	ProModelitem itemEdge;
	status = ProSelectionModelitemGet(selEdge, &itemEdge);
	ProModelitem itemVertex;
	itemVertex.id = itemEdge.id;
	itemVertex.type = PRO_EDGE_END;
	itemVertex.owner = itemEdge.owner;
	ProSelection selVertex;
	status = ProSelectionAlloc(NULL, &itemVertex, &selVertex);

	// 检查UDF文件
	CString strUDFPath = theApp.m_strAppPath + L"Resource\\extendbyedge.gph";
	if (IsFileExist(strUDFPath))
	{
		ProUdfdata udfdata;
		status = ProUdfdataAlloc(&udfdata);
		// 设置UDF路径
		ProPath pathUDF;
		wcsncpy_s(pathUDF, PRO_PATH_SIZE, strUDFPath, _TRUNCATE);
		status = ProUdfdataPathSet(udfdata, pathUDF);

		// 获取参照项
		ProUdfRequiredRef* parrRef = NULL;
		status = ProUdfdataRequiredreferencesGet(udfdata, &parrRef);
		ProLine prompt[2];
		status = ProUdfrequiredrefPromptGet(parrRef[0], prompt[0]);
		status = ProUdfrequiredrefPromptGet(parrRef[1], prompt[1]);

		// 设置参照
		ProUdfreference refUDF[2];
		status = ProUdfreferenceAlloc(prompt[0], selEdge, PRO_B_TRUE, &refUDF[0]);
		status = ProUdfreferenceAlloc(prompt[1], selVertex, PRO_B_TRUE, &refUDF[1]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[0]);
		status = ProUdfdataReferenceAdd(udfdata, refUDF[1]);

		// 设置偏移值
		ProUdfvardim vardim;
		status = ProUdfvardimAlloc(L"d23", dExtendDist, PROUDFVARTYPE_DIM, &vardim);
		status = ProUdfdataUdfvardimAdd(udfdata, vardim);

		// 创建UDF特征
		ProGroup udf;
		status = ProUdfCreate(ProMdlToSolid(pMdl), udfdata, NULL, NULL, 0, &udf);
		if (status == PRO_TK_NO_ERROR)
		{
			ProFeature* pFeatArray = NULL;
			status = ProGroupFeaturesCollect(&udf, &pFeatArray);
			// 获取特征中的面
			/*vector<ProModelitem> arrSrfs;
			status = ProFeatureGeomitemVisit(&pFeatArray[1], PRO_QUILT, FeatureGeomsGetAction, NULL, &arrSrfs);
			itemQlt.id = arrSrfs[0].id;
			itemQlt.owner = arrSrfs[0].owner;
			itemQlt.type = arrSrfs[0].type;*/
			nRet = pFeatArray[1].id;
			status = ProGroupUngroup(&udf);
			status = ProArrayFree((ProArray*)&pFeatArray);
		}
		status = ProUdfdataFree(udfdata);
		status = ProUdfreferenceFree(refUDF[0]);
		status = ProUdfreferenceFree(refUDF[1]);
	}
	else
		nRet = ER_INVALID_UDF;
	return nRet;
}

// 延展到面
int ExtendQuiltToSrf(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/ProSelection selSrf)
{
	int nRet = ER_CREATE_FAIL;
	ProError status;

	// 检查输入
	if (selQuilt->sel_type != SEL_3D_SRF_LIST || selSrf->sel_type != SEL_3D_SRF)
		return ER_BAD_INPUT;

	// 解析底面（应该为平面）
	ProModelitem itemBottomSrf;
	status = ProSelectionModelitemGet(selSrf, &itemBottomSrf);
	ProGeomitemdata* pdataBottomSrf = NULL;
	status = ProGeomitemdataGet(&itemBottomSrf, &pdataBottomSrf);
	if (pdataBottomSrf->data.p_surface_data->type != PRO_SRF_PLANE)
	{
		status = ProGeomitemdataFree(&pdataBottomSrf);
		return ER_BAD_INPUT;
	}

	// 获取面组中与底面平行且距离最近的面（需要先移除该面）
	ProModelitem itemQuilt;
	status = ProSelectionModelitemGet(selQuilt, &itemQuilt);
	ProQuilt quilt;
	status = ProGeomitemToQuilt(&itemQuilt, &quilt);
	vector<ProSurface> arrQltSrfs;
	status = ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrQltSrfs);
	double dDistance = 0.0;
	vector<ProSelection> arrSelQltSrfs;
	int nSrfIndex = -1;
	for (int i=0; i<(int)arrQltSrfs.size(); i++)
	{
		double dDistanceTemp;
		ProModelitem itemQltSrfTemp;
		status = ProSurfaceToGeomitem(ProMdlToSolid(pMdl), arrQltSrfs[i], &itemQltSrfTemp);
		ProSelection selQltSrfTemp;
		status = ProSelectionAlloc(NULL, &itemQltSrfTemp, &selQltSrfTemp);
		arrSelQltSrfs.push_back(selQltSrfTemp);
		status = ProGeomitemDistanceEval(selQltSrfTemp, selSrf, &dDistanceTemp);
		/*ProGeomitemdata* dataQltSrf = NULL;
		status =ProSurfaceDataGet(arrQltSrfs[i], &dataQltSrf);*/
		if (status == PRO_TK_NO_ERROR && dDistanceTemp != 0.0)
		{
			if (dDistance == 0.0 || dDistanceTemp < dDistance)
			{
				dDistance = dDistanceTemp;
				nSrfIndex = i;
			}
		}
	}

	if (nSrfIndex >= 0)
	{
		// 获取需要移除的面的全部边
		vector<ProContour> arrTrimContour;
		status = ProSurfaceContourVisit(arrQltSrfs[nSrfIndex], SurfaceExtContoursGetAction, NULL, &arrTrimContour);
		vector<ProEdge> arrTrimEdges;
		status = ProContourEdgeVisit(arrQltSrfs[nSrfIndex], arrTrimContour[0], ContourEdgesGetAction, NULL, &arrTrimEdges);
		vector<ProSelection> arrSelTrimEdges;
		for (int i=0; i<(int)arrTrimEdges.size(); i++)
		{
			ProModelitem itemTrimEdge;
			status = ProEdgeToGeomitem(ProMdlToSolid(pMdl), arrTrimEdges[i], &itemTrimEdge);
			ProSelection selTrimEdge;
			status = ProSelectionAlloc(NULL, &itemTrimEdge, &selTrimEdge);
			arrSelTrimEdges.push_back(selTrimEdge);
		}
		// 根据边判断面的方向
		int nTrimEdgeDir;
		status = ProEdgeDirGet(arrTrimEdges[0], arrQltSrfs[nSrfIndex], &nTrimEdgeDir);
		nTrimEdgeDir = nTrimEdgeDir*(-1);

		int nExtendCount = 0;	// 记录需要偏移的面的个数

		// 移除该面
		int nTrimID = TrimSurface(pMdl, selQuilt, arrSelTrimEdges, nTrimEdgeDir);
		if (nTrimID > 0)
		{
			// 相关特征id数据（用于最后创建组）
			int* pArrFeatIDs = NULL;
			status = ProArrayAlloc(0, sizeof(int), 1, (ProArray*)&pArrFeatIDs);
			status = ProArrayObjectAdd((ProArray*)&pArrFeatIDs, 0, 1, &nTrimID);

			// 获取需要延展的面的边界及对应的面
			ProFeature featTrim;
			featTrim.id = nTrimID;
			featTrim.owner = pMdl;
			featTrim.type = PRO_FEATURE;
			vector<ProModelitem>arrItemExtendEdges;
			status = ProFeatureGeomitemVisit(&featTrim, PRO_EDGE, FeatureGeomsGetAction, NULL, &arrItemExtendEdges);
			for (int i=0; i<(int)arrItemExtendEdges.size(); i++)
			{
				ProEdge edgeExtend;
				status = ProGeomitemToEdge(&arrItemExtendEdges[i], &edgeExtend);
				ProEdge edgeNeighbors[2];
				ProSurface srfNeighbor[2];
				status = ProEdgeNeighborsGet(edgeExtend, &edgeNeighbors[0], &edgeNeighbors[1], &srfNeighbor[0], &srfNeighbor[1]);
				int nNeighbor = 0;
				if (srfNeighbor[0] == NULL && srfNeighbor[1] != NULL)
				{
					nNeighbor = 1;
				}
				else if (srfNeighbor[1] == NULL && srfNeighbor[0] != NULL)
				{
					nNeighbor = 0;
				}
				else
					continue;

				// 计算面的实际延展距离
				double dRealDis = dDistance;
				ProGeomitemdata* pdataNeighborSrf = NULL;
				status =ProSurfaceDataGet(srfNeighbor[nNeighbor], &pdataNeighborSrf);
				int nTypeNeighborSrf = pdataNeighborSrf->data.p_surface_data->type;
				if (nTypeNeighborSrf == PRO_SRF_PLANE)
				{
					// 如果是平面，获取面的法矢
					ProVector vector1, vector2;
					VectorCopy(pdataNeighborSrf->data.p_surface_data->srf_shape.plane.e3, vector1);
					VectorCopy(pdataBottomSrf->data.p_surface_data->srf_shape.plane.e3, vector2);
					double dCosAngle = GetVectorCosAngle(vector1, vector2);
					if (dCosAngle != 1)
					{
						dRealDis = dDistance/sqrt(1-dCosAngle*dCosAngle);
					}
				}
				else if (nTypeNeighborSrf == PRO_SRF_CYL)
				{
					// 如果是圆柱面，获取面的法矢
					ProVector vector1, vector2;
					VectorCopy(pdataNeighborSrf->data.p_surface_data->srf_shape.cylinder.e1, vector1);
					VectorCopy(pdataBottomSrf->data.p_surface_data->srf_shape.plane.e3, vector2);
					double dCosAngle = GetVectorCosAngle(vector1, vector2);
					if (dCosAngle != 1)
					{
						dRealDis = dDistance/sqrt(1-dCosAngle*dCosAngle);
					}
				}
				else if (nTypeNeighborSrf == PRO_SRF_CONE)
				{
					// 如果是锥面
					ProVector vector1, vector2;
					VectorCopy(pdataNeighborSrf->data.p_surface_data->srf_shape.cone.e1, vector1);
					VectorCopy(pdataBottomSrf->data.p_surface_data->srf_shape.plane.e3, vector2);
					double dAngle = CalArcCos(GetVectorCosAngle(vector1, vector2));
					dAngle -= pdataNeighborSrf->data.p_surface_data->srf_shape.cone.alpha;
					if (dAngle != 0)
					{
						dRealDis = dDistance/sin(dAngle);
					}
				}
				ProGeomitemdataFree(&pdataNeighborSrf);

				/*ProModelitem itemEdgeNeighbor;
				status =ProEdgeToGeomitem(ProMdlToSolid(pMdl), edgeNeighbors[nNeighbor], &itemEdgeNeighbor);
				ProSelection selEdgeNeighbor;
				status = ProSelectionAlloc(NULL, &itemEdgeNeighbor, &selEdgeNeighbor);

				status = ProGeomitemAngleEval(selEdgeNeighbor, selSrf, &dAngel);
				if (dAngel != 0.0)
				dDistance = dDistance/sin(dAngel);*/
				nExtendCount ++;

				ProSelection selExtendEdge;
				status = ProSelectionAlloc(NULL, &arrItemExtendEdges[i], &selExtendEdge);
				int nExtendID = ExtendSrfByEdge(pMdl, selExtendEdge, dRealDis);
				if (nExtendID > 0)
				{
					status = ProArrayObjectAdd((ProArray*)&pArrFeatIDs, 0, 1, &nExtendID);
				}
				else
					break;
			}

			// 创建组
			int nFeatsCount;
			status = ProArraySizeGet(pArrFeatIDs, &nFeatsCount);
			if (nFeatsCount == (1+nExtendCount))
			{
				ProGroup group;
				status = ProLocalGroupCreate(ProMdlToSolid(pMdl), pArrFeatIDs, nFeatsCount, L"Extend", &group);
				nRet = group.id;
			}
			else if (nFeatsCount == 1)
			{
				nRet = pArrFeatIDs[0];
			}

			status = ProArrayFree((ProArray*)&pArrFeatIDs);
		}
	}
	status = ProGeomitemdataFree(&pdataBottomSrf);
	return nRet;
}

// 输出特征的xml
void ExportElemTreeXML()
{
	ProError status;

	ProSelection *pSelOpt = NULL;
	int nSelCount = 0;
	status = ProSelect("feature", 1, NULL, NULL, NULL, NULL, &pSelOpt, &nSelCount);
	if (status == PRO_TK_NO_ERROR && nSelCount > 0)
	{
		ProFeature feat;
		status = ProSelectionModelitemGet(pSelOpt[0], &feat);
		ProElement elemTree;
		status = ProFeatureElemtreeExtract(&feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree);
		if (status == PRO_TK_NO_ERROR)
		{
			ProName nameFeat;
			status = ProModelitemNameGet(&feat, nameFeat);
			CString strXml = nameFeat;
			strXml.Format(L"D:\\%s.xml", strXml);
			ProPath pathXML;
			wcsncpy_s(pathXML, PRO_PATH_SIZE, strXml, _TRUNCATE);
			status = ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, pathXML);
		}
		else
		{
			MessageBox(NULL, L"无法获取该特征的特征树！", L"Error", MB_OK);
		}
	}
}

//===================================================================================================


