#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat guidedFilter(Mat I, Mat P, int r, float eps, float s)
{
    int rows = I.rows;
    int cols = I.cols;
    //缩小图像
    Mat small_I, small_P;
    Size smallSize(int(round(s*cols)), int(round(s*rows)));
    resize(I, small_I, smallSize, 0, 0, CV_INTER_CUBIC);
    resize(P, small_P, smallSize, 0, 0, CV_INTER_CUBIC);
    //缩放均值平滑窗口的半径
    int small_r = int(round(r * s));
    Size winSize(2 * small_r + 1, 2 * small_r + 1);
    //均值平滑
    Mat mean_small_I, mean_small_P;
    boxFilter(small_I, mean_small_I, CV_64FC1, winSize);
    boxFilter(small_P, mean_small_P, CV_64FC1, winSize);
    //small_I .* small_p的均值平滑
    Mat small_Ip = small_I.mul(small_P);
    Mat mean_small_Ip;
    boxFilter(small_Ip, mean_small_Ip, CV_64FC1, winSize);
    //协方差
    Mat cov_small_Ip = mean_small_Ip - mean_small_I.mul(mean_small_P);
    //均值平滑
    Mat mean_small_II;
    boxFilter(small_I.mul(small_I), mean_small_II, CV_64FC1, winSize);
    //方差
    Mat var_small_I = mean_small_II - mean_small_I.mul(mean_small_I);
    Mat small_a = cov_small_Ip / (var_small_I + eps);
    Mat small_b = mean_small_P - small_a.mul(mean_small_I);
    //对small_a 和 small_b 进行均值平滑
    Mat mean_small_a, mean_small_b;
    boxFilter(small_a, mean_small_a, CV_64FC1, winSize);
    boxFilter(small_b, mean_small_b, CV_64FC1, winSize);
    //放大
    Mat mean_a, mean_b;
    resize(mean_small_a, mean_a, I.size(), 0, 0, CV_INTER_LINEAR);
    resize(mean_small_b, mean_b, I.size(), 0, 0, CV_INTER_LINEAR);
    Mat q = mean_a.mul(I) + mean_b;
    
    return q;
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty())
    {
        perror("imread");
        exit(-1);
    }
    Mat fI;
    img.convertTo(fI, CV_64FC1, 1.0 / 255);
    Mat q = guidedFilter(fI, fI, 7, 0.04, 0.1);
    
    //细节增强
    Mat I_enhanced = (fI - q) * 5 + q;
    normalize(I_enhanced, I_enhanced, 1, 0, NORM_MINMAX, CV_32FC1);

    imshow("src", img);
    imshow("gui", q);
    imshow("enhanced", I_enhanced);

    waitKey(0);

    return 0;
}