//===================================================================================================
// Summary:
//		��������ʵ�ֺ���
// Date:
//		2017-04-09
// Author:
//		
//===================================================================================================

#ifndef __PreGlobal_h__
#define __PreGlobal_h__

//===================================================================================================

// ����
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

// �˵�
#include <ProMenu.h>
#include <ProMenuBar.h>
#include <ProPopupmenu.h>

// ѡ��
#include <ProSrfcollection.h>
#include <ProCrvcollection.h>
#include <ProSelection.h>
#include <ProSelbuffer.h>

// ���Ρ���׼
#include <ProPoint.h>
#include <ProCurve.h>
#include <ProEdge.h>
#include <ProSurface.h>
#include <ProQuilt.h>
#include <ProAxis.h>
#include <ProCsys.h>

#include <ProIntfimport.h >

// ���
#include <ProColor.h>
#include <prodev_dgm.h>

// ����
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
// ��ͨ�û���������

// ���ǰ����
ProError ActivateCurrentPreWindow();

// ��õ�ǰģ��  
ProMdl GetCurrentMdl();

// ˢ�µ�ǰ����
void InvalidateDrawing();

// ִ��ӳ���
void ExecuteMapKey(const CString &strMacro, ProModelitem *pMdlItem = NULL);

// ִ��ӳ���
void ExecuteMapKey(const CString &strMacro, ProSelection pSel);

// ����ӳ������ں���������ִ�У�
void LoadMapKey(const CString &strMacro, ProModelitem *pMdlItem = NULL);

// ����ӳ������ں���������ִ�У�
void LoadMapKey(const CString &strMacro, ProSelection pSel);

// ��ʾ��ʾ��Ϣ
void ShowMessageTip(const CString& strPrompt);

// ��ȡѡ����������
ProType GetSelectionType(ProSelection pSel);

// ѡ�����
BOOL SelectObject(/*output*/vector<ProSelection> &arrSelObj, 
				  /*input*/char *pSelOpt, /*input*/int nCount = 1, /*input*/ProSelection* pArrSelInit = NULL);

// ����STEPģ��
BOOL LoadStep(const CString& strFilePath, ProMdl& pMdl);

// ���ö������ɫ
void SetSurfColor(/*input*/const ProModelitem& item, 
				  /*input*/double dRed, /*input*/double dGreen, /*input*/double dBlue);

// �����������ζ���֮��ľ���
BOOL MeasureDistance(ProGeomitem& item1, ProGeomitem& item2,
					 double& dDistance, Pro3dPnt& pnt1, Pro3dPnt& pnt2);

// ���������xml
void ExportElemTreeXML();

//===================================================================================================
// ����ѧ������

// �ж�����ƽ��
BOOL CheckVectorParallel(const ProVector pc1, const ProVector pv2);

// ��ȡ�����нǵ�����ֵ
double GetVectorCosAngle(const ProVector v1, const ProVector v2);

// ��������
void VectorCopy(const ProPoint3d from, ProPoint3d to);

// ��ȡ����������Sin��Cos��ֵ
double GetCorrectSinCosValue(double dCosValue);

// ���㷴����ֵ
double CalArcCos(double dValue);

// ���㷴����ֵ
double CalArcSin(double dValue);

//===================================================================================================
// �����ݻ�ȡ�͹��˺�����

// ��ȡ�����е����м�����
ProError FeatureGeomsGetAction(ProGeomitem* pDatum, ProError status, ProAppData pData);

// ��ȡ�����е�������
ProError QuiltSurfacesGetAction(ProSurface surface, ProError status, ProAppData pData);

// ��ȡ����ڻ�
ProError SurfaceInnerContoursGetAction(ProContour contour, ProError status, ProAppData pData);

// ��ȡ����⻷
ProError SurfaceExtContoursGetAction(ProContour contour, ProError status, ProAppData pData);

// ��ȡ�������б�
ProError ContourEdgesGetAction(ProEdge Edge, ProError status, ProAppData data);

// ��ȡģ�͵�һ����
ProError OneSurfaceGetAction(ProSurface surface, ProError status, ProAppData pData);

// ��ȡģ�͵���������
ProError SolidQuiltsGetAction(ProQuilt quilt, ProError status, ProAppData pData);

// ��ȡ�����е����гߴ�
ProError FeatureDimensionGetAction(ProDimension* dimension, ProError status, ProAppData pData);

// ��ȡ�������ߵ�����
ProError CurveCompGetAction(ProCurve pCurve, ProCurve pComp, int Index, ProBoolean flip, ProError status, ProAppData data);

//===================================================================================================
// �����幦�ܺ�����

// ��������
BOOL CreateFeature(/*input*/ProMdl pMdl, /*input*/ProElement elemRoot, /*output*/ProFeature& feat,
				   ProFeatureCreateOptions optCreate = PRO_FEAT_CR_NO_OPTS);

// ����������
int CreateFeatGroup(ProMdl pMdl, vector<int>& arrFeatID, const CString& strGroupName);

// ������׼ƽ��
int CreateDatumPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selSrf, /*output*/ProModelitem& itemPlane);

// ����ʵ�廯����
int CreateSolidify(/*input*/ProMdl pMdl, /*input*/ProSelection selQuiltOrPlane, /*input*/int nSide = PRO_SOLIDIFY_SIDE_ONE);

// ������
int CreateSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemQlt);

// ͨ�����Ʒ�ʽ������
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*output*/ProModelitem& itemSrf);

// ͨ�����Ʒ�ʽ�����棨�ų�1���ڻ���
int CreateSurfaceByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/ProSelection selInputEdge, 
						/*output*/ProModelitem& itemSrf);

// ͨ�����ƽ�淽ʽ��������
int CreateSurfByFillPlane(/*input*/ProMdl pMdl, /*input*/ProSelection selPlane, /*output*/ProModelitem& itemQlt);

// ͨ�����ݻ淽ʽ������
int CreateSurfByFillSketch(/*input*/ProMdl pMdl, /*input*/ProSelection selSketchFeat, /*output*/ProModelitem& itemQlt);

// ͨ�������ߴ����棨�߽��ϣ�
int CreateSurfByTwoEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2,
					/*output*/ProModelitem& itemQlt);

// ͨ���������ߴ����棨�߽��ϣ�
int CreateSurfByTwoCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selCurve1, /*input*/ProSelection selCurve2,
						   /*output*/ProModelitem& itemQlt);

// ͨ��һ����޼���
int TrimQuiltByEdgeGroup(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
						   /*input*/vector<ProSelection> arrSelEdges, /*input*/int nTrimDir);

// ͨ�������޼���
int TrimQuiltByCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
					   /*input*/ProSelection selCurve, /*input*/int nTrimDir);

// ͨ�������޼��棨��������չ��������������ཻ��
BOOL TrimQuiltByExtendCurveToContour(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
									 /*input*/ProSelection selCurve, /*input*/ProSelection selContourCurve, 
									 /*input*/int nTrimDir,
									 ProFeature& feat);

// ͨ�������޼��棨��������չ��������������ཻ��
BOOL TrimQuiltByExtendCurve(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, 
							/*input*/ProSelection selCurve,  /*input*/ProSelection selContourCurve, 
							/*input*/int nTrimDir,
							ProFeature& feat);

// ͨ��������׼���޼���
int TrimSurfaceBySurf(/*input*/ProMdl pMdl, /*input*/ProSelection selCutQlt, 
					  /*input*/ProSelection selRefSrf, int nTrimDir);

// �޸Ĳü�����
int ReverseTrimDirection(/*input*/ProFeature feat);

// �����ϲ�����
int MergeQuilts(/*input*/ProMdl pMdl, /*input*/const vector<ProSelection> &arrSelQuilts, 
			   /*input*/ProSrfMrgType mrgType, ProFeature& feat);

// �޸ĺϲ�����
BOOL ChangeMergeDir(ProFeature featMerge, int nIndex);

// ƫ������
int OffsetQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// ƫ�����飨���ɲ��棩
int OffsetSurfaceWithSide(/*input*/ProMdl pMdl, /*input*/ProSelection selInputQlt, /*input*/double dOffset);

// ƫ������
int OffsetSurface(/*input*/ProMdl pMdl, /*input*/ProSelection selInputSrf, /*input*/double dOffset,
			   /*output*/ProModelitem& itemQlt);

// ͨ��ͶӰ��ʽ��������
int CreateCurveByProject(ProMdl pMdl, ProSelection selQuilt, ProSelection selEdge);

// ���浼����stl�ļ�
BOOL ExportStl(const vector<ProGeomitem> &arrSurfs, const CString& strStlPath, 
			   double dAngleCntrol, double dChordHeight);

// �Ƴ������е���
int RemoveSurfInQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selQltSrf);

// ��չ���һ����
int ExtendQuiltByEdge(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge, /*input*/double dExtendDist);

// ��չ�������������
int ExtendQuiltByContour(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/double dExtendDist);

// ��չ�������ο���
int ExtendQuiltToSurf(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/ProSelection selSrf);

// ͨ����ʼ�ߺ���ֹ�ߴ�������
int CreateFromToCurveByCopy(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, 
							/*output*/ProModelitem& itemCurve);

// ͨ����ʼ�ߺ���ֹ�ߴ���Ŀ����
int CreateFromToChain(/*input*/ProMdl pMdl, /*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2);

// �Ӻ���
int ThickQuilt(/*input*/ProMdl pMdl, /*input*/ProSelection selQuilt, /*input*/double dThick);

// ������������
int CreatePublishGeom(ProMdl pMdl, ProSelection selQuilt);

// �������Ƽ���
int CreateCopyGeom(ProMdl pMdl, ProSelection selRefMdl, ProSelection selPubFeat, ProModelitem& itemQuilt);

// ͨ�������ཻ��������
int CreateCurveByIntersect(ProMdl pMdl, ProSelection selQuilt1, ProSelection selQuilt2, ProModelitem& itemCurve);

// ͨ����������⻷��������
int CreateCurveByCopyContour(ProMdl pMdl, ProSelection selQuilt, ProModelitem& itemCompCurve);

// ͨ����չʹ�������ཻ
BOOL ExtendToInterSect(ProMdl pMdl, ProModelitem& itemQuilt1, ProModelitem& itemQuilt2, int& nFeatID1, int& nFeatID2);

//===================================================================================================
// ������������

// ��ȡ�����ڵĻ����ڻ���
BOOL GetContourByEdge(/*input*/ProSelection selEdge, /*output*/vector<ProSelection> &arrSelEdge, /*output*/int& nDir);

// ͨ������ĳ���ߵ�����
BOOL GetNeighborSurfByEdge(ProSurface pSurface, ProEdge pPublicEdge,
						   ProSurface &pNeighborSurf);

// ������������Ƿ�����
BOOL CheckTwoQuiltNeighbor(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2);

// ������������Ƿ��ཻ
BOOL CheckTwoQuiltInstersect(ProMdl pMdl, ProQuilt quilt1, ProQuilt quilt2);

// �ж��������Ƿ���
BOOL IsEdgeInSameSurf(/*input*/ProSelection selEdge1, /*input*/ProSelection selEdge2, /*output*/ProSelection& selSurf);

// ��ȡָ�����������е�������
void GetNeighborSurfInQuilt(ProMdl pMdl, ProSurface surf, ProQuilt quilt, vector<int>&arrNeighborID);

// �������������ת��Ϊ����
BOOL QuiltToSurf(ProModelitem itemQuilt, ProModelitem& itemSurf);

// ��ȡ�����ϵ�һ����
void GetPointOnSurface(ProMdl pMdl, ProSurface surf, Pro3dPnt& pntOnSurf);

// �жϵ��Ƿ���������
BOOL IsPntInsideSurf(ProMdl pMdl, ProSurface surf, Pro3dPnt pntOnSurf);

// ��ȡģ�͵����ߴ磨����ĶԽ��߳��ȣ�
double GetMaxSize(ProMdl pMdl);

// ��ȡ�����Ĭ��ƫ�뷽��
int GetDefaultOffsetDirection(ProMdl pMdl, ProSurface surf);

// ��ȡ�����Ĭ��ƫ�뷽��
int GetDefaultOffsetDirection(ProQuilt quilt);

// ��ȡ��������
double GetQuiltArea(ProQuilt);

#endif
