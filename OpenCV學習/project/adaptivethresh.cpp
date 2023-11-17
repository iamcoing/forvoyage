#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

enum METHOD{
    MEAN,
    GAUSS,
    MEDIAN,
};

Mat adaptiveThresh(Mat I, int radius, float ratio, METHOD method = MEAN){
    //第一步，对图像矩阵进行平滑处理
    Mat smooth;
    switch(method){
        case MEAN:  //均值平滑
            boxFilter(I, smooth, CV_32FC1, Size(2 * radius + 1, 2 * radius + 1));
            break; 
        case GAUSS: //高斯平滑
            GaussianBlur(I, smooth, Size(2 * radius + 1, 2 * radius + 1), 0, 0);
            break;
        case MEDIAN://中值平滑
            medianBlur(I, smooth, 2 * radius + 1);
            break;
        default:
            break;
    }
    //第二步， 平滑结果乘以比例系数，然后图像矩阵与其做差
    I.convertTo(I, CV_32FC1);
    smooth.convertTo(smooth, CV_32FC1);
    //Mat diff = I - (1.0 - ratio) * smooth;
    //第三步， 阈值处理，当大于或等于0时，输出值为255；反之，输出值为0
    Mat out = Mat::zeros(diff.size(), CV_8UC1);
    for(int r = 0; r < out.rows; r++){
        for(int c = 0; c < out.cols; c++){
            if(diff.at<float>(r, c) >= 0)
                out.at<uchar>(r, c) = 256;
        }
    }

    return out;
}

int main(int argc, const char * argv[])
{

    return 0;
}