/*******************************************************************************
* Copyright(c) ArcSoft, All right reserved.
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
* and confidential information.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
* AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy,
* distribute, modify, or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout
* thereof.
*********************************************************************************/

#ifndef _ARCSOFT_SDK_ASF_H_
#define _ARCSOFT_SDK_ASF_H_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASF_NONE				0x00000000	//������
#define ASF_FACE_DETECT			0x00000001	//�˴�detect������tracking����detection��������֮һ�������ѡ����detect mode ȷ��
#define ASF_FACERECOGNITION		0x00000004	//��������
#define ASF_AGE					0x00000008	//����
#define ASF_GENDER				0x00000010	//�Ա�
#define ASF_FACE3DANGLE			0x00000020	//3D�Ƕ�
#define ASF_LIVENESS			0x00000080	//RGB����
#define ASF_IR_LIVENESS			0x00000400	//IR����



	//���ģʽ
	enum ASF_DetectMode{
		ASF_DETECT_MODE_VIDEO = 0x00000000,		//Videoģʽ��һ�����ڶ�֡�������
		ASF_DETECT_MODE_IMAGE = 0xFFFFFFFF		//Imageģʽ��һ�����ھ�̬ͼ�ĵ��μ��
	};


	//���ʱ�������Ƕȵ����ȼ������ĵ��г�ʼ���ӿ�����ͼʾ˵������ο�
	enum ASF_OrientPriority {
		ASF_OP_0_ONLY = 0x1,		// ����Ԥ����������
		ASF_OP_90_ONLY = 0x2,		// ����0����ʱ����ת90��ķ���
		ASF_OP_270_ONLY = 0x3,		// ����0����ʱ����ת270��ķ���
		ASF_OP_180_ONLY = 0x4,		// ����0����ת180��ķ�����ʱ�롢˳ʱ��Ч��һ����
		ASF_OP_ALL_OUT = 0x5		// ȫ�Ƕ�
	};

	//orientation �Ƕȣ���ʱ�뷽��
	enum ASF_OrientCode {
		ASF_OC_0 = 0x1,			// 0 degree 
		ASF_OC_90 = 0x2,		// 90 degree 
		ASF_OC_270 = 0x3,		// 270 degree 
		ASF_OC_180 = 0x4,   	// 180 degree 
		ASF_OC_30 = 0x5,		// 30 degree 
		ASF_OC_60 = 0x6,		// 60 degree 
		ASF_OC_120 = 0x7,		// 120 degree 
		ASF_OC_150 = 0x8,		// 150 degree 
		ASF_OC_210 = 0x9,		// 210 degree 
		ASF_OC_240 = 0xa,		// 240 degree 
		ASF_OC_300 = 0xb,		// 300 degree 
		ASF_OC_330 = 0xc		// 330 degree 
	};

	//���ģ��
	enum ASF_DetectModel {
		ASF_DETECT_MODEL_RGB = 0x1	//RGBͼ����ģ��
		//Ԥ����չ�������ģ��
	};

	//�����ȶԿ�ѡ��ģ��
	enum ASF_CompareModel{
		ASF_LIFE_PHOTO = 0x1,	//����������֮��������ȶԣ��Ƽ���ֵ0.80
		ASF_ID_PHOTO = 0x2		//����֤���ջ���������֤����֮��������ȶԣ��Ƽ���ֵ0.82
	};

	//�汾��Ϣ
	typedef struct {
		MPChar Version;				// �汾��
		MPChar BuildDate;			// ��������
		MPChar CopyRight;			// Copyright
	}ASF_VERSION, *LPASF_VERSION;

	//ͼ������
	typedef LPASVLOFFSCREEN LPASF_ImageData;

	//��������Ϣ
	typedef struct {
		MRECT		faceRect;		// ��������Ϣ
		MInt32		faceOrient;		// ����ͼ��ĽǶȣ����Բο� ArcFaceCompare_OrientCode
	} ASF_SingleFaceInfo, *LPASF_SingleFaceInfo;

	//��������Ϣ
	typedef struct {
		MRECT* 		 faceRect;			// ��������Ϣ
		MInt32*		 faceOrient;		// ����ͼ��ĽǶȣ����Բο� ArcFaceCompare_OrientCode . 
		MInt32		 faceNum;			// ��⵽����������
		MInt32*      faceID;			// face ID��IMAGEģʽ�²�����FaceID
	}ASF_MultiFaceInfo, *LPASF_MultiFaceInfo;

	// �����ļ���Ϣ
	typedef struct {
		MPChar startTime;		//��ʼʱ��
		MPChar endTime;			//��ֹʱ��
		MPChar platform;		//ƽ̨
		MPChar sdkType;			//sdk����
		MPChar appId;			//APPID
		MPChar sdkKey;			//SDKKEY
		MPChar sdkVersion;		//SDK�汾��
		MPChar fileVersion;		//�����ļ��汾��
	}ASF_ActiveFileInfo, *LPASF_ActiveFileInfo;

	/*******************************************************************************************
	* ��ȡ�����ļ���Ϣ�ӿ�
	*******************************************************************************************/
	MRESULT ASFGetActiveFileInfo(
		LPASF_ActiveFileInfo  activeFileInfo  // [out] �����ļ���Ϣ
		);

	/*******************************************************************************************
	* ���߼���ӿ�
	*******************************************************************************************/
	MRESULT ASFOnlineActivation(
		MPChar				AppId,			// [in]  APPID	��������
		MPChar				SDKKey			// [in]  SDKKEY	��������
		);

	/*******************************************************************************************
	* ���߼���ӿڣ��ýӿ���ASFOnlineActivation�ӿڹ���һ�£��Ƽ�ʹ�øýӿ�
	*******************************************************************************************/
	MRESULT ASFActivation(
		MPChar				AppId,			// [in]  APPID	��������
		MPChar				SDKKey			// [in]  SDKKEY	��������
		);

	/************************************************************************
	* ��ʼ������
	************************************************************************/
	MRESULT ASFInitEngine(
		ASF_DetectMode		detectMode,					// [in]	AF_DETECT_MODE_VIDEO ��Ƶģʽ������������ͷԤ������Ƶ�ļ�ʶ��
		//		AF_DETECT_MODE_IMAGE ͼƬģʽ�������ھ�̬ͼƬ��ʶ��
		ASF_OrientPriority	detectFaceOrientPriority,	// [in]	��������ĽǶ�����ֵ���ο� ArcFaceCompare_OrientPriority
		MInt32				detectFaceScaleVal,			// [in] ������ֵ����ʾ����С�����ߴ磬�óߴ���������ߴ������ͼƬ���ߵ�ռ��
		// video ģʽ��Чֵ��Χ[2, 32], �Ƽ�ֵΪ 16
		// image ģʽ��Чֵ��Χ[2, 32], �Ƽ�ֵΪ 32
		MInt32				detectFaceMaxNum,			// [in] �����Ҫ������������
		MInt32				combinedMask,				// [in] �û�ѡ����Ҫ���Ĺ�����ϣ��ɵ�������
		MHandle*			hEngine						// [out] ��ʼ�����ص�����handle
		);

	/******************************************************
	* VIDEOģʽ:����׷�� IMAGEģʽ:�������
	******************************************************/
	MRESULT ASFDetectFaces(
		MHandle				hEngine,							// [in] ����handle
		MInt32				width,								// [in] ͼƬ����
		MInt32				height,								// [in] ͼƬ�߶�
		MInt32				format,								// [in] ��ɫ�ռ��ʽ
		MUInt8*				imgData,							// [in] ͼƬ����
		LPASF_MultiFaceInfo	detectedFaces,						// [out]��⵽��������Ϣ 
		ASF_DetectModel		detectModel = ASF_DETECT_MODEL_RGB	// [in] Ԥ���ֶΣ���ǰ�汾ʹ��Ĭ�ϲ�������
		);

	/******************************************************
	* VIDEOģʽ:����׷�� IMAGEģʽ:�������
	* ͼ�������Խṹ����ʽ���룬�Բ��ø����ֽڶ��뷽ʽ��ͼ������Ը���
	******************************************************/
	MRESULT ASFDetectFacesEx(
		MHandle				hEngine,							// [in] ����handle
		LPASF_ImageData		imgData,							// [in] ͼƬ����
		LPASF_MultiFaceInfo	detectedFaces,						// [out] ��⵽��������Ϣ
		ASF_DetectModel		detectModel = ASF_DETECT_MODEL_RGB	// [in]	Ԥ���ֶΣ���ǰ�汾ʹ��Ĭ�ϲ�������
		);

	//******************************** ������ֵ���� **********************************************
	typedef struct {
		MFloat		thresholdmodel_BGR;
		MFloat		thresholdmodel_IR;
	}ASF_LivenessThreshold, *LPASF_LivenessThreshold;

	/************************************************************************
	* ȡֵ��Χ[0-1]��Ĭ��ֵ BGR:0.5 IR:0.7�� �û����Ը���ʵ���������ò�ͬ����ֵ
	************************************************************************/
	MRESULT ASFSetLivenessParam(
		MHandle					hEngine,		// [in] ����handle
		LPASF_LivenessThreshold threshold		// [in] �������Ŷ�
		);

	/************************************************************************
	* ����/�Ա�/����3D�Ƕȣ��ýӿڽ�֧��RGBͼ�񣩣����֧��4��������Ϣ��⣬�������ַ���δ֪
	* RGB�����֧�ֵ�������⣬�ýӿڲ�֧�ּ��IR����
	************************************************************************/
	MRESULT ASFProcess(
		MHandle				hEngine,			// [in] ����handle
		MInt32				width,				// [in] ͼƬ����
		MInt32				height,				// [in] ͼƬ�߶�
		MInt32				format,				// [in] ��ɫ�ռ��ʽ
		MUInt8*				imgData,			// [in] ͼƬ����
		LPASF_MultiFaceInfo	detectedFaces,		// [in] ������Ϣ���û����ݴ����Ĺ���ѡ����Ҫʹ�õ�������
		MInt32				combinedMask		// [in] ֻ֧�ֳ�ʼ��ʱ��ָ����Ҫ���Ĺ��ܣ���processʱ��һ��������Ѿ�ָ���Ĺ��ܼ��м���ɸѡ
		//      �����ʼ����ʱ��ָ�����������Ա���process��ʱ�����ֻ������䣬���ǲ��ܼ���������Ա�֮��Ĺ���    
		);

	/************************************************************************
	* ����/�Ա�/����3D�Ƕȣ��ýӿڽ�֧��RGBͼ�񣩣����֧��4��������Ϣ��⣬�������ַ���δ֪
	* RGB�����֧�ֵ�������⣬�ýӿڲ�֧�ּ��IR����
	* ͼ�������Խṹ����ʽ���룬�Բ��ø����ֽڶ��뷽ʽ��ͼ������Ը���
	************************************************************************/
	MRESULT ASFProcessEx(
		MHandle				hEngine,			// [in] ����handle
		LPASF_ImageData		imgData,			// [in] ͼƬ����
		LPASF_MultiFaceInfo detectedFaces,		// [in] ������Ϣ���û����ݴ����Ĺ���ѡ����Ҫʹ�õ�������
		MInt32				combinedMask		// [in] ֻ֧�ֳ�ʼ��ʱ��ָ����Ҫ���Ĺ��ܣ���processʱ��һ��������Ѿ�ָ���Ĺ��ܼ��м���ɸѡ
		//      �����ʼ����ʱ��ָ�����������Ա���process��ʱ�����ֻ������䣬���ǲ��ܼ���������Ա�֮��Ĺ��� 
		);

	/************************************************************************
	* �ýӿ�Ŀǰ��֧�ֵ�����IR�����⣨��֧�����䡢�Ա�3D�Ƕȵļ�⣩��Ĭ��ȡ��һ������
	************************************************************************/
	MRESULT ASFProcess_IR(
		MHandle				hEngine,			// [in] ����handle
		MInt32				width,				// [in] ͼƬ����
		MInt32				height,				// [in] ͼƬ�߶�
		MInt32				format,				// [in] ��ɫ�ռ��ʽ
		MUInt8*				imgData,			// [in] ͼƬ����
		LPASF_MultiFaceInfo	detectedFaces,		// [in] ������Ϣ���û����ݴ����Ĺ���ѡ����Ҫʹ�õ�������
		MInt32				combinedMask		// [in] Ŀǰֻ֧�ִ���ASF_IR_LIVENESS���ԵĴ��룬�ҳ�ʼ���ӿ���Ҫ���� 
		);

	/************************************************************************
	* �ýӿ�Ŀǰ��֧�ֵ�����IR�����⣨��֧�����䡢�Ա�3D�Ƕȵļ�⣩��Ĭ��ȡ��һ������
	* ͼ�������Խṹ����ʽ���룬�Բ��ø����ֽڶ��뷽ʽ��ͼ������Ը���
	************************************************************************/
	MRESULT ASFProcessEx_IR(
		MHandle				hEngine,			// [in] ����handle
		LPASF_ImageData		imgData,			// [in] ͼƬ����
		LPASF_MultiFaceInfo detectedFaces,		// [in] ������Ϣ���û����ݴ����Ĺ���ѡ����Ҫʹ�õ�������
		MInt32				combinedMask		// [in] Ŀǰֻ֧�ִ���ASF_IR_LIVENESS���ԵĴ��룬�ҳ�ʼ���ӿ���Ҫ����
		);

	/************************************************************************
	* ��������
	************************************************************************/
	MRESULT ASFUninitEngine(
		MHandle hEngine
		);

	/************************************************************************
	* ��ȡ�汾��Ϣ
	************************************************************************/
	const ASF_VERSION ASFGetVersion();

	//******************************** ����ʶ����� *********************************************
	typedef struct {
		MByte*		feature;		// ����������Ϣ
		MInt32		featureSize;	// ����������Ϣ����	
	}ASF_FaceFeature, *LPASF_FaceFeature;

	/************************************************************************
	* ������������ȡ
	************************************************************************/
	MRESULT ASFFaceFeatureExtract(
		MHandle					hEngine,		// [in]	����handle
		MInt32					width,			// [in] ͼƬ����
		MInt32					height,			// [in] ͼƬ�߶�
		MInt32					format,			// [in] ��ɫ�ռ��ʽ
		MUInt8*					imgData,		// [in] ͼƬ����
		LPASF_SingleFaceInfo	faceInfo,		// [in] ��������λ�úͽǶ���Ϣ
		LPASF_FaceFeature		feature			// [out] ��������
		);

	/************************************************************************
	* ������������ȡ
	* ͼ�������Խṹ����ʽ���룬�Բ��ø����ֽڶ��뷽ʽ��ͼ������Ը���
	************************************************************************/
	MRESULT ASFFaceFeatureExtractEx(
		MHandle					hEngine,		// [in]	����handle
		LPASF_ImageData			imgData,		// [in] ͼ������
		LPASF_SingleFaceInfo	faceInfo,		// [in] ��������λ�úͽǶ���Ϣ
		LPASF_FaceFeature		feature			// [out] ��������
		);

	/************************************************************************
	* ���������ȶԣ��Ƽ���ֵ ASF_LIFE_PHOTO��0.80  ASF_ID_PHOTO��0.82
	************************************************************************/
	MRESULT ASFFaceFeatureCompare(
		MHandle					hEngine,						// [in] ����handle
		LPASF_FaceFeature		feature1,						// [in] ���Ƚ���������1
		LPASF_FaceFeature		feature2,						// [in] ���Ƚ���������2
		MFloat*					confidenceLevel,				// [out] �ȽϽ�������Ŷ���ֵ
		ASF_CompareModel		compareModel = ASF_LIFE_PHOTO	// [in] ASF_LIFE_PHOTO������������֮��������ȶ�
		//		ASF_ID_PHOTO������֤���ջ�֤���պ�������֮��������ȶ�
		);

	//******************************** ������� **********************************************
	typedef struct {
		MInt32*	ageArray;				// "0" ������ȷ��������0����ֵ������������������
		MInt32	num;					// ������������
	}ASF_AgeInfo, *LPASF_AgeInfo;

	/************************************************************************
	* ��ȡ������Ϣ
	************************************************************************/
	MRESULT ASFGetAge(
		MHandle hEngine,				// [in] ����handle
		LPASF_AgeInfo ageInfo			// [out] ��⵽��������Ϣ
		);

	//******************************** �Ա���� **********************************************
	typedef struct {
		MInt32*	genderArray;			// "0" ��ʾ ����, "1" ��ʾ Ů��, "-1" ��ʾ��ȷ��
		MInt32	num;					// ������������	
	}ASF_GenderInfo, *LPASF_GenderInfo;

	/************************************************************************
	* ��ȡ�Ա���Ϣ
	************************************************************************/
	MRESULT ASFGetGender(
		MHandle hEngine,				// [in] ����handle
		LPASF_GenderInfo genderInfo		// [out] ��⵽���Ա���Ϣ
		);

	//******************************** ����3D �Ƕ���Ϣ ***********************************
	typedef struct {
		MFloat* roll;
		MFloat* yaw;
		MFloat* pitch;
		MInt32* status;					//0: ������������ֵ������
		MInt32 num;
	}ASF_Face3DAngle, *LPASF_Face3DAngle;

	/************************************************************************
	* ��ȡ3D�Ƕ���Ϣ
	************************************************************************/
	MRESULT ASFGetFace3DAngle(
		MHandle hEngine,					// [in] ����handle
		LPASF_Face3DAngle p3DAngleInfo		// [out] ��⵽����3D �Ƕ���Ϣ
		);

	//******************************** ������Ϣ ***********************************
	typedef struct {
		MInt32*	isLive;			// [out] �ж��Ƿ����ˣ� 0�������ˣ�
		//						1�����ˣ�
		//						-1����ȷ���� 
		//						-2:����������>1��
		//                      -3: ������С
		//                      -4: �Ƕȹ���
		//                      -5: ���������߽� 
		MInt32 num;
	}ASF_LivenessInfo, *LPASF_LivenessInfo;

	/************************************************************************
	* ��ȡRGB������
	************************************************************************/
	MRESULT ASFGetLivenessScore(
		MHandle hEngine,					// [in] ����handle
		LPASF_LivenessInfo livenessInfo		// [out] ���RGB������
		);

	/************************************************************************
	* ��ȡIR������
	************************************************************************/
	MRESULT ASFGetLivenessScore_IR(
		MHandle				hEngine,			// [in] ����handle
		LPASF_LivenessInfo	 irLivenessInfo		// [out] ��⵽IR������
		);


#ifdef __cplusplus
}
#endif
#endif