#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

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

int main(int argc, const char **argv)
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if( !img.data ){
        perror("imread");
        exit(-1);
    }
    equalHist(img);
    return 0;
}