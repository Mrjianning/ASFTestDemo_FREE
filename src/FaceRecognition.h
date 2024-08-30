#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

#define NSCALE 16 
#define FACENUM 5

class FaceRecognition {
private:
    ASF_ActiveFileInfo activeFileInfo;
    MHandle handle;
    string appId;
    string sdkKey;

public:
    FaceRecognition(const string& appId, const string& sdkKey);
    ~FaceRecognition();

    // 人脸检测
    MRESULT detectFace(ASVLOFFSCREEN inputImg, ASF_MultiFaceInfo& detectedFaces);
    // 特征提取
    MRESULT extractFeatures(ASVLOFFSCREEN inputImg, ASF_SingleFaceInfo& faceInfo, ASF_FaceFeature& feature);
    // 人脸对比
    float compareFaces(ASF_FaceFeature& feature1, ASF_FaceFeature& feature2);
    // 活体检测
    void performLivenessDetection(const cv::Mat& image, const ASF_MultiFaceInfo& faceInfo);

    // 格式调整
    ASVLOFFSCREEN convertMatToASVLOFFSCREEN(const cv::Mat& mat);
    cv::Mat prepareImageForDetection(const cv::Mat& image);
    static void AdjustAndCropImage(const cv::Mat& src, cv::Mat& dst, int x, int y, int& width, int& height);
};

#endif // FACERECOGNITION_H
