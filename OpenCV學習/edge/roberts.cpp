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

void roberts(InputArray src, OutputArray dst, int depth, 
int x = 1, int y = 0, int borderType = BORDER_DEFAULT)
{
    CV_Assert( !(x == 0 && y == 0) );
    Mat roberts_1 = (Mat_<float>(2, 2) << 1, 0, 0, -1);
    Mat roberts_2 = (Mat_<float>(2, 2) << 0, 1, -1, 0);
    //当x不等于零时，src和roberts1卷积
    if(x != 0 && y == 0){
        conv2D(src, roberts_1, dst, depth, Point(0, 0), borderType);
    }
    //当y不等于零时，src和roberts2卷积
    if(y != 0 && x == 0){
        conv2D(src, roberts_2, dst, depth, Point(1, 0), borderType);
    }
}

int main(int argc, const char * argv[])
{
    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(img.empty()){
        perror("imread");
        exit(-1);
    }    
    
    imshow("原图", img);

    Mat img_roberts1;
    roberts(img, img_roberts1, CV_32FC1, 1, 0);
    Mat img_roberts2;
    roberts(img, img_roberts2, CV_32FC1, 0, 1);

    Mat abs_img_roberts1, abs_img_roberts2;
    convertScaleAbs(img_roberts1, abs_img_roberts1, 1, 0);
    convertScaleAbs(img_roberts2, abs_img_roberts2, 1, 0);
    imshow("135 方向的边缘", abs_img_roberts1);
    imshow("45 方向的边缘", abs_img_roberts2);

    Mat img_roberts1_2, img_roberts2_2, edge;
    pow(img_roberts1, 2.0, img_roberts1_2);
    pow(img_roberts2, 2.0, img_roberts2_2);
    sqrt(img_roberts1_2 + img_roberts2_2, edge);

    edge.convertTo(edge, CV_8UC1);
    imshow("边缘", edge);

    waitKey(0);

    return 0;
}