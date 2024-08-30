#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace std::chrono;

#define NSCALE 16 
#define FACENUM	5

// 定义一个人脸识别类
class FaceRecognition {
private:
    ASF_ActiveFileInfo activeFileInfo;
    MHandle handle;
    string appId;
    string sdkKey;

public:
   FaceRecognition(const string& appId, const string& sdkKey) : appId(appId), sdkKey(sdkKey), handle(nullptr) {
		// 使用 const_cast 来转换 const char* 到 MPChar
		MRESULT res = ASFOnlineActivation(const_cast<char*>(appId.c_str()), const_cast<char*>(sdkKey.c_str()));
		if (res != MOK && res != MERR_ASF_ALREADY_ACTIVATED) {
			cout << "激活SDK失败，错误代码：" << res << endl;
		} else {
			cout << "SDK激活成功或已经激活。" << endl;
			//SDK版本信息
			const ASF_VERSION version = ASFGetVersion();
			cout << "Version" <<  version.Version << endl;
			cout << "BuildDate" <<  version.BuildDate << endl;
			cout << "CopyRight" <<  version.CopyRight << endl;
		}

		int mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
		res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, NSCALE, FACENUM, mask, &handle);
		if (res != MOK) {
			cout << "初始化引擎失败，错误代码：" << res << endl;
		} else {
			cout << "引擎初始化成功。" << endl;
		}
	}

    ~FaceRecognition() {
        if (handle) {
            MRESULT res = ASFUninitEngine(handle);
            cout << (res == MOK ? "引擎反初始化成功。" : "引擎反初始化失败。") << endl;
        }
    }

    // 人脸检测
    MRESULT detectFace(ASVLOFFSCREEN inputImg,ASF_MultiFaceInfo& detectedFaces);
	// 特征提取  										
    MRESULT extractFeatures(ASVLOFFSCREEN inputImg,  ASF_SingleFaceInfo& faceInfo,  ASF_FaceFeature& feature);
	// 人脸对比    	
    float compareFaces( ASF_FaceFeature& feature1,  ASF_FaceFeature& feature2);
	// 活体检测
    void performLivenessDetection(const cv::Mat& image, const ASF_MultiFaceInfo& faceInfo);

    // 格式调整
	ASVLOFFSCREEN convertMatToASVLOFFSCREEN(const cv::Mat& mat);
	cv::Mat  prepareImageForDetection(const cv::Mat& image);
    static void AdjustAndCropImage(const cv::Mat& src, cv::Mat& dst, int x, int y, int& width, int& height);
};


// 调整图像以符合ArcSoft SDK要求的图像格式
void FaceRecognition::AdjustAndCropImage(const cv::Mat& src, cv::Mat& dst, int x, int y, int& width, int& height) {
    if (x + width > src.cols) width = src.cols - x;
    if (y + height > src.rows) height = src.rows - y;
    width = (width / 4) * 4; 		// 保证宽度为4的倍数
    cv::Rect roi(x, y, width, height);
    dst = src(roi).clone(); 		// 克隆需要的区域
}

cv::Mat FaceRecognition::prepareImageForDetection(const cv::Mat& image) {
    cv::Mat adjustedImage;
    int width = image.cols, height = image.rows;
    AdjustAndCropImage(image, adjustedImage, 0, 0, width, height);
    return adjustedImage;
}

ASVLOFFSCREEN FaceRecognition::convertMatToASVLOFFSCREEN(const cv::Mat& image) {
    ASVLOFFSCREEN inputImg = {0};
    inputImg.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    inputImg.i32Width = image.cols;
    inputImg.i32Height = image.rows;
    inputImg.ppu8Plane[0] = image.data;
    inputImg.pi32Pitch[0] = image.step;
	return inputImg;
}

// 人脸检测实现
MRESULT FaceRecognition::detectFace(ASVLOFFSCREEN inputImg,ASF_MultiFaceInfo& detectedFaces) {

    return ASFDetectFacesEx(handle, &inputImg, &detectedFaces);
   
}

// 特征提取实现
MRESULT FaceRecognition::extractFeatures(ASVLOFFSCREEN inputImg,  ASF_SingleFaceInfo& faceInfo,  ASF_FaceFeature& feature) {

	return ASFFaceFeatureExtractEx(handle, &inputImg, &faceInfo, &feature);
	
}

// 人脸比对实现
float FaceRecognition::compareFaces( ASF_FaceFeature& feature1,  ASF_FaceFeature& feature2) {
   
    MFloat confidenceLevel = 0.0; // 初始化置信度
    MRESULT res = ASFFaceFeatureCompare(handle, &feature1, &feature2, &confidenceLevel);
    if (res != MOK) {
        std::cerr << "ASFFaceFeatureCompare failed: " << res << std::endl;
        return 0.0f; // 如果比对失败，返回0.0或其他表示失败的值
    }
    
    std::cout << "ASFFaceFeatureCompare success: " << confidenceLevel << std::endl;
    return confidenceLevel; // 返回检测到的置信度
}

// 活体检测实现
void FaceRecognition::performLivenessDetection(const cv::Mat& image, const ASF_MultiFaceInfo& faceInfo) {
    
}

// 主函数
int main() {
    // 使用FaceRecognition类
    FaceRecognition faceRec("inyUZFeQCNeaVyGHHiFBAKCX5K4B1mdfTkFkKuVZ2up", "4GdYnUbo6nNRbcxPtFEp7uK1hfSSyEq8hXoYV8fxu5xa");

    // ===================================================加载图像1=======================
    cv::Mat image = cv::imread("../images/11.jpg", cv::IMREAD_COLOR);
    if (image.empty()) {
        cout << "无法加载图像！" << endl;
        return -1;
    }

	// 0、确保图像为4的倍数宽度并转为ArcSoft要求的格式
	cv::Mat adjustedImage = faceRec.prepareImageForDetection(image);
	ASVLOFFSCREEN inputImg = faceRec.convertMatToASVLOFFSCREEN(adjustedImage);

    // 1、进行人脸检测
	ASF_MultiFaceInfo faceInfo = {0};
    MRESULT res=faceRec.detectFace(inputImg,faceInfo);
	if (res != MOK) {
        std::cout << "人脸检测失败, 错误代码: " << res << std::endl;
    }

	// 2、进行人脸特征提取
	std::map<int, ASF_FaceFeature> features;
    for (int i = 0; i < faceInfo.faceNum; ++i) {
		// 将检测到的人脸框绘制到图像上
        MRECT& rect = faceInfo.faceRect[i];
        cv::rectangle(image, cv::Point(rect.left, rect.top), cv::Point(rect.right, rect.bottom), cv::Scalar(0, 255, 0), 2);

		// 特征提取
		ASF_SingleFaceInfo SingleDetectedFaces = { 0 };	 	// 保存单个人脸
		ASF_FaceFeature feature = { 0 };               	// 保存人脸特征
		ASF_FaceFeature copyfeature1 = { 0 }; 

		SingleDetectedFaces.faceRect=faceInfo.faceRect[i];
		SingleDetectedFaces.faceOrient = faceInfo.faceOrient[i];

		MRESULT res=faceRec.extractFeatures(inputImg,SingleDetectedFaces,feature);
		if (res != MOK)
		{
			printf("%s ASFFaceFeatureExtractEx 1 fail: %d\n", res);
		}else
		{
			cout << "特征提取成功:" <<  i << endl;
			//拷贝feature，否则第二次进行特征提取，会覆盖第一次特征提取的数据，导致比对的结果为1
			copyfeature1.featureSize = feature.featureSize;
			copyfeature1.feature = (MByte *)malloc(feature.featureSize);
			memset(copyfeature1.feature, 0, feature.featureSize);
			memcpy(copyfeature1.feature, feature.feature, feature.featureSize);

			std::cout << "Feature Size: " << feature.featureSize << "\n";
			std::cout << "Feature Data (Hex): ";
			for (int i = 0; i < feature.featureSize; ++i) {
				std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(feature.feature[i]) << " ";
				if ((i + 1) % 16 == 0) std::cout << "\n"; // 每16字节换行，方便查看
			}
			std::cout << std::dec << "\n"; // 恢复到十进制输出
			
			features[i]=copyfeature1;
		}
    }

	// ===================================================加载图像2=======================
    cv::Mat image2 = cv::imread("../images/12.jpg", cv::IMREAD_COLOR);
    if (image2.empty()) {
        cout << "无法加载图像！" << endl;
        return -1;
    }

	// 0、确保图像为4的倍数宽度并转为ArcSoft要求的格式
	cv::Mat adjustedImage2 = faceRec.prepareImageForDetection(image2);
	ASVLOFFSCREEN inputImg2 = faceRec.convertMatToASVLOFFSCREEN(adjustedImage2);

    // 1、进行人脸检测
	ASF_MultiFaceInfo faceInfo2 = {0};
    MRESULT res2=faceRec.detectFace(inputImg2,faceInfo2);
	if (res2 != MOK) {
        std::cout << "人脸检测失败, 错误代码: " << res << std::endl;
    }

	// 2、进行人脸特征提取
	std::map<int, ASF_FaceFeature> features2;
    for (int i = 0; i < faceInfo2.faceNum; ++i) {
		// 将检测到的人脸框绘制到图像上
        MRECT& rect = faceInfo2.faceRect[i];
        cv::rectangle(image2, cv::Point(rect.left, rect.top), cv::Point(rect.right, rect.bottom), cv::Scalar(0, 255, 0), 2);

		// 特征提取
		ASF_SingleFaceInfo SingleDetectedFaces = { 0 };	 	// 保存单个人脸
		ASF_FaceFeature feature1 = { 0 };               	// 保存人脸特征

		SingleDetectedFaces.faceRect=faceInfo.faceRect[i];
		SingleDetectedFaces.faceOrient = faceInfo.faceOrient[i];

		MRESULT res2=faceRec.extractFeatures(inputImg2,SingleDetectedFaces,feature1);
		if (res2 != MOK)
		{
			printf("%s ASFFaceFeatureExtractEx 1 fail: %d\n", res2);
		}else
		{
			cout << "特征2提取成功:" <<  i << endl;
			features2[i]=feature1;
		}
    }

	// 3、人脸对比
	float condif=faceRec.compareFaces(features[0],features2[0]);
	cout << "相似度：" <<  condif << endl;

	// 转换相似度分数为字符串
    std::string similarityText = "Similarity: " + std::to_string(condif) + "%";
	// 设置文本显示的位置和样式
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.7;
    int thickness = 2;
    cv::Point textOrg(10, 30);  // 设置文本在图片上的起始位置

    // 将文本绘制到第一张图片上
    cv::putText(image, similarityText, textOrg, fontFace, fontScale, cv::Scalar(0, 255, 0), thickness);


    // 显示结果
     // 创建第一个窗口并显示第一张图片
    cv::namedWindow("Face Detection 1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Face Detection 1", image);

    // 创建第二个窗口并显示第二张图片
    cv::namedWindow("Face Detection 2", cv::WINDOW_AUTOSIZE);
    cv::imshow("Face Detection 2", image2);
    cv::waitKey(0);

    return 0;
}
