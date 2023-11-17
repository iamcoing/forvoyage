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

void sharr(InputArray src, OutputArray dst, int ddepth, int x, int y = 0, int borderType = BORDER_DEFAULT)
{
    CV_Assert(! (x == 0 && y == 0) );
    // Mat scharr_x = (Mat_<float>(3, 3) << 3, 0, -3, 10, 0, -10, 3, 0, -3);
    // Mat scharr_y = (Mat_<float>(3, 3) << 3, 10, 3, 0, 0, 0, -3, -10, -3);

    Mat scharr_x = (Mat_<float>(3, 3) << 0, 3, 10, -3, 0, 3, 10, -3, 0);
    Mat scharr_y = (Mat_<float>(3, 3) << 10, 3, 0, -3, 0, -3, 0, -3, -10);

    if( x != 0 && y == 0)
    {
        conv2D(src, scharr_x, dst, ddepth, Point(-1, -1), borderType);
    }
    else if( x == 0 && y != 0)
    {
        conv2D(src, scharr_y, dst, ddepth, Point(-1, -1), borderType);
    }

    return;
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    if(img.empty()){
        perror("image read");
        exit(-1);
    }
    Mat dst;
    sharr(img, dst, -1, 0, 1);
    imshow("tmp", dst);
    sharr(dst, dst, -1, 1, 0);
    imshow("src", img);
    imshow("dst", dst);

    waitKey(0);
    return 0;
}