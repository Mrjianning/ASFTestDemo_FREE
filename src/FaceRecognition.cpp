#include "FaceRecognition.h"

FaceRecognition::FaceRecognition(const string& appId, const string& sdkKey) : appId(appId), sdkKey(sdkKey), handle(nullptr) {
    // 使用 const_cast 来转换 const char* 到 MPChar
    MRESULT res = ASFOnlineActivation(const_cast<char*>(appId.c_str()), const_cast<char*>(sdkKey.c_str()));
    if (res != MOK && res != MERR_ASF_ALREADY_ACTIVATED) {
        cout << "激活SDK失败，错误代码：" << res << endl;
    } else {
        cout << "SDK激活成功或已经激活。" << endl;
        // SDK版本信息
        const ASF_VERSION version = ASFGetVersion();
        cout << "Version: " <<  version.Version << endl;
        cout << "BuildDate: " <<  version.BuildDate << endl;
        cout << "CopyRight: " <<  version.CopyRight << endl;
    }

    int mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
    res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, NSCALE, FACENUM, mask, &handle);
    if (res != MOK) {
        cout << "初始化引擎失败，错误代码：" << res << endl;
    } else {
        cout << "引擎初始化成功。" << endl;
    }
}

FaceRecognition::~FaceRecognition() {
    if (handle) {
        MRESULT res = ASFUninitEngine(handle);
        cout << (res == MOK ? "引擎反初始化成功。" : "引擎反初始化失败。") << endl;
    }
}

MRESULT FaceRecognition::detectFace(ASVLOFFSCREEN inputImg, ASF_MultiFaceInfo& detectedFaces) {
    return ASFDetectFacesEx(handle, &inputImg, &detectedFaces);
}

MRESULT FaceRecognition::extractFeatures(ASVLOFFSCREEN inputImg, ASF_SingleFaceInfo& faceInfo, ASF_FaceFeature& feature) {
    return ASFFaceFeatureExtractEx(handle, &inputImg, &faceInfo, &feature);
}

float FaceRecognition::compareFaces(ASF_FaceFeature& feature1, ASF_FaceFeature& feature2) {
    MFloat confidenceLevel = 0.0; // 初始化置信度
    MRESULT res = ASFFaceFeatureCompare(handle, &feature1, &feature2, &confidenceLevel);
    if (res != MOK) {
        std::cerr << "ASFFaceFeatureCompare failed: " << res << std::endl;
        return 0.0f; // 如果比对失败，返回0.0或其他表示失败的值
    }
    std::cout << "ASFFaceFeatureCompare success: " << confidenceLevel << std::endl;
    return confidenceLevel; // 返回检测到的置信度
}

void FaceRecognition::performLivenessDetection(const cv::Mat& image, const ASF_MultiFaceInfo& faceInfo) {
    // 活体检测实现
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

cv::Mat FaceRecognition::prepareImageForDetection(const cv::Mat& image) {
    cv::Mat adjustedImage;
    int width = image.cols, height = image.rows;
    AdjustAndCropImage(image, adjustedImage, 0, 0, width, height);
    return adjustedImage;
}

void FaceRecognition::AdjustAndCropImage(const cv::Mat& src, cv::Mat& dst, int x, int y, int& width, int& height) {
    if (x + width > src.cols) width = src.cols - x;
    if (y + height > src.rows) height = src.rows - y;
    width = (width / 4) * 4; // 保证宽度为4的倍数
    cv::Rect roi(x, y, width, height);
    dst = src(roi).clone(); // 克隆需要的区域
}
