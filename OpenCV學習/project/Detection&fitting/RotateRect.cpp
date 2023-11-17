#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    //构造旋转矩形
    RotatedRect rRect(Point2f(200, 200), Point2f(90, 150), 0);
    //计算旋转矩形的4个顶点，存储为一个4行2列的单通道float类型的Materix
    Mat vertices;
    boxPoints(rRect, vertices);
    //在黑色画板上画出该旋转矩形
    Mat img = Mat::zeros(Size(400, 400), CV_8UC1);
    for(int i = 0; i < 4; i++)
    {
        //相邻的点
        Point p1 = (Point)vertices.row(i);
        int j = (i + 1) % 4;
        Point p2 = (Point)vertices.row(j);
        line(img, p1, p2, Scalar(255));
    }
    //显示旋转矩形
    imshow("rotateRect", img);
    waitKey(0);

    return 0;   
}