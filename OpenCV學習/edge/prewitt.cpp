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

void prewitt(InputArray src, OutputArray dst, int depth, 
int x, int y = 0, int borderType = BORDER_DEFAULT)
{
    CV_Assert( !(x == 0 && y == 0) );
    if(x != 0 && y == 0){
        Mat prewitt_x_y = (Mat_<float>(3, 1) << 1, 1, 1);
        Mat prewitt_x_x = (Mat_<float>(1, 3) << 1, 0, -1);
        seqConv2D_Y_X(src, dst, depth, prewitt_x_y, prewitt_x_x, Point(-1, -1), borderType);
    }
    if(y != 0 && x == 0){
        Mat prewitt_y_x = (Mat_<float>(1, 3) << 1, 1, 1);
        Mat prewitt_y_y = (Mat_<float>(3, 1) << 1, 0, -1);
        seqConv2D_X_Y(src, dst, depth, prewitt_y_x, prewitt_y_y, Point(-1, -1), borderType);
    }
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        perror("image read");
        exit(-1);
    }
    Mat img_prewitt_x;
    prewitt(img, img_prewitt_x, CV_32FC1, 1, 0);

    Mat img_prewitt_y;
    prewitt(img, img_prewitt_y, CV_32FC1, 0, 1);

    Mat abs_img_prewitt_x, abs_img_prewitt_y;
    convertScaleAbs(img_prewitt_x, abs_img_prewitt_x, 1, 0);
    convertScaleAbs(img_prewitt_y, abs_img_prewitt_y, 1, 0);

    imshow("horzion", abs_img_prewitt_x);
    imshow("slip", abs_img_prewitt_y);

    Mat img_prewitt_x2, img_prewitt_y2;
    pow(img_prewitt_x, 2.0, img_prewitt_x2);
    pow(img_prewitt_y, 2.0, img_prewitt_y2);

    Mat edge;

    sqrt(img_prewitt_x2 + img_prewitt_y2, edge);

    imshow("edge", edge);

    waitKey(0);

    return 0;
}
