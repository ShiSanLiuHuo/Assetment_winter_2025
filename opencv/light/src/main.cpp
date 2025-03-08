#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

struct ContourComparator {
    bool operator()(const vector<Point>& a, const vector<Point>& b) {
        return contourArea(a) > contourArea(b);
    }
};

vector<Point2f> detect_lampbar(Mat img) {
    Mat hsv, v_channel, binary;
    vector<Point2f> output_points;
    
    // 高光区域提取
    cvtColor(img, hsv, COLOR_BGR2HSV);
    vector<Mat> channels;
    split(hsv, channels);
    threshold(channels[2], binary, 220, 255, THRESH_BINARY); // V通道阈值
    
    // 形态学优化
    Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
    morphologyEx(binary, binary, MORPH_CLOSE, kernel);
    morphologyEx(binary, binary, MORPH_OPEN, kernel);

    // 轮廓检测与分级筛选
    vector<vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    // 按面积排序并筛选前3个最大轮廓
    sort(contours.begin(), contours.end(), ContourComparator());
    int valid_cnt = min((int)contours.size(), 3);

    for (int i = 0; i < valid_cnt; ++i) {
        auto& cnt = contours[i];
        if (contourArea(cnt) < 500) continue;

        // 几何特征筛选
        RotatedRect rect = minAreaRect(cnt);
        float aspect = max(rect.size.width, rect.size.height) / 
                      min(rect.size.width, rect.size.height);
        if (aspect < 4.0) continue;

        // PCA主轴端点检测
        Mat data_pts(cnt.size(), 2, CV_64F);
        for (int j = 0; j < data_pts.rows; ++j) {
            data_pts.at<double>(j, 0) = cnt[j].x;
            data_pts.at<double>(j, 1) = cnt[j].y;
        }

        PCA pca(data_pts, Mat(), PCA::DATA_AS_ROW);
        Point2f center(pca.mean.at<double>(0, 0), 
                 pca.mean.at<double>(0, 1));
        Point2f vec(pca.eigenvectors.at<double>(0, 0),
                   pca.eigenvectors.at<double>(0, 1));

        // 计算投影极值
        vector<double> projections;
        for (auto& pt : cnt) {
            projections.push_back(vec.x*(pt.x-center.x) + vec.y*(pt.y-center.y));
        }
        auto mm = minmax_element(projections.begin(), projections.end());
        
        // 计算端点坐标
        Point2f p1 = center + vec * (*mm.first);
        Point2f p2 = center + vec * (*mm.second);
        output_points.push_back(p1);
        output_points.push_back(p2);
    }

    // 按X坐标排序输出点
    sort(output_points.begin(), output_points.end(),
        [](const Point2f& a, const Point2f& b) { return a.x < b.x; });

    return output_points;
}

int main() {
    Mat input = imread("../box/image.png");
    if (input.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    vector<Point2f> lamp_points = detect_lampbar(input);
    Mat result = input.clone();

    // 绘制检测结果
    for (auto& pt : lamp_points) {
        circle(result, pt, 8, Scalar(0, 255, 0), -1);
        circle(result, pt, 4, Scalar(0, 0, 255), -1);
    }

    imwrite("lamp_result.png", result);
    waitKey(0);

    return 0;
}