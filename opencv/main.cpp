#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // 加载图像
    Mat src = imread("Ketchup_Packet.png");
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }

    // 预处理图像
    Mat gray, blurred, edges;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0); // 模糊处理
    Canny(blurred, edges, 50, 150); // 边缘检测

    // 寻找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // 筛选轮廓并计算透视变换矩阵
    for (size_t i = 0; i < contours.size(); i++)
    {
        vector<Point> contourPoly;
        approxPolyDP(contours[i], contourPoly, 0.02 * arcLength(contours[i], true), true);
        if (contourPoly.size() == 4) // 假设番茄酱包是矩形
        {
            Rect boundingBox = boundingRect(contourPoly);
            float ratio = boundingBox.width / (float)boundingBox.height;
            if (ratio > 1.8 && ratio < 2.2) // 检查是否接近2:1的比例
            {
                // 对轮廓进行排序以便正确地应用透视变换
                Point2f srcQuad[4];
                for (int j = 0; j < 4; j++)
                    srcQuad[j] = contourPoly[j];

                // 定义目标尺寸
                int maxWidth = boundingBox.width;
                int maxHeight = boundingBox.height / 2;
                Point2f dstQuad[4] = { Point2f(0, 0), Point2f(maxWidth - 1, 0), Point2f(maxWidth - 1, maxHeight - 1), Point2f(0, maxHeight - 1) };

                // 计算透视变换矩阵
                Mat transform = getPerspectiveTransform(srcQuad, dstQuad);

                // 应用透视变换
                Mat result;
                warpPerspective(src, result, transform, Size(maxWidth, maxHeight));

                imshow("Result", result);
                waitKey(0);
            }
        }
    }

    return 0;
}