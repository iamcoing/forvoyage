#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    //点集
    Mat points = (Mat_<float>(5, 2) << -1, -1, -5, -1, -1, -2, -5, -2, -2, -5);
    //计算点集的最小外包旋转矩形
    RotatedRect rRect = minAreaRect(points);
    //打印旋转矩形的信息
    cout << "angle " << rRect.angle << endl;
    cout << "center " << rRect.center << endl;
    cout << "size " << rRect.size << endl;

    cout << points << endl;

    //构建旋转矩形
    RotatedRect rRect(Point2f(200, 200), Point2f(90, 150), -60);
    //计算旋转矩形的4个顶点，存储为一个4行2列的单通道float类型的矩阵
    Mat vertices;
    boxPoints(rRect, vertices);
    //打印4个顶点
    cout << vertices << endl;
    //画出该旋转矩形
    

    return 0;
}