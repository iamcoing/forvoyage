#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

extern void conv2D(InputArray src, InputArray kernel, OutputArray dst, int depth, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);
extern void seqConv2D_Y_X(InputArray src, OutputArray src_kerY_kerX, int depth, InputArray kernelY, InputArray kernelX, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);
extern void seqConv2D_X_Y(InputArray src, OutputArray src_kerX_kerY, int depth, InputArray kernelX, InputArray kernelY, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);

Mat gaussConv(Mat I, float sigma, int s)
{
    //构建水平方向上的非归一化的高斯核
    Mat xkernel = Mat::zeros(1, s, CV_32FC1);
    //中心位置
    int cs = (s - 1) / 2;
    //方差
    float sigma2 = pow(sigma, 2.0);
    for(int c = 0; c < s; c++){
        float norm2 = pow(float(c - cs), 2.0);
        xkernel.at<float>(0, c) = exp(-norm2 / (2 * sigma2));
    }
    //将xkernel转置，得到垂直方向上的卷积核
    Mat ykernel = xkernel.t();
    //分离卷积核的卷积运算
    Mat gauConv;
    seqConv2D_X_Y(I, gauConv, CV_32F, xkernel, ykernel);
    gauConv.convertTo(gauConv, CV_32F, 1.0 / sigma2);
    
    return gauConv;
}
Mat DoG(Mat I, float sigma, int s, float k = 1.1)
{
    //与标准差为sigma的非归一化的高斯核卷积
    Mat Ig = gaussConv(I, sigma, s);
    //与标准差为k * sigma 的非归一化的高斯核卷积
    Mat Igk = gaussConv(I, k*sigma, s);
    //两个高斯卷积结果做差
    Mat doG = Igk - Ig;
    return doG;
}

// int main(int argc, const char *argv[])
// {
//     Mat Img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
//     if(Img.empty())
//     {
//         cout << "image read failed" << endl;
//         return -1;
//     }
//     //高斯差分
//     float sigma = 2;
//     int win = 13;
//     float k = 1.05;
//     Mat doG = DoG(Img, sigma, win, k);
//     //自动阈值处理
//     Mat edge;
//     threshold(doG, edge, 0, 255, THRESH_BINARY);
//     //显示二值化的边缘
//     imshow("DoG", edge);

//     waitKey(0);

//     return 0;
// }