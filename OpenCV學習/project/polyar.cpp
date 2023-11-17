#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdio>

using namespace cv;
using namespace std;

// I,图像数据， center：中心点数据， minr： 最小的直径 mintheta：最小的角度
// thetaStep： 角度的变换步长，rStep：距离的变换步长
Mat polar(Mat I, Point2f center, Size size, float minr = 0, \
float mintheta = 0, float thetaStep = 1.0 / 4, float rStep = 1)
{
    //创建转换后的窗口尺寸
    //构造一个二维的mat，参数类型为float型
    Mat ri = Mat::zeros(Size(1, size.height), CV_32FC1);
    for(int i = 0; i < size.height; i++)
    {
        ri.at<float>(i, 0) = minr + i * rStep;
    }
    Mat r = repeat(ri, 1, size.width);

    Mat thetaj = Mat::zeros(Size(size.width, 1), CV_32FC1);
    for(int j = 0; j < size.width; j++)
        thetaj.at<float>(0, j) = mintheta + j * thetaStep;
    //重复函数，thetaj沿竖方向重复
    Mat theta = repeat(thetaj, size.height, 1);

    //将极坐标转换为笛卡尔坐标
    Mat x, y;
    polarToCart(r, theta, x, y, true);

    //将坐标原点移动到中心点
    x += center.x;
    y += center.y;

    //最近临插值
    Mat dst = 125 * Mat::ones(size, CV_8UC1);
    for(int i = 0; i < size.height; i++)
        for(int j = 0; j < size.width; j++)
        {
            //绘制中心点
            float xij = x.at<float>(i, j);
            float yij = y.at<float>(i, j);
            int nearestx = int(round(xij));
            int nearesty = int(round(yij));

            if( (0 <= nearestx && nearestx < I.cols) &&
                (0 <= nearesty && nearesty < I.rows))
                dst.at<uchar>(i, j) = I.at<uchar>(nearesty, nearesty);
        }
    return dst;
}

/*
    极坐标操作比较耗时，有时候只对距离变换中心一定范围内的点进行极坐标变换，可以令minr=270,
    rStep = 1,从而使输出的图像在70(340 - 270)，设圆环中心点为（508,503）令mintheta=0, thetaStep = 0.25,
    由于圆环角度0-360, 则输出图像的宽度大约等于360 × 4 = 1440
**/
int main(int argc, const char * argv[])
{
    Mat I = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if(I.empty()){
        cout << "no img read" << endl;
        return -1;
    }
    // float thetaStep = 1.0 / 4;
    // float minr = 70.0;
    // Size size(int(360 / thetaStep), 70);
    Mat dst;
    Point2f center(490, 490);
    float M = 130;
    logPolar(I, dst, center, M, WARP_FILL_OUTLIERS);
    //dst = polar(I, Point2f(490, 490), size, minr);
    // linearPolar(I, dst, Point2f(490, 490), 750, CV_INTER_LINEAR);
    //沿水平方向处理
    //flip(dst, dst, 0);
    imshow("img", I);
    imshow("xy axis", dst);

    waitKey(0);

    return 0;
}