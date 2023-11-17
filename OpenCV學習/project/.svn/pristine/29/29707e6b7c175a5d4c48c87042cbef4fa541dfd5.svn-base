#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void conv2D(InputArray src, InputArray kernel, OutputArray dst,
int depth, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT)
{
    Mat kernelFlip;
    flip(kernel, kernelFlip, -1);
    filter2D(src, dst, depth, kernelFlip, anchor, 0.0, borderType);
}

void seqConv2D_Y_X(InputArray src, OutputArray src_kerY_kerX, 
int depth, InputArray kernelY, InputArray kernelX, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT)
{
    Mat src_kerY;
    conv2D(src, kernelY, src_kerY, depth, anchor, borderType);
    conv2D(src_kerY, kernelX, src_kerY_kerX, depth, anchor, borderType);
}

void seqConv2D_X_Y(InputArray src, OutputArray src_kerX_kerY, 
int depth, InputArray kernelX, InputArray kernelY, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT)
{
    Mat src_kerX;
    conv2D(src, kernelX, src_kerX, depth, anchor, borderType);
    conv2D(src_kerX, kernelY, src_kerX_kerY, depth, anchor, borderType);
}

void getLoGKernel(float sigma, int length, Mat & kernelX, Mat & kernelY)
{
    kernelX.create(Size(length, 1), CV_32FC1);
    kernelY.create(Size(1, length), CV_32FC1);
    int center = (length - 1) / 2;
    double sigma2 = pow(sigma, 2.0);
    //构建可分离的高斯拉普拉斯核
    for(int c = 0; c < length; c++){
        float norm2 = pow(c - center, 2.0);
        kernelY.at<float>(c, 0) = exp(-norm2 / (2 * sigma2));
        kernelX.at<float>(0, c) = (norm2 / sigma2 - 1.0)*kernelY.at<float>(c, 0);
    }
}

Mat LoG(InputArray image, float sigma, int win)
{
    Mat kernelX, kernelY;
    //获得两个分离的卷积核
    getLoGKernel(sigma, win, kernelX, kernelY);
    //先进行水平卷积，再进行垂直卷积
    Mat covXY;
    seqConv2D_X_Y(image, covXY, CV_32FC1, kernelX, kernelY);
    //卷积核转置
    Mat kernelX_T = kernelX.t();
    Mat kernelY_T = kernelY.t();
    //先进行垂直卷积，再进行水平卷积
    Mat covYX;
    seqConv2D_Y_X(image, covYX, CV_32FC1, kernelX_T, kernelY_T);
    //计算两个卷积结果的和，得到高斯拉普拉斯卷积
    Mat LoGCov;
    add(covXY, covYX, LoGCov);

    return LoGCov;
}

// int main(int argc, const char *argv[])
// {
//     Mat yu = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
//     if(yu.empty())
//     {
//         cout << "im read failed" << endl;
//         return -1;
//     }

//     //高斯拉普拉斯卷积
//     float sigma = 4;
//     int win = 25;
//     Mat log = LoG(yu, sigma, win);
//     Mat edge;
//     threshold(log, edge, 255, 127, THRESH_BINARY);
//     imshow("LoG", edge);

//     waitKey(0);
//     return 0;
// }