#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat calcGrayHist(const Mat &image)
{
    Mat histogram = Mat::zeros(Size(256, 1), CV_32SC1);
    int rows = image.rows;
    int cols = image.cols;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < image.cols; c++)
        {
            int index = int( image.at<uchar>(r, c) );
            histogram.at<int>(0, index) += 1;
        }
    }

    return histogram;
}

int otsu(const Mat & image, Mat & OtsuThreshImage)
{
    //计算灰度直方图
    Mat histogram = calcGrayHist(image);
    //归一化灰度直方图
    Mat normHist;
    histogram.convertTo(normHist, CV_32FC1, 1.0 / (image.rows * image.cols), 0.0);
    //计算累加直方图（零阶累积矩）和一阶累积矩
    Mat zeroCumuMoment = Mat::zeros(Size(256, 1), CV_32FC1);
    Mat oneCumuMoment  = Mat::zeros(Size(256, 1), CV_32FC1);
    for(int i = 0; i < 256; i++){
        if(i == 0){
            zeroCumuMoment.at<float>(0, i) = normHist.at<float>(0, i);
            oneCumuMoment.at<float>(0, 1) = i*normHist.at<float>(0, i);
        }else{
            zeroCumuMoment.at<float>(0, i) = zeroCumuMoment.at<float>(0, i-1) + 
            normHist.at<float>(0, i);
            oneCumuMoment.at<float>(0, 1) = oneCumuMoment.at<float>(0, i-1) + 
            i*normHist.at<float>(0, i);
        }
    }
    //计算类间方差
    Mat variance = Mat::zeros(Size(256, 1), CV_32FC1);
    //总平均值
    float mean = oneCumuMoment.at<float>(0, 256);
    for(int i = 0; i < 256; i++){
        if(zeroCumuMoment.at<float>(0, i) == 0 || zeroCumuMoment.at<float>(0, i) == 1)
            variance.at<float>(0, i) = 0;
        else{
            float cofficient = zeroCumuMoment.at<float>(0, i) * (1.0 - zeroCumuMoment.at<float>(0, i));
            variance.at<float>(0, i) = pow(mean * zeroCumuMoment.at<float>(0, i)
            - oneCumuMoment.at<float>(0, i), 2.0) / cofficient;
        }
    }
    //找到阈值
    Point maxLoc;
    minMaxLoc(variance, NULL, NULL, NULL, &maxLoc);
    int otsuThresh = maxLoc.x;
    
    //阈值处理
    threshold(image, OtsuThreshImage, otsuThresh, 255, THRESH_BINARY);
    return otsuThresh;
}

int mian(int argc, const char * argv[])
{

    return 0;
}