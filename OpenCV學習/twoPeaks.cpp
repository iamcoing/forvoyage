#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

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

Mat equalHist(Mat image)
{
    CV_Assert(image.type() == CV_8UC1);
    int rows = image.rows;
    int cols = image.cols;

    Mat grayHist = calcGrayHist(image);

    Mat zeroCumMonet = Mat::zeros(Size(256, 1), CV_32SC1);

    for (int p = 0; p < 256; p++)
    {
        if (p == 0)
            zeroCumMonet.at<int>(0, p) = grayHist.at<int>(0, 0);
        else
            zeroCumMonet.at<int>(0, p) = zeroCumMonet.at<int>(0, p - 1) +
                                         grayHist.at<int>(0, p);
    }

    Mat outPut_q = Mat::zeros(Size(256, 1), CV_8UC1);
    float cofficient = 256.0 / (rows * cols);
    for(int p = 0; p < 256; p++){
        float q = cofficient * zeroCumMonet.at<int>(0, p) - 1;
        if(q >= 0)
            outPut_q.at<uchar>(0, p) = uchar(floor(q));
        else
            outPut_q.at<uchar>(0, p) = 0;
    }

    Mat equalHistImage = Mat::zeros(image.size(), CV_8UC1);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            int p = image.at<uchar>(r, c);
            equalHistImage.at<uchar>(r, c) = outPut_q.at<uchar>(0, p);
        }
        return equalHistImage;
    }
}

int threshTwoPeaks(const Mat &image, Mat &thresh_out)
{
    //计算灰度直方图
    Mat histogram = calcGrayHist(image);
    //找到灰度直方图最大峰值对应的灰度值
    Point firstPeakLoc;
    minMaxLoc(histogram, NULL, NULL, NULL, &firstPeakLoc);
    int firstPeak = firstPeakLoc.x;
    //寻找灰度直方图第二个峰值对应的灰度值
    Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);
    for(int k = 0; k < 256; k++){
        int hist_k = histogram.at<int>(0, k);
        measureDists.at<float>(0, k) = pow(float(k - firstPeak), 2) * hist_k;
    }
    Point secondPeakLoc;
    minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakLoc);
    int secondPeak = secondPeakLoc.x;
    //找到两个峰值之间的最小值对应的灰度值，作为阈值
    Point threshLoc;
    int thresh = 0;
    //第一个峰值在第二个峰值的左侧
    if(firstPeak < secondPeak){
        minMaxLoc(histogram.colRange(firstPeak, secondPeak), NULL, NULL, &threshLoc);
        thresh = firstPeak + threshLoc.x + 1;
    }
    //第一个峰值在第二个峰值右侧
    else{
        minMaxLoc(histogram.colRange(secondPeak, firstPeak), NULL, NULL, &threshLoc);
        thresh = secondPeak + threshLoc.x + 1;
    }
    //阈值分割
    threshold(image, thresh_out, thresh, 255, THRESH_BINARY);
    return thresh;
}

int main(int argc, const char * argv[])
{
    
    return 0;
}