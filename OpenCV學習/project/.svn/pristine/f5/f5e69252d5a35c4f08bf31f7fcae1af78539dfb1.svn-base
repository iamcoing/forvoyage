#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat medianSmooth(const Mat & I, Size size, int borderType = BORDER_DEFAULT)
{
    CV_Assert(I.type() == CV_8UC1);
    int H = size.height;
    int W = size.width;

    CV_Assert(H > 0 && W > 0);
    CV_Assert(H % 2 == 1 && W % 2 == 1);

    int h = (H - 1)/2;
    int w = (W - 1)/2;

    Mat Ip;

    copyMakeBorder(I, Ip, h, h, w, w, borderType);
    
    int rows = I.rows;
    int cols = I.cols;

    Mat medianI(I.size(), CV_8UC1);
    int i = 0, j = 0;

    int index = (H * W - 1)/2;
    for(int r = h; r < h + rows; r++)
    {
        for(int c = w; c < w + cols; c++)
        {
            Mat region = Ip(Rect(c - w, r - h, W, H)).clone();
            
            region = region.reshape(1, 1);
            
            cv::sort(region, region, CV_SORT_EVERY_ROW);

            uchar mValue = region.at<uchar>(0, index);
            medianI.at<uchar>(i, j) = mValue;
            j++;
        }
        i++;
        j = 0;
    }

    return medianI;
}

int main(int argc, const char * argv[])
{
    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if( image.empty() ){
        perror("imread");
        return -1;
    }
    Mat medianImage = medianSmooth(image, Size(9, 9));
    imshow("source", image);
    imshow("midSth", medianImage);

    waitKey(0);

    return 0;
}