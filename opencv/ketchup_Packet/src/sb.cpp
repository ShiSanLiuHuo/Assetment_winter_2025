#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

// 排序函数，按顺时针排序四个点：左上、右上、右下、左下
void sortPoints(vector<Point2f>& points) {
    // 计算中心点
    Point2f center(0, 0);
    for (const auto& pt : points) {
        center += pt;
    }
    
    float size = static_cast<float>(points.size());
    center.x /= size;
    center.y /= size;

    // 按相对于中心的角度排序
    vector<pair<double, int>> angles(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        double dx = points[i].x - center.x;
        double dy = points[i].y - center.y;
        angles[i] = make_pair(atan2(dy, dx), i);
    }
    sort(angles.begin(), angles.end());

    // 重新排序点
    vector<Point2f> sorted(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        sorted[i] = points[angles[i].second];
    }

    // 调整顺序确保为左上、右上、右下、左下
    if (points.size() == 4) {
        swap(sorted[3], sorted[2]);
        swap(sorted[0], sorted[3]);
    }
    points = sorted;
}

int main() {
    Mat src = imread("../imgs/Ketchup_Packet.png");
    if (src.empty()) {
        cerr << "Image not found!" << endl;
        return -1;
    }

    // 转换到HSV颜色空间
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    // 提取红色区域
    Mat lower_red, upper_red;
    inRange(hsv, Scalar(0, 150, 70), Scalar(10, 255, 255), lower_red); // 微调了颜色范围
    inRange(hsv, Scalar(170, 150, 70), Scalar(180, 255, 255), upper_red);
    Mat red_mask = lower_red | upper_red;

    // 形态学操作
    Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7)); // 增大核尺寸
    morphologyEx(red_mask, red_mask, MORPH_CLOSE, kernel);

    // 查找轮廓
    vector<vector<Point>> contours;
    findContours(red_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 遍历所有轮廓
    bool found = false;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > 500) { // 降低面积阈值
            vector<Point> approx;
            double epsilon = arcLength(contours[i], true) * 0.02; // 根据轮廓周长动态调整epsilon
            approxPolyDP(contours[i], approx, epsilon, true);

            if (approx.size() == 4) { // 找到了四边形
                // 转换为Point2f并排序
                vector<Point2f> src_pts;
                for (const auto& pt : approx) {
                    src_pts.emplace_back(pt);
                }
                sortPoints(src_pts);

                // 目标点（2:1宽高比）
                int width = 500;
                vector<Point2f> dst_pts = {
                    Point2f(0, 0),
                    Point2f(width, 0),
                    Point2f(width, width / 2),
                    Point2f(0, width / 2)
                };

                // 计算透视变换矩阵
                Mat M = getPerspectiveTransform(src_pts, dst_pts);
                Mat warped;
                warpPerspective(src, warped, M, Size(width, width / 2));

                // 保存结果
                imwrite("ans_tomato.png", warped);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cerr << "No suitable contour found." << endl;
    }

    return 0;
}