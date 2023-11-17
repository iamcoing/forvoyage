#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

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

int factorial(int n)
{
    if(n == 0 || n == 1)
        return 1;
    
    return n * factorial(n - 1);
}

Mat getPascalSmooth(int n)
{
    Mat pascalSmooth = Mat::zeros(Size(n, 1), CV_32FC1);
    for(int i = 0; i < n; i++)
        pascalSmooth.at<float>(0, i) = factorial(n - 1) / (factorial(i) * factorial(n - 1 - i));
    return pascalSmooth;
}

Mat getPascalDiff(int n)
{
    Mat pascalDiff = Mat::zeros(Size(n, 1), CV_32FC1);
    Mat pascalSmooth_previous = getPascalSmooth(n - 1);
    for(int i = 0; i < n; i++)
    {
        if(i == 0)
            pascalDiff.at<float>(0, i) = 1.0;
        else if(i == n - 1)
            pascalDiff.at<float>(0, i) = -1.0;
        else 
            pascalDiff.at<float>(0, i) = pascalSmooth_previous.at<float>(0, i)
            -pascalSmooth_previous.at<float>(0, i - 1);
    }
    return pascalDiff;
}

Mat sobel(Mat image, int x_flag, int y_flag, int ddepth, int winSize, int borderType)
{
    CV_Assert( winSize >= 1 && winSize % 2 == 1 );
    Mat pascalDiff = getPascalDiff(winSize);
    Mat pascalSmooth = getPascalSmooth(winSize);
    Mat image_con_sobel;
    if( x_flag )
    {
        seqConv2D_Y_X(image, image_con_sobel, ddepth, pascalSmooth.t(),
        pascalDiff, Point(-1, -1), borderType);
    }
    else  if( x_flag == 0 && y_flag != 0 )
    {
        seqConv2D_X_Y(image, image_con_sobel, ddepth, pascalSmooth,
        pascalDiff.t(), Point(-1, -1), borderType);
    }

    return image_con_sobel;
}

int main(int argc, const char * argv[])
{
    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(image.empty())
    {
        perror("imread");
        exit(-1);
    }
    Mat outimg = sobel(image, 1, 0, CV_32FC1, 3, BORDER_DEFAULT);

    Mat sobimg;
    // Sobel(image, sobimg, CV_32FC1, 1, 0, 5, 1.0, 1.0, BORDER_DEFAULT);

    imshow("output", outimg);
    // imshow("sobel", sobimg);

    waitKey(0);

    return 0;
}