#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 读取带噪声图像
    Mat img = imread("../imgs/enoz_noise.png");
    if (img.empty()) {
        cerr << "Image not found!" << endl;
        return -1;
    }

    // 均值去噪
    Mat denoised;
    fastNlMeansDenoisingColored(img, denoised, 15, 10, 7, 21);

    // 中值滤波去除孤立噪声点
    Mat median_blurred;
    medianBlur(denoised, median_blurred, 3);

    // 提升对比度
    Mat contrast_adjusted;
    convertScaleAbs(median_blurred, contrast_adjusted, 1.3, 10);

    /* // 转换到Lab颜色空间
    Mat lab_image;
    cvtColor(contrast_adjusted, lab_image, COLOR_BGR2Lab);

    // 分割通道
    vector<Mat> lab_planes(2);
    split(lab_image, lab_planes); 

    // 减少高光
    Ptr<CLAHE> clahe = createCLAHE(2);
    clahe->setClipLimit(3);
    Mat dst;
    clahe->apply(lab_planes[0], dst);
    dst.copyTo(lab_planes[0]);

    // 合并通道
    merge(lab_planes, lab_image);

    // 转回BGR颜色空间
    Mat bgr_image;
    cvtColor(lab_image, bgr_image, COLOR_Lab2BGR);

    // 转换到HSV颜色空间以调整亮度和饱和度
    Mat hsv_image;
    cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);

    // 分离通道
    vector<Mat> channels;
    split(hsv_image, channels);

    // 降低亮度
    channels[2] = max(channels[2] * 0.8, Mat(hsv_image.size(), CV_8UC1, Scalar(0)));

    // 增加饱和度
    channels[1] = min(channels[1] * 1.1, Mat(hsv_image.size(), CV_8UC1, Scalar(255)));

    // 合并通道
    merge(channels, hsv_image);

    // 转换回BGR颜色空间
    Mat final_result;
    cvtColor(hsv_image, final_result, COLOR_HSV2BGR); */

    // 保存结果
    imwrite("result3.png", contrast_adjusted);
    waitKey(0);
    return 0;
}