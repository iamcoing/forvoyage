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
